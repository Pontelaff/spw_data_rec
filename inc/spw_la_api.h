/**
 * \file spw_la_api.h
 * \brief Definitions provided by the SpaceWire Link Analyser API.
 * \author Stephen Mudie\n
 *         STAR-Dundee Ltd\n
 *         STAR House,\n
 *         166 Nethergate,\n 
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains the declarations of functions
 * and the definitions of types and macros provided
 * by the SpaceWire Link Analyser API.
 *
 * Copyright &copy; 2018 STAR-Dundee Ltd
 */

#ifndef SPW_LA_API_H
#define SPW_LA_API_H



#ifdef __cplusplus
extern "C" {
#endif


#include "star-dundee_types.h"
#include "star-dundee_64_bit_types.h"
#include "star_device_handle.h"
#include "star-api.h"



#if defined(_WIN32) || defined(_WIN64)

    #ifdef STAR_LA_EXPORTS
        #define STAR_LA_LINK __declspec(dllexport)
    #else
        #define STAR_LA_LINK __declspec(dllimport)
    #endif	/* STAR_API_EXPORTS */

    #ifdef _WIN64
        #define STAR_LA_CC
    #else
        #define STAR_LA_CC __stdcall
    #endif	/* WIN64 */

#else

    #define STAR_LA_LINK
    #define STAR_LA_CC

#endif	/* defined(_WIN32) || defined(_WIN64) */


/* ENUMERATIONS */

/**
 * \ingroup SpWLAAPI
 * Link Analyser receivers.
 */
typedef enum 
{
    STAR_LA_RECEIVER_A = 0,
    STAR_LA_RECEIVER_B = 1

} STAR_LA_RECEIVER;

/** 
 * \ingroup SpWLAAPI
 * Link Analyser data comparators.
 */
typedef enum
{
    STAR_LA_SHORT1 = 0,
    STAR_LA_SHORT2 = 1,
    STAR_LA_SHORT3 = 2,
    STAR_LA_LONG1 = 3

} STAR_LA_DATACOMPARATOR;

/**
 * \ingroup SpWLAAPI
 * Link analyser packet count type.
 */
typedef enum
{
    STAR_LA_EQUAL_TO = 0,
    STAR_LA_GREATER_THAN = 1,
    STAR_LA_LESS_THAN = 2    

} STAR_LA_PACKET_COUNT_TYPE;

/** 
 * \ingroup SpWLAAPI
 * Link Analyser data comparison types.
 */
typedef enum
{
    STAR_LA_DATA = 0,
    STAR_LA_HEADER = 1,
    STAR_LA_HEADERCHECKDATA = 2,
    STAR_LA_EOP = 3,
    STAR_LA_EEP = 4

} STAR_LA_DATACOMPARATOR_TYPE;

/**
 * \ingroup SpWLAAPI
 * Link analyser trigger state.
 */
typedef enum
{
    STAR_LA_TRIGGERSTATE_WAITING = 0,
    STAR_LA_TRIGGERSTATE_STARTED = 1, /* Waiting to trigger */
    STAR_LA_TRIGGERSTATE_TRIGGERED = 2, /* Waiting for post trigger */
    STAR_LA_TRIGGERSTATE_COMPLETE = 3 /* Holding memory contents */

} STAR_LA_TRIGGERSTATE;

/**
 * \ingroup SpWLAAPI
 * Link analyser trigger sequence source.
 *
 * \note For extended trigger source configuration,
 *       each source is a bit flag
 */
typedef enum
{
    STAR_LA_TRIGGER_SEQ_SOURCE_RECEIVER_A = 1,
    STAR_LA_TRIGGER_SEQ_SOURCE_RECEIVER_B = 2,
    STAR_LA_TRIGGER_SEQ_SOURCE_LINKSTATE = 4,
    STAR_LA_TRIGGER_SEQ_SOURCE_EXT_TRIGGER = 8

} STAR_LA_TRIGGER_SEQ_SOURCE;

/**
 * \ingroup SpWLAAPI
 * Link analyser trigger sequence trigger event.
 */
typedef enum
{
    STAR_LA_TRIGGER_EVENT_TIMECODE_COMPARATOR = 0,
    STAR_LA_TRIGGER_EVENT_SHORT1 = 1,
    STAR_LA_TRIGGER_EVENT_SHORT2 = 2,
    STAR_LA_TRIGGER_EVENT_SHORT3 = 3,
    STAR_LA_TRIGGER_EVENT_LONG = 4,
    STAR_LA_TRIGGER_EVENT_PACKETCOUNT_COMPARATOR = 5,    
    STAR_LA_TRIGGER_EVENT_TIMECODE = 6,
    STAR_LA_TRIGGER_EVENT_HEADER = 7,
    STAR_LA_TRIGGER_EVENT_DATA = 8,
    STAR_LA_TRIGGER_EVENT_EEP = 9,
    STAR_LA_TRIGGER_EVENT_EOP = 10,
    STAR_LA_TRIGGER_EVENT_ESCAPE = 11,
    STAR_LA_TRIGGER_EVENT_NULL = 12,
    STAR_LA_TRIGGER_EVENT_FCT = 13,
    STAR_LA_TRIGGER_EVENT_CHARACTER_SEQ_ERROR = 14,
    STAR_LA_TRIGGER_EVENT_ESCAPE_ERROR = 15,
    STAR_LA_TRIGGER_EVENT_CREDIT_ERROR = 16,
    STAR_LA_TRIGGER_EVENT_PARITY_ERROR = 17,
    STAR_LA_TRIGGER_EVENT_DISCONNECT_ERROR = 18,
    STAR_LA_TRIGGER_EVENT_GOTBIT = 19,
    STAR_LA_TRIGGER_EVENT_ALL_ERRORS = 20,
    STAR_LA_TRIGGER_EVENT_ERROR_INJECTION = 21,
    STAR_LA_TRIGGER_EVENT_LINK_SYNCHRONISED = 22,
    STAR_LA_TRIGGER_EVENT_TRIGGER_IN = 23

} STAR_LA_TRIGGER_EVENT;

/**
 * \ingroup SpWLAAPI
 * Link analyser traffic types.
 */
typedef enum
{
    STAR_LA_TRAFFIC_TYPE_HEADER = 0,
    STAR_LA_TRAFFIC_TYPE_DATA = 1,
    STAR_LA_TRAFFIC_TYPE_EOP = 2,
    STAR_LA_TRAFFIC_TYPE_EEP = 3,
    STAR_LA_TRAFFIC_TYPE_DATA_SEQ_ERROR = 4,
    STAR_LA_TRAFFIC_TYPE_EOP_SEQ_ERROR = 5,
    STAR_LA_TRAFFIC_TYPE_EEP_SEQ_ERROR = 6,
    STAR_LA_TRAFFIC_TYPE_TIMECODE = 7,
    STAR_LA_TRAFFIC_TYPE_TIMECODE_SEQ_ERROR = 8,
    STAR_LA_TRAFFIC_TYPE_FCT = 9,    
    STAR_LA_TRAFFIC_TYPE_ESCAPE = 10,       /* Valid for Link Analyser Mk 1 only */
    STAR_LA_TRAFFIC_TYPE_NULL = 11,
    STAR_LA_TRAFFIC_TYPE_PARITY_ERROR = 12, /* Valid for Link Analyser Mk 1 only */
    STAR_LA_TRAFFIC_TYPE_ESCAPE_ESCAPE_ERROR = 13,
    STAR_LA_TRAFFIC_TYPE_ESCAPE_EOP_ERROR = 14,
    STAR_LA_TRAFFIC_TYPE_ESCAPE_EEP_ERROR = 15,
    STAR_LA_TRAFFIC_TYPE_GOT_BIT = 16,
    STAR_LA_TRAFFIC_TYPE_NO_CHARACTER = 17,
    STAR_LA_TRAFFIC_TYPE_DISCONNECT_ERROR = 18 /* Valid for Link Analyser Mk 2 only */

} STAR_LA_TRAFFIC_TYPE;

/**
 * \ingroup SpWLAAPI
 * Link analyser error types.
 */
typedef enum
{
    STAR_LA_ERROR_TYPE_DISCONNECT = 0,
    STAR_LA_ERROR_TYPE_SIMULTANEOUS_TRANSITION = 1,
    STAR_LA_ERROR_TYPE_BURST = 2

} STAR_LA_ERROR_TYPE;

/**
 * \ingroup SpWLAAPI
 * Link analyser types.
 */
typedef enum
{
    STAR_LA_LINK_ANALYSER_TYPE_MK1 = 0,
    STAR_LA_LINK_ANALYSER_TYPE_MK2 = 1,
    STAR_LA_LINK_ANALYSER_TYPE_MK3 = 2

} STAR_LA_LINK_ANALYSER_TYPE;

/**
 * \ingroup SpWLAAPI
 * Link Analyser external trigger in mode.
 */
typedef enum
{
    STAR_LA_TRIGGER_IN_MODE_DISABLED = 0,
    STAR_LA_TRIGGER_IN_MODE_RISING_EDGE = 1,
    STAR_LA_TRIGGER_IN_MODE_FALLING_EDGE = 2,
    STAR_LA_TRIGGER_IN_MODE_DUAL_EDGE = 3

} STAR_LA_TRIGGER_IN_MODE;

/**
 * \ingroup SpWLAAPI
 * Link Analyser external trigger out pulse mode.
 */
typedef enum
{
    STAR_LA_TRIGGER_OUT_MODE_ACTIVE_LOW = 0,
    STAR_LA_TRIGGER_OUT_MODE_ACTIVE_HIGH = 1    

} STAR_LA_TRIGGER_OUT_MODE;

/**
 * \ingroup SpWLAAPI
 * Link Analyser bit-stream memory size.
 */
typedef enum
{
    STAR_LA_BITSTREAM_MEM_SIZE_512_BYTES = 0,
    STAR_LA_BITSTREAM_MEM_SIZE_1_KiB = 1,
    STAR_LA_BITSTREAM_MEM_SIZE_2_KiB = 2,
    STAR_LA_BITSTREAM_MEM_SIZE_4_KiB = 3,
    STAR_LA_BITSTREAM_MEM_SIZE_8_KiB = 4,
    STAR_LA_BITSTREAM_MEM_SIZE_16_KiB = 5,
    STAR_LA_BITSTREAM_MEM_SIZE_32_KiB = 6,
    STAR_LA_BITSTREAM_MEM_SIZE_64_KiB = 7

} STAR_LA_BITSTREAM_MEM_SIZE;

/**
 * \ingroup SpWLAAPI
 * Link Analyser character memory size.
 */
typedef enum
{
    STAR_LA_CHARACTER_MEM_SIZE_1_MiB = 0,
    STAR_LA_CHARACTER_MEM_SIZE_2_MiB = 1,
    STAR_LA_CHARACTER_MEM_SIZE_4_MiB = 2,
    STAR_LA_CHARACTER_MEM_SIZE_8_MiB = 3,
    STAR_LA_CHARACTER_MEM_SIZE_16_MiB = 4,
    STAR_LA_CHARACTER_MEM_SIZE_32_MiB = 5,
    STAR_LA_CHARACTER_MEM_SIZE_64_MiB = 6,
    STAR_LA_CHARACTER_MEM_SIZE_128_MiB = 7,
    STAR_LA_CHARACTER_MEM_SIZE_256_MiB = 8,
    STAR_LA_CHARACTER_MEM_SIZE_512_MiB = 9

} STAR_LA_CHARACTER_MEM_SIZE;

/**
 * \ingroup SpWLAAPI
 * Structure representing link analyser event.
 */
typedef struct
{
    /** Event type */
    STAR_LA_TRAFFIC_TYPE type;

    /** Event data (only applicable for header, data character or time-code) */
    U8 data;

    /** Disconnect error flag. NOTE: ONLY VALID FOR LINK ANALYSER MK 1 */
    U8 disconnectError;

    /** Parity error flag. NOTE: ONLY VALID FOR LINK ANALYSER MK 2 */
    U8 parityError;

    /** Receiver credit error flag. */
    U8 receiverCreditError;

    /** Transmitter credit error flag. */
    U8 transmitterCreditError;

} STAR_LA_Event;

/**
 * \ingroup SpWLAAPI
 * Structure representing link analyser traffic.
 */
typedef struct
{
    /** The event that occurred in the 'A' direction. */
    STAR_LA_Event linkAEvent;

    /** The event that occurred in the 'B' direction. */
    STAR_LA_Event linkBEvent;

    /** The traffic sample number relative to the trigger
    at which the event occurred.
    NOTE: Use this in combination with character memory
    capture clock period to calculate the traffic
    time relative to the trigger in seconds */
    S64 time;

} STAR_LA_Traffic;

/**
 * \ingroup SpWLAAPI
 * Structure representing a link analyser Mk 3 event. This
 * is supported both by the link analyser Mk 2 and Mk 3.
 *
 * \note This should be used as an alternative to the
 *       STAR_LA_Event structure as it uses less memory.
 */
typedef struct
{
    /** Event type */
    U8 type;

    /** Event data (only applicable for header, data character or time-code) */
    U8 data;

    /**
     * Errors bit field
     * 
     * \note Use STAR_LA_MK3_ParityError, STAR_LA_MK3_ReceiverCreditError
     *       and STAR_LA_MK3_TransmitterCreditError functions to extract
     *       error flags.
     */
    U8 errors;

} STAR_LA_MK3_Event;

/**
 * \ingroup SpWLAAPI
 * Structure representing link analyser Mk3 traffic. This is
 * supported both by the link analyser Mk 2 and Mk 3.
 *
 * \note This should be used as an alternative to the
 *       STAR_LA_Traffic structure as it uses less memory.
 */
typedef struct
{
    /** The traffic sample number relative to the trigger
    at which the event occurred.
    NOTE: Use this in combination with character memory
    capture clock period to calculate the traffic
    time relative to the trigger in seconds */
    S64 time;

    /** The event that occurred in the 'A' direction. */
    STAR_LA_MK3_Event linkAEvent;

    /** The event that occurred in the 'B' direction. */
    STAR_LA_MK3_Event linkBEvent;

} STAR_LA_MK3_Traffic;

/**
 * \ingroup SpWLAAPI
 * Structure representing link analyser
 * bit-stream traffic.
 */
typedef struct
{
    /** Link A data in bit */
    unsigned linkADataInBit : 1;

    /** Link A strobe in bit */
    unsigned linkAStrobeInBit : 1;

    /** Link B data in bit */
    unsigned linkBDataInBit : 1;

    /** Link B strobe in bit */
    unsigned linkBStrobeInBit : 1;

    /** The bit-stream traffic sample number relative
    to the trigger at which the event occurred.
    NOTE: Use this in combination with bit-stream memory
    capture clock period to calculate the bit-stream
    traffic time relative to the trigger in seconds */
    S64 time;

} STAR_LA_BitStreamTraffic;

/**
 * \ingroup SpWLAAPI
 * Structure representing the SpaceWire Link Analyser.
 */
typedef struct
{
    /** The device handle (for Mk 1 and Mk 2 versions) */
    star_device_handle device;

    /** The device ID (for STAR-System versions i.e. Mk 3) */
    STAR_DEVICE_ID deviceID;

    /** The link analyser type */
    STAR_LA_LINK_ANALYSER_TYPE linkAnalyserType;

} STAR_LA_LinkAnalyser;

/**
 * \ingroup SpWLAAPI
 * Structure representing extended external
 * trigger out global events.
 */
typedef struct
{
    /** Analyser triggered */
    unsigned analyserTrigger : 1;

    /** External trigger in */
    unsigned triggerIn : 1;

} STAR_LA_ExtTrigOutGlobalEvents;

/**
 * \ingroup SpWLAAPI
 * Structure representing extended external
 * trigger out receiver events.
 */
typedef struct
{
    /** Time-code */
    unsigned timecode : 1;

    /** Packet header character */
    unsigned packetHeader : 1;

    /** Data character */
    unsigned data : 1;

    /** EEP */
    unsigned eep : 1;

    /** EOP */
    unsigned eop : 1;

    /** Escape character */
    unsigned escape : 1;

    /** NULL character */
    unsigned null : 1;

    /** FCT character */
    unsigned fct : 1;

    /** Got first bit after disconnect event */
    unsigned gotBit : 1;

    /** Character sequence error */
    unsigned charSeqError : 1;

    /** Escape error */
    unsigned escapeError : 1;

    /** Credit error */
    unsigned creditError : 1;

    /** Parity error */
    unsigned parityError : 1;

    /** Disconnect error */
    unsigned disconnectError : 1;

    /** All errors */
    unsigned allErrors : 1;

    /** Error injection started */
    unsigned errorInjection : 1;

} STAR_LA_ExtTrigOutRxEvents;


/* API FUNCTIONS */

/* Link analyser access */

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetLinkAnalyser(star_device_handle device,
                        STAR_LA_LinkAnalyser *pLinkAnalyser);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetSSLinkAnalyser(STAR_DEVICE_ID deviceID,
                          STAR_LA_LinkAnalyser *pLinkAnalyser);

/* API Information */

STAR_LA_LINK void STAR_LA_CC
STAR_LA_GetAPIVersion(int *pMajor, int *pMinor, int *pEdit, int *pPatch);

/* Device information */

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetDeviceVersion(STAR_LA_LinkAnalyser linkAnalyser,
                         U8 *pMajor, U8 *pMinor, U16 *pEdit, U16 *pPatch);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetBuildDate(STAR_LA_LinkAnalyser linkAnalyser,
                     U8 *pYear, U8 *pMonth, U8 *pDay, U8 *pHour, U8 *pMinute);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetSerialNumber(STAR_LA_LinkAnalyser linkAnalyser, U8 serialNum[11]);

/* System clock speed */
STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetSystemClockSpeed(STAR_LA_LinkAnalyser linkAnalyser,
                            U32 *pClockSpeed);

/* Loopback mode */

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetLoopBackModeEnabled(STAR_LA_LinkAnalyser linkAnalyser, U8 *pEnabled);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_EnableLoopBackMode(STAR_LA_LinkAnalyser linkAnalyser, U8 enable);

/* Character filters */

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetRecordNChars(STAR_LA_LinkAnalyser linkAnalyser, U8 *pNCharEnBit);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetRecordNChars(STAR_LA_LinkAnalyser linkAnalyser, U8 enabled);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetRecordTimeCodes(STAR_LA_LinkAnalyser linkAnalyser, U8 *pTimeCodeEnBit);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetRecordTimeCodes(STAR_LA_LinkAnalyser linkAnalyser, U8 enabled);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetRecordFCTs(STAR_LA_LinkAnalyser linkAnalyser, U8 *pFCTEnBit);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetRecordFCTs(STAR_LA_LinkAnalyser linkAnalyser, U8 enabled);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetRecordNulls(STAR_LA_LinkAnalyser linkAnalyser, U8 *pNullEnBit);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetRecordNulls(STAR_LA_LinkAnalyser linkAnalyser, U8 enabled);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetRecordedCharacters(STAR_LA_LinkAnalyser linkAnalyser,
                              U8 *pNullEnBit, U8 *pFCTEnBit,
                              U8 *pTimeCodeEnBit, U8 *pNCharEnBit);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetRecordedCharacters(STAR_LA_LinkAnalyser linkAnalyser, U8 enNulls,
                              U8 enFCTs, U8 enTimeCodes, U8 enNChars);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetRecordOnlyPacketHeader(STAR_LA_LinkAnalyser linkAnalyser,
                                  U8 *pHeaderOnlyEnBit);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetRecordOnlyPacketHeader(STAR_LA_LinkAnalyser linkAnalyser,
                                  U8 enabled);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetRecordOnlyPacketHeaderCount(STAR_LA_LinkAnalyser linkAnalyser,
                                       U8 *pPacketHeaderCount);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetRecordOnlyPacketHeaderCount(STAR_LA_LinkAnalyser linkAnalyser,
                                       U8 count);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_RecordOnlyPacketHeader(STAR_LA_LinkAnalyser linkAnalyser,
                               U8 enable, U8 packetHeaderCount);

/* Data comparator operations */

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetDataComparator(STAR_LA_LinkAnalyser linkAnalyser,
                          STAR_LA_RECEIVER receiver,
                          STAR_LA_DATACOMPARATOR dataComp,
                          STAR_LA_DATACOMPARATOR_TYPE *dataCompType,
                          U8 *dataCharToCompare, U8 *enabled);

STAR_LA_LINK STAR_LA_DATACOMPARATOR_TYPE *STAR_LA_CC
STAR_LA_GetDataComparatorTypes(STAR_LA_LinkAnalyser linkAnalyser,
                               STAR_LA_RECEIVER receiver,
                               STAR_LA_DATACOMPARATOR dataComp,
                               U8 *comparisonNum);

STAR_LA_LINK void STAR_LA_CC
STAR_LA_FreeDataComparatorTypes(STAR_LA_DATACOMPARATOR_TYPE *dataCompType);

STAR_LA_LINK U8 *STAR_LA_CC
STAR_LA_GetDataComparatorDataChars(STAR_LA_LinkAnalyser linkAnalyser,
                                   STAR_LA_RECEIVER receiver,
                                   STAR_LA_DATACOMPARATOR dataComp,
                                   U8 *comparisonNum);

STAR_LA_LINK void STAR_LA_CC
STAR_LA_FreeDataComparatorDataComparisonValues(U8 *dataCharsToCompare);

STAR_LA_LINK U8 *STAR_LA_CC
STAR_LA_GetDataComparatorEnabled(STAR_LA_LinkAnalyser linkAnalyser,
                                 STAR_LA_RECEIVER receiver,
                                 STAR_LA_DATACOMPARATOR dataComp,
                                 U8 *comparisonNum);

STAR_LA_LINK void STAR_LA_CC
STAR_LA_FreeDataComparatorEnabledFlags(U8 *enabled);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetDataComparatorAtIndex(STAR_LA_LinkAnalyser linkAnalyser,
                                 STAR_LA_RECEIVER receiver,
                                 STAR_LA_DATACOMPARATOR dataComp,
                                 U8 dataCompIndex,
                                 STAR_LA_DATACOMPARATOR_TYPE dataCompType,
                                 U8 dataCharToCompare, U8 enabled);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetDataComparatorDataCharAtIndex(STAR_LA_LinkAnalyser linkAnalyser,
                                         STAR_LA_RECEIVER receiver,
                                         STAR_LA_DATACOMPARATOR dataComp,
                                         U8 dataCompIndex, U8 *dataChar);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetDataComparatorTypeAtIndex(STAR_LA_LinkAnalyser linkAnalyser,
                                     STAR_LA_RECEIVER receiver,
                                     STAR_LA_DATACOMPARATOR dataComp,
                                     U8 dataCompIndex,
                                     STAR_LA_DATACOMPARATOR_TYPE *dataCompType);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetDataComparatorEnabledAtIndex(STAR_LA_LinkAnalyser linkAnalyser,
                                        STAR_LA_RECEIVER receiver,
                                        STAR_LA_DATACOMPARATOR dataComp,
                                        U8 dataCompIndex, U8 *enabled);

/* Time-code comparator operations */

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetTimeCodeComparator(STAR_LA_LinkAnalyser linkAnalyser,
                              STAR_LA_RECEIVER receiver, U8 timeCodeValue,
                              U8 checkFlags, U8 flagsValue);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetTimeCodeComparatorValue(STAR_LA_LinkAnalyser linkAnalyser,
                                   STAR_LA_RECEIVER receiver,
                                   U8 *timeCodeValue);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetTimeCodeComparatorCheckFlags(STAR_LA_LinkAnalyser linkAnalyser,
                                        STAR_LA_RECEIVER receiver,
                                        U8 *checkFlags);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetTimeCodeComparatorFlagsValue(STAR_LA_LinkAnalyser linkAnalyser,
                                        STAR_LA_RECEIVER receiver,
                                        U8 *flagsValue);

/* Packet count comparator operations */

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetPacketCountComparator(STAR_LA_LinkAnalyser linkAnalyser,
                                 STAR_LA_RECEIVER receiver,
                                 STAR_LA_PACKET_COUNT_TYPE packetCountType,
                                 U32 packetCount);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetPacketCountComparatorType(STAR_LA_LinkAnalyser linkAnalyser,
                                     STAR_LA_RECEIVER receiver,
                                     STAR_LA_PACKET_COUNT_TYPE *packetCountType);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetPacketCountComparatorValue(STAR_LA_LinkAnalyser linkAnalyser,
                                      STAR_LA_RECEIVER receiver,
                                      U32 *packetCount);

/* Post trigger value operations */

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetPostTriggerMemory(STAR_LA_LinkAnalyser linkAnalyser,
                             U32 *pPostTrigger);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetPostTriggerMemory(STAR_LA_LinkAnalyser linkAnalyser,
                             U32 postTrigger);

/* Trigger delay operations */

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetTriggerDelay(STAR_LA_LinkAnalyser linkAnalyser, U32 *pTriggerDelay);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetTriggerDelay(STAR_LA_LinkAnalyser linkAnalyser, U32 triggerDelay);

/* Trace memory operations */

STAR_LA_LINK STAR_LA_Traffic *STAR_LA_CC
STAR_LA_GetAllRecordedTraffic(STAR_LA_LinkAnalyser linkAnalyser,
                              U32 *pTrafficCount,
                              double *pCharCaptureClockPeriod);

STAR_LA_LINK STAR_LA_MK3_Traffic *STAR_LA_CC
STAR_LA_MK3_GetAllRecordedTraffic(STAR_LA_LinkAnalyser linkAnalyser,
                                  U32 *pTrafficCount,
                                  double *pCharCaptureClockPeriod);

STAR_LA_LINK void STAR_LA_CC
STAR_LA_FreeRecordedTrafficMemory(STAR_LA_Traffic *pTrafficStart);

STAR_LA_LINK void STAR_LA_CC
STAR_LA_MK3_FreeRecordedTrafficMemory(STAR_LA_MK3_Traffic *pTrafficStart);

STAR_LA_LINK U32 STAR_LA_CC
STAR_LA_GetMaximumRecordedEvents(STAR_LA_LinkAnalyser linkAnalyser);

/* Traffic error interpretation operations */

STAR_LA_LINK U8 STAR_LA_CC STAR_LA_MK3_ParityError(U8 errors);

STAR_LA_LINK U8 STAR_LA_CC STAR_LA_MK3_ReceiverCreditError(U8 errors);

STAR_LA_LINK U8 STAR_LA_CC STAR_LA_MK3_TransmitterCreditError(U8 errors);

/* Trigger state machine operations */

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetTriggerState(STAR_LA_LinkAnalyser linkAnalyser,
                        STAR_LA_TRIGGERSTATE *pTriggerState);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_InitialiseToWaiting(STAR_LA_LinkAnalyser linkAnalyser);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_StartRecording(STAR_LA_LinkAnalyser linkAnalyser);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_WaitForTrigger(STAR_LA_LinkAnalyser linkAnalyser);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_WaitForComplete(STAR_LA_LinkAnalyser linkAnalyser);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetDataRecorded(STAR_LA_LinkAnalyser linkAnalyser, U8 *dataRecorded);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_TriggerFired(STAR_LA_LinkAnalyser linkAnalyser, U8 *pTriggerFired);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_ForceTrigger(STAR_LA_LinkAnalyser linkAnalyser);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_StopRecording(STAR_LA_LinkAnalyser linkAnalyser);

/* Trigger sequence operations */

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetTriggerSequence(STAR_LA_LinkAnalyser linkAnalyser, U8 sequenceNum,
                           STAR_LA_TRIGGER_SEQ_SOURCE triggerSequenceSource,
                           STAR_LA_TRIGGER_EVENT triggerEvent,
                           U8 frequency, U8 enabled);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetTriggerSequenceSource(STAR_LA_LinkAnalyser linkAnalyser,
                                 U8 sequenceNum,
                                 STAR_LA_TRIGGER_SEQ_SOURCE *pTriggerSequenceSource);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetTriggerSequenceTriggerEvent(STAR_LA_LinkAnalyser linkAnalyser,
                                       U8 sequenceNum,
                                       STAR_LA_TRIGGER_EVENT *pTriggerEvent);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetTriggerSequenceFrequency(STAR_LA_LinkAnalyser linkAnalyser,
                                    U8 sequenceNum, U8 *pFrequency);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetTriggerSequenceEnabled(STAR_LA_LinkAnalyser linkAnalyser,
                                  U8 sequenceNum, U8 *pEnabled);

/* Extended trigger sequence operations */

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetExtTriggerSequenceSource(STAR_LA_LinkAnalyser linkAnalyser,
                                    U8 sequenceNum,
                                    U8 sourceMask);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetExtTriggerSequenceSourceEvent(STAR_LA_LinkAnalyser linkAnalyser,
                                         U8 sequenceNum,
                                         STAR_LA_TRIGGER_SEQ_SOURCE source,
                                         STAR_LA_TRIGGER_EVENT triggerEvent);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetExtTriggerSequenceCount(STAR_LA_LinkAnalyser linkAnalyser,
                                   U8 sequenceNum,
                                   U8 count);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetExtTriggerSequenceEnabled(STAR_LA_LinkAnalyser linkAnalyser,
                                     U8 sequenceNum,
                                     U8 enabled);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetExtTriggerSequenceSource(STAR_LA_LinkAnalyser linkAnalyser,
                                    U8 sequenceNum,
                                    U8 *pSourceMask);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetExtTriggerSequenceSourceEvent(STAR_LA_LinkAnalyser linkAnalyser,
                                         U8 sequenceNum,
                                         STAR_LA_TRIGGER_SEQ_SOURCE source,
                                         STAR_LA_TRIGGER_EVENT *pTriggerEvent);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetExtTriggerSequenceCount(STAR_LA_LinkAnalyser linkAnalyser,
                                   U8 sequenceNum,
                                   U8 *pCount);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetExtTriggerSequenceEnabled(STAR_LA_LinkAnalyser linkAnalyser,
                                     U8 sequenceNum,
                                     U8 *pEnabled);

/* Link statistics operations */

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetDataRate(STAR_LA_LinkAnalyser linkAnalyser,
                    STAR_LA_RECEIVER receiver, U32 *dataRate);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetDataCount(STAR_LA_LinkAnalyser linkAnalyser,
                     STAR_LA_RECEIVER receiver, U32 *dataCount);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetEOPCount(STAR_LA_LinkAnalyser linkAnalyser,
                    STAR_LA_RECEIVER receiver, U32 *eopCount);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetEEPCount(STAR_LA_LinkAnalyser linkAnalyser,
                    STAR_LA_RECEIVER receiver, U32 *eepCount);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetFCTCount(STAR_LA_LinkAnalyser linkAnalyser,
                    STAR_LA_RECEIVER receiver, U32 *fctCount);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetNullCount(STAR_LA_LinkAnalyser linkAnalyser,
                     STAR_LA_RECEIVER receiver, U32 *nullCount);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetTimeCodeCount(STAR_LA_LinkAnalyser linkAnalyser,
                         STAR_LA_RECEIVER receiver, U32 *timeCodeCount);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetDisconnectErrorCount(STAR_LA_LinkAnalyser linkAnalyser,
                                STAR_LA_RECEIVER receiver,
                                U32 *disconnectErrorCount);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetParityErrorCount(STAR_LA_LinkAnalyser linkAnalyser,
                            STAR_LA_RECEIVER receiver, U32 *parityErrorCount);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetCreditErrorCount(STAR_LA_LinkAnalyser linkAnalyser,
                            STAR_LA_RECEIVER receiver, U32 *creditErrorCount);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetEscapeErrorCount(STAR_LA_LinkAnalyser linkAnalyser,
                            STAR_LA_RECEIVER receiver, U32 *escapeErrorCount);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetCharSequenceErrorCount(STAR_LA_LinkAnalyser linkAnalyser,
                                  STAR_LA_RECEIVER receiver,
                                  U32 *charSequenceErrorCount);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetStatisticsUpdatedToggle(STAR_LA_LinkAnalyser linkAnalyser,
                                   U8 *statsUpdatedToggle);

/* Error Injection Operations */

STAR_LA_LINK int STAR_LA_CC
STAR_LA_StartPeriodicErrorInjection(STAR_LA_LinkAnalyser linkAnalyser,
                                    STAR_LA_RECEIVER receiver);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_StopPeriodicErrorInjection(STAR_LA_LinkAnalyser linkAnalyser,
                                   STAR_LA_RECEIVER receiver);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_PeriodicErrorInjectionStarted(STAR_LA_LinkAnalyser linkAnalyser,
                                      STAR_LA_RECEIVER receiver,
                                      U8 *errorInjectionStarted);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_InjectSingleError(STAR_LA_LinkAnalyser linkAnalyser,
                          STAR_LA_RECEIVER receiver);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetErrorInjectionType(STAR_LA_LinkAnalyser linkAnalyser,
                              STAR_LA_RECEIVER receiver,
                              STAR_LA_ERROR_TYPE *errorType);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetErrorInjectionType(STAR_LA_LinkAnalyser linkAnalyser,
                              STAR_LA_RECEIVER receiver,
                              STAR_LA_ERROR_TYPE errorType);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetErrorInjectionLength(STAR_LA_LinkAnalyser linkAnalyser,
                                STAR_LA_RECEIVER receiver,
                                U16 *errorLength);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetErrorInjectionLength(STAR_LA_LinkAnalyser linkAnalyser,
                                STAR_LA_RECEIVER receiver,
                                U16 errorLength);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetErrorInjectionInterval(STAR_LA_LinkAnalyser linkAnalyser,
                                  STAR_LA_RECEIVER receiver,
                                  U32 *errorInterval);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetErrorInjectionInterval(STAR_LA_LinkAnalyser linkAnalyser,
                                  STAR_LA_RECEIVER receiver,
                                  U32 errorInterval);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetErrorInjectionCycleTime(STAR_LA_LinkAnalyser linkAnalyser,
                                   double *errorCycleTime);

/* Bit-Stream Memory Operations */
STAR_LA_LINK STAR_LA_BitStreamTraffic *STAR_LA_CC
STAR_LA_GetAllRecordedBitStreamTraffic(STAR_LA_LinkAnalyser linkAnalyser,
                                       U32 *pBitStreamTrafficCount,
                                       double *pBitCaptureClockPeriod);

STAR_LA_LINK void STAR_LA_CC
STAR_LA_FreeBitStreamTraffic(STAR_LA_BitStreamTraffic *trafficStart);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetBitStreamPostTriggerMemory(STAR_LA_LinkAnalyser linkAnalyser,
                                      U32 *pPostTrigger);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetBitStreamPostTriggerMemory(STAR_LA_LinkAnalyser linkAnalyser,
                                      U32 postTrigger);

STAR_LA_LINK U32 STAR_LA_CC
STAR_LA_GetMaximumBitStreamRecordedEvents(STAR_LA_LinkAnalyser linkAnalyser);

/* External Trigger In Operations */
STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetTriggerInMode(STAR_LA_LinkAnalyser linkAnalyser,
                         STAR_LA_TRIGGER_IN_MODE *pTriggerInMode);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetTriggerInMode(STAR_LA_LinkAnalyser linkAnalyser,
                         STAR_LA_TRIGGER_IN_MODE triggerInMode);

/* External Trigger Out Operations */
STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetTriggerOutPulseMode(STAR_LA_LinkAnalyser linkAnalyser,
                               STAR_LA_TRIGGER_OUT_MODE *pTriggerOutPulseMode);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetTriggerOutPulseMode(STAR_LA_LinkAnalyser linkAnalyser,
                               STAR_LA_TRIGGER_OUT_MODE triggerOutPulseMode);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetTriggerOutPulseLength(STAR_LA_LinkAnalyser linkAnalyser,
                                 U16 *pPulseLength);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetTriggerOutPulseLength(STAR_LA_LinkAnalyser linkAnalyser,
                                 U16 pulseLength);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GenerateTriggerOutPulse(STAR_LA_LinkAnalyser linkAnalyser);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_ArmTriggerOut(STAR_LA_LinkAnalyser linkAnalyser);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_DisarmTriggerOut(STAR_LA_LinkAnalyser linkAnalyser);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_IsTriggerOutArmed(STAR_LA_LinkAnalyser linkAnalyser, U8 *pArmedFlag);

/* Extended External Trigger Out Operations */
STAR_LA_LINK int STAR_LA_CC
STAR_LA_IsExtTriggerOutSupported(STAR_LA_LinkAnalyser linkAnalyser,
                                 U8 *pSupported);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetExtTriggerOutGlobalEvents(STAR_LA_LinkAnalyser linkAnalyser,
                                     STAR_LA_ExtTrigOutGlobalEvents *pEvents);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetExtTriggerOutGlobalEvents(STAR_LA_LinkAnalyser linkAnalyser,
                                     STAR_LA_ExtTrigOutGlobalEvents events);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetExtTriggerOutReceiverEvents(STAR_LA_LinkAnalyser linkAnalyser,
                                       STAR_LA_RECEIVER receiver,
                                       STAR_LA_ExtTrigOutRxEvents *pEvents);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetExtTriggerOutReceiverEvents(STAR_LA_LinkAnalyser linkAnalyser,
                                       STAR_LA_RECEIVER receiver,
                                       STAR_LA_ExtTrigOutRxEvents events);

/* Character Memory Usage Operations */
STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetCharacterMemoryUsageBeforeTrigger(STAR_LA_LinkAnalyser linkAnalyser,
                                             U32 *pSamplesUsed);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetCharacterMemoryUsageAfterTrigger(STAR_LA_LinkAnalyser linkAnalyser,
                                            U32 *pSamplesUsed);

/* Memory Partition Operations */
STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetBitStreamMemorySize(STAR_LA_LinkAnalyser linkAnalyser,
                               STAR_LA_BITSTREAM_MEM_SIZE *pBitStreamMemSize);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetBitStreamMemorySize(STAR_LA_LinkAnalyser linkAnalyser,
                               STAR_LA_BITSTREAM_MEM_SIZE bitStreamMemSize);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetCharacterMemorySize(STAR_LA_LinkAnalyser linkAnalyser,
                               STAR_LA_CHARACTER_MEM_SIZE *pCharacterMemSize);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetCharacterMemorySize(STAR_LA_LinkAnalyser linkAnalyser,
                               STAR_LA_CHARACTER_MEM_SIZE characterMemSize);

/* Identify (flash LEDs) Operations */
STAR_LA_LINK int STAR_LA_CC
STAR_LA_Identify(STAR_LA_LinkAnalyser linkAnalyser);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetIdentifyFlashCount(STAR_LA_LinkAnalyser linkAnalyser,
                              U8 *pFlashCount);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetIdentifyFlashCount(STAR_LA_LinkAnalyser linkAnalyser,
                              U8 flashCount);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_GetIdentifyFlashPeriod(STAR_LA_LinkAnalyser linkAnalyser,
                               U16 *pFlashPeriod);

STAR_LA_LINK int STAR_LA_CC
STAR_LA_SetIdentifyFlashPeriod(STAR_LA_LinkAnalyser linkAnalyser,
                               U16 flashPeriod);


#ifdef __cplusplus
}
#endif



#endif  /* SPW_LA_API_H */





