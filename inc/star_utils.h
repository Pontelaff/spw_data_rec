/**
 * @brief Prints the author, name and version info of a module, if available.
 *
 * @param versionInfo Struct which stores the module info.
 */
void printVersionInfo(STAR_VERSION_INFO * versionInfo);

/**
 * @brief Prints device name, serial number and firmware version of the specified device.
 *
 * @param deviceID The STAR-Dundee device of which the information should be printed.
 */
void printDeviceInfo(STAR_DEVICE_ID deviceID);