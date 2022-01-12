#include <stdio.h>
#include "spw_la_api.h"
//#include "star-dundee_types.h"
#include "LA_interface.h"


int main()
{
    /* The Link Analyser in use */
    STAR_LA_LinkAnalyser linkAnalyser;
    linkAnalyser.linkAnalyserType = STAR_LA_LINK_ANALYSER_TYPE_MK3;

    /* Holds the traffic count */
    U32 trafficCount = 0;

    /* Holds the character capture clock period */
    double charCaptureClockPeriod = 0;

    /* The recorded traffic */
    STAR_LA_MK3_Traffic *pTraffic;

    LA_printApiVersion();

    if (TRUE == LA_MK3_detectDevice(&linkAnalyser))
    {
        /* Configure Link Analyser for recording */
        LA_configRecording(linkAnalyser);
        /* Record SpaceWire traffic */
        if(TRUE == LA_MK3_recordTraffic(linkAnalyser, &pTraffic, &trafficCount, &charCaptureClockPeriod))
        {
            /* Print recorded traffic */
            LA_MK3_printRecordedTraffic(pTraffic, &trafficCount, &charCaptureClockPeriod);
            /* Free the traffic */
            STAR_LA_MK3_FreeRecordedTrafficMemory(pTraffic);
        }
    }

    return 0;
}