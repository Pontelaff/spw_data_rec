/**
 * \file stream_item_types.h
 *
 * \brief Structures used to represent stream items such as packets, time-codes and link control tokens
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * The types in this file are used to represent the traffic and link
 * control tokens that may be sent on a SpaceWire link.
 *
 * Copyright &copy; 2012 STAR-Dundee Ltd.
 */
#ifndef STAR_API_STREAM_ITEM_TYPES_H
#define STAR_API_STREAM_ITEM_TYPES_H

#include "star-dundee_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup items
 * The kinds of object that are represented as stream items. Used by #STAR_STREAM_ITEM to show
 * what kind of item it is wrapping.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef enum
{
    /** Packet, see #STAR_SPACEWIRE_PACKET */
    STAR_STREAM_ITEM_TYPE_SPACEWIRE_PACKET,

    /** Time-code, see: #STAR_TIMECODE*/
    STAR_STREAM_ITEM_TYPE_TIMECODE,

    /**
     * A change in the state of a link, see: #STAR_LINK_STATE_EVENT
     *
     * \versionAdded \ref changes_v2_0_beta_1
     */
    STAR_STREAM_ITEM_TYPE_LINK_STATE_EVENT,

    /** Contiguous chunk of data within a single packet, see: #STAR_DATA_CHUNK */
    STAR_STREAM_ITEM_TYPE_DATA_CHUNK,

    /**
     * A change in the speed of a link, see: #STAR_LINK_SPEED_EVENT
     *
     * \versionAdded \ref changes_v2_0_beta_1
     */
    STAR_STREAM_ITEM_TYPE_LINK_SPEED_EVENT,

    /** An error injection control word, see: #STAR_ERROR_IN_DATA_INJECT */
    STAR_STREAM_ITEM_TYPE_ERROR_INJECT,

    /**
     * Timestamp of the last data received on the link, see:
     * #STAR_TIMESTAMP_EVENT
     *
     * \versionAdded \ref changes_v3_6
     */
    STAR_STREAM_ITEM_TYPE_TIMESTAMP_EVENT,

    /**
     * SpaceFibre broadcast message, see: #STAR_BROADCAST_MESSAGE
     *
     * \versionAdded \ref changes_v4_00
     */
    STAR_STREAM_ITEM_TYPE_BROADCAST_MESSAGE

} STAR_STREAM_ITEM_TYPE;


/**
 * \ingroup items
 * The kinds of End of packet marker that may be present.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef enum  {

    /** Error occurred determining EOP type */
    STAR_EOP_TYPE_INVALID,
    /** End of Packet marker*/
    STAR_EOP_TYPE_EOP,
    /** Error End of Packet marker*/
    STAR_EOP_TYPE_EEP,
    /** No End of Packet marker present */
    STAR_EOP_TYPE_NONE

} STAR_EOP_TYPE;


/**
 * \ingroup items
 * Events that can occur which affect the state of the SpaceWire link.
 * \note Link state events are not supported by the PCI Mk2 card.  The appropriate
 * link state change event enable function must be called to allow these events
 * to be generated.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 * \versionChanged \ref changes_v4_00
 */
typedef struct
{
    /** The number of times that the event(s) has occurred. */
    U8 count;

    /** Receive credit error */
    unsigned int receiveCreditError : 1;
    /** Transmit credit error */
    unsigned int transmitCreditError : 1;
    /** Escape error */
    unsigned int escapeError : 1;
    /** Parity error */
    unsigned int parityError : 1;
    /** Disconnect error */
    unsigned int disconnectError : 1;
    /** Link running */
    unsigned int linkRunning : 1;

    /** The port on which the events occurred */
    U8 port;

} STAR_LINK_STATE_EVENT;


/**
 * \ingroup items
 * A structure used to describe a SpaceWire address.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef struct
{
    /** Array of SpaceWire path address elements */
    U8* pPath;
    /** The length of the address */
    U16 pathLength;

} STAR_SPACEWIRE_ADDRESS;


/**
 * \ingroup items
 * A structure used to describe a chunk of contiguous SpaceWire data,
 * within a single packet.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef struct star_data_chunk
{
    struct star_data_chunk* pNext;
    struct star_data_chunk* pPrev;
    /** Pointer to the data buffer itself */
    unsigned char *data;
    /** Length of data in the buffer */
    U32 dataLength;
    /** Whether the chunk of data represents the start of a new packet */
    int isStart;
    /** The type of end of packet marker this data chunk has, if any */
    STAR_EOP_TYPE eop;

} STAR_DATA_CHUNK;


/**
 * \ingroup items
 * A structure used to describe a SpaceWire packet.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef struct
{
    /** The packets address (optional)
    \note This member is provided for convenience only. There is
          no difference between specifying a packet with an address of [ \c 0xFE, \c 0xAB ]
          and data of [ \c 0x01, \c 0x02, \c 0x03, \c 0x04 ], and specifying a packet with no
          explicit address, and data of [ \c 0xFE, \c 0xAB, \c 0x01, \c 0x02, \c 0x03, \c 0x04 ] */
    STAR_SPACEWIRE_ADDRESS* address;
    /** The data that makes up the packet */
    void* dataChunks;

} STAR_SPACEWIRE_PACKET;


/**
 * \ingroup items
 * A structure used to describe a SpaceWire time-code.
 * \note Currently some STAR-System compatible devices only support transmitting
 *       time-codes using channel 0.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef struct
{
    /** Count of the number of time-codes since the last time-code was transmitted */
    U16 numSinceLastTx;
    /** Value of the time-code */
    U8 value;
} STAR_TIMECODE;


