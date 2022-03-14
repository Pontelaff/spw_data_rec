/**
 * @file arg_parser.h
 * @author Jonas Gesch (jonas.gesch@dlr.de)
 * @brief Contains functions for parsuing command line argument
 * @version 0.1
 * @date 2022-01-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <argp.h>

/* Saves configuration arcording to input arguments */
struct settings {
    char  *args[2];     /* serial number & record duration */
    char  enNull;       /* enable recording NULLs */
    char  enFCT;        /* enable recording FCTs */
    char  enTimecode;   /* enable recording Timecodes */
    char  enNChar;      /* enable recording NChars */
    char  trigFCT;      /* enable trigger on FCT */
    char  recv;         /* receiver to trigger on (A=0, B=1) */
};

typedef struct settings Settings;


/* Program documentation */
static const char doc[] = "Software for recording SpaceWire traffic\v"
                    "This program records specified SpaceWire Characters"
                    "for an adjustable amount of time using a STAR-Dundee"
                    "SpaceWire Link Analyzer Mk3 and decodes the FEE data-"
                    "packets used in the PLATO project.";

/* A description of the arguments we accept */
static const char args_doc[] = "SERIAL_NO SECONDS";

/* Available options */
static struct argp_option options[] = {
    {"chars",   'c', "EN_CHARS",    0,  "Which characters to record, given as 4-bit value (0-15)"},
    {"trigfct", 'f', 0, 0, "Trigger on FCT instead of Timecode" },
    {"receiver", 'r', "RECV", 0, "Which receiver to set the trigger for (A/B)"},
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

//const char *argp_program_version = "spw_package_decode 0.1";
//const char *argp_program_bug_address = "<jonas.gesch@dlr.de>";
