#include <stdio.h>
#include <stdbool.h>
#include "spw_la_api.h"
#include "star-dundee_types.h"
#include "star-api.h"
#include "recordTraffic.h"

/**
 * @brief Prints the author, name and version info of a module, if available.
 *
 * @param versionInfo Struct which stores the module info.
 */
void printVersionInfo(STAR_VERSION_INFO * versionInfo)
{
    /* Get module name */
    char * moduleName = versionInfo->name;

    /* Get module author */
    char * moduleAuthor = versionInfo->author;

    /* Get major version number */
    U16 major = versionInfo->major;

    /* Get minor version number */
    U16 minor = versionInfo->minor;

    /* Get edit version number */
    U16 edit = versionInfo->edit;

    /* Get patch version number */
    U16 patch = versionInfo->patch;

    /* Print version information string */
    printf("%s %u.%u.%u.%u\n", moduleName, major, minor, edit, patch);

    /* If module has a name */
    if(strlen(moduleName) > 0)
    {
        /* Print module name */
        printf("   Module name: %s\n", moduleName);
    }

    /* If module has an author */
    if(strlen(moduleAuthor) > 0)
    {
        /* Print module author */
        printf(" Module author: %s\n", moduleAuthor);
    }

    /* Print module author */
    printf("Module version: v%u.%u", major, minor);

    /* If there is an edit version number */
    if (edit)
    {
        /* Print edit version number */
        printf("(%u)", edit);
    }

    /* If there is a patch version number */
    if (patch)
    {
        /* Print patch version number */
        printf("p%u", patch);
    }

    /* Take new line */
    printf("\n");
}

/**
 * @brief Prints the SpaceWire Link Analyser API version.
 *
 */
void LA_printApiVersion()
{
    int major, minor, edit, patch;
    /* Get the API version */
    STAR_LA_GetAPIVersion(&major, &minor, &edit, &patch);
    /* Display the API version */
    printf("SpaceWire Link Analyser API version v%d.%02d", major, minor);
    if (edit)
    {
        printf(" edit %d", edit);
    }
    if (patch)
    {
        printf(" patch level %d", patch);
    }
    puts("\n");
}

/**
 * @brief Prints device name, serial number and firmware version of the specified device.
 *
 * @param deviceID The STAR-Dundee device of which the information should be printed.
 */
void printDeviceInfo(STAR_DEVICE_ID deviceID)
{
    /* Print device name and serial number */
    printf("Device Name: %s\n", STAR_getDeviceName(deviceID));
    printf("Serial Number: %s\n", STAR_getDeviceSerialNumber(deviceID));
    /* Get firmware version */
    STAR_VERSION_INFO * firmwareVersion = STAR_getDeviceFirmwareVersion(deviceID);
    /* Print firmware version label */
    printf("Firmware version:");
    /* Print firmware version */
    printVersionInfo(firmwareVersion);
}

/**
 * @brief Prints the version information for the specified Link Analyser device.
 *
 * @param linkAnalyser The link analyser device to print the version information for.
 */
void LA_printDeviceVersion(STAR_LA_LinkAnalyser linkAnalyser)
{
    U8 major, minor;
    U16 edit, patch;
    /* Get the device version */
    if (!STAR_LA_GetDeviceVersion(linkAnalyser, &major, &minor, &edit, &patch))
    {
        puts("Error, unable to get device version");
    }
    else
    {
        /* Display the device version */
        printf("Device version: v%d.%02d", major, minor);
        if (edit)
        {
            printf(" edit %d", edit);
        }
        if (patch)
        {
            printf(" patch level %d", patch);
        }
        puts("");
    }
}

/**
 * @brief Scans for Link Analyser Mk3 Devices and saves the deviceID of the last found LA device.
 *
 * @param linkAnalyser struct, where the deviceID of the detected LA is stored
 * @return true when at least one LA has been detected
 * @return false  when no devices have been detected
 */
bool LA_MK3_detectDevice(STAR_LA_LinkAnalyser *linkAnalyser)
{
    bool success = FALSE;
    /* Initialise device count to 0 */
    U32 deviceCount = 0;
    /* Build date values */
    U8 year, month, day, hour, minute;
    /* Counter for loop */
    unsigned int index;
    /* Device type to detect */
    STAR_DEVICE_TYPE deviceType =  STAR_DEVICE_LINK_ANALYSER_MK3;
    /* Get list of Link Analyser Mk3 devices */
    STAR_DEVICE_ID *devices = STAR_getDeviceListForTypes(&deviceType, 1, &deviceCount);

    if (deviceCount)
    {
        printf("Detected %d device(s) of 'Type STAR_DEVICE_LINK_ANALYSER_MK3'\n", deviceCount);
        /* For all devices */
        for(index = 0; (devices != NULL) && (index < deviceCount); index++)
        {

            /** Store deviceID
            /* \todo check for specific serial number
             */
            linkAnalyser->deviceID = devices[index];
            /* Print device name, serial number, firmware version and device version */
            printDeviceInfo(devices[index]);
            LA_printDeviceVersion(*linkAnalyser);

            if(STAR_LA_GetBuildDate(*linkAnalyser, &year, &month, &day, &hour, &minute))
            {
                /* Print build date of the device */
                printf("Device build date: %d/%d/%02d @ %d:%02d\n", day, month, year, hour, minute);
            }
            else
            {
                printf("Unable to read build date\n");
            }
        }
        success = TRUE;
    }
    else
    {
        /* No Link Analyser Mk3 device detected.  */
        printf("No device of 'Type STAR_DEVICE_LINK_ANALYSER_MK3' have been detected\n");
    }

    /* Destroy device list */
    STAR_destroyDeviceList(devices);

    return success;
}

int main()
{
    /* The Link Analyser in use */
    STAR_LA_LinkAnalyser linkAnalyser;
    linkAnalyser.linkAnalyserType = STAR_LA_LINK_ANALYSER_TYPE_MK3;

    /* Holds the traffic count */
    U32 trafficCount = 0;

    /* Holds the character capture clock period */
    double charCaptureClockPeriod = 0;

    /* The recorded traffic */
    STAR_LA_MK3_Traffic *pTraffic;

    LA_printApiVersion();

    if (TRUE == LA_MK3_detectDevice(&linkAnalyser))
    {
        /* Configure Link Analyser for recording */
        LA_configRecording(linkAnalyser);
        /* Record SpaceWire traffic */
        if(TRUE == LA_MK3_recordTraffic(linkAnalyser, &pTraffic, &trafficCount, &charCaptureClockPeriod))
        {
            /* Print recorded traffic */
            LA_MK3_printRecordedTraffic(pTraffic, &trafficCount, &charCaptureClockPeriod);
            /* Free the traffic */
            STAR_LA_MK3_FreeRecordedTrafficMemory(pTraffic);
        }
    }

    return 0;
}