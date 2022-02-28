#include <stdlib.h>
#include "arg_parser.h"


error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure */
    Settings *config = state->input;

    switch (key)
    {
    case 'c':
        /* Set enable flags from individual bits of the 4-bit input */
        config->enNull      = ((char)atoi(arg) >> (char)3) & (char)1;
        config->enFCT       = ((char)atoi(arg) >> (char)2) & (char)1;
        config->enTimecode  = ((char)atoi(arg) >> (char)1) & (char)1;
        config->enNChar     = ((char)atoi(arg) >> (char)0) & (char)1;
        break;

    case 't':
        /* Enable triggering on FCT */
        config->trigFCT = 1;
        break;

    case ARGP_KEY_ARG:
        if (state->arg_num >= 2)
            /* Too many arguments. */
            argp_usage(state);
        config->args[state->arg_num] = arg;
        break;

    case ARGP_KEY_END:
        if (state->arg_num < 2)
            /* Not enough arguments. */
            argp_usage(state);
        break;

    default:
        return ARGP_ERR_UNKNOWN;
    }

    return 0;
}
