#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "spw_la_api.h"
#include "arg_parser.h"
#include "dataLogging.h"


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


void printSettings(Settings settings)
{
    fputs("### Settings\n", stdout);
    /* Print duration of record in seconds */
    fprintf(stdout, "# Record Duration:\t%ss\n", settings.args[1]);
    /* Print max displayed record duration before the trigger milliseconds */
    fprintf(stdout, "# PreTrig Duration:\t%dms\n", settings.preTrigger);

    /* Print trigger */
    fputs("# Trigger:\t\t\t", stdout);
    if (1 == settings.trigFCT)
    {
        fputs("FCT\n", stdout);
    }
    else
    {
        fputs("Timecode\n", stdout);
    }

    /* Print chars enabled for recording */
    fprintf(stdout, "# Enable NULLs:\t\t%d\n", settings.enNull);
    fprintf(stdout, "# Enable FCTs:\t\t%d\n", settings.enFCT);
    fprintf(stdout, "# Enable Timecodes:\t%d\n", settings.enTimecode);
    fprintf(stdout, "# Enable NChars:\t%d\n", settings.enNChar);

    return;
}


void LA_printApiVersion(void)
{
    int major, minor, edit, patch;
    /* Get the API version */
    STAR_LA_GetAPIVersion(&major, &minor, &edit, &patch);
    /* Display the API version */
    fprintf(stdout, "# API version:\t\tv%d.%02d", major, minor);
    /* Print edit and patch level, if available */
    if (edit)
    {
        fprintf(stdout, " edit %d", edit);
    }
    if (patch)
    {
        fprintf(stdout, " patch level %d", patch);
    }

    /* New line */
    fputs("\n", stdout);
}


int LA_printDeviceVersion(STAR_LA_LinkAnalyser linkAnalyser)
{
    U8 major, minor;
    U16 edit, patch;
    /* Get the device version */
    if (!STAR_LA_GetDeviceVersion(linkAnalyser, &major, &minor, &edit, &patch))
    {
        fputs("Error, unable to get device version\n", stderr);
        return 0;
    }
    else
    {
        /* Display the device version */
        fprintf(stdout, "# Device version:\tv%d.%02d", major, minor);
        /* Print edit and patch level, if available */
        if (edit)
        {
            fprintf(stdout, " edit %d", edit);
        }
        if (patch)
        {
            fprintf(stdout, " patch level %d", patch);
        }

        /* New line */
        fputs("\n", stdout);
    }

    return 1;
}


void printFirmwareVersion(STAR_VERSION_INFO * firmwareVersion)
{
    /* Get module name */
    char * moduleName = firmwareVersion->name;
    /* Get module author */
    char * moduleAuthor = firmwareVersion->author;
    /* Get major version number */
    U16 major = firmwareVersion->major;
    /* Get minor version number */
    U16 minor = firmwareVersion->minor;
    /* Get edit version number */
    U16 edit = firmwareVersion->edit;
    /* Get patch version number */
    U16 patch = firmwareVersion->patch;


    /* If module has a name */
    if(strlen(moduleName) > (size_t)0)
    {
        /* Print module name */
        fprintf(stdout, "%s", moduleName);
    }

    /* Print version information string */
    fprintf(stdout, "v%d.%02u", major, minor);
    /* Print edit and patch level, if available */
    if (edit)
    {
        fprintf(stdout, " edit %u", edit);
    }
    if (patch)
    {
        fprintf(stdout, " patch level %u", patch);
    }

    /* If module has an author */
    if(strlen(moduleAuthor) > (size_t)0)
    {
        /* Print module author */
        fprintf(stdout, "   Author: %s\n", moduleAuthor);
    }

    /* New line */
    fputs("\n", stdout);
}


int LA_printBuildDate(STAR_LA_LinkAnalyser linkAnalyser)
{
    /* Build date values */
    U8 year, month, day, hour, minute;

    /* Return value */
    int charsWritten = 0;

    if(!STAR_LA_GetBuildDate(linkAnalyser, &year, &month, &day, &hour, &minute))
        {
            /* Error */
            fputs("Unable to read build date\n", stderr);
        }
        else
        {
            /* Print build date of the device */
            charsWritten = fprintf(stdout, "# Build date:\t\t%d-%02d-%02d %02d:%02d\n", year, month, day, hour, minute);
        }

    return charsWritten;
}


int LA_printInfo(STAR_LA_LinkAnalyser linkAnalyser)
{
    /* ID of the used Link Analyser device */
    STAR_DEVICE_ID deviceID = linkAnalyser.deviceID;
    /* Holds the firmware version of the used Link Analyser device */
    STAR_VERSION_INFO* firmware_version = NULL;


    /* Get the firmware version */
    firmware_version = STAR_getDeviceFirmwareVersion(deviceID);
    if (NULL == firmware_version)
    {
        /* Firmware version could not be read */
        return 0;
    }

    fputs("### Link Analyser\n", stdout);
    /* Print API version */
    LA_printApiVersion();
    /* Print device name and serial number */
    fprintf(stdout, "# Device name:\t\t%s\n", STAR_getDeviceName(deviceID));
    fprintf(stdout, "# Serial number:\t%s\n", STAR_getDeviceSerialNumber(deviceID));
    /* Print device version */
    if(!LA_printDeviceVersion(linkAnalyser))
    {
        return 0;
    }
    /* Print firmware version  */
    fputs("# Firmware version:\t", stdout);
    printFirmwareVersion(firmware_version);
    /* Print build date */
    if(!LA_printBuildDate(linkAnalyser))
    {
        return 0;
    }

    return 1;
}

