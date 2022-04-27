/**
 * @file config_logger.h
 * @author Jonas Gesch (jonas.gesch@dlr.de)
 * @brief Contains functions for writing the configuration of the software and
 *      additional info regarding the Link Analyser to stdout so that it can be
 *      used as a hexdump header.
 * @version 0.2.0
 * @date 2022-03-23
 *
 */

#include "spw_la_api.h"

typedef struct settings Settings;

/**
 * @brief Prints the settings as configured by the input arguments to stdout.
 *
 * @param settings The settings struct to be printed.
 */
void printConfig(Settings settings);

/**
 * @brief Prints the build date of a Link Analyser device.
 *
 * @param linkAnalyser The Link Analyser to print the build date for.
 * @return A non-zero integer on success.
 */
int LA_printBuildDate(STAR_LA_LinkAnalyser linkAnalyser);

/**
 * @brief Prints all available information for a Link Analyzer device.
 *
 * @param linkAnalyser The Link Analyser device to print the information for.
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
 * @param linkAnalyser The Link Analyser device to print the version information for.
 * @return A non-zero integer on success.
 */
int LA_printDeviceVersion(STAR_LA_LinkAnalyser linkAnalyser);

/**
 * @brief Prints the name, version info and author of a module, if available.
 *
 * @param firmwareVersion Struct which stores the module info.
 */
void printFirmwareVersion(STAR_VERSION_INFO *firmwareVersion);

/**
 * @brief Creates a formatted string for a timestamp.
 *
 * @param timestamp The timestamp to create the string for.
 * @return A pointer to the timestamp as a string.
 */
char *timeToStr(struct timespec *timestamp);

/**
 * @brief Prints meta data of the recording to stdout.
 *
 * @param triggerTime The timestamp of when the trigger occurred.
 * @param settings The settings as configured by the input arguments.
 * @param linkAnalyser The Link Analyzer device used to record data.
 * @return A non-zero integer on success.
 */
int printConfigHeader(struct timespec *triggerTime, Settings settings, STAR_LA_LinkAnalyser linkAnalyser);
