#include <stdlib.h>
#include "arg_parser.h"


/* Parse a single option. */
error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
    Settings *config = state->input;

    switch (key)
    {
    // case 'd':
    //     config->device = arg;
    //     break;
    // case 't':
    //     config->time = atof(arg);
    //     break;
    case 'c':
        config->enNull      = (atoi(arg) >> 3) & 1;
        config->enFCT       = (atoi(arg) >> 2) & 1;
        config->enTimecode  = (atoi(arg) >> 1) & 1;
        config->enNChar     = (atoi(arg) >> 0) & 1;
        break;
    case 't':
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
