#include <librdkafka/rdkafka.h>
#include <uuid/uuid.h>
#include <json-c/json.h>
#include <spw_la_api.h>
#include "packet_archiver.h"
#include "arg_parser.h"
#include "data_logger.h"


#define ARR_SIZE(arr) ( sizeof((arr)) / sizeof((arr[0])) )

/* Optional per-message delivery callback (triggered by poll() or flush())
 * when a message has been successfully delivered or permanently
 * failed delivery (after retries).
 */
static void dr_msg_cb (rd_kafka_t *kafka_handle, const rd_kafka_message_t *rkmessage, void *opaque)
{
    if (rkmessage->err) {
        fprintf(stderr, "Message delivery failed: %s", rd_kafka_err2str(rkmessage->err));
    }
}


static int32_t sendKafkaMessage(rd_kafka_t *producer, const char *topic, uint8_t *buffer, size_t length)
{
    rd_kafka_resp_err_t err;

    err = rd_kafka_producev(producer,
                            RD_KAFKA_V_TOPIC(topic),
                            RD_KAFKA_V_MSGFLAGS(RD_KAFKA_MSG_F_COPY),
                            //RD_KAFKA_V_KEY((void*)key, key_len),
                            RD_KAFKA_V_VALUE(buffer, length),
                            RD_KAFKA_V_OPAQUE(NULL),
                            RD_KAFKA_V_END);

    if (err) {
        fprintf(stderr, "Failed to produce to topic %s: %s", topic, rd_kafka_err2str(err));
        if(err == RD_KAFKA_RESP_ERR__QUEUE_FULL)
        {
            rd_kafka_poll(producer, 1000);
        }
        return 1;
    } else {
        fprintf(stderr, "Produced event to topic %s: value = %12s\n", topic, buffer);
    }

    rd_kafka_poll(producer, 0);
}

static uint32_t createCapturePacket(Settings settings, PacketInfo packetInfo, uint8_t* msg, uint32_t* msg_len)
{
	uint32_t ret = 0;
	uuid_t binuuid;

	// struct timeval rawtime;
	// struct tm* timeinfo;
	// char time_str[24];

    uint32_t interfaceIdLength = sizeof(packetInfo.interfaceId);

	struct json_object* obj;

	if(*msg_len < 36 + sizeof(packetInfo.captureTime) + interfaceIdLength + packetInfo.rawDataLength)
	{
		fputs("\nMessage length exceeding buffer size.\n", stderr);
	}else
	{
		/*
		 * Generate a UUID. We're not done yet, though,
		 * for the UUID generated is in binary format
		 * (hence the variable name). We must 'unparse'
		 * binuuid to get a usable 36-character string.
		 */
		uuid_generate_random(binuuid);

		/*
		 * uuid_unparse() doesn't allocate memory for itself, so do that with
		 * malloc(). 37 is the length of a UUID (36 characters), plus '\0'.
		 */
		char uuid_[37];

		/*
		 * Produces a UUID string at uuid consisting of letters
		 * whose case depends on the system's locale.
		 */
		uuid_unparse(binuuid, uuid_);

		// gettimeofday(&rawtime, NULL);
		// timeinfo = localtime(&rawtime.tv_sec);
		// strftime(time_str, 24, "%Y-%m-%dT%H:%M:%S", timeinfo);
		// sprintf(&time_str[19], ".%.3d",(int)(rawtime.tv_usec / 1000));


		obj = json_object_new_object();
		json_object_object_add(obj, "uuid", json_object_new_string(uuid_));
		json_object_object_add(obj, "capture_time", json_object_new_string(packetInfo.captureTime));
		json_object_object_add(obj, "interface_id", json_object_new_string_len(packetInfo.interfaceId, interfaceIdLength));
		json_object_object_add(obj, "test_id", json_object_new_string(settings.kafka_testId));
		json_object_object_add(obj, "test_version", json_object_new_string(settings.kafka_testVersion));
		json_object_object_add(obj, "asw_version", json_object_new_string(settings.version));
		json_object_object_add(obj, "db_version", json_object_new_string(settings.kafka_dbVersion));
		json_object_object_add(obj, "raw_data", json_object_new_string_len(packetInfo.rawData, packetInfo.rawDataLength));

		*msg_len = sprintf((char*)msg, "%s", json_object_to_json_string(obj));

		json_object_put(obj);

        ret = 1;
	}

	return ret;
}

