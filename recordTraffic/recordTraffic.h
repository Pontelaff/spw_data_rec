/**
 * @file recordTraffic.h
 * @author Jonas Gesch (jonas.gesch@dlr.de)
 * @brief Contains functions needed for recording data in a STAR-Dundee SpaceWire Link Analyzer Mk3 device
 * @version 0.1
 * @date 2021-12-15
 *
 */

#include "spw_la_api.h"

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
