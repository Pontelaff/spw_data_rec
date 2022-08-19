/**
 * @file main.c
 * @author Jonas Gesch (jonas.gesch@dlr.de)
 * @brief This program records specified SpaceWire characters for an adjustable
 * amount of time using a STAR-Dundee SpaceWire Link Analyzer Mk3 and writes the
 * individual packets into a formatted hexdump, which can be imported into Wireshark.
 * Alternatively the data can be saved in a raw event based format or archived to a
 * database using the Kafka messaging system.
 *
 * @version 0.4.1
 * @date 2021-12-15
 *
 */
#include <stdio.h>
#include <time.h>
#include <spw_la_api.h>
#include "arg_parser.h"
#include "LA_interface.h"
#include "data_logger.h"
#include "packet_archiver.h"

#define VERSION "v0.4.1"

const static char* flagToString(char flag)
{
    return flag ? "enabled" : "disabled";
}

static void printConfiguration(Settings config)
{
    /* Print software name, version and author*/
    fprintf(stderr,
            "+---------------------------+\n"
            "|    spw_data_rec %6s    |\n"
            "|  authored by Jonas Gesch  |\n"
            "+---------------------------+\n",
            VERSION);

    /* Print configuration */
    fprintf(stderr, "\nCapture Configuration:\n"
                    "LA serial number: %s\n"
                    "Record duration: %ss\n"
                    "Record duration before trigger: %dms\n"
                    "Record NULLs: %s\n"
                    "Record FCTs: %s\n"
                    "Record Timecodes: %s\n"
                    "Record NChars: %s\n"
                    "Trigger: %s on receiver %c\n"
                    "Capture log format: %s\n\n",
                    config.args[0], config.args[1], config.preTrigger,
                    flagToString(config.enNull), flagToString(config.enFCT),
                    flagToString(config.enTimecode), flagToString(config.enNChar),
                    config.trigFCT ? "FCT" : "Timecode", config.recv ? 'B' : 'A',
                    config.verbose ? "Event based" : "Hexdump");

    if (NULL != config.kafka_topic)
    {
        fprintf(stderr, "Kafka Configuration:\n"
                    "Topic: %s\n"
                    "Test ID: %s\n"
                    "Test version: %s\n"
                    "Interface ID In: %s\n"
                    "Interface ID Out: %s\n"
                    "ASW version: %s\n"
                    "DB version: %s\n\n",
                    config.kafka_topic, config.kafka_testId, config.kafka_testVersion,
                    config.kafka_interfaceIdIn, config.kafka_interfaceIdOut,
                    config.kafka_aswVersion, config.kafka_dbVersion);
    }

    return;
}

int main(int argc, char **argv)
{
    /* Contains settings needed
     to configure the Link Analyser */
    Settings config;

    /* Default values. */
    config.version = VERSION;
    config.enNull = 0;
    config.enFCT = 1;
    config.enTimecode = 1;
    config.enNChar = 1;
    config.trigFCT = 0;
    config.recv = 1;
    config.preTrigger = 3000;
    config.verbose = 0;
    config.kafka_topic = NULL;
    config.kafka_testId = NULL;
    config.kafka_testVersion = NULL;
    config.kafka_interfaceIdIn = NULL;
    config.kafka_interfaceIdOut = NULL;
    config.kafka_dbVersion = NULL;
    config.kafka_aswVersion = NULL;

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
    if(0 != argp_parse(&argp, argc, argv, 0, 0, &config))
    {
        fputs("Unable to parse arguments. Aborting program.\n\n", stderr);
        return 1;
    }
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
            /* Print captured traffic data */
            LA_MK3_printRecordedTraffic(linkAnalyser, pTraffic, config, &trafficCount, &charCaptureClockPeriod, &triggerTime);
            if (NULL != config.kafka_topic)
            {
                /* Archive traffic via kafka messaging system */
                LA_MK3_archiveCapturedPackets(config, pTraffic, &trafficCount, &charCaptureClockPeriod, &triggerTime, config.preTrigger);
            }
            /* Free the traffic */
            STAR_LA_MK3_FreeRecordedTrafficMemory(pTraffic);
        }
    }

    fputs("\n", stderr);

    return 0;
}