static int32_t LA_MK3_assembleDataPacket(FILE *dataStream, PacketInfo *packet, STAR_LA_MK3_Event event, const double *deltaToTrigger, struct timespec *triggerTime)
{
    /* Total number of bytes in the packet */
    int32_t packetBytes = 0;

    /* Size of the content in the data stream file */
    int32_t fileSize = ftell(dataStream);

    /* Start new packet, if header event is detected */
    if ((0 == fileSize) && (STAR_LA_TRAFFIC_TYPE_HEADER == event.type))
    {
        char *timestampStr = LA_MK3_getPacketTimestamp(deltaToTrigger, triggerTime);
        packet->captureTime = timestampStr;
        fprintf(dataStream, "%02x", event.data);
        packet->rawDataLength++;
        free(timestampStr);
    }
    /* Add byte to packet, if data event is detected */
    else if ((0 < fileSize) && (STAR_LA_TRAFFIC_TYPE_DATA == event.type)) // Also header events?
    {
        /* Print byte */
        fprintf(dataStream, "%02x", event.data);
    }
    /* End packet and return number of bytes in data stream file, if EOP or EEP event is detected */
    else if (STAR_LA_TRAFFIC_TYPE_EOP == event.type || STAR_LA_TRAFFIC_TYPE_EEP == event.type)
    {
        packetBytes = fileSize;
    }
    /* Print Timecode directly to hexdump */
    else if (STAR_LA_TRAFFIC_TYPE_TIMECODE == event.type)
    {
        /* Ignore Timecodes
        char *timestampStr = LA_MK3_getPacketTimestamp(deltaToTrigger, triggerTime);
        fprintf(stdout, "\n%s %s\n", packet->interfaceId, timestampStr);
        fprintf(stdout, "%02x\n", 0, event.data);
        free(timestampStr);
        */
    }
    else 
    {
        fprintf(stderr, "\nUnexpected Event Type: %s\n", GetEventTypeString(event.type));
    }

    return packetBytes;
}

