/**
 * @file LA_interface.h
 * @author Jonas Gesch (jonas.gesch@dlr.de)
 * @brief Contains functions needed for recording data in a STAR-Dundee SpaceWire Link Analyzer Mk3 device
 * @version 0.1
 * @date 2021-12-15
 *
 */

#include <stdbool.h>

/**
 * @brief Prints the SpaceWire Link Analyser API version.
 *
 */
void LA_printApiVersion();

/**
 * @brief Scans for Link Analyser Mk3 Devices and saves the deviceID of the last found LA device.
 *
 * @param linkAnalyser struct, where the deviceID of the detected LA is stored
 * @return true when at least one LA has been detected
 * @return false  when no devices have been detected
 */
bool LA_MK3_detectDevice(STAR_LA_LinkAnalyser *linkAnalyser);

/**
 * @brief Prints the version information for the specified Link Analyser device.
 *
 * @param linkAnalyser The link analyser device to print the version information for.
 */
void LA_printDeviceVersion(STAR_LA_LinkAnalyser linkAnalyser);

/**
 * @brief Configures a Link Analyser device for recording.
 *
 * @param linkAnalyser The Link Analyser device to configure for recording.
 */
void LA_configRecording(STAR_LA_LinkAnalyser linkAnalyser);

/**
 * @brief Gets traffic on a Link Analyser device.
 *
 * @param linkAnalyser The Link Analyser device to get the traffic from.
 * @param ppTraffic Pointer to the address where the recorded traffic is written to.
 * @param trafficCount The number of STAR_LA_Traffic structures.
 * @param charCaptureClockPeriod The character capture clock period.
 * @return true when successfull
 */
bool LA_MK3_recordTraffic(STAR_LA_LinkAnalyser linkAnalyser, STAR_LA_MK3_Traffic **ppTraffic, U32 *trafficCount, double *charCaptureClockPeriod);

/**
 * @brief Prints previously recorded traffic.
 *
 * @param pTraffic The address where the recorded traffic is read from.
 * @param trafficCount The number of STAR_LA_Traffic structures.
 * @param charCaptureClockPeriod The character capture clock period.
 */
void LA_MK3_printRecordedTraffic(STAR_LA_MK3_Traffic *pTraffic, U32 *trafficCount, double *charCaptureClockPeriod);