/**
 * \ingroup items
 * A structure used to describe a link speed change event.
 * \note Link speed events are not supported by the PCI Mk2 card.  The appropriate
 * link speed change event enable function must be called to allow these events
 * to be generated.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 * \versionChanged \ref changes_v4_00
 */
typedef struct
{
    /**
     * The new link speed which has been adopted, represented in bit/s.
     * Note that this value is approximate.
     */
    U32 linkSpeed;

    /** The port on which the event occurred */
    U8 port;

} STAR_LINK_SPEED_EVENT;


/**
 * \ingroup items
 * The types of error that may be injected in to data in a packet.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
typedef enum  {

    /** Parity error. */
    STAR_ERROR_IN_DATA_PARITY,

    /** Disconnect error. */
    STAR_ERROR_IN_DATA_DISCONNECT

} STAR_ERROR_IN_DATA_TYPE;


/**
 * \ingroup items
 * A structure used to describe an error in data event.
 * \note Not all devices support this item type, and that receiving these types
 * is not possible.
 */
typedef struct
{
    /**
     * Type of error to inject on a following data character
     */
    STAR_ERROR_IN_DATA_TYPE errorType;
} STAR_ERROR_IN_DATA_INJECT;


/**
 * \ingroup items
 * An enum used to define different types of timestamps that can be received.
 *
 * \versionAdded \ref changes_v3_6
 */
typedef enum
{
    /** Timestamp for data. */
    STAR_TIMESTAMP_TYPE_DATA,

    /** Timestamp for link state event. */
    STAR_TIMESTAMP_TYPE_LINK_STATE,

    /** Timestamp for link speed event. */
    STAR_TIMESTAMP_TYPE_LINK_SPEED,

    /** Timestamp for time-code. */
    STAR_TIMESTAMP_TYPE_TIMECODE,

    /** Timestamp for error in data. */
    STAR_TIMESTAMP_TYPE_ERROR
} STAR_TIMESTAMP_TYPE;


/**
 * \ingroup items
 * An enum used to define the direction of a timestamp.
 *
 * \versionAdded \ref changes_v3_6
 */
typedef enum
{
    /** Timestamp is applied to transmitted data */
    STAR_TIMESTAMP_DIRECTION_TRANSMIT,

    /** Timestamp is applied to received data */
    STAR_TIMESTAMP_DIRECTION_RECEIVE
} STAR_TIMESTAMP_DIRECTION;


/**
 * \ingroup items
 * A structure used to describe a timestamp on receipt of data on the link.
 *
 * \versionAdded \ref changes_v3_6
 */
typedef struct
{
    /** The system clock frequency of the device. */
    U32 systemClockFrequency;

    /** The type of data that the timestamp represents. */
    STAR_TIMESTAMP_TYPE type;

    /** Whether timestamp relates to tansmitted or received data. */
    STAR_TIMESTAMP_DIRECTION direction;

    /** The number of synchronisation pulses when the start of the packet was
        transmitted or received. */
    U32 startSyncPulseCount;

    /** The number of clock cycles counted when the start of the packet was
        transmitted or received. */
    U32 startClockCycleCount;

    /** The number of clock cycles counted when the last synchronisation
        pulse before the start of the packet was received. */
    U32 startTotalCycleCount;

    /** The number of synchronisation pulses when the end of the packet was
        transmitted or received. */
    U32 endSyncPulseCount;

    /** The number of clock cycles counted when the end of the packet was
        transmitted or received. */
    U32 endClockCycleCount;

    /** The number of clock cycles counted when the last synchronisation
        pulse before the end of the packet was received. */
    U32 endTotalCycleCount;

} STAR_TIMESTAMP_EVENT;


/**
 * \ingroup items
 * An enum used to define the status flags of a broadcast message.
 *
 * \versionAdded \ref changes_v4_00
 */
typedef enum
{
    /** The LATE flag of a broadcast message status field */
    STAR_BROADCAST_MESSAGE_STATUS_FLAG_LATE = 0x1,

    /** The DELAYED flag of a broadcast message status field */
    STAR_BROADCAST_MESSAGE_STATUS_FLAG_DELAYED = 0x2

} STAR_BROADCAST_MESSAGE_STATUS_FLAG;

/**
 * \ingroup items
 * A structure used to describe a SpaceFibre broadcast message.
 *
 * \versionAdded \ref changes_v4_00
 */
typedef struct
{
    /** The broadcast message's channel (BC field) */
    U8 channel;

    /** The broadcast message's type (B_TYPE field). */
    U8 type;

    /** The broadcast message's status (STATUS field). */
    U8 status;

    /** The broadcast message's first data word */
    U32 dataWord1;

    /** The broadcast message's second data word */
    U32 dataWord2;

} STAR_BROADCAST_MESSAGE;


/**
 * \ingroup items
 * A wrapper for Stream Item objects.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef struct STAR_STREAM_ITEM
{
    /** The type of stream item pointed to by item */
    STAR_STREAM_ITEM_TYPE itemType;

    /** A stream item object */
    void *item;

    /**
     * \internal
     * The receive operation that the stream item was received by, or NULL if it
     * wasn't received by a receive operation.
     */
    void *pReceivedOperation;

    /**
     * \internal
     * The next stream item in the list of stream items, if this is a receive
     * operation stream item.
     */
    struct STAR_STREAM_ITEM *pNext;

} STAR_STREAM_ITEM;

#ifdef __cplusplus
}
#endif

#endif  /* STAR_API_STREAM_ITEM_TYPES_H */
