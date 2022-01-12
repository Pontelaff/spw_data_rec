#include <stdbool.h>
#include "star-api.h"
#include "star_utils.h"


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