#include <stdio.h>
#include "spw_la_api.h"
#include "LA_interface.h"
#include "star_utils.h"

#define TRIG_DELAY 5.0

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


void LA_printApiVersion()
{
    int major, minor, edit, patch;
    /* Get the API version */
    STAR_LA_GetAPIVersion(&major, &minor, &edit, &patch);
    /* Display the API version */
    printf("SpaceWire Link Analyser API version v%d.%02d", major, minor);
    if (edit)
    {
        printf(" edit %d", edit);
    }
    if (patch)
    {
        printf(" patch level %d", patch);
    }
    puts("\n");
}


bool LA_MK3_detectDevice(STAR_LA_LinkAnalyser *linkAnalyser)
{
    bool success = FALSE;
    /* Initialise device count to 0 */
    U32 deviceCount = 0;
    /* Build date values */
    U8 year, month, day, hour, minute;
    /* Counter for loop */
    unsigned int index;
    /* Device type to detect */
    STAR_DEVICE_TYPE deviceType =  STAR_DEVICE_LINK_ANALYSER_MK3;
    /* Get list of Link Analyser Mk3 devices */
    STAR_DEVICE_ID *devices = STAR_getDeviceListForTypes(&deviceType, 1, &deviceCount);

    if (deviceCount)
    {
        printf("Detected %d device(s) of Type 'STAR_DEVICE_LINK_ANALYSER_MK3'\n", deviceCount);
        /* For all devices */
        for(index = 0; (devices != NULL) && (index < deviceCount); index++)
        {

            /** Store deviceID
            /* \todo check for specific serial number
             */
            linkAnalyser->deviceID = devices[index];
            /* Print device name, serial number, firmware version and device version */
            printDeviceInfo(devices[index]);
            LA_printDeviceVersion(*linkAnalyser);

            if(STAR_LA_GetBuildDate(*linkAnalyser, &year, &month, &day, &hour, &minute))
            {
                /* Print build date of the device */
                printf("Device build date: %d/%d/%02d @ %d:%02d\n", day, month, year, hour, minute);
            }
            else
            {
                printf("Unable to read build date\n");
            }
        }
        success = TRUE;
    }
    else
    {
        /* No Link Analyser Mk3 device detected.  */
        printf("No device of Type 'STAR_DEVICE_LINK_ANALYSER_MK3' have been detected\n");
    }

    /* Destroy device list */
    STAR_destroyDeviceList(devices);

    return success;
}


void LA_printDeviceVersion(STAR_LA_LinkAnalyser linkAnalyser)
{
    U8 major, minor;
    U16 edit, patch;
    /* Get the device version */
    if (!STAR_LA_GetDeviceVersion(linkAnalyser, &major, &minor, &edit, &patch))
    {
        puts("Error, unable to get device version");
    }
    else
    {
        /* Display the device version */
        printf("Device version: v%d.%02d", major, minor);
        if (edit)
        {
            printf(" edit %d", edit);
        }
        if (patch)
        {
            printf(" patch level %d", patch);
        }
        puts("");
    }
}


void LA_configRecording(STAR_LA_LinkAnalyser linkAnalyser)
{
    /* Trigger delay in seconds and clock cycles */
    double trigDelaySeconds = TRIG_DELAY;
    U32 trigDelayClkCycles = 0;

    /* System clock speed and capture clock reference period */
    U32 clkSpeed = 0;
    double captureClkRefPeriod = 0.0;

    /* Configure the device to record all characters except NULL */
    if (!STAR_LA_SetRecordedCharacters(linkAnalyser, 0, 1, 1, 1))
    {
        puts("Unable to enable recording all characters");
        return;
    }

    /* Disable recording just the packet header */
    if (!STAR_LA_SetRecordOnlyPacketHeader(linkAnalyser, 0))
    {
        puts("Unable to disable recording only packet header");
        return;
    }

    /* Set the first stage of the trigger sequence to fire on receipt of time-code comparator character on receiver A */
    if (!STAR_LA_SetTriggerSequence(linkAnalyser, 0, STAR_LA_TRIGGER_SEQ_SOURCE_RECEIVER_B, STAR_LA_TRIGGER_EVENT_TIMECODE, 1, 1))
    {
        /* Print error */
        puts("Failed to set first stage of trigger sequence");
    }
    else
    {
        /* Print success */
        puts("First stage of trigger sequence set to fire on receipt of a timecode on receiver B");
    }

    /* Get the system clock speed */
    if (!STAR_LA_GetSystemClockSpeed(linkAnalyser, &clkSpeed))
    {
        /* Print error */
        puts("Failed to get system clock speed");
    }
    /* Calculate the capture clock reference period */
    captureClkRefPeriod = 1.0 / clkSpeed;
    /* Calculate the trigger delay in clock cycles */
    trigDelayClkCycles = (trigDelaySeconds / captureClkRefPeriod) + 0.5;

    /* Set the trigger delay */
    if (!STAR_LA_SetTriggerDelay(linkAnalyser, trigDelayClkCycles))
    {
        printf("Unable to set the trigger delay to %lf\n seconds", trigDelaySeconds);
        return;
    }

    /* Set the amount of memory to be recorded after the delayed trigger to 0 */
    if (!STAR_LA_SetPostTriggerMemory(linkAnalyser, 0))
    {
        puts("Unable to set the size of post trigger memory");
        return;
    }

    /* Initialise to the waiting state */
    /* (in case the device was already recording) */
    if (!STAR_LA_InitialiseToWaiting(linkAnalyser))
    {
        puts("Unable to initialise to waiting");
        return;
    }
}


bool LA_MK3_recordTraffic(STAR_LA_LinkAnalyser linkAnalyser, STAR_LA_MK3_Traffic **ppTraffic, U32 *trafficCount, double *charCaptureClockPeriod)
{
    /* Start recording */
    if (!STAR_LA_StartRecording(linkAnalyser))
    {
        puts("Unable to start recording");
        return false;
    }

    puts("Recording, waiting on trigger...");
    /* Wait on the device triggering */
    if (!STAR_LA_WaitForTrigger(linkAnalyser))
    {
        puts("Unable to wait for the trigger");
        return false;
    }
    puts("Triggered, waiting on completion...");
    /* Wait on the recording to complete */
    if (!STAR_LA_WaitForComplete(linkAnalyser))
    {
        puts("Unable to wait for completion");
        return false;
    }
    puts("Completed!");
    /* Get the recorded traffic */
    *ppTraffic = STAR_LA_MK3_GetAllRecordedTraffic(linkAnalyser, trafficCount, charCaptureClockPeriod);

    if (!*ppTraffic)
    {
        puts("Error, unable to get all recorded traffic");
        return false;
    }
    else
    {
        return true;
    }
}


void LA_MK3_printRecordedTraffic(STAR_LA_MK3_Traffic *pTraffic, U32 *trafficCount, double *charCaptureClockPeriod)
{
    /* Loop Counter */
    U32 i = 0;

    printf("Index\t\tTime\t\tEvent A Type\t\tError\t\tEvent B Type\t\tError\n");
    for (i = 0; i < *trafficCount; i++)
    {
        /* Print events after triger */
        //if (-TRIG_DELAY*1000 <= pTraffic[i].time * *charCaptureClockPeriod * 1000 )
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
            printf("%d\t\t", i);
            /* Print time */
            printf( "%010.4fms\t", timeInMilliSeconds+TRIG_DELAY*1000);
            /* Print link A event type */
            printf("%s\t\t\t", linkAEventType);
            /* Print link A error flag */
            printf("%s\t\t", linkAError);
            /* Print link B event type */
            printf("%s\t\t", linkBEventType);
            /* Print link B error flag */
            printf("%s", linkBError);
            /* Line break */
            puts("");
        }
    }
}