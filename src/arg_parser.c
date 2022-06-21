#include <stdlib.h>
#include <string.h>
#include "arg_parser.h"

static int setArchiveSettings(char **str, char *delim, char **setting)
{
    /* Return value */
    int ret = 0;

    if (*str != NULL)
    {
        *setting = *str;
        *str = strtok(NULL, delim);
        ret = 1;
    }

    return ret;
}

static int parseArchiveOptions(char *arg, Settings *config)
{
    /* String delimiter */
    char delim[] = " ";
    /* Separate individual tokens */
	char *ptr = strtok(arg, delim);
    /* Loop counter */
    int counter = 0;
    /* Buffer tor write excess parameters into */
    char excessBuffer[32] = "";

    char** kafka_config[KAFKA_ARGS] = {
        &config->kafka_topic,
        &config->kafka_testId,
        &config->kafka_testVersion,
        &config->kafka_interfaceIdIn,
        &config->kafka_interfaceIdOut,
        &config->kafka_dbVersion,
        &config->kafka_aswVersion
    };

    for (counter = 0; counter < KAFKA_ARGS; counter++){
        if (0 == setArchiveSettings(&ptr, delim, kafka_config[counter]))
        {
            return counter;
        }
    }

    /* Check for excess parameters */
    while (0 != setArchiveSettings(&ptr, delim, (char**)&excessBuffer))
    {
        counter++;
    }

    return counter;
}

error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure */
    Settings *config = state->input;
    /* Number of parsed arguments for archiving via kafka */
    int parsedKafkaArgs = 0;

    switch (key)
    {
    case 'a':
        /* Reading arguments for archiving of captured data */
        parsedKafkaArgs = parseArchiveOptions(arg, config);
        if(KAFKA_ARGS != parsedKafkaArgs)
        {
            fprintf(stderr, "\nWrong number of arguments for 'archive' option.\n"
                    "%d argument(s) parsed. Should be %d.\n", parsedKafkaArgs, KAFKA_ARGS);
            return ARGP_KEY_ERROR;
        }
        break;

    case 'c':
        /* Set enable flags from individual bits of the 4-bit input */
        config->enNull      = ((char)atoi(arg) >> (char)3) & (char)1;
        config->enFCT       = ((char)atoi(arg) >> (char)2) & (char)1;
        config->enTimecode  = ((char)atoi(arg) >> (char)1) & (char)1;
        config->enNChar     = ((char)atoi(arg) >> (char)0) & (char)1;
        break;

    case 'f':
        /* Enable triggering on FCT */
        config->trigFCT = 1;
        break;

    case 'r':
        /* Set receiver for trigger */
        if (!strcmp("A", arg) || !strcmp("a", arg))
        {
            config->recv = 0;
        }
        else if (!strcmp("B", arg) || !strcmp("b", arg))
        {
            config->recv = 1;
        }
        else
        {
            fputs("\nWrong input for trigger source. Using default receiver B\n", stderr);
        }
        break;

    case 'p':
        /* Set pre trigger duration */
        config->preTrigger = atoi(arg);
        break;

    case 'v':
        /* Enable verbose capture logs */
        config->verbose = 1;
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
