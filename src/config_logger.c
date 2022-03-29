#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "arg_parser.h"
#include "spw_la_api.h"
#include "config_logger.h"

void printConfig(Settings settings)
{
    fputs("### Configuration\n", stdout);
    /* Print duration of record in seconds */
    fprintf(stdout, "# Record duration:     %ss\n", settings.args[1]);
    /* Print max displayed record duration before the trigger milliseconds */
    fprintf(stdout, "# PreTrig duration:    %dms\n", settings.preTrigger);

    /* Print trigger event*/
    fprintf(stdout, "# Trigger event:       %s\n", settings.trigFCT ? "FCT" : "Timecode");

    /* Print chars enabled for recording */
    fprintf(stdout, "# Enable NULLs:        %d\n", settings.enNull);
    fprintf(stdout, "# Enable FCTs:         %d\n", settings.enFCT);
    fprintf(stdout, "# Enable Timecodes:    %d\n", settings.enTimecode);
    fprintf(stdout, "# Enable NChars:       %d\n", settings.enNChar);

    return;
}

void LA_printApiVersion(void)
{
    int major, minor, edit, patch;
    /* Get the API version */
    STAR_LA_GetAPIVersion(&major, &minor, &edit, &patch);
    /* Display the API version */
    fprintf(stdout, "# API version:         v%d.%02d", major, minor);
    /* Print edit and patch level, if available */
    if (edit)
    {
        fprintf(stdout, " edit %d", edit);
    }
    if (patch)
    {
        fprintf(stdout, " patch level %d", patch);
    }

    /* New line */
    fputs("\n", stdout);
}

int LA_printDeviceVersion(STAR_LA_LinkAnalyser linkAnalyser)
{
    U8 major, minor;
    U16 edit, patch;
    /* Get the device version */
    if (!STAR_LA_GetDeviceVersion(linkAnalyser, &major, &minor, &edit, &patch))
    {
        fputs("Error, unable to get device version\n", stderr);
        return 0;
    }
    else
    {
        /* Display the device version */
        fprintf(stdout, "# Device version:      v%d.%02d", major, minor);
        /* Print edit and patch level, if available */
        if (edit)
        {
            fprintf(stdout, " edit %d", edit);
        }
        if (patch)
        {
            fprintf(stdout, " patch level %d", patch);
        }

        /* New line */
        fputs("\n", stdout);
    }

    return 1;
}

void printFirmwareVersion(STAR_VERSION_INFO *firmwareVersion)
{
    /* Get module name */
    char *moduleName = firmwareVersion->name;
    /* Get module author */
    char *moduleAuthor = firmwareVersion->author;
    /* Get major version number */
    U16 major = firmwareVersion->major;
    /* Get minor version number */
    U16 minor = firmwareVersion->minor;
    /* Get edit version number */
    U16 edit = firmwareVersion->edit;
    /* Get patch version number */
    U16 patch = firmwareVersion->patch;

    /* If module has a name */
    if (strlen(moduleName) > (size_t)0)
    {
        /* Print module name */
        fprintf(stdout, "%s", moduleName);
    }

    /* Print version information string */
    fprintf(stdout, "v%d.%02u", major, minor);
    /* Print edit and patch level, if available */
    if (edit)
    {
        fprintf(stdout, " edit %u", edit);
    }
    if (patch)
    {
        fprintf(stdout, " patch level %u", patch);
    }

    /* If module has an author */
    if (strlen(moduleAuthor) > (size_t)0)
    {
        /* Print module author */
        fprintf(stdout, "   Author: %s\n", moduleAuthor);
    }

    /* New line */
    fputs("\n", stdout);
}

int LA_printBuildDate(STAR_LA_LinkAnalyser linkAnalyser)
{
    /* Build date values */
    U8 year, month, day, hour, minute;

    /* Return value */
    int charsWritten = 0;

    if (!STAR_LA_GetBuildDate(linkAnalyser, &year, &month, &day, &hour, &minute))
    {
        /* Error */
        fputs("Unable to read build date\n", stderr);
    }
    else
    {
        /* Print build date of the device */
        charsWritten = fprintf(stdout, "# Build date:          %d-%02d-%02d %02d:%02d\n", year, month, day, hour, minute);
    }

    return charsWritten;
}

int LA_printInfo(STAR_LA_LinkAnalyser linkAnalyser)
{
    /* ID of the used Link Analyser device */
    STAR_DEVICE_ID deviceID = linkAnalyser.deviceID;
    /* Holds the firmware version of the used Link Analyser device */
    STAR_VERSION_INFO *firmware_version = NULL;

    /* Get the firmware version */
    firmware_version = STAR_getDeviceFirmwareVersion(deviceID);
    if (NULL == firmware_version)
    {
        /* Firmware version could not be read */
        return 0;
    }

    fputs("### Link Analyser\n", stdout);
    /* Print API version */
    LA_printApiVersion();
    /* Print device name and serial number */
    fprintf(stdout, "# Device name:         %s\n", STAR_getDeviceName(deviceID));
    fprintf(stdout, "# Serial number:       %s\n", STAR_getDeviceSerialNumber(deviceID));
    /* Print device version */
    if (!LA_printDeviceVersion(linkAnalyser))
    {
        return 0;
    }
    /* Print firmware version  */
    fputs("# Firmware version:    ", stdout);
    printFirmwareVersion(firmware_version);
    /* Print build date */
    if (!LA_printBuildDate(linkAnalyser))
    {
        return 0;
    }

    fputs("\n\n", stdout);

    return 1;
}

char *timeToStr(struct timespec *timestamp)
{
    double fractional_seconds;
    int microseconds;
    char *timeString = NULL;
    char buff[100];
    size_t streamSize = 0;
    FILE *timeStream = open_memstream(&timeString, &streamSize);

    /* Format date and time and save in buffer */
    strftime(buff, sizeof buff, "%FT%T", localtime(&timestamp->tv_sec));

    /* Adjust precision of fractional seconds */
    fractional_seconds = (double)timestamp->tv_nsec;
    fractional_seconds /= 1e3;
    //fractional_seconds = round(fractional_seconds);
    microseconds = (int)fractional_seconds;

    /* Combine timestring */
    fprintf(timeStream, "%s.%06d", buff, microseconds);
    fclose(timeStream);

    return timeString;
}

int printHexdumpHeader(struct timespec *triggerTime, Settings settings, STAR_LA_LinkAnalyser linkAnalyser)
{
    char *triggerTimeStr = timeToStr(triggerTime);

    /* Print time, at which the trigger fired */
    fprintf(stdout, "# Trigger timestamp:   %s\n", triggerTimeStr);
    free(triggerTimeStr);

    /* Print software version */
    fprintf(stdout, "# Software version:    spw_package_decode %s\n", settings.version);
    fputs("\n", stdout);

    /* Print configuration set by input arguments */
    printConfig(settings);
    fputs("\n", stdout);

    /* Print information for the Link Analyser device and API */
    if (!LA_printInfo(linkAnalyser))
    {
        fputs("Error while printing Link Analyser info\n", stderr);
        return 0;
    }

    return 1;
}
