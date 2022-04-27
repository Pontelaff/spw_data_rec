/**
 * @file data_logger.h
 * @author Jonas Gesch (jonas.gesch@dlr.de)
 * @brief Contains functions for logging the recorded traffic as a
 *      formatted packet based hexdump to stdout.
 * @version 0.3.1
 * @date 2022-02-28
 *
 */

/* Amount of bytes for the header of a new packet */
#define HEADER_BYTES 12

/* Amount of payload bytes to be displayed per line */
#define BYTES_PER_LINE 8

/* A single data packet for an individual receiver */
struct dataPacket
{
    U32 bytesReceived;       /* The number of bytes currently received */
    STAR_LA_MK3_Event event; /* The current type of event on the receiver */
    FILE *packetStream;      /* The file to write the packet's data to */
    char direction;          /* The direction of traffic (Recv A = 'I'; Recv B = 'O') */
};

/**
 * @brief Gets the absolute timestamp of a packet as a formatted string.
 *
 * @param deltaToTrigger The time difference between the trigger and the current packet in seconds.
 * @param triggerTime The timestamp at which the Link Analyser device was triggered.
 * @return A pointer to the formatted string. Needs to be freed after use.
 */
char *LA_MK3_getPacketTimestamp(const double *deltaToTrigger, struct timespec *triggerTime);

/**
 * @brief Checks the event of a receiver and acts accordingly
 *
 * @param packet The struct for the receiver.
 * @param deltaToTrigger The time difference between the trigger and the current packet in seconds.
 * @param triggerTime The timestamp at which the Link Analyser device was triggered.
 * @return The amount of bytes in the packet, if the packet was terminated. Otherwise zero.
 */
unsigned int LA_MK3_printByte(struct dataPacket *packet, const double *deltaToTrigger, struct timespec *triggerTime);

/**
 * @brief Prints the captured data in a packet based hexdump format.
 *
 * @param pTraffic The address where the recorded traffic is read from.
 * @param trafficCount The number of STAR_LA_Traffic structures.
 * @param charCaptureClockPeriod The character capture clock period.
 * @param triggerTime The timestamp of when the trigger occurred.
 * @param preTrigger The maximum duration for which packets received BEFORE the trigger are displayed.
 */
void LA_MK3_printHexdumpData(STAR_LA_MK3_Traffic *pTraffic, const U32 *trafficCount, const double *charCaptureClockPeriod, struct timespec *triggerTime, const int preTrigger);

/**
 * @brief Prints the configuration data and captured data as a hexdump.
 * 
 * @param linkAnalyser The Link Analyser device used for captureing the data traffic.
 * @param pTraffic The address where the recorded traffic is read from.
 * @param settings The application settings as configured by the input arguments. 
 * @param trafficCount The number of STAR_LA_Traffic structures.
 * @param charCaptureClockPeriod The character capture clock period.
 * @param triggerTime The timestamp of when the trigger occurred.
 * @return A non-zero integer on success.
 */
int LA_MK3_printRecordedTraffic(STAR_LA_LinkAnalyser linkAnalyser, STAR_LA_MK3_Traffic *pTraffic, Settings settings, const U32 *trafficCount, const double *charCaptureClockPeriod, struct timespec *triggerTime);

/**
 * @brief Prints previously recorded event based traffic.
 *
 * @param pTraffic The address where the recorded traffic is read from.
 * @param trafficCount The number of STAR_LA_Traffic structures.
 * @param charCaptureClockPeriod The character capture clock period.
 * @param preTrigger The maximum duration for which packets received BEFORE the trigger are displayed.
 */
void LA_MK3_printEventCaptureLog(STAR_LA_MK3_Traffic *pTraffic, const U32 *trafficCount, const double *charCaptureClockPeriod, const int preTrigger);
