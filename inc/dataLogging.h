/**
 * @file dataLogging.h
 * @author Jonas Gesch (jonas.gesch@dlr.de)
 * @brief Contains functions for logging the recorded traffic as a formatted packet based hexdump
 * @version 0.1
 * @date 2022-02-28
 *
 * @copyright Copyright (c) 2022
 *
 */




/**
 * @brief Prints all available information for a Link Analyzer device.
 * 
 * @param linkAnalyser The Link Analyser defice to print the information for.
 * @return A non-zero integer on success. 
 */
int LA_printInfo(STAR_LA_LinkAnalyser linkAnalyser);

/**
 * @brief Prints the SpaceWire Link Analyser API version.
 *
 */
void LA_printApiVersion(void);

/**
 * @brief Prints the version information for the specified Link Analyser device.
 *
 * @param linkAnalyser The link analyser device to print the version information for.
 * 
 * @return A non-zero integer on success.
 */
int LA_printDeviceVersion(STAR_LA_LinkAnalyser linkAnalyser);


/**
 * @brief Prints the name, version info and author of a module, if available.
 *
 * @param versionInfo Struct which stores the module info.
 */
void printFirmwareVersion(STAR_VERSION_INFO * firmwareVersion);

/**
 * @brief Prints previously recorded traffic.
 *
 * @param pTraffic The address where the recorded traffic is read from.
 * @param trafficCount The number of STAR_LA_Traffic structures.
 * @param charCaptureClockPeriod The character capture clock period.
 */
void LA_MK3_printRecordedTraffic(STAR_LA_MK3_Traffic *pTraffic, const U32 *trafficCount, const double *charCaptureClockPeriod);
