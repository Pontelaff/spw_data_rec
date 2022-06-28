#include <stdio.h>
#include <stdlib.h>
#include <spw_la_api.h>
#include "config_logger.h"
#include "data_logger.h"
#include "arg_parser.h"

char *GetEventTypeString(U8 trafficType)
{
    if (trafficType == STAR_LA_TRAFFIC_TYPE_HEADER)
    {
        return "Header";
    }
    else if (trafficType == STAR_LA_TRAFFIC_TYPE_DATA)
    {
        return "Data";
    }
    else if (trafficType == STAR_LA_TRAFFIC_TYPE_EOP)
    {
        return "EOP";
    }
    else if (trafficType == STAR_LA_TRAFFIC_TYPE_EEP)
    {
        return "EEP";
    }
    else if (trafficType == STAR_LA_TRAFFIC_TYPE_DATA_SEQ_ERROR)
    {
        return "Data seq error";
    }
    else if (trafficType == STAR_LA_TRAFFIC_TYPE_EOP_SEQ_ERROR)
    {
        return "EOP seq error";
    }
    else if (trafficType == STAR_LA_TRAFFIC_TYPE_EEP_SEQ_ERROR)
    {
        return "EEP seq error";
    }
    else if (trafficType == STAR_LA_TRAFFIC_TYPE_TIMECODE)
    {
        return "Time-code";
    }
    else if (trafficType == STAR_LA_TRAFFIC_TYPE_TIMECODE_SEQ_ERROR)
    {
        return "Time-code seq error";
    }
    else if (trafficType == STAR_LA_TRAFFIC_TYPE_FCT)
    {
        return "FCT";
    }
    else if (trafficType == STAR_LA_TRAFFIC_TYPE_ESCAPE)
    {
        return "Escape";
    }
    else if (trafficType == STAR_LA_TRAFFIC_TYPE_NULL)
    {
        return "Null";
    }
    else if (trafficType == STAR_LA_TRAFFIC_TYPE_PARITY_ERROR)
    {
        return "Parity Error";
    }
    else if (trafficType == STAR_LA_TRAFFIC_TYPE_ESCAPE_ESCAPE_ERROR)
    {
        return "Escape-escape error";
    }
    else if (trafficType == STAR_LA_TRAFFIC_TYPE_ESCAPE_EOP_ERROR)
    {
        return "Escape-EOP error";
    }
    else if (trafficType == STAR_LA_TRAFFIC_TYPE_ESCAPE_EEP_ERROR)
    {
        return "Escape-EEP error";
    }
    else if (trafficType == STAR_LA_TRAFFIC_TYPE_GOT_BIT)
    {
        return "Got Bit";
    }
    else if (trafficType == STAR_LA_TRAFFIC_TYPE_NO_CHARACTER)
    {
        return "No character";
    }
    else if (trafficType == STAR_LA_TRAFFIC_TYPE_DISCONNECT_ERROR)
    {
        return "Disconnect error";
    }
    /* Return error string, traffic type unknown */
    return "ERROR:Unknown";
}

static char *GetErrorString(U8 errors)
{
    if (STAR_LA_MK3_ParityError(errors))
    {
        return "Parity";
    }
    else if (STAR_LA_MK3_ReceiverCreditError(errors))
    {
        return "Rx Credit";
    }
    else if (STAR_LA_MK3_TransmitterCreditError(errors))
    {
        return "Tx Credit";
    }
    return "None";
}

int LA_MK3_getPacketTimestamp(const double *deltaToTrigger, struct timespec *triggerTime, char *timeString)
{
    int ret = 0;

    /* Time stamp for current packet */
    struct timespec packetTimestamp;
    U32 cnt = 0;

    /* Split delta into seconds and nanoseconds */
    long seconds = (long)*deltaToTrigger;
    long nanoSec = (long)((*deltaToTrigger - seconds) * 1000000000);

    /* Calculate packet timestamp by adding delta to trigger timestamp */
    packetTimestamp.tv_nsec = triggerTime->tv_nsec + nanoSec;
    packetTimestamp.tv_sec = triggerTime->tv_sec + seconds;
    if (1000000000 <= packetTimestamp.tv_nsec)
    {
        /* Adjust values, if nanoseconds >= 1 second */
        packetTimestamp.tv_nsec -= 1000000000;
        packetTimestamp.tv_sec += 1;
    }
    else if (0 > packetTimestamp.tv_nsec)
    {
        /* Adjust values, if nanoseconds are negative */
        packetTimestamp.tv_nsec += 1000000000;
        packetTimestamp.tv_sec -= 1;
    }

    /* Format packet timestamp */
    ret = timeToStr(&packetTimestamp, timeString);

    return ret;
}

unsigned int LA_MK3_printByte(struct dataPacket *packet, const double *deltaToTrigger, struct timespec *triggerTime)
{
    /* Total number of bytes in the packet */
    unsigned int packetBytes = 0;
    /* Timestamp of the current packet */
    char timestampStr[30];

    /* Start new packet with preceding timestamp, if header event is detected */
    if ((0 == packet->bytesReceived) && (STAR_LA_TRAFFIC_TYPE_HEADER == packet->event.type))
    {
        LA_MK3_getPacketTimestamp(deltaToTrigger, triggerTime, timestampStr);
        fprintf(packet->packetStream, "%c %s\n", packet->direction, timestampStr);
        fprintf(packet->packetStream, "%06X %02X", packet->bytesReceived, packet->event.data);
        packet->bytesReceived++;
    }
    /* Add byte to packet, if data event is detected */
    else if ((0 < packet->bytesReceived) && (STAR_LA_TRAFFIC_TYPE_DATA == packet->event.type)) // Also print header events?
    {
        if ((HEADER_BYTES <= packet->bytesReceived) && (0 == (packet->bytesReceived - HEADER_BYTES) % BYTES_PER_LINE))
        {
            /* Start new line with byte offset */
            fprintf(packet->packetStream, "\n%06X", packet->bytesReceived);
        }
        /* Print byte */
        fprintf(packet->packetStream, " %02X", packet->event.data);
        packet->bytesReceived++;
    }
    /* End packet and return number of bytes received, if EOP or EEP event is detected */
    else if (STAR_LA_TRAFFIC_TYPE_EOP == packet->event.type || STAR_LA_TRAFFIC_TYPE_EEP == packet->event.type)
    {
        packetBytes = (unsigned int)packet->bytesReceived;
        packet->bytesReceived = 0;
    }
    /* Print Timecode directly to hexdump */
    else if (STAR_LA_TRAFFIC_TYPE_TIMECODE == packet->event.type)
    {
        LA_MK3_getPacketTimestamp(deltaToTrigger, triggerTime, timestampStr);
        fprintf(stdout, "\n%c %s\n", packet->direction, timestampStr);
        fprintf(stdout, "%06X %02X\n", 0, packet->event.data);
    }

    return packetBytes;
}

void LA_MK3_printHexdumpData(STAR_LA_MK3_Traffic *pTraffic, const U32 *trafficCount, const double *charCaptureClockPeriod, struct timespec *triggerTime, const int preTrigger)
{
    /* Loop counter */
    U32 i = 0;
    /* Time difference of the current traffic event to the trigger in seconds */
    double deltaToTrigger = 0.0;

    /* String of packet received on receiver A */
    char *packetA = NULL;
    /* Size of packet string for receiver A */
    size_t packetSizeA = 0;
    /* Struct collecting events received on receiver A */
    struct dataPacket receiverA;
    receiverA.bytesReceived = 0;
    receiverA.direction = 'I';
    receiverA.packetStream = open_memstream(&packetA, &packetSizeA);

    /* String of packet received on receiver B */
    char *packetB = NULL;
    /* Size of packet string for receiver B */
    size_t packetSizeB = 0;
    /* Struct collecting events received on receiver B */
    struct dataPacket receiverB;
    receiverB.bytesReceived = 0;
    receiverB.direction = 'O';
    receiverB.packetStream = open_memstream(&packetB, &packetSizeB);

    for (i = 0; i < *trafficCount; i++)
    {
        deltaToTrigger = pTraffic[i].time * *charCaptureClockPeriod;
        /* Print packets, starting at set pre trigger duration */
        if (-preTrigger <= (deltaToTrigger * 1000.0))
        {
            receiverA.event = pTraffic[i].linkAEvent;
            receiverB.event = pTraffic[i].linkBEvent;

            /* Print packet from receiver A, if complete */
            if (0 != LA_MK3_printByte(&receiverA, &deltaToTrigger, triggerTime))
            {
                fclose(receiverA.packetStream);
                fprintf(stdout, "\n%s\n", packetA);
                free(packetA);
                receiverA.packetStream = open_memstream(&packetA, &packetSizeA);
            }

            /* Print packet from receiver B, if complete */
            if (0 != LA_MK3_printByte(&receiverB, &deltaToTrigger, triggerTime))
            {
                fclose(receiverB.packetStream);
                fprintf(stdout, "\n%s\n", packetB);
                free(packetB);
                receiverB.packetStream = open_memstream(&packetB, &packetSizeB);
            }
        }
    }

    /* Close files */
    fclose(receiverA.packetStream);
    fclose(receiverB.packetStream);

    /* Print incomplete packets */
    if (0 < packetSizeA)
    {
        fprintf(stdout, "%s\n", packetA);
        fputs("### Incomplete packet ###\n", stdout);
    }
    if (0 < packetSizeB)
    {
        fprintf(stdout, "%s\n", packetB);
        fputs("### Incomplete packet ###\n", stdout);
    }

    fputs("\n", stdout);
    fputs("Printing hexdump completed\n", stderr);

    /* Free memory */
    free(packetA);
    free(packetB);

    return;
}

int LA_MK3_printRecordedTraffic(STAR_LA_LinkAnalyser linkAnalyser, STAR_LA_MK3_Traffic *pTraffic, Settings settings, const U32 *trafficCount, const double *charCaptureClockPeriod, struct timespec *triggerTime)
{
    /* Return value */
    int success = 0;

    /* Print config header for hexdump */
    success = printConfigHeader(triggerTime, settings, linkAnalyser);

    if (0 == success)
    {
        fputs("\nPrinting capture log aborted\n", stderr);
    }
    else
    {
        fputs("\nPrinting capture log...\n", stderr);
        if (0 == settings.verbose)
        {
            /* Print recorded traffic data as hexdump */
            LA_MK3_printHexdumpData(pTraffic, trafficCount, charCaptureClockPeriod, triggerTime, settings.preTrigger);
        }
        else
        {
            /* Print event based log of captured data */
            LA_MK3_printEventCaptureLog(pTraffic, trafficCount, charCaptureClockPeriod, settings.preTrigger);
            success = 1;
        }
    }

    return success;
}

void LA_MK3_printEventCaptureLog(STAR_LA_MK3_Traffic *pTraffic, const U32 *trafficCount, const double *charCaptureClockPeriod, const int preTrigger)
{
    /* Loop counter */
    U32 i = 0;

    fprintf(stdout, "Index   Time            Event A Type        Event A Data    Error        Event B Type        Event B Data    Error\n");
    for (i = 0; i < *trafficCount; i++)
    {
        /* Print events after trigger */
        if (-preTrigger <= (pTraffic[i].time * *charCaptureClockPeriod * 1000))
        {
            /* Convert event types to strings */
            char *linkAEventType = GetEventTypeString(pTraffic[i].linkAEvent.type);
            char *linkBEventType = GetEventTypeString(pTraffic[i].linkBEvent.type);
            /* Get Event Data */
            U8 linkAEventData = pTraffic[i].linkAEvent.data;
            U8 linkBEventData = pTraffic[i].linkBEvent.data;
            /* Convert time to milliseconds */
            double timeInMilliSeconds = pTraffic[i].time * *charCaptureClockPeriod * 1000;
            /* Get error detected flags */
            char *linkAError = GetErrorString(pTraffic[i].linkAEvent.errors);
            char *linkBError = GetErrorString(pTraffic[i].linkBEvent.errors);
            /* Print index */
            fprintf(stdout, "%-8d", i);
            /* Print time */
            fprintf(stdout, "%010.4fms    ", timeInMilliSeconds);
            /* Print link A event type */
            fprintf(stdout, "%-20s", linkAEventType);
            /* Print link A event data */
            fprintf(stdout, "%02X              ", linkAEventData);
            /* Print link A error flag */
            fprintf(stdout, "%-13s", linkAError);
            /* Print link B event type */
            fprintf(stdout, "%-20s", linkBEventType);
            /* Print link B event data */
            fprintf(stdout, "%02X              ", linkBEventData);
            /* Print link B error flag */
            fprintf(stdout, "%s\n", linkBError);
        }
    }

    fputs("Printing event based capture log completed\n", stderr);

    return;
}
