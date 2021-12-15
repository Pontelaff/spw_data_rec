#include <stdio.h>
#include "recordTraffic.h"

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
void get_all_recorded_traffic_mk3(STAR_LA_LinkAnalyser linkAnalyser)
{
    /* Holds the traffic count */
    U32 trafficCount = 0;
    /* Holds the character capture clock period */
    double charCaptureClockPeriod = 0;
    /* The recorded traffic */
    STAR_LA_MK3_Traffic *pTraffic;
    /* Counter */
    U32 i = 0;

    /* Configure the device to record all characters */
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

    /* Set the amount of memory to be recorded after the trigger to be
    /* the same as the memory recorded before the trigger */
    if (!STAR_LA_SetPostTriggerMemory(linkAnalyser, STAR_LA_GetMaximumRecordedEvents(linkAnalyser) / 2))
    {
        puts("Unable to set the size of post trigger memory");
        return;
    }

    /* Set the trigger delay to be 0 */
    if (!STAR_LA_SetTriggerDelay(linkAnalyser, 0))
    {
        puts("Unable to set the trigger delay to 0");
        return;
    }

    /* Initialise to the waiting state */
    /* (in case the device was already recording) */
    if (!STAR_LA_InitialiseToWaiting(linkAnalyser))
    {
        puts("Unable to initialise to waiting");
        return;
    }

    /* Start recording */
    if (!STAR_LA_StartRecording(linkAnalyser))
    {
        puts("Unable to start recording");
        return;
    }

    puts("Recording, waiting on trigger...");
    /* Wait on the device triggering */
    if (!STAR_LA_WaitForTrigger(linkAnalyser))
    {
        puts("Unable to wait for the trigger");
        return;
    }

    puts("Triggered, waiting on completion...");
    /* Wait on the recording to complete */
    if (!STAR_LA_WaitForComplete(linkAnalyser))
    {
        puts("Unable to wait for completion");
        return;
    }
    puts("Completed!");
    /* Get the recorded traffic */
    pTraffic = STAR_LA_MK3_GetAllRecordedTraffic(linkAnalyser,
                                                 &trafficCount,
                                                 &charCaptureClockPeriod);
    if (!pTraffic)
    {
        puts("Error, unable to get all recorded traffic");
    }
    else
    {
         printf("Index\t\tTime\t\tEvent A Type\t\tError\t\tEvent B Type\t\tError\n");
         for (i = 0; i < trafficCount; i++)
         {
            /* Convert event types to strings */
            char *linkAEventType = GetEventTypeString(pTraffic[i].linkAEvent.type);
            char *linkBEventType = GetEventTypeString(pTraffic[i].linkBEvent.type);
            /* Convert time to milliseconds */
            double timeInMilliSeconds = pTraffic[i].time
                                        * charCaptureClockPeriod * 1000;
            /* Get error detected flags */
            char *linkAError = GetErrorString(pTraffic[i].linkAEvent.errors);
            char *linkBError = GetErrorString(pTraffic[i].linkBEvent.errors);
            /* Print index */
            printf("%d\t\t", i);
            /* Print time */
            printf( "%010.4fms\t", timeInMilliSeconds);
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
         /* Free the traffic */
         STAR_LA_MK3_FreeRecordedTrafficMemory(pTraffic);
    }
}