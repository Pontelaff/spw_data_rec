/**
 * @file main.c
 * @author Jonas Gesch (jonas.gesch@dlr.de)
 * @brief This program records specified SpaceWire Characters for an adjustable
 * amount of time using a STAR-Dundee SpaceWire Link Analyzer Mk3 and writes the
 * individual packets into a formatted hexdump, which can be imported into Wireshark.
 * @version 0.2.0
 * @date 2021-12-15
 * 
 */
#include <stdio.h>
#include <time.h>
#include "spw_la_api.h"
#include "LA_interface.h"
#include "dataLogging.h"

#define VERSION "v0.2.0"

static void printConfiguration(Settings config)
{
    /* Print software name, version and author*/
    fprintf(stderr, "+---------------------------+\n"
                    "| spw_package_decode %6s |\n"
                    "|  authored by Jonas Gesch  |\n"
                    "+---------------------------+\n", VERSION);    

    /* Print configuration */
    fprintf(stderr, "\nConfiguration:"
                    "\nSerial number = %s\n"
                    "Record for %s seconds\n"
                    "Display %dms before trigger\n"
                    "EnChars = %d, %d, %d, %d\n"
                    "Trigger = %s on receiver %c\n\n",
                    config.args[0], config.args[1], config.preTrigger,
                    config.enNull, config.enFCT, config.enTimecode, config.enNChar,
                    config.trigFCT ? "FCT" : "Timecode", config.recv ? 'B' : 'A');

                    return;
}

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
    config.recv = 1;
    config.preTrigger = 3000;
    config.version = VERSION;

    /* The Link Analyser in use */
    STAR_LA_LinkAnalyser linkAnalyser;
    linkAnalyser.linkAnalyserType = STAR_LA_LINK_ANALYSER_TYPE_MK3;

    /* Duration of data capture after trigger in seconds */
    double captureDuration = 0.0;

    /* Holds the traffic count */
    U32 trafficCount = 0;

    /* Holds the character capture clock period */
    double charCaptureClockPeriod = 0.0;

    /* The recorded traffic */
    STAR_LA_MK3_Traffic *pTraffic;

    /* Timestamp for when the Link Analyser device was triggered */
    struct timespec triggerTime;

    /* Parse command line arguments */
    argp_parse(&argp, argc, argv, 0, 0, &config);
    sscanf(config.args[1], "%lf", &captureDuration);

    /* Print config info to stderr */
    printConfiguration(config);

    /* Detect device  matching serial number */
    if (0 != LA_MK3_detectDevice(&linkAnalyser, config.args[0]))
    {
        /* Configure Link Analyser for recording */
        LA_configRecording(linkAnalyser, config);
        /* Record SpaceWire traffic */
        if (0 != LA_MK3_recordTraffic(linkAnalyser, &pTraffic, &trafficCount, &charCaptureClockPeriod, &captureDuration, &triggerTime))
        {
            if (!printHexdumpHeader(&triggerTime, config, linkAnalyser))
            {
                fputs("Printing hexdump aborted\n", stderr);
            }
            else
            {
                /* Print recorded traffic */
                LA_MK3_printHexdump(pTraffic, &trafficCount, &charCaptureClockPeriod, &triggerTime, config.preTrigger);
            }
            /* Free the traffic */
            STAR_LA_MK3_FreeRecordedTrafficMemory(pTraffic);
        }
    }

    fputs("\n", stderr);

    return 0;
}