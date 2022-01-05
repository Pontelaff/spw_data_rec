#include <stdio.h>
#include "spw_la_api.h"
#include "star-dundee_types.h"
#include "star-api.h"
#include "recordTraffic.h"
//#include "examples.h"
//#include "utilities.h"

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

void print_api_version()
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

void firmwareVersionExample()
{
    /* Initialise device count to 0 */
    U32 deviceCount = 0;
    /* Counter for loop */
    unsigned int index;
    /* Get device list */
    STAR_DEVICE_ID *devices = STAR_getDeviceList(&deviceCount);
    printf("Detected %d devices\n", deviceCount);


    if (deviceCount)
    {
        /* For all devices */
        for(index = 0; (devices != NULL) && (index < deviceCount); index++)
        {
            /* Get firmware version */
            STAR_VERSION_INFO * firmwareVersion = STAR_getDeviceFirmwareVersion(
                devices[index]);
            /* Print firmware version label */
            printf("Firmware version:");
            /* Print firmware version */
            printVersionInfo(firmwareVersion);
        }
    }
    else
    {
        //printf("No devices detected!\n");
    }
    /* Destroy device list */
    STAR_destroyDeviceList(devices);
}

void detectLinkAnalyser(STAR_LA_LinkAnalyser *linkAnalyser)
{
    /* Initialise device count to 0 */
    U32 deviceCount = 0;
    /* Counter for loop */
    unsigned int index;
    /* Get list of Link Analyser Mk3 devices */
    STAR_DEVICE_ID *devices = STAR_getDeviceListForTypes(&(linkAnalyser->linkAnalyserType), 1, &deviceCount);
    printf("Detected %d device(s) of 'Type STAR_DEVICE_LINK_ANALYSER_MK3'\n", deviceCount);


    if (deviceCount)
    {
        /* For all devices */
        for(index = 0; (devices != NULL) && (index < deviceCount); index++)
        {
            /* Print device name and serial number */
            printf("Device Name: %s\n", STAR_getDeviceName(devices[index]));
            printf("Serial Number: %s\n", STAR_getDeviceSerialNumber(devices[index]));
            /* Get firmware version */
            STAR_VERSION_INFO * firmwareVersion = STAR_getDeviceFirmwareVersion(
                devices[index]);
            /* Print firmware version label */
            printf("Firmware version:");
            /* Print firmware version */
            printVersionInfo(firmwareVersion);

            /** Store deviceID
            /* \todo check for specific serial number
             */
            linkAnalyser->deviceID = devices[index];
        }
    }
    else
    {
        linkAnalyser->deviceID = STAR_DEVICE_UNKNOWN;
    }

    /* Destroy device list */
    STAR_destroyDeviceList(devices);
}


int main()
{
    STAR_LA_LinkAnalyser linkAnalyser;
    linkAnalyser.linkAnalyserType = STAR_DEVICE_LINK_ANALYSER_MK3;

    print_api_version();

    detectLinkAnalyser(&linkAnalyser);

    //get_all_recorded_traffic_mk3(linkAnalyser);

    return 0;
}