/**
 * @file packet_archiver.h
 * @author Jonas Gesch (jonas.gesch@dlr.de)
 * @brief Contains functions for archiving captured data in a cassandra database
 *      using the Kafka messaging system.
 * @version 0.4.0
 * @date 2022-04-27
 */

#include <librdkafka/rdkafka.h>
#include <spw_la_api.h>

/* Maximum buffer available for kafka messages */
#define BUF_SIZE 1000050

/* Size of the static part of the kafka message in bytes,
consisting of the size ofJSON keys, uuid and timestamp */
#define STATIC_MESSAGE_LENGTH 207

typedef struct settings Settings;

typedef struct packetInfo
{
	char    captureTime[30];	/* Packet timestamp */
	char 	*interfaceId;	/* Receiver on which the packet was received */
	char 	*rawData;    		/* String of raw packet data */
	size_t 	rawDataLength;		/* Size of raw data string */
} PacketInfo;

/**
 * @brief This function creates data packets from recorded events and archives them in the database as individual kafka messages.
 *
 * @param settings The settings of this application containing static information to be sent via kafka.
 * @param pTraffic The recorded events.
 * @param trafficCount The amount of recorded events.
 * @param charCaptureClockPeriod The character capture clock period.
 * @param triggerTime The timestamp at which the Link Analyser was triggered.
 * @param preTrigger The maximum duration for which packets received BEFORE the trigger are archived.
 * @return A non-zero value on success.
 */
int32_t LA_MK3_archiveCapturedPackets(Settings settings, STAR_LA_MK3_Traffic *pTraffic, const U32 *trafficCount, const double *charCaptureClockPeriod, struct timespec *triggerTime, const int preTrigger);