/**
 * @file LA_interface.h
 * @author Jonas Gesch (jonas.gesch@dlr.de)
 * @brief Contains functions needed for recording data in a STAR-Dundee SpaceWire Link Analyzer Mk3 device
 * @version 0.2.0
 * @date 2021-12-15
 *
 */

#include "spw_la_api.h"

/**
 * @brief Scans for Link Analyser Mk3 Devices and saves the deviceID of the last found LA device.
 *
 * @param linkAnalyser struct, where the deviceID of the detected LA is stored.
 * 
 * @return A non-zero integer on success.
 */
int LA_MK3_detectDevice(STAR_LA_LinkAnalyser *linkAnalyser, const char *serialNumber);

/**
 * @brief Configures a Link Analyser device for recording.
 *
 * @param linkAnalyser The Link Analyser device to configure for recording.
 */
void LA_configRecording(STAR_LA_LinkAnalyser linkAnalyser, Settings config);

/**
 * @brief Gets traffic on a Link Analyser device.
 *
 * @param linkAnalyser The Link Analyser device to get the traffic from.
 * @param ppTraffic Pointer to the address where the recorded traffic is written to.
 * @param trafficCount The number of STAR_LA_Traffic structures.
 * @param charCaptureClockPeriod The character capture clock period.
 * @param captureDuration The duration in seconds that is recorded after the trigger.
 * 
 * @return A non-zero integer on success.
 */
int LA_MK3_recordTraffic(STAR_LA_LinkAnalyser linkAnalyser, STAR_LA_MK3_Traffic **ppTraffic, U32 *trafficCount,
                         double *charCaptureClockPeriod, const double *captureDuration, struct timespec *triggerTime);