int32_t LA_MK3_archiveCapturedPackets(Settings settings, STAR_LA_MK3_Traffic *pTraffic, const U32 *trafficCount, const double *charCaptureClockPeriod, struct timespec *triggerTime, const int preTrigger)
{
	rd_kafka_t* producer; /* Producer instance handle */
	rd_kafka_conf_t* conf; /* Temporary configuration object */
	char errstr[512]; /* librdkafka API error reporting buffer */
	const char* brokers = "RMC-070402DL"; /* Argument: broker list */
	const char* topic = "test"; /* Argument: topic to produce to */

    uint8_t buffer[BUF_SIZE];

    /* Loop counter */
    U32 i = 0;

    /* Return value */
    int32_t ret = 1;

    /* Struct collecting packet data received on receiver A */
    PacketInfo receiverA;
    receiverA.rawData = "0123456789abcdef";
    receiverA.interfaceId = "Receiver A";
    receiverA.captureTime = "noon";
    FILE *packetA = open_memstream(&receiverA.rawData, &receiverA.rawDataLength);

    /* Struct collecting packet data received on receiver B */
    PacketInfo receiverB;
    receiverB.rawData = "Hello There";
    receiverB.interfaceId = "Receiver B";
    receiverB.captureTime = "evening";
    FILE *packetB = open_memstream(&receiverB.rawData, &receiverB.rawDataLength);

    /* Load the relevant configuration sections. */
    conf = rd_kafka_conf_new();

    /* Set bootstrap broker(s) as a comma-separated list of
	 * host or host:port (default port 9092).
	 * librdkafka will use the bootstrap brokers to acquire the full
	 * set of brokers from the cluster. */
	if(rd_kafka_conf_set(conf, "bootstrap.servers", brokers, errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK)
	{
		fprintf(stderr, "%s\n", errstr);
		return 0;
	}

    /* Set the delivery report callback.
	 * This callback will be called once per message to inform
	 * the application if delivery succeeded or failed.
	 * See dr_msg_cb() above.
	 * The callback is only triggered from rd_kafka_poll() and
	 * rd_kafka_flush(). */
    rd_kafka_conf_set_dr_msg_cb(conf, dr_msg_cb);

    /*
	 * Create producer instance.
	 *
	 * NOTE: rd_kafka_new() takes ownership of the conf object
	 *       and the application must not reference it again after
	 *       this call.
	 */
    producer = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
    if (!producer) {
        fprintf(stderr, "Failed to create new producer: %s", errstr);
        return 0;
    }

    // Configuration object is now owned, and freed, by the rd_kafka_t instance.
    conf = NULL;



    int32_t length = sizeof(buffer);

    rd_kafka_resp_err_t err;

    for (i = 0; i < *trafficCount; i++)
    {
        /* Time difference of the current traffic event to the trigger in seconds */
        double deltaToTrigger = pTraffic[i].time * *charCaptureClockPeriod;
        /* Print packets, starting at set pre trigger duration */
        if (-preTrigger <= (deltaToTrigger * 1000.0))
        {
            /* Create kafka packet for receiver A, if complete */
            if (0 != LA_MK3_assembleDataPacket(packetA, &receiverA, pTraffic[i].linkAEvent, &deltaToTrigger, triggerTime))
            {
                fclose(packetA);
                createCapturePacket(settings, receiverA, buffer, &length);
                sendKafkaMessage(producer, topic, buffer, length);
                free(receiverA.rawData);
                free(receiverA.captureTime);
                packetA = open_memstream(&receiverA.rawData, &receiverA.rawDataLength);
            }

            /* Create kafka packet for receiver B, if complete */
            if (0 != LA_MK3_assembleDataPacket(packetB, &receiverB, pTraffic[i].linkBEvent, &deltaToTrigger, triggerTime))
            {
                fclose(packetB);
                createCapturePacket(settings, receiverB, buffer, &length);
                sendKafkaMessage(producer, topic, buffer, length);
                free(receiverA.rawData);
                free(receiverB.captureTime);
                packetB = open_memstream(&receiverB.rawData, &receiverB.rawDataLength);
            }
        }
    }


    // Block until the messages are all sent.
    fputs("Flushing final messages..", stderr);
    rd_kafka_flush(producer, 10 * 1000);

    if (rd_kafka_outq_len(producer) > 0) {
        fprintf(stderr, "%d message(s) were not delivered", rd_kafka_outq_len(producer));
        ret = 0;
    }

    //fprintf(stderr, "%d events were produced to topic %s.", message_count, topic);

    rd_kafka_destroy(producer);


    /* Close files */
    fclose(packetA);
    fclose(packetB);

    /* Free memory */
    free(receiverA.rawData);
    free(receiverA.captureTime);
    free(receiverB.rawData);
    free(receiverB.captureTime);


    /* Wait for final messages to be delivered or fail.
	 * rd_kafka_flush() is an abstraction over rd_kafka_poll() which
	 * waits for all messages to be delivered. */
	fprintf(stderr, "%% Flushing final messages..\n");
	rd_kafka_flush(producer, 10 * 1000 /* wait for max 10 seconds */);

	/* If the output queue is still not empty there is an issue
	 * with producing messages to the clusters. */
	if(rd_kafka_outq_len(producer) > 0) {
		fprintf(stderr, "%% %d message(s) were not delivered\n", rd_kafka_outq_len(producer));
        ret = 0;
    }

	/* Destroy the producer instance */
	rd_kafka_destroy(producer);

    return 1;
}