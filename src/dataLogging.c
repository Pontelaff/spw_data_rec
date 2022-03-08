#include <stdio.h>
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


int printSettings(Settings settings)
{
    fputs("### Settings\n", stdout);
    /* Print duration of record in seconds */
    fprintf(stdout, "# Record Duration:\t%ss\n", settings.args[1]);

    /* Print trigger */
    fputs("# Trigger:\t\t\t", stdout);
    if (settings.trigFCT)
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
    if(strlen(moduleName) > 0)
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
    if(strlen(moduleAuthor) > 0)
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

    if(STAR_LA_GetBuildDate(linkAnalyser, &year, &month, &day, &hour, &minute))
        {
            /* Print build date of the device */
            charsWritten = fprintf(stdout, "# Build date:\t\t%d-%02d-%02d %02d:%02d\n", year, month, day, hour, minute);
        }
        else
        {
            /* Error */
            fputs("Unable to read build date\n", stderr);
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

char *timeToStr(struct timespec *timestamp)
{
  char time_str[127];
  double fractional_seconds;
  int milliseconds;
  struct tm tm;
  char *timeString;

  timeString = malloc(256);

  memset(&tm, 0, sizeof(struct tm));
  sprintf(time_str, "%ld UTC", timestamp->tv_sec);

  /* Convert our timespec into broken down time */
  strptime(time_str, "%s %U", &tm);

  /* Do the math to convert nanoseconds to integer microseconds */
  fractional_seconds = (double) timestamp->tv_nsec;
  fractional_seconds /= 1e3;
  //fractional_seconds = round(fractional_seconds);
  milliseconds = (int) fractional_seconds;

  /* Print date and time withouth microseconds */
  strftime(time_str, sizeof(time_str), "%Y-%m-%dT%H:%M:%S", &tm);

  /* add on the fractional seconds */
  sprintf(timeString, "%s.%d", time_str, milliseconds);

  return timeString;
}

int printHexdumpHeader(struct timespec *triggerTime, Settings settings, STAR_LA_LinkAnalyser linkAnalyser)
{
    char* triggerTimeStr = timeToStr(triggerTime);
    /* Print time, at which the trigger fired */
    fprintf(stdout, "# Trigger timestamp: %s\n", triggerTimeStr);
    free(triggerTimeStr);
    fputs("\n", stdout);

    /* Print configuration set by input arguments */
    if (!printSettings(settings))
    {
        fputs("Error while printing settings\n", stderr);
        return 0;
    }
    fputs("\n", stdout);

    /* Print information for the Link Analyser device and API */
    if (!LA_printInfo(linkAnalyser))
    {
        fputs("Error while printing Link Analyser info\n", stderr);
        return 0;
    }

    return 1;
}

void printHexdumpPacketHeader(STAR_LA_MK3_Traffic *pTraffic, U32 *index, const double *charCaptureClockPeriod, struct timespec *triggerTime)
{
    struct timespec packetTimestamp = *triggerTime;
    U32 cnt = 0;

    /* Convert time to microseconds */
    double timeUS = pTraffic[*index].time * *charCaptureClockPeriod;

    long seconds = (long)timeUS;
    long nanoSec = (long)((timeUS - seconds) * 1000000000);

    packetTimestamp.tv_nsec += nanoSec;
    packetTimestamp.tv_sec += seconds;
    if ( 1000000000 <= packetTimestamp.tv_nsec )
    {
        packetTimestamp.tv_nsec -= 1000000000;
        packetTimestamp.tv_sec += 1;
    }
    else if ( 0 > packetTimestamp.tv_nsec)
    {
        packetTimestamp.tv_nsec += 1000000000;
        packetTimestamp.tv_sec -= 1;
    }


    /* Print time */
    char* packetTimeStr = timeToStr(&packetTimestamp);
    /* Print time, at which the trigger fired */
    fprintf(stdout, "\n\n%s", packetTimeStr);
    free(packetTimeStr);
    fprintf(stdout, "\n%06X", 0);
    for (cnt = 0; cnt < 12; cnt++)
    {
        /* Skip every event that is not a header or data */
        while ( STAR_LA_TRAFFIC_TYPE_DATA != pTraffic[*index].linkBEvent.type
                && STAR_LA_TRAFFIC_TYPE_HEADER != pTraffic[*index].linkBEvent.type)
        {
            *index = *index + 1;
        }
        fprintf(stdout, " %02X", pTraffic[*index].linkBEvent.data);
        *index = *index + 1;
    }
    
}

void LA_MK3_printHexdump(STAR_LA_MK3_Traffic *pTraffic, const U32 *trafficCount, const double *charCaptureClockPeriod, struct timespec *triggerTime)
{
    /* Loop Counter */
    U32 i = 0;
    /* Loop Counter */
    U32 bytesWritten = 0;

    U32 headerPrinted = 0;

    for (i = 0; i < *trafficCount; i++)
    {

        /* Convert time to milliseconds */
        double timeInMilliSeconds = pTraffic[i].time * *charCaptureClockPeriod * 1000;
        /* Print events after triger */
        if (-PRE_TRIGGER_MS <= timeInMilliSeconds )
        {
            if (!headerPrinted && STAR_LA_TRAFFIC_TYPE_HEADER == pTraffic[i].linkBEvent.type)
            {
                printHexdumpPacketHeader(pTraffic, &i, charCaptureClockPeriod, triggerTime);
                bytesWritten = 12;
                headerPrinted = 1;
            }
            else if (headerPrinted && (STAR_LA_TRAFFIC_TYPE_EOP == pTraffic[i].linkBEvent.type || STAR_LA_TRAFFIC_TYPE_EEP == pTraffic[i].linkBEvent.type))
            {
                headerPrinted = 0;
            }
            else if (headerPrinted && STAR_LA_TRAFFIC_TYPE_DATA == pTraffic[i].linkBEvent.type)
            {
                if (0 == (bytesWritten-12) % 8)
                {
                    fprintf(stdout, "\n%06X", bytesWritten);
                }
                fprintf(stdout, " %02X", pTraffic[i].linkBEvent.data);
                bytesWritten++;
            }
        }
    }

    fputs("\n", stdout);

    return;
}


void LA_MK3_printRecordedTraffic(STAR_LA_MK3_Traffic *pTraffic, const U32 *trafficCount, const double *charCaptureClockPeriod)
{
    /* Loop Counter */
    U32 i = 0;

    fprintf(stdout, "Index\t\tTime\t\tEvent A Type\t\tError\t\tEvent B Type\t\tError\n");
    for (i = 0; i < *trafficCount; i++)
    {
        /* Print events after triger */
        if (-PRE_TRIGGER_MS <= pTraffic[i].time * *charCaptureClockPeriod * 1000 )
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