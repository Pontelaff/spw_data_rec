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

/* Maximum recorded duration BEFORE the trigger in milliseconds */
#define PRE_TRIGGER_MS 3000

/* Amount of bytes for the header of a new packet */
#define HEADER_BYTES 12

/* Amount of payload bytes to be displayed per line */
#define BYTES_PER_LINE 8



/**
 * @brief Prints the header containing all necesarry information for the following
 *      hexdump to stdout.
 * 
 * @param triggerTime The timestamp of when the trigger occured.
 * @param settings The settings as configured by the input arguments.
 * @param linkAnalyser The Link Analyser device used for the data recording.
 * 
 * @return A non-zero integer on success.
 */
int printHexdumpHeader(struct timespec *triggerTime, Settings settings, STAR_LA_LinkAnalyser linkAnalyser);

/**
 * @brief Prints the settings as configured by the input arguments to stdout.
 * 
 * @param settings The settings struct to be printed.
 * @return A non-zero integer on success. 
 */
int printSettings(Settings settings);

/**
 * @brief Prints all available information for a Link Analyzer device.
 * 
 * @param linkAnalyser The Link Analyser device to print the information for.
 * 
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
 * 
 * @return A non-zero integer on success.
 */
int LA_printDeviceVersion(STAR_LA_LinkAnalyser linkAnalyser);

/**
 * @brief Prints the header of a hexdump file containing meta data of the recording.
 * 
 * @param triggerTime The timestamp of when the trigger occured.
 * @param settings The settings that were configured by the input arguments.
 * @param linkAnalyser The Link Analyzer device that was used to record data.
 * 
 * @return A non-zero integer on success. 
 */
int printHexdumpHeader(struct timespec *triggerTime, Settings settings, STAR_LA_LinkAnalyser linkAnalyser);

/**
 * @brief Prints the timestamp and packet header.
 * 
 * @param pTraffic The address where the recorded traffic is read from.
 * @param index The current index in the traffic recording.
 * @param charCaptureClockPeriod The character capture clock period.
 * @param triggerTime The timestamp of when the trigger occured.
 */
void LA_MK3_printHexdumpPacketHeader(STAR_LA_MK3_Traffic *pTraffic, U32 *index, const double *charCaptureClockPeriod, struct timespec *triggerTime);

/**
 * @brief Prints the captured data in a packet based hexdump format.
 * 
 * @param pTraffic The address where the recorded traffic is read from.
 * @param trafficCount The number of STAR_LA_Traffic structures.
 * @param charCaptureClockPeriod The character capture clock period.
 * @param triggerTime The timestamp of when the trigger occured.
 */
void LA_MK3_printHexdump(STAR_LA_MK3_Traffic *pTraffic, const U32 *trafficCount, const double *charCaptureClockPeriod, struct timespec *triggerTime);

/**
 * @brief Prints the name, version info and author of a module, if available.
 *
 * @param versionInfo Struct which stores the module info.
 */
void printFirmwareVersion(STAR_VERSION_INFO * firmwareVersion);

/**
 * @brief Gets the event type of either the A or B event (assuming both events can't occur simoultaneously).
 * 
 * @param traffic The traffic object to get the event type for.
 * @return STAR_LA_TRAFFIC_TYPE The type of the occured event.
 */
STAR_LA_TRAFFIC_TYPE LA_MK3_getEventType(STAR_LA_MK3_Traffic traffic);

/**
 * @brief Prints previously recorded traffic.
 *
 * @param pTraffic The address where the recorded traffic is read from.
 * @param trafficCount The number of STAR_LA_Traffic structures.
 * @param charCaptureClockPeriod The character capture clock period.
 */
void LA_MK3_printRecordedTraffic(STAR_LA_MK3_Traffic *pTraffic, const U32 *trafficCount, const double *charCaptureClockPeriod);