static char *timeToStr(struct timespec *timestamp)
{
    double fractional_seconds;
    int microseconds;
    char *timeString = NULL;
    char buff[100];
    size_t streamSize = 0;
    FILE *timeStream = open_memstream(&timeString, &streamSize);

    /* Format date and time and save in buffer */
    strftime(buff, sizeof buff, "%FT%T", localtime(&timestamp->tv_sec));

    /* Adjust precision of fractional seconds */
    fractional_seconds = (double)timestamp->tv_nsec;
    fractional_seconds /= 1e3;
    //fractional_seconds = round(fractional_seconds);
    microseconds = (int)fractional_seconds;

    /* Combine timestring */
    fprintf(timeStream, "%s.%06d\n", buff, microseconds);
    fclose(timeStream);

    return timeString;
}

int printHexdumpHeader(struct timespec *triggerTime, Settings settings, STAR_LA_LinkAnalyser linkAnalyser)
{
    char *triggerTimeStr = timeToStr(triggerTime);
    /* Print time, at which the trigger fired */
    fprintf(stdout, "# Trigger timestamp: %s\n", triggerTimeStr);
    free(triggerTimeStr);
    fputs("\n", stdout);

    /* Print configuration set by input arguments */
    printSettings(settings);
    fputs("\n", stdout);

    /* Print information for the Link Analyser device and API */
    if (!LA_printInfo(linkAnalyser))
    {
        fputs("Error while printing Link Analyser info\n", stderr);
        return 0;
    }

    return 1;
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
    if ( 1000000000 <= packetTimestamp.tv_nsec )
    {
        /* Adjust values, if nanoseconds >= 1 second */
        packetTimestamp.tv_nsec -= 1000000000;
        packetTimestamp.tv_sec += 1;
    }
    else if ( 0 > packetTimestamp.tv_nsec)
    {
        /* Adjust values, if nanoseconds are negative */
        packetTimestamp.tv_nsec += 1000000000;
        packetTimestamp.tv_sec -= 1;
    }

    /* Print packet timestamp */
    char* packetTimeStr = timeToStr(&packetTimestamp);
    fprintf(stdout, "\n\n%c %s", receiver ? 'O' : 'I', packetTimeStr);
    free(packetTimeStr);

    /* Start packet at byte index 0 */
    fputs("\n000000", stdout);
    /* Print header data */
    for (cnt = 0; cnt < 12; cnt++)
    {
        trafficEvent = receiver ? pTraffic[*index].linkBEvent : pTraffic[*index].linkAEvent;

        while ( (STAR_LA_TRAFFIC_TYPE_DATA != trafficEvent.type)
                && (STAR_LA_TRAFFIC_TYPE_HEADER != trafficEvent.type) )
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

void LA_MK3_printHexdump(STAR_LA_MK3_Traffic *pTraffic, const U32 *trafficCount, const double *charCaptureClockPeriod, struct timespec *triggerTime, const int preTrigger)
{
    /* Loop counter */
    U32 i = 0;
    /* Amount of bytes written to hexfile */
    U32 bytesWritten = 0;
    /* A non-zero value, if the header for the
       current package has already been printed */
    U32 headerPrinted = 0;
    /* Payload data byte */
    U8 payloadData = 0;

    for (i = 0; i < *trafficCount; i++)
    {
        /* Time difference of the current traffic event to the trigger in seconds */
        double deltaToTrigger = pTraffic[i].time * *charCaptureClockPeriod;
        /* Print packets, starting at set pre trigger duration */
        if ( -preTrigger <= (deltaToTrigger * 1000.0) )
        {
            if ( !headerPrinted && (STAR_LA_TRAFFIC_TYPE_HEADER == LA_MK3_getEventType(pTraffic[i])) )
            {
                if ( (*trafficCount - i) > HEADER_BYTES )
                {
                    /* Print header */
                    LA_MK3_printHexdumpPacketHeader(pTraffic, &i, &deltaToTrigger, triggerTime);
                    bytesWritten = HEADER_BYTES;
                    headerPrinted = 1;
                }
            }
            else if ( headerPrinted && ( STAR_LA_TRAFFIC_TYPE_EOP == LA_MK3_getEventType(pTraffic[i])) )
            {
                /* End of packet */
                headerPrinted = 0;
            }
            else if ( headerPrinted && (STAR_LA_TRAFFIC_TYPE_DATA == LA_MK3_getEventType(pTraffic[i])) )
            {
                if (0 == (bytesWritten-HEADER_BYTES) % BYTES_PER_LINE)
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
        if ( -preTrigger <= (pTraffic[i].time * *charCaptureClockPeriod * 1000) )
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