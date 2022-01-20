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

struct settings {
    char  *args[2];     /* serial number & record duration */
    char  enNull;       /* enable recording NULLs */
    char  enFCT;        /* enable recording FCTs */
    char  enTimecode;   /* enable recording Timecodes */
    char  enNChar;      /* enable recording NChars */
    char  trigFCT;      /* enable trigger on FCT */
};

typedef struct settings Settings;


/* Program documentation */
static char doc[] = "Software for recording SpaceWire traffic\v"
                    "This program records specified SpaceWire Characters"
                    "for an adjustable amount of time using a STAR-Dundee"
                    "SpaceWire Link Analyzer Mk3 and decodes the FEE data-"
                    "packets used in the PLATO project.";

/* A description of the arguments we accept */
static char args_doc[] = "SERIAL_NO SECONDS";

/* Available options */
static struct argp_option options[] = {
    {"chars",   'c', "EN_CHARS",    0,  "Which charecters to record, given as 4-bit value (0-15)"},
    {"trigfct", 't', 0, 0, "Trigger on FCT instead of Timecode" },
    { 0 }
};

error_t parse_opt(int key, char *arg, struct argp_state *state);

static struct argp argp = { options, parse_opt, args_doc, doc };

//const char *argp_program_version = "spw_package_decode 0.1";
//const char *argp_program_bug_address = "<jonas.gesch@dlr.de>";
