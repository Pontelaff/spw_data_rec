/**
 * @file recordTraffic.h
 * @author Jonas Gesch (jonas.gesch@dlr.de)
 * @brief Contains functions needed for recording data in a STAR-Dundee SpaceWire Link Analyzer Mk3 device
 * @version 0.1
 * @date 2021-12-15
 *
 */

#include "spw_la_api.h"

static char *GetEventTypeString(U8 trafficType);

void get_all_recorded_traffic_mk3(STAR_LA_LinkAnalyser linkAnalyser);