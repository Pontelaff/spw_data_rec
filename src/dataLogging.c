#include <stdio.h>
#include "spw_la_api.h"
#include"arg_parser.h"
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


void LA_printApiVersion(void)
{
    int major, minor, edit, patch;
    /* Get the API version */
    STAR_LA_GetAPIVersion(&major, &minor, &edit, &patch);
    /* Display the API version */
    fprintf(stdout, "#LA API version:\t\tv%d.%02d", major, minor);
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
        fprintf(stdout, "#LA device version:\t\tv%d.%02d", major, minor);
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

    int charsWritten = 0;

    if(STAR_LA_GetBuildDate(linkAnalyser, &year, &month, &day, &hour, &minute))
        {
            /* Print build date of the device */
            charsWritten = fprintf(stdout, "#LA build date:\t\t\t%d-%02d-%02d %02d:%02d\n", year, month, day, hour, minute);
        }
        else
        {
            fputs("Unable to read build date\n", stderr);
        }

    return charsWritten;
}


int LA_printInfo(STAR_LA_LinkAnalyser linkAnalyser)
{
    STAR_DEVICE_ID deviceID = linkAnalyser.deviceID;
    STAR_VERSION_INFO* firmware_version = NULL;


    /* Get the firmware version */
    firmware_version = STAR_getDeviceFirmwareVersion(deviceID);
    if (NULL == firmware_version)
    {
        return 0;
    }

    /* Print API version */
    LA_printApiVersion();
    /* Print device name and serial number */
    fprintf(stdout, "#LA device name:\t\t%s\n", STAR_getDeviceName(deviceID));
    fprintf(stdout, "#LA serial number:\t\t%s\n", STAR_getDeviceSerialNumber(deviceID));
    /* Print device version */
    if(!LA_printDeviceVersion(linkAnalyser))
    {
        return 0;
    }
    /* Print firmware version  */
    fputs("#LA firmware version:\t", stdout);
    printFirmwareVersion(firmware_version);
    /* Print build date */
    if(!LA_printBuildDate(linkAnalyser))
    {
        return 0;
    }

    return 1;
}

void LA_MK3_printRecordedTraffic(STAR_LA_MK3_Traffic *pTraffic, const U32 *trafficCount, const double *charCaptureClockPeriod)
{
    /* Loop Counter */
    U32 i = 0;

    fprintf(stdout, "Index\t\tTime\t\tEvent A Type\t\tError\t\tEvent B Type\t\tError\n");
    for (i = 0; i < *trafficCount; i++)
    {
        /* Print events after triger */
        //if (-triggerDelay*1000 <= pTraffic[i].time * *charCaptureClockPeriod * 1000 )
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