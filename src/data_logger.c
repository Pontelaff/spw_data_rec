#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "spw_la_api.h"
#include "config_logger.h"
#include "data_logger.h"

static char *GetEventTypeString(U8 trafficType)
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

void LA_MK3_printHexdumpPacketHeader(STAR_LA_MK3_Traffic *pTraffic, U32 *index, const double *deltaToTrigger, struct timespec *triggerTime)
{
    /* Determines if header is read from receiver A (0) or B (1) */
    char receiver = -1;

    /* Event of the selected receiver */
    STAR_LA_MK3_Event trafficEvent;

    /* Time stamp for current packet */
    struct timespec packetTimestamp = *triggerTime;
    U32 cnt = 0;

    /* Split delta into seconds and nanoseconds */
    long seconds = (long)*deltaToTrigger;
    long nanoSec = (long)((*deltaToTrigger - seconds) * 1000000000);

    /* Determine receiver side */
    if (STAR_LA_TRAFFIC_TYPE_HEADER == pTraffic[*index].linkAEvent.type)
    {
        receiver = 0;
    }
    else if (STAR_LA_TRAFFIC_TYPE_HEADER == pTraffic[*index].linkBEvent.type)
    {
        receiver = 1;
    }

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

    /* Print packet timestamp */
    char *packetTimeStr = timeToStr(&packetTimestamp);
    fprintf(stdout, "\n\n%c %s", receiver ? 'O' : 'I', packetTimeStr);
    free(packetTimeStr);

    /* Start packet at byte index 0 */
    fputs("\n000000", stdout);
    /* Print header data */
    for (cnt = 0; cnt < 12; cnt++)
    {
        trafficEvent = receiver ? pTraffic[*index].linkBEvent : pTraffic[*index].linkAEvent;

        while ((STAR_LA_TRAFFIC_TYPE_DATA != trafficEvent.type) && (STAR_LA_TRAFFIC_TYPE_HEADER != trafficEvent.type))
        {
            /* Skip every event that is not a header or data */
            *index = *index + 1;
            trafficEvent = receiver ? pTraffic[*index].linkBEvent : pTraffic[*index].linkAEvent;
        }
        /* Print header byte */
        fprintf(stdout, " %02X", trafficEvent.data);
        *index = *index + 1;
    }

    return;
}

STAR_LA_TRAFFIC_TYPE LA_MK3_getEventType(STAR_LA_MK3_Traffic traffic)
{
    if ( (STAR_LA_TRAFFIC_TYPE_HEADER == traffic.linkAEvent.type)
        || (STAR_LA_TRAFFIC_TYPE_HEADER == traffic.linkBEvent.type) )
    {
        return STAR_LA_TRAFFIC_TYPE_HEADER;
    }
    else if ( (STAR_LA_TRAFFIC_TYPE_DATA == traffic.linkAEvent.type)
            || (STAR_LA_TRAFFIC_TYPE_DATA == traffic.linkBEvent.type) )
    {
        return STAR_LA_TRAFFIC_TYPE_DATA;
    }
    else if ( (STAR_LA_TRAFFIC_TYPE_EOP == traffic.linkAEvent.type)
            || (STAR_LA_TRAFFIC_TYPE_EOP == traffic.linkBEvent.type)
            || (STAR_LA_TRAFFIC_TYPE_EEP == traffic.linkAEvent.type)
            || (STAR_LA_TRAFFIC_TYPE_EEP == traffic.linkBEvent.type) )
    {
        return STAR_LA_TRAFFIC_TYPE_EOP;
    }

    return STAR_LA_TRAFFIC_TYPE_NULL;
}

char *LA_MK3_getPacketTimestamp(const double *deltaToTrigger, struct timespec *triggerTime)
{
    /* Time stamp for current packet */
    struct timespec packetTimestamp = *triggerTime;
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

    /* Print packet timestamp */
    char *packetTimeStr = timeToStr(&packetTimestamp);
    //fprintf(stdout, "\n\n%c %s", receiver ? 'O' : 'I', packetTimeStr);
    //free(packetTimeStr);

    return packetTimeStr;
}

unsigned int LA_MK3_printByte(struct dataPacket packet, const double *deltaToTrigger, struct timespec *triggerTime)
{
    /* Total number of bytes in the packet */
    unsigned int packetBytes = 0;

    /* Start new packet with preceeding timestamp, if header event is detected */
    if ((0 == packet.bytesReceived) && (STAR_LA_TRAFFIC_TYPE_HEADER == packet.event.type))
    {
        char *timestampStr = LA_MK3_getPacketTimestamp(deltaToTrigger, triggerTime);
        fprintf(packet.packetStream, "%c %s\n", packet.direction, timestampStr);
        fprintf(packet.packetStream, "%06X %02X", packet.bytesReceived, packet.event.data);
        packet.bytesReceived++;
        free(timestampStr);
    }
    /* Add byte to packet, if data event is detected */
    else if (0 < packet.bytesReceived && (STAR_LA_TRAFFIC_TYPE_DATA == packet.event.type))          // Also print header events?
    {
        if ( (HEADER_BYTES <= packet.bytesReceived) && (0 == (packet.bytesReceived - HEADER_BYTES) % BYTES_PER_LINE) )
        {
            /* Start new line with byte offset */
            fprintf(packet.packetStream, "\n%06X", packet.bytesReceived);
        }
        /* Print byte */
        fprintf(packet.packetStream, " %02X", packet.event.data);
        packet.bytesReceived++;
    }
    /* End packet and return number of bytes received, if EOP or EEP event is detected */
    else if (STAR_LA_TRAFFIC_TYPE_EOP == packet.event.type || STAR_LA_TRAFFIC_TYPE_EEP == packet.event.type)
    {
        packetBytes = (unsigned int)packet.bytesReceived;
        packet.bytesReceived = 0;
    }

    return packetBytes;
}

void LA_MK3_printHexdump(STAR_LA_MK3_Traffic *pTraffic, const U32 *trafficCount, const double *charCaptureClockPeriod, struct timespec *triggerTime, const int preTrigger)
{
    /* Loop counter */
    U32 i = 0;

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
        /* Time difference of the current traffic event to the trigger in seconds */
        double deltaToTrigger = pTraffic[i].time * *charCaptureClockPeriod;
        /* Print packets, starting at set pre trigger duration */
        if (-preTrigger <= (deltaToTrigger * 1000.0))
        {
            receiverA.event = pTraffic[i].linkAEvent;
            receiverB.event = pTraffic[i].linkBEvent;

            /* Print packet from reveiver A, if complete */
            if (0 != LA_MK3_printByte(receiverA, &deltaToTrigger, triggerTime))
            {
                fclose(receiverA.packetStream);
                fprintf(stdout, "%s\n\n", packetA);
                free(packetA);
                receiverA.packetStream = open_memstream(&packetA, &packetSizeA);
            }

            /* Print packet from reveiver B, if complete */
            if (0 != LA_MK3_printByte(receiverB, &deltaToTrigger, triggerTime))
            {
                fclose(receiverB.packetStream);
                fprintf(stdout, "%s\n\n", packetB);
                free(packetB);
                receiverB.packetStream = open_memstream(&packetB, &packetSizeB);
            }
        }
    }

    /* Print incomplete packets */
    fclose(receiverA.packetStream);
    if (0 < packetSizeA)
    {
        fprintf(stdout, "%s\n", packetA);
        fputs("### Incomplete packet ###\n", stdout);
        free(packetA);
    }

    fclose(receiverB.packetStream);
    if (0 < packetSizeB)
    {
        fprintf(stdout, "%s\n", packetB);
        fputs("### Incomplete packet ###\n", stdout);
        free(packetB);
    }

    fputs("\n", stdout);

    return;
}

