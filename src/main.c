#include <stdio.h>
#include "spw_la_api.h"
#include "LA_interface.h"


int main(int argc, char **argv)
{
    /* Contains settings needed
     to configure the Link Analyser */
    Settings config;

    /* Default values. */
    config.enNull = 0;
    config.enFCT = 1;
    config.enTimecode = 1;
    config.enNChar = 1;
    config.trigFCT = 0;

    /* Parse command line arguments */
    argp_parse(&argp, argc, argv, 0, 0, &config);

    /* Print configuration */
    printf("serial number = %s\nrecord for %s seconds\n"
           "enChars = %d, %d, %d, %d\ntrigger = %s\n",
           config.args[0], config.args[1],
           config.enNull, config.enFCT, config.enTimecode, config.enNChar,
           config.trigFCT ? "FCT" : "Timecode");

    /* The Link Analyser in use */
    STAR_LA_LinkAnalyser linkAnalyser;
    linkAnalyser.linkAnalyserType = STAR_LA_LINK_ANALYSER_TYPE_MK3;

    /* Duration of data capture after trigger in seconds */
    double captureDuration = 0.0;
    sscanf(config.args[1], "%lf", &captureDuration);

    /* Holds the traffic count */
    U32 trafficCount = 0;

    /* Holds the character capture clock period */
    double charCaptureClockPeriod = 0.0;

    /* The recorded traffic */
    STAR_LA_MK3_Traffic *pTraffic;

    LA_printApiVersion();

    /* Detect device  matching serial number */
    if (TRUE == LA_MK3_detectDevice(&linkAnalyser, config.args[0]))
    {
        /* Configure Link Analyser for recording */
        LA_configRecording(linkAnalyser, config);
        /* Record SpaceWire traffic */
        if (TRUE == LA_MK3_recordTraffic(linkAnalyser, &pTraffic, &trafficCount, &charCaptureClockPeriod, &captureDuration))
        {
            /* Print recorded traffic */
            LA_MK3_printRecordedTraffic(pTraffic, &trafficCount, &charCaptureClockPeriod);
            /* Free the traffic */
            STAR_LA_MK3_FreeRecordedTrafficMemory(pTraffic);
        }
    }

    return 0;
}