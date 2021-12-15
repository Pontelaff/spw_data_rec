/**
 * \file stream_item.h
 *
 * \brief  Functions create and modify items that can be sent over a SpaceWire network
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Copyright &copy; 2012 STAR-Dundee Ltd.
 */

#ifndef STAR_API_STREAM_ITEM_H
#define STAR_API_STREAM_ITEM_H

#include "common.h"
#include "stream_item_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup items
 * Disposes of a given stream item.  All stream items which have been explicitly
 * created, e.g. by calls to \ref STAR_createDataChunk(),
 * \ref STAR_createPacket(), etc. should be destroyed using this function.
 *
 * Stream items returned by \ref STAR_getTransferItem() can optionally be
 * disposed using this function.  If these stream items are not disposed, they
 * will continue to exist until the receive operation is disposed of by calling
 * STAR_disposeTransferOperation() or when the receive operation is resubmitted.
 * For infinite receive operations (or receives for large numbers of packets) it
 * is necessary to destroy the received stream items to ensure the PC's memory
 * is not filled.
 *
 * @param[in] item    Pointer to  item to destroy
 *
 * \versionAdded \ref changes_v0_8
 */
void STAR_API_CC STAR_destroyStreamItem(
    _In_ _Post_ptr_invalid_ STAR_STREAM_ITEM* item);


/*********** SpaceWire Packets ************/

/**
 * \ingroup items
 * Creates a new SpaceWire packet stream item from user provided data.  This
 * stream item should be destroyed when it is no longer required by calling
 * STAR_destroyStreamItem().
 *
 * @param[in] address  Optional pointer to address created with STAR_createAddress
 *                     \note It is safe to dispose of this buffer
 *                           after this function completes.
 * @param[in] data     Optional pointer to data buffer
 *                     \note The contents of this buffer are copied into data structures
 *                           managed by the API. It is safe to dispose of this buffer
 *                           after this function completes.
 * @param     dataLen  Length of the data buffer
 * @param     eopType  End of packet marker type for the packet (may be none)
 *
 * @return SpaceWire Packet item
 *
 * \versionAdded \ref changes_v0_8
 */
_Check_return_ STAR_STREAM_ITEM * STAR_API_CC STAR_createPacket(
                _In_opt_ STAR_SPACEWIRE_ADDRESS *address,
                _In_opt_count_(dataLen)  unsigned char *data,
                unsigned int dataLen,
                STAR_EOP_TYPE eopType);

/**
 * \ingroup items
 *
 * Creates a SpaceWire address. The address should be destroyed when it is no
 * longer required using STAR_destroyAddress().
 *
 * @param[in] path  Address path
 *                     \note The contents of this buffer are copied into data structures
 *                           managed by the API. It is safe to dispose of this buffer
 *                           after this function completes.
 * @param pathLen        Length of the path buffer
 *
 * @return Pointer to newly created address
 *
 * \versionAdded \ref changes_v0_8
 */
_Check_return_ STAR_SPACEWIRE_ADDRESS* STAR_API_CC STAR_createAddress(
    _In_count_(pathLen) unsigned char* path, U16 pathLen);

/**
 * \ingroup items
 *
 * Disposes of  a SpaceWire address created with STAR_createAddress().
 *
 * @param[in] address    SpaceWire address to destroy
 *
 * \versionAdded \ref changes_v0_8
 */
void STAR_API_CC STAR_destroyAddress(
    _In_ _Post_ptr_invalid_ STAR_SPACEWIRE_ADDRESS* address);

/**
 * \ingroup items
 * Updates the address of a given packet.
 *
 * @param[in] packet        SpaceWire packet
 * @param[in] address       SpaceWire packet
 *
 * @return                1 if the update was successful, else 0
 *
 * \versionAdded \ref changes_v0_8
 */
int STAR_API_CC STAR_setPacketAddress(_Inout_ STAR_SPACEWIRE_PACKET* packet,
    _In_ STAR_SPACEWIRE_ADDRESS* address);

/**
 * \ingroup items
 * Gets the packet's data as an array of bytes.
 * This function copies the data from the various chunks that make up a packet into
 * a new array.
 *
 * @param[in]  packet            SpaceWire packet
 * @param[out] dataLength        Pointer to value to be updated to be size of array
 *
 * @return                Pointer to data, or NULL if no data is present
 *
 *  \note As this function creates a new buffer to store the data, this buffer
 *        must be freed with a call to STAR_destroyPacketData() when it is no
 *        longer in use.
 *
 * \versionAdded \ref changes_v0_8
 */
_Ret_opt_bytecap_x_(*dataLength) unsigned char* STAR_API_CC STAR_getPacketData(
    _In_ STAR_SPACEWIRE_PACKET* packet, _Out_ unsigned int* dataLength);

/**
 * \ingroup items
 * Frees packet data previously created by a call to STAR_getPacketData().
 *
 * @param pData the data to be freed
 *
 * \versionAdded \ref changes_v1_1
 */
void STAR_API_CC STAR_destroyPacketData(
    _In_ _Post_ptr_invalid_ unsigned char *pData);

/**
 * \ingroup items
 * Gets a copy of a packet's address structure. Note that received packets
 * will not have an address structure set. This function is only useful for
 * accessing the address member of a packet already created by the user.
 *
 *  \note As this function creates a copy of the address, this must be freed
 *        with a call to STAR_destroyAddress() when it is no longer in use.
 *
 * @param[in] packet         Packet to get address member from.
 *
 * @return  Pointer to copy of packet's address member.
 *
 * \versionAdded \ref changes_v1_12
 */
_Ret_opt_cap_c_(1) STAR_SPACEWIRE_ADDRESS* STAR_API_CC STAR_getPacketAddress(
                    _In_ STAR_SPACEWIRE_PACKET* packet);

/**
 * \ingroup items
 * Gets the length of a given packet.
 *
 * @param[in] packet      SpaceWire packet
 *
 * @return                The length in bytes of the packet (data + address)
 *
 * \versionAdded \ref changes_v0_8
 */
unsigned int STAR_API_CC STAR_getPacketLength(
    _In_ STAR_SPACEWIRE_PACKET* packet);

/**
 * \ingroup items
 * Gets the end of packet marker type of a packet.
 *
 * @param packet        SpaceWire packet
 *
 * @return              The EOP type of the packet
 *
 * \versionAdded \ref changes_v1_1
 */
STAR_EOP_TYPE STAR_API_CC STAR_getPacketEOP(_In_ STAR_SPACEWIRE_PACKET* packet);


/**
 * \ingroup items
 * Sets the end of packet marker type of a packet.
 *
 * @param[in]  packet        SpaceWire packet
 * @param      eopType       End of packet marker type
 *
 * @return 1 if EOP was successfully set, else 0
 *
 * \versionAdded \ref changes_v1_1
 */
int STAR_API_CC STAR_setPacketEOP(_In_ STAR_SPACEWIRE_PACKET* packet,
    STAR_EOP_TYPE eopType);

/*********** Data Chunks ************/

/**
 * \ingroup items
 * Creates a data chunk stream item, used to refer to part of a packet.  This
 * stream item should be destroyed when it is no longer required by calling
 * STAR_destroyStreamItem().
 *
 * @param[in] data       Pointer to start of chunk data
 *                            \note The contents of this buffer are copied into
 *                             data structures managed by the API. It is safe
 *                             to dispose of this buffer after this function
 *                             completes.
 * @param    dataLen    The length of the chunk. The maximum size permitted
 *                      for a data chunk is 0xffff. Attempts to create a chunk
 *                      larger than this will fail.
 * @param    isStart    Whether the chunk is the start of a packet (1) or not (0)
 * @param    eopType    The end of packet marker type for the chunk (can be No EOP)
 *
 * @return              Pointer to newly created data chunk, or NULL if an
 *                      error occurred.
 *
 * \versionAdded \ref changes_v0_8
 */
_Check_return_ STAR_STREAM_ITEM * STAR_API_CC STAR_createDataChunk(
                    _In_opt_count_(dataLen)  unsigned char *data,
                    unsigned int dataLen,
                    int isStart,
                    STAR_EOP_TYPE eopType);

/**
 * \ingroup items
 * Gets a pointer to a data chunk stream item's data.
 *
 * @param chunk         Data chunk
 * @param len           Pointer to user provided value that will be updated to
 *                      the length of the returned data
 *
 * @return               Pointer to data, or NULL if no data is present. Note
 *                      that this is a pointer to the chunk's data, not a copy
 *                      of the data, so the data should not be freed, and will
 *                      cease to exist if the data chunk is destroyed.
 *
 * \versionAdded \ref changes_v0_8
 * \versionChanged \ref changes_v1_1
 */
unsigned char* STAR_API_CC STAR_getChunkData(_In_ STAR_DATA_CHUNK* chunk,
    _Out_ unsigned int* len);

/**
 * \ingroup items
 * Gets the length of a data chunk stream item's data.
 *
 * @param chunk           Data chunk
 *
 * @return                Length of data member
 *
 * \versionAdded \ref changes_v0_8
 */
unsigned int STAR_API_CC STAR_getChunkDataLength(_In_ STAR_DATA_CHUNK* chunk);

/**
 * \ingroup items
 * Gets the End Of Packet marker type of a data chunk stream item.
 *
 * @param chunk         Data chunk
 *
 * @return              The EOP type of the chunk or STAR_EOP_TYPE_INVALID if an if an invalid
 *                      chunk was passed in
 *
 * \versionAdded \ref changes_v0_8
 */
STAR_EOP_TYPE STAR_API_CC STAR_getChunkEop(_In_ STAR_DATA_CHUNK* chunk);

/**
 * \ingroup items
 * Gets whether a data chunk stream item is at the start of a packet.
 *
 * @param chunk         Data chunk
 *
 * @return              Whether the chunk is at the start of a packet (1) or not (0), or -1 if an invalid
 *                      chunk was passed in
 *
 * \versionAdded \ref changes_v0_8
 */
int STAR_API_CC STAR_getChunkSop(_In_ STAR_DATA_CHUNK* chunk);


/*********** Time Codes ************/

/**
 * \ingroup items
 * Creates a Time-code stream item.  This stream item should be destroyed when
 * it is no longer required by calling STAR_destroyStreamItem().
 *
 * @param value           Time-code value
 *
 * @return                Time-code stream item
 *
 * \versionAdded \ref changes_v0_8
 */
STAR_STREAM_ITEM * STAR_API_CC STAR_createTimeCode(U8 value);

/**
 * \ingroup items
 * Gets the value of a given time-code stream item.
 *
 * @param pTimeCode       Time-code to get value of
 *
 * @return                Time-code value
 *
 * \versionAdded \ref changes_v0_8
 */
U8 STAR_API_CC STAR_getTimeCodeValue(_In_ STAR_TIMECODE *pTimeCode);

/**
 * \ingroup items
 * Sets the value of a given time-code stream item.
 *
 * @param pTimeCode        time-code to set value of
 * @param value            new value
 *
 * @return                none
 *
 * \versionAdded \ref changes_v0_8
 */
void STAR_API_CC STAR_setTimeCodeValue(_In_ STAR_TIMECODE *pTimeCode, U8 value);


/*********** Link errors ************/

/**
 * \ingroup items
 * Creates an error in data stream item.  This stream item can be transmitted to
 * cause an error to occur on the next data character to be transmitted.  The
 * stream item should be destroyed when it is no longer required by calling
 * STAR_destroyStreamItem().
 *
 * @param errorType       the type of error to inject.
 *
 * @return                Error in data stream item.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
STAR_STREAM_ITEM * STAR_API_CC STAR_createErrorInData(
    STAR_ERROR_IN_DATA_TYPE errorType);


/*********** Link state events ************/

/**
 * \ingroup items
 * Gets the count of a given link state event stream item, indicating the number
 * of times the specified event has occurred.
 *
 * @param pLinkStateEvent the link state event to get the count for
 *
 * @return the link state event's count
 *
 * \versionAdded \ref changes_v2_0_beta_1
 */
U8 STAR_API_CC STAR_getLinkStateEventCount(
    _In_ STAR_LINK_STATE_EVENT *pLinkStateEvent);

/**
 * \ingroup items
 * Gets the port related to a given link state change event stream item.
 *
 * @param pLinkStateEvent the link state event to get the port for
 *
 * @return the port related to the link state event
 *
 * \versionAdded \ref changes_v4_01
 */
U8 STAR_API_CC STAR_getLinkStateEventPort(
    _In_ STAR_LINK_STATE_EVENT *pLinkStateEvent);

/**
 * \ingroup items
 * Gets whether a link state event stream item includes a disconnect error.
 *
 * @param pLinkStateEvent the link state event to check for a disconnect error
 *
 * @return whether the link state event includes a disconnect error
 *
 * \versionAdded \ref changes_v2_0_beta_1
 */
int STAR_API_CC STAR_isLinkStateEventDisconnectError(
    _In_ STAR_LINK_STATE_EVENT *pLinkStateEvent);

/**
 * \ingroup items
 * Gets whether a link state event stream item includes an escape error.
 *
 * @param pLinkStateEvent the link state event to check for an escape error
 *
 * @return whether the link state event includes an escape error
 *
 * \versionAdded \ref changes_v2_0_beta_1
 */
int STAR_API_CC STAR_isLinkStateEventEscapeError(
    _In_ STAR_LINK_STATE_EVENT *pLinkStateEvent);

/**
 * \ingroup items
 * Gets whether a link state event stream item includes a link running state
 * change.
 *
 * @param pLinkStateEvent the link state event to check for a link running state
 *                        change
 *
 * @return whether the link state event includes a link running state change
 *
 * \versionAdded \ref changes_v2_0_beta_1
 */
int STAR_API_CC STAR_isLinkStateEventLinkRunning(
    _In_ STAR_LINK_STATE_EVENT *pLinkStateEvent);

/**
 * \ingroup items
 * Gets whether a link state event stream item includes a parity error.
 *
 * @param pLinkStateEvent the link state event to check for a parity error
 *
 * @return whether the link state event includes a parity error
 *
 * \versionAdded \ref changes_v2_0_beta_1
 */
int STAR_API_CC STAR_isLinkStateEventParityError(
    _In_ STAR_LINK_STATE_EVENT *pLinkStateEvent);

/**
 * \ingroup items
 * Gets whether a link state event stream item includes a receive credit error.
 *
 * @param pLinkStateEvent the link state event to check for a receive credit
 *                        error
 *
 * @return whether the link state event includes a receive credit error
 *
 * \versionAdded \ref changes_v2_0_beta_1
 */
int STAR_API_CC STAR_isLinkStateEventReceiveCreditError(
    _In_ STAR_LINK_STATE_EVENT *pLinkStateEvent);

/**
 * \ingroup items
 * Gets whether a link state event stream item includes a transmit credit error.
 *
 * @param pLinkStateEvent the link state event to check for a transmit credit
 *                        error
 *
 * @return whether the link state event includes a transmit credit error
 *
 * \versionAdded \ref changes_v2_0_beta_1
 */
int STAR_API_CC STAR_isLinkStateEventTransmitCreditError(
    _In_ STAR_LINK_STATE_EVENT *pLinkStateEvent);

/**
 * \ingroup items
 * Gets the link speed of a given link speed change event stream item.
 *
 * @param pLinkSpeedEvent the link speed event to get the link speed from
 *
 * @return the link speed from the link speed event
 *
 * \versionAdded \ref changes_v2_0_beta_1
 */
U32 STAR_API_CC STAR_getLinkSpeedEventSpeed(
    _In_ STAR_LINK_SPEED_EVENT *pLinkSpeedEvent);

/**
 * \ingroup items
 * Gets the port related to a given link speed change event stream item.
 *
 * @param pLinkSpeedEvent the link speed event to get the port for
 *
 * @return the port related to the link speed event
 *
 * \versionAdded \ref changes_v4_01
 */
U8 STAR_API_CC STAR_getLinkSpeedEventPort(
    _In_ STAR_LINK_SPEED_EVENT *pLinkSpeedEvent);


/*********** Timestamps ************/

/**
 * \ingroup items
 * Gets the start time value from a timestamp event stream item.  This value
 * relates to the start of the data being received in whole seconds and the
 * remainder in nanoseconds.
 *
 * @param pTimestampEvent the timestamp event to get the timestamp from
 * @param syncPulseFrequency the frequency of synchronisation pulses in hertz
 * @param[out] pSeconds outputs the number of whole seconds of the timestamp
 * @param[out] pRemainder outputs the remainder in nanoseconds
 *
 * \versionAdded \ref changes_v3_6
 */
void STAR_API_CC STAR_getTimestampEventStartValue(
    _In_ STAR_TIMESTAMP_EVENT *pTimestampEvent, U32 syncPulseFrequency,
    _Out_ U32 *pSeconds, _Out_ U32 *pRemainder);

/**
 * \ingroup items
 * Gets the end time value from a timestamp event stream item.  This value
 * relates to the end of the data being received in whole seconds and the
 * remainder in nanoseconds.
 *
 * @param pTimestampEvent the timestamp event to get timestamp from
 * @param syncPulseFrequency the frequency of synchronisation pulses in hertz
 * @param[out] pSeconds outputs the number of whole seconds of the timestamp
 * @param[out] pRemainder outputs the remainder in nanoseconds
 *
 * \versionAdded \ref changes_v3_6
 */
void STAR_API_CC STAR_getTimestampEventEndValue(
    _In_ STAR_TIMESTAMP_EVENT *pTimestampEvent, U32 syncPulseFrequency,
    _Out_ U32 *pSeconds, _Out_ U32 *pRemainder);

/**
 * \ingroup items
 * Gets the timestamp type information from a timestamp event stream item.
 *
 * @param pTimestampEvent the timestamp event to get the type of
 *
 * @return the timestamp type, or -1 if an invalid timestamp was passed in
 *
 * \versionAdded \ref changes_v3_6
 */
STAR_TIMESTAMP_TYPE STAR_API_CC STAR_getTimestampEventType(
    _In_ STAR_TIMESTAMP_EVENT *pTimestampEvent);

/**
 * \ingroup items
 * Gets the timestamp direction information from a timestamp event stream item.
 *
 * @param pTimestampEvent the timestamp event to get the direction of
 *
 * @return the timestamp direction, or -1 if an invalid timestamp was passed in
 *
 * \versionAdded \ref changes_v3_6
 */
STAR_TIMESTAMP_DIRECTION STAR_API_CC STAR_getTimestampEventDirection(
    _In_ STAR_TIMESTAMP_EVENT *pTimestampEvent);

/**
 * \ingroup items
 * Gets the timestamp counter values as returned by the hardware, from a
 * timestamp event stream item.
 *
 * @param pTimestampEvent the timestamp event to get timestamp from
 * @param[out] pStartSyncPulseCount outputs the start sync pulse counter
 * @param[out] pStartClockCycleCount outputs the start clock cycle counter
 * @param[out] pStartTotalCycleCount outputs the start clock cycle counter
 *             value recorded at the previous sync pulse
 * @param[out] pEndSyncPulseCount outputs the end sync pulse counter
 * @param[out] pEndClockCycleCount outputs the end sync pulse counter
 * @param[out] pEndTotalCycleCount outputs the end clock cycle counter value
 *             recorded at the previous sync pulse
 * @param[out] pClockFrequency outputs the clock frequency
 *
 * \versionAdded \ref changes_v3_6
 */
void STAR_API_CC STAR_getTimestampEventRawCounters(
    _In_ STAR_TIMESTAMP_EVENT *pTimestampEvent,
    _Out_opt_ U32 *pStartSyncPulseCount, _Out_opt_ U32 *pStartClockCycleCount,
    _Out_opt_ U32 *pStartTotalCycleCount, _Out_opt_ U32 *pEndSyncPulseCount,
    _Out_opt_ U32 *pEndClockCycleCount, _Out_opt_ U32 *pEndTotalCycleCount,
    _Out_opt_ U32 *pClockFrequency);

/*********** Broadcast Messages ************/

/**
 * \ingroup items
 * Creates a broadcast message stream item.  This stream item should be
 * destroyed when it is no longer required by calling STAR_destroyStreamItem().
 *
 * @param channel           The broadcast message channel
 * @param type              The broadcast message type
 * @param status            The broadcast message status
 * @param dataWord1         The first data word of the broadcast message
 * @param dataWord2         The second data word of the broadcast message
 *
 * @return                  Pointer to newly created broadcast message,
 *                          or NULL if an error occurred.
 *
 * \versionAdded \ref changes_v4_00
 */
STAR_STREAM_ITEM *STAR_API_CC STAR_createBroadcastMessage(_In_ U8 channel,
    _In_ U8 type, _In_ U8 status, _In_ U32 dataWord1, _In_ U32 dataWord2);

/**
 * \ingroup items
 * Gets the broadcast message channel from a broadcast message stream item.
 *
 * @param pBroadcastMessage the broadcast message
 *
 * @return the broadcast message's channel
 *
 * \versionAdded \ref changes_v4_00
 */
U8 STAR_API_CC STAR_getBroadcastMessageChannel(
    _In_ STAR_BROADCAST_MESSAGE *pBroadcastMessage);

/**
 * \ingroup items
 * Gets the broadcast message type from a broadcast message stream item.
 *
 * @param pBroadcastMessage the broadcast message
 *
 * @return the broadcast message's type
 *
 * \versionAdded \ref changes_v4_00
 */
U8 STAR_API_CC STAR_getBroadcastMessageType(
    _In_ STAR_BROADCAST_MESSAGE *pBroadcastMessage);

/**
 * \ingroup items
 * Gets the broadcast message status from a broadcast message stream item.
 *
 * @param pBroadcastMessage the broadcast message
 *
 * @return the broadcast message's status
 *
 * \versionAdded \ref changes_v4_00
 */
U8 STAR_API_CC STAR_getBroadcastMessageStatus(
    _In_ STAR_BROADCAST_MESSAGE *pBroadcastMessage);

/**
 * \ingroup items
 * Gets the broadcast message late flag from a broadcast message stream item.
 *
 * @param pBroadcastMessage the broadcast message
 *
 * @return the broadcast message's late flag
 *
 * \versionAdded \ref changes_v4_00
 */
U8 STAR_API_CC STAR_getBroadcastMessageLateFlag(
    _In_ STAR_BROADCAST_MESSAGE *pBroadcastMessage);

/**
 * \ingroup items
 * Gets the broadcast message delayed flag from a broadcast message stream item.
 *
 * @param pBroadcastMessage the broadcast message
 *
 * @return the broadcast message's delayed flag
 *
 * \versionAdded \ref changes_v4_00
 */
U8 STAR_API_CC STAR_getBroadcastMessageDelayedFlag(
    _In_ STAR_BROADCAST_MESSAGE *pBroadcastMessage);

/**
 * \ingroup items
 * Gets the broadcast message first data word from a broadcast message stream
 * item.
 *
 * @param pBroadcastMessage the broadcast message
 *
 * @return the broadcast message's first data word
 *
 * \versionAdded \ref changes_v4_00
 */
U32 STAR_API_CC STAR_getBroadcastMessageDataWord1(
    _In_ STAR_BROADCAST_MESSAGE *pBroadcastMessage);

/**
 * \ingroup items
 * Gets the broadcast message second data word from a broadcast message stream
 * item.
 *
 * @param pBroadcastMessage the broadcast message
 *
 * @return the broadcast message's second data word
 *
 * \versionAdded \ref changes_v4_00
 */
U32 STAR_API_CC STAR_getBroadcastMessageDataWord2(
    _In_ STAR_BROADCAST_MESSAGE *pBroadcastMessage);

#ifdef __cplusplus
}
#endif

#endif    /* STAR_API_STREAM_ITEM_H */