void old_LA_MK3_printHexdump(STAR_LA_MK3_Traffic *pTraffic, const U32 *trafficCount, const double *charCaptureClockPeriod, struct timespec *triggerTime, const int preTrigger)
{
    /* Loop counter */
    U32 i = 0;
    /* Amount of bytes written to hexfile */
    U32 bytesWritten = 0;
    /* A non-zero value, if the header for the
       current packet has already been printed */
    U32 headerPrinted = 0;
    /* Payload data byte */
    U8 payloadData = 0;

    for (i = 0; i < *trafficCount; i++)
    {
        /* Time difference of the current traffic event to the trigger in seconds */
        double deltaToTrigger = pTraffic[i].time * *charCaptureClockPeriod;
        /* Print packets, starting at set pre trigger duration */
        if (-preTrigger <= (deltaToTrigger * 1000.0))
        {
            if (!headerPrinted && (STAR_LA_TRAFFIC_TYPE_HEADER == LA_MK3_getEventType(pTraffic[i])))
            {
                if ((*trafficCount - i) > HEADER_BYTES)
                {
                    /* Print header */
                    LA_MK3_printHexdumpPacketHeader(pTraffic, &i, &deltaToTrigger, triggerTime);
                    bytesWritten = HEADER_BYTES;
                    headerPrinted = 1;
                }
            }
            else if (headerPrinted && (STAR_LA_TRAFFIC_TYPE_EOP == LA_MK3_getEventType(pTraffic[i])))
            {
                /* End of packet */
                headerPrinted = 0;
            }
            else if (headerPrinted && (STAR_LA_TRAFFIC_TYPE_DATA == LA_MK3_getEventType(pTraffic[i])))
            {
                if (0 == (bytesWritten - HEADER_BYTES) % BYTES_PER_LINE)
                {
                    /* Start new line with byte offset */
                    fprintf(stdout, "\n%06X", bytesWritten);
                }
                /* Get data byte */
                if (STAR_LA_TRAFFIC_TYPE_DATA == pTraffic[i].linkAEvent.type)
                {
                    payloadData = pTraffic[i].linkAEvent.data;
                }
                else if (STAR_LA_TRAFFIC_TYPE_DATA == pTraffic[i].linkBEvent.type)
                {
                    payloadData = pTraffic[i].linkBEvent.data;
                }
                /* Print byte */
                fprintf(stdout, " %02X", payloadData);
                bytesWritten++;
            }
        }
    }

    fputs("\n", stdout);

    return;
}

void LA_MK3_printRecordedTraffic(STAR_LA_MK3_Traffic *pTraffic, const U32 *trafficCount, const double *charCaptureClockPeriod, const int preTrigger)
{
    /* Loop counter */
    U32 i = 0;

    fprintf(stdout, "Index\t\tTime\t\tEvent A Type\t\tError\t\tEvent B Type\t\tError\n");
    for (i = 0; i < *trafficCount; i++)
    {
        /* Print events after triger */
        if (-preTrigger <= (pTraffic[i].time * *charCaptureClockPeriod * 1000))
        {
            /* Convert event types to strings */
            char *linkAEventType = GetEventTypeString(pTraffic[i].linkAEvent.type);
            char *linkBEventType = GetEventTypeString(pTraffic[i].linkBEvent.type);
            /* Convert time to milliseconds */
            double timeInMilliSeconds = pTraffic[i].time * *charCaptureClockPeriod * 1000;
            /* Get error detected flags */
            char *linkAError = GetErrorString(pTraffic[i].linkAEvent.errors);
            char *linkBError = GetErrorString(pTraffic[i].linkBEvent.errors);
            /* Print index */
            fprintf(stdout, "%d\t\t", i);
            /* Print time */
            fprintf(stdout, "%010.4fms\t", timeInMilliSeconds);
            /* Print link A event type */
            fprintf(stdout, "%s\t\t\t", linkAEventType);
            /* Print link A error flag */
            fprintf(stdout, "%s\t\t", linkAError);
            /* Print link B event type */
            fprintf(stdout, "%s\t\t", linkBEventType);
            /* Print link B error flag */
            fprintf(stdout, "%s\n", linkBError);
        }
    }
}