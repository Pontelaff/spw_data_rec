/**
 * @file arg_parser.h
 * @author Jonas Gesch (jonas.gesch@dlr.de)
 * @brief Contains functions for parsing command line argument
 * @version 0.4.0
 * @date 2022-01-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <argp.h>

#define KAFKA_ARGS 7

/* Saves configuration according to input arguments */
typedef struct settings {
    char *args[2];              /* Serial number & record duration */
    char *version;              /* Version of this software */
    char  enNull;               /* Enable recording NULLs */
    char  enFCT;                /* Enable recording FCTs */
    char  enTimecode;           /* Enable recording Timecodes */
    char  enNChar;              /* Enable recording NChars */
    char  trigFCT;              /* Enable trigger on FCT */
    char  recv;                 /* Receiver to trigger on (A=0, B=1) */
    int   preTrigger;           /* Maximum displayed record duration in ms before the trigger */
    char  verbose;              /* Print readable event based capture logs */
    char *kafka_topic;          /* Kafka topic to archive data to */
	char *kafka_testId;         /* String of the current test ID */
	char *kafka_testVersion;    /* String of the current test version */
    char *kafka_interfaceIdIn;  /* Name of the interface on receiver A */
    char *kafka_interfaceIdOut;  /* Name of the interface on receiver B */
	char *kafka_dbVersion;      /* String of the current database version */
	char *kafka_aswVersion;      /* String of the current database version */
} Settings;


/* Program documentation */
static const char doc[] = "Software for recording SpaceWire traffic\v"
                    "This program records specified SpaceWire Characters "
                    "for an adjustable amount of time using a STAR-Dundee "
                    "SpaceWire Link Analyzer Mk3 and writes the individual "
                    "packets into a formatted hexdump, which can be imported "
                    "into Wireshark and archived to a database via the Kafka "
                    "messaging system. Additionally it is also possible to "
                    "log the individual events occurring on both receivers "
                    "of the Link Analyser.";

/* A description of the arguments we accept */
static const char args_doc[] = "SERIAL_NO SECONDS";

/* Available options */
static struct argp_option options[] = {
    {"chars",   'c', "EN_CHARS",    0,  "Which characters to record, given as 4-bit value (0-15)"},
    {"trigfct", 'f', 0, 0, "Trigger on FCT instead of Timecode" },
    {"receiver", 'r', "RECV", 0, "Which receiver to set the trigger for (A/B)"},
    {"pretrigger", 'p', "MILLIS", 0, "Maximum record duration in milliseconds to display"
                                    " BEFORE the device was triggered"},
    {"verbose", 'v', 0, 0, "Write readable event based capture logs instead of packet based hexdumps"},
    {"archive", 'a', "'TOPIC TEST_ID TEST_VERS IF_ID_IN IF_ID_OUT DB_VERS ASW_VERS'", 0, "Archive the captured data to a Kafka TOPIC"},
    { 0 }
};

/**
 * @brief Parses a single command line argument
 *
 * @param key An integer specifying which option this is (taken
             from the KEY field in each struct argp_option), or
             ARGP_KEY_ARG specifying a non-option argument, or
             ARGP_KEY_END meaning that all arguments have been parsed
 * @param arg For an option KEY, the string value of its argument, or
             NULL if it has none
 * @param state A pointer to a struct argp_state, containing various
             useful information about the parsing state; used here
             are the INPUT field, which reflects the INPUT argument to
             argp_parse, and the ARG_NUM field, which is the number of
             the current non-option argument being parsed
 * @return either 0, meaning success, ARGP_ERR_UNKNOWN, meaning the given
             KEY wasn’t recognized, or an errno value indicating some other
             error
 */
error_t parse_opt(int key, char *arg, struct argp_state *state);

/* The argp parser */
static struct argp argp = { options, parse_opt, args_doc, doc };

//const char *argp_program_version = "spw_data_rec 0.1";
//const char *argp_program_bug_address = "<jonas.gesch@dlr.de>";
