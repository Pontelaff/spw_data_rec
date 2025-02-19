#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "arg_parser.h"
#include "LA_interface.h"

int LA_MK3_detectDevice(STAR_LA_LinkAnalyser *linkAnalyser, const char *serialNumber)
{
    int success = 0;
    /* Initialise device count to 0 */
    U32 deviceCount = 0;
    /* Device type to detect */
    STAR_DEVICE_TYPE deviceType = STAR_DEVICE_LINK_ANALYSER_MK3;
    /* Get list of Link Analyser Mk3 devices */
    STAR_DEVICE_ID *devices = STAR_getDeviceListForTypes(&deviceType, 1, &deviceCount);

    if (deviceCount)
    {
        fprintf(stderr, "Detected %d device(s) of Type 'STAR_DEVICE_LINK_ANALYSER_MK3'\n", deviceCount);
        /* Counter for loop */
        unsigned int index;
        /* For all devices */
        for (index = 0; (devices != NULL) && (index < deviceCount); index++)
        {
            if (!(strcmp(serialNumber, STAR_getDeviceSerialNumber(devices[index]))))
            {
                /* Store deviceID */
                linkAnalyser->deviceID = devices[index];
                success = 1;
            }
            else
            {
                fputs("Unable to match serial number\n", stderr);
            }
        }
    }
    else
    {
        /* No Link Analyser Mk3 device detected.  */
        fprintf(stderr, "No device of Type 'STAR_DEVICE_LINK_ANALYSER_MK3' has been detected\n");
    }

    /* Destroy device list */
    STAR_destroyDeviceList(devices);

    return success;
}

void LA_configRecording(STAR_LA_LinkAnalyser linkAnalyser, Settings config)
{
    /* Trigger event type */
    STAR_LA_TRIGGER_EVENT trigEvent = 0;

    /* Receiver to trigger on */
    STAR_LA_TRIGGER_SEQ_SOURCE trigSource = 0;

    if (config.trigFCT)
    {
        /* Trigger on FCT */
        trigEvent = STAR_LA_TRIGGER_EVENT_FCT;
    }
    else
    {
        /* Trigger on Timecode */
        trigEvent = STAR_LA_TRIGGER_EVENT_TIMECODE;
    }

    if (!config.recv)
    {
        /* Trigger on receiver A */
        trigSource = STAR_LA_TRIGGER_SEQ_SOURCE_RECEIVER_A;
    }
    else
    {
        /* Trigger on Receiver B */
        trigSource = STAR_LA_TRIGGER_SEQ_SOURCE_RECEIVER_B;
    }

    /* Configure the device to record the specified characters */
    if (!STAR_LA_SetRecordedCharacters(linkAnalyser, config.enNull, config.enFCT, config.enTimecode, config.enNChar))
    {
        fputs("Unable to enable recording of selected characters\n", stderr);
        return;
    }

    /* Disable recording just the packet header */
    if (!STAR_LA_SetRecordOnlyPacketHeader(linkAnalyser, 0))
    {
        fputs("Unable to disable recording only packet header\n", stderr);
        return;
    }

    /* Set the first stage of the trigger sequence to fire on receipt of time-code or FCT event on receiver A */
    if (!STAR_LA_SetTriggerSequence(linkAnalyser, 0, trigSource, trigEvent, 1, 1))
    {
        /* Print error */
        fputs("Failed to set first stage of trigger sequence\n", stderr);
    }
    else
    {
        /* Print success */
        fputs("First stage of trigger sequence has been set\n", stderr);
    }

    /* Set the trigger delay to 0 */
    if (!STAR_LA_SetTriggerDelay(linkAnalyser, 0))
    {
        fputs("Unable to set the trigger delay\n", stderr);
        return;
    }

    /* Set the amount of events to be recorded after the trigger to maximum */
    if (!STAR_LA_SetPostTriggerMemory(linkAnalyser, STAR_LA_GetMaximumRecordedEvents(linkAnalyser)))
    {
        fputs("Unable to set the size of post trigger memory\n", stderr);
        return;
    }

    /* Initialise to the waiting state */
    /* (in case the device was already recording) */
    if (!STAR_LA_InitialiseToWaiting(linkAnalyser))
    {
        fputs("Unable to initialise to waiting\n", stderr);
        return;
    }
}

int LA_MK3_recordTraffic(STAR_LA_LinkAnalyser linkAnalyser, STAR_LA_MK3_Traffic **ppTraffic, U32 *trafficCount,
                         double *charCaptureClockPeriod, const double *captureDuration, struct timespec *triggerTime)
{
    /* Holds the trigger state */
    STAR_LA_TRIGGERSTATE triggerState = STAR_LA_TRIGGERSTATE_WAITING;

    /* Integer part of capture duration in seconds */
    unsigned int captureDurationS = (unsigned int)*captureDuration;
    /* Decimal part of capture duration in microseconds */
    __useconds_t captureDurationUS = (__useconds_t)((*captureDuration - (double)captureDurationS) * 1000000.0);

    /* Start recording */
    if (!STAR_LA_StartRecording(linkAnalyser))
    {
        fputs("Unable to start recording\n", stderr);
        return 0;
    }
    fputs("Recording, waiting on trigger...\n", stderr);

    /* Wait on the device triggering */
    do
    {
        if (!STAR_LA_GetTriggerState(linkAnalyser, &triggerState))
        {
            fputs("Unable to get trigger state\n", stderr);
            return 0;
        }
    } while (STAR_LA_TRIGGERSTATE_TRIGGERED != triggerState);
    /* Save timestamp */
    if (clock_gettime(CLOCK_REALTIME, triggerTime))
    {
        fputs("error clock_gettime\n", stderr);
    }
    fprintf(stderr, "Triggered, continue recording for %.3f seconds\n", *captureDuration);

    /* Delay for specified capture duration */
    if (0 < captureDurationS)
    {
        if (0 != sleep(captureDurationS))
        {
            fprintf(stderr, "Unable to delay for %u seconds", captureDurationS);
            return 0;
        }
    }
    if (0 < captureDurationUS)
    {
        if (0 != usleep(captureDurationUS))
        {
            fprintf(stderr, "Unable to delay for %ld microseconds", captureDurationUS);
            return 0;
        }
    }

    /* Force trigger */
    if (!STAR_LA_ForceTrigger(linkAnalyser))
    {
        /* Print error */
        fputs("Unable to force trigger\n", stderr);
    }
    else
    {
        /* Print success */
        fputs("Recording completed\n", stderr);
    }

    /* Get the recorded traffic */
    *ppTraffic = STAR_LA_MK3_GetAllRecordedTraffic(linkAnalyser, trafficCount, charCaptureClockPeriod);

    if (!*ppTraffic)
    {
        fputs("Error, unable to get all recorded traffic\n", stderr);
        return 0;
    }

    return 1;
}
