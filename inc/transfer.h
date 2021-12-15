/**
 * \file transfer.h
 *
 * \brief Declarations of functions provided by STAR-API relating to transfer
 *        operations.
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

#ifndef STAR_API_TRANSFER_OPERATIONS_H
#define STAR_API_TRANSFER_OPERATIONS_H

#include "types.h"
#include "transfer_types.h"
#include "stream_item_types.h"

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \ingroup transfers
 * Receive a single packet on a previously opened channel in to the buffer
 * specified.  This function is provided to simplify the process of receiving
 * packets, but is not as efficient or as flexible as using
 * STAR_createRxOperation(), STAR_submitTransferOperation(), etc.  This function
 * should not be used if high performance or low CPU utilisation is required.
 * Note that if the received packet is longer than the buffer provided, the end
 * of the packet will be dropped and the end of packet marker will indicate
 * there was no end of packet marker.  Note also that a limitation of this
 * function is that the operation may timeout, but a packet is consumed and will
 * not be received in a subsequent call to the function.  To work around these
 * issues use the alternative functions mentioned above.
 *
 * @param channelId     The identifier of a previously opened channel.  The
 *                      channel must have been opened as an in or in/out channel
 * @param pPacketData   Pointer to a previously allocated buffer which will be
 *                      updated to contain the received packet
 * @param pPacketLength Pointer to a variable which should contain the length
 *                      of the buffer, and which will be updated to contain the
 *                      length of the packet
 * @param pEopType      Pointer to a variable which will be updated to contain
 *                      the end of packet marker type of the packet
 * @param timeout       The maximum time in milliseconds to wait for the packet
 *                      to be received, or -1 to wait indefinitely
 *
 * @return the status of the operation
 *
 * \versionAdded \ref changes_v1_1
 */
STAR_TRANSFER_STATUS STAR_API_CC STAR_receivePacket(
    _In_ STAR_CHANNEL_ID channelId,
    _Out_bytecap_(*pPacketLength) void *pPacketData,
    _Inout_ unsigned int *pPacketLength, _Out_ STAR_EOP_TYPE *pEopType,
    _In_ int timeout);


/**
 * \ingroup transfers
 * Transmit a single packet on a previously opened channel from the buffer
 * specified.  This function is provided to simplify the process of transmitting
 * packets, but is not as efficient or as flexible as using
 * STAR_createTxOperation(), STAR_submitTransferOperation(), etc. This function
 * should not be used if high performance or low CPU utilisation is required.
 *
 *
 * @param channelId    The identifier of a previously opened channel.  The
 *                     channel must have been opened as an out or in/out channel
 * @param pPacketData  A pointer to a previously allocated buffer which should
 *                     contain the packet to be transmitted
 * @param packetLength The length of the buffer, and therefore the length of the
 *                     packet
 * @param eopType      The end of packet marker type to be added to the end of
 *                     the packet
 * @param timeout      The maximum time in milliseconds to wait for the packet
 *                     to be transmitted, or -1 to wait indefinitely
 *
 * @return the status of the operation
 *
 * \versionAdded \ref changes_v1_1
 */
STAR_TRANSFER_STATUS STAR_API_CC STAR_transmitPacket(
    _In_ STAR_CHANNEL_ID channelId,
    _In_opt_bytecount_(packetLength) void *pPacketData,
    _In_ unsigned int packetLength, _In_ STAR_EOP_TYPE eopType,
    _In_ int timeout);


/**
 * \ingroup transfers
 * Creates a transmit operation that can be submitted later.  The transmit
 * operation returned should be disposed of by calling
 * STAR_disposeTransferOperation() when it is no longer required.
 *
 * @param ppItems               Array of pointers to Stream Items to be sent
 *                              \note It is safe to free this array after the
 *                                    function has completed.
 * @param streamItemCount       Count of the items in the ppItems array
 *
 * @return            Pointer to a STAR_TRANSFER_OPERATION
 *
 * \versionAdded \ref changes_v0_8
 */
_Check_return_ STAR_TRANSFER_OPERATION* STAR_API_CC STAR_createTxOperation(
    _In_count_(streamItemCount) STAR_STREAM_ITEM **ppItems,
    unsigned int streamItemCount);

/**
 * \ingroup transfers
 * Creates a receive operation that can then be submitted.  The receive
 * operation returned should be disposed of by calling
 * STAR_disposeTransferOperation() when it is no longer required.
 *
 * @param    itemCount      The maximum number of stream items to receive (the
 *                          size of an individual stream item is not limited).
 *                          This can be -1 to receive an unlimited number of
 *                          items
 *                          \note When a receive operation receiving an
 *                          unlimited (or very large) number of items, the
 *                          received stream items must be destroyed to ensure
 *                          the received stream items do not consume all the
 *                          PC's memory.
 * @param   mask            Bitmask with flags set for the type of traffic one
 *                          wishes to receive
 *                          \note It is not possible to receive whole packets at
 *                          the same time as link control tokens/data chunks
 *
 * @return        a pointer to a #STAR_TRANSFER_OPERATION object
 *
 * \versionAdded \ref changes_v0_8
 * \versionChanged \ref changes_v1_1
 */
_Check_return_ STAR_TRANSFER_OPERATION* STAR_API_CC STAR_createRxOperation(
    int itemCount, STAR_RECEIVE_MASK mask);

/**
 * \ingroup transfers

 * This function is an alternative to STAR_createRxOperation that creates
 * receive operations to be submitted to a channel opened with the
 * STAR_openChannelToLocalDeviceEx function.
 *
 * Using the alternative receive operations, data is copied directly into
 * pre-allocate buffers instead of dynamically allocated packets.
 *
 * Note that receive operations created with STAR_createRxOperation should
 * not be submitted to a channel opened using STAR_openChannelToLocalDeviceEx.
 * Similarly, receive operations created with STAR_createRxOperationEx should
 * not be submitted to a channel opened using STAR_openChannelToLocalDevice.
 *
 * The receive operation returned should be disposed of by calling
 * STAR_disposeTransferOperation() when it is no longer required.
 *
 * @param ppBuffers         An array of pointers to pre-allocated buffers
 *                          that received stream items will be written to.
 * @param numBuffers        The number of buffers in the ppBuffers array.
 * @param bufferSize        The size of the buffers in the ppBuffers array.
 *                          \note the size of the buffers should be large
 *                          enough to receive the largest chunk generated by
 *                          the hardware, which is generally 4096 or 8192 bytes.
 * @param mask              Bitmask with flags set for the type of traffic one
 *                          wishes to receive.
 *                          \note Due to the use of fixed-size pre-allocated
 *                          buffers, STAR_RECEIVE_CHUNK should be used to
 *                          receive data instead of STAR_RECEIVE_PACKET.
 *
 * @return        a pointer to a #STAR_TRANSFER_OPERATION
 *
 * \versionAdded \ref changes_v4_00
 */
_Check_return_ STAR_TRANSFER_OPERATION* STAR_API_CC STAR_createRxOperationEx(
    _In_count_(numBuffers) U8 **ppBuffers, _In_ unsigned int numBuffers,
    _In_ unsigned int bufferSize, _In_ STAR_RECEIVE_MASK mask);

/**
* \ingroup transfers
*
 * Submits a single transfer operation.  Once a transfer operation has
 * completed, it can be resubmitted.  This can be useful for receiving a number
 * of packets in a loop, or transmitting the same packet repeatedly.  Note that
 * if an operation is resubmitted before it has completed, the original
 * operation will be cancelled and then the new one submitted.
 *
 * @param     channelId        Channel to submit operation on
 * @param[in] transferOp       #STAR_TRANSFER_OPERATION to submit
 *
 * @return      1 if operation was successfully submitted, else 0
 *
 * \versionAdded \ref changes_v0_8
 * \versionChanged \ref changes_v1_1
 */
int STAR_API_CC STAR_submitTransferOperation(
    _In_ STAR_CHANNEL_ID channelId,
    _In_ STAR_TRANSFER_OPERATION *transferOp);
/**
 * \ingroup transfers
 *
 * Submits a list of transfer operations.  Once a transfer operation has
 * completed, it can be resubmitted.  This can be useful for receiving a number
 * of packets in a loop, or transmitting the same packet repeatedly.  Note that
 * if an operation is resubmitted before it has completed, the original
 * operation will be cancelled and then the new one submitted.  This means it is
 * not useful to submit a list containing multiple instances of the same
 * operation.
 *
 * @param        channelId      Channel to submit operations on
 * @param[in]     transferOps   Array of pointers to #STAR_TRANSFER_OPERATION to be submitted
 * @param        count          Count of operations in array
 *
 * @return      1 if all operations were successfully submitted, else 0
 *
 * \versionAdded \ref changes_v0_8
 * \versionChanged \ref changes_v1_1
 */
int STAR_API_CC STAR_submitTransferOperationList(
    STAR_CHANNEL_ID channelId,
    _In_count_(count) STAR_TRANSFER_OPERATION **transferOps,
    unsigned int count);

/**
 * \ingroup transfers
 *
 * Blocks until a given transfer operation has completed, or the timeout period
 * expires.  Note that the operation will continue to transmit or receive once
 * this function returns if it has not yet completed.  The operation will only
 * be stopped if it completes, there is an error, or it is cancelled by calling
 * STAR_cancelTransferOperation().  If the operation passed to this function has
 * been submitted but has not yet completed when the function returns, the
 * return value will be #STAR_TRANSFER_STATUS_STARTED as the operation has
 * started but not yet completed.  This function can be called multiple times
 * for the same operation to wait until that operation completes.
 *
 * @param[in] pTransferOperation    Operation to wait on
 * @param     timeout               Max time in milliseconds to wait, or -1 to
 *                                  wait indefinitely
 *
 * @return        result status of the operation
 *
 * \versionAdded \ref changes_v1_1
 */
STAR_TRANSFER_STATUS STAR_API_CC STAR_waitOnTransferOperationCompletion(
    _In_ STAR_TRANSFER_OPERATION *pTransferOperation, int timeout);

/**
* \ingroup transfers
 *
 * Gets the status of a transfer operation.
 *
 * @param pTransferOperation    Operation to get status of
 *
 * @return        result status of the operation
 *
 * \versionAdded \ref changes_v1_1
 */
STAR_TRANSFER_STATUS STAR_API_CC STAR_getTransferOperationStatus(
    _In_ STAR_TRANSFER_OPERATION *pTransferOperation);

/**
 * \ingroup transfers
 *
 * Cancels any waits in progress on a transfer operation.  Note that this does
 * not cancel the transfer operation it simply cancels any calls to
 * STAR_waitOnTransferOperationCompletion().
 *
 * @param pTransferOperation    Operation to cancel waits on
 *
 * \versionAdded \ref changes_v1_1
 */
void STAR_API_CC STAR_cancelTransferOperationWaits(
    _In_ STAR_TRANSFER_OPERATION *pTransferOperation);

/**
 * \ingroup transfers
 *
 * Gets the current count of stream items received by the operation and
 * available to be obtained using STAR_getTransferItem().  Note that this may be
 * less than the number of stream items received if some of those stream items
 * have been destroyed by calling STAR_destroyStreamItem() or
 * STAR_destroyTransferItemList() with the destroyItems parameter set to a
 * non-zero value.
 *
 * @param pReceiveOperation        Operation to get received item count for
 *
 * @return      Count of received stream items
 *
 * \versionAdded \ref changes_v0_8
 */
unsigned int STAR_API_CC STAR_getTransferItemCount(
    _In_ STAR_TRANSFER_OPERATION *pReceiveOperation);

/**
 * \ingroup transfers
 *
 * Gets the stream item at a given index of a receive operation.
 *
 * Stream items returned can optionally be disposed by calling
 * STAR_destroyStreamItem().  If these stream items are not disposed, they
 * will continue to exist until the receive operation is disposed of by calling
 * STAR_disposeTransferOperation() or when the receive operation is resubmitted.
 *
 * @param pReceiveOperation     Operation to get item from
 * @param index                 Item within operation to access
 *
 * @return      Item requested
 *
 * \versionAdded \ref changes_v0_8
 */
STAR_STREAM_ITEM* STAR_API_CC STAR_getTransferItem(
    _In_ STAR_TRANSFER_OPERATION *pReceiveOperation, unsigned int index);

/**
 * \ingroup transfers
 *
 * Gets the next stream item in the list of received stream items.
 * This is quicker than using STAR_getTransferItem().
 *
 * @param pStreamItem    Current stream item
 *
 * @return               Next stream item
 *
 * \versionAdded \ref changes_v3_10
 */
STAR_STREAM_ITEM* STAR_API_CC STAR_getNextTransferItem(
    STAR_STREAM_ITEM* pStreamItem);

/**
 * \ingroup transfers
 *
 * Gets the entire list of transfer items associated with a receive.  The array
 * returned must be destroyed by calling STAR_destroyTransferItemList().
 *
 * @param[in]  pReceiveOperation    Operation to get items from
 * @param[out] count                Count of items in returned array
 *
 * @return      Array of pointers stream item associated with this receive
 *
 * \versionAdded \ref changes_v0_8
 */
_Ret_opt_cap_(*count) STAR_STREAM_ITEM** STAR_API_CC STAR_getTransferItemList(
    _In_ STAR_TRANSFER_OPERATION *pReceiveOperation, _Out_ unsigned int* count);

/**
 * \ingroup transfers
 * Destroys a given stream item array, returned by a call to
 * STAR_getTransferItemList(), optionally destroying each of the elements in the
 * array.  If the stream items are not destroyed they will continue to exist
 * until destroyed by calling STAR_destroyStreamItem() or by disposing of the
 * receive operation by calling STAR_disposeTransferOperation().    For
 * infinite receive operations (or receives for large numbers of packets) it is
 * necessary to destroy the received stream items to ensure the PC's memory is
 * not filled.
 *
 * @param[in] transferItemList    The array of stream items to destroy
 * @param transferItemListCount The number of elements in the array of stream
 *                              items
 * @param destroyItems If non-zero, each stream item will also be destroyed
 *
 * \versionAdded \ref changes_v3_0
 */
void STAR_API_CC STAR_destroyTransferItemList(
    _In_ _Post_ptr_invalid_ STAR_STREAM_ITEM **transferItemList,
    unsigned int transferItemListCount, int destroyItems);

/**
 * \ingroup transfers
 *
 * Cancels a given transfer operation.
 * \note It is not currently possible to cancel a transmit operation.
 *
 * @param pTransferOperation    Operation to cancel
 *
 * @return      1 if a receive was cancelled, else 0
 *
 * \versionAdded \ref changes_v1_1
 */
int STAR_API_CC STAR_cancelTransferOperation(
    _In_ STAR_TRANSFER_OPERATION *pTransferOperation);

/**
 * \ingroup transfers
 *
 * Cancels a transfer operation (if it is not already complete) and indicates
 * that the memory associated with the operation should be freed when the
 * operation is no longer in use.  Note that this is a dispose function rather
 * than a destroy function, as it only indicates to the API that the transfer
 * operation should be destroyed when it is possible to do so.  If the API is in
 * the process of transmitting or receiving traffic on the operation it may not
 * be possible to destroy the operation immediately.
 *
 * @param pTransferOperation    a pointer to the operation to be disposed of
 *
 * @return      1 if operation was disposed of successfully, otherwise 0
 *
 * \versionAdded \ref changes_v1_1
 */
int STAR_API_CC STAR_disposeTransferOperation(
    _In_ _Post_ptr_invalid_ STAR_TRANSFER_OPERATION *pTransferOperation);



/**
 * \ingroup transfers
 *
 * Registers a call-back function that will be called when an operation completes.
 *
 * \note Each callback is made using a separate thread, to ensure that STAR-System
 * internal processing is not blocked by long-running callbacks.  Due to operating
 * system scheduling it is not possible to guarantee that callbacks will execute in
 * the same order as they are triggered.
 *
 * @param pTransferOperation    Operation to add listener for
 * @param listenerFunc          Call-back function
 * @param pContextInfo          A pointer to memory which will be provided when
 *                              the call-back function is called
 *
 * @return    Non-Zero if call-back was registered successfully
 *
 * \versionAdded \ref changes_v0_8
 */
int STAR_API_CC STAR_registerTransferCompletionListener(
    _In_ STAR_TRANSFER_OPERATION *pTransferOperation,
    STAR_TransferOperationListenerFunc listenerFunc,
    _In_opt_ void* pContextInfo);

/**
 * \ingroup transfers
 *
 * Unregisters a call-back function that will be called when an operation completes.
 *
 *
 * @param pTransferOperation    Operation to remove listener for
 * @param listenerFunc          Call-back function
 *
 * @return    Non-Zero if call-back was unregistered successfully
 *
 * \versionAdded \ref changes_v0_8
 */
int STAR_API_CC STAR_unregisterTransferCompletionListener(
    _In_ STAR_TRANSFER_OPERATION *pTransferOperation,
    STAR_TransferOperationListenerFunc listenerFunc);

/**
 * \ingroup transfers
 *
 * Access function to get a SpaceWire address path.
 *
 * @param pSpaceWireAddress    A pointer to a STAR_SPACEWIRE_ADDRESS struct.
 *
 * @return    A pointer to a SpaceWire address path.
 *
 * \versionAdded \ref changes_v4_0_beta1
 */
U8* STAR_API_CC
STAR_getSpaceWireAddressPath(STAR_SPACEWIRE_ADDRESS* pSpaceWireAddress);

/**
 * \ingroup transfers
 *
 * Access function to get a SpaceWire address path length.
 *
 * @param pSpaceWireAddress    A pointer to a STAR_SPACEWIRE_ADDRESS struct.
 *
 * @return    A SpaceWire address path length.
 *
 * \versionAdded \ref changes_v4_0_beta1
 */
U16 STAR_API_CC
STAR_getSpaceWireAddressPathLength(STAR_SPACEWIRE_ADDRESS* pSpaceWireAddress);

/**
 * \ingroup transfers
 *
 * Gets the number of items received by an ex receive operation.
 *
 * @param pTransferOp   A pointer to a STAR_TRANSFER_OPERATION.
 *
 * @return      The number of items received by an ex receive operation, or
 *              -1 if an error occurred.
 *
 * \versionAdded \ref changes_v4_00
 */
int STAR_API_CC STAR_rxOperationExGetNumItems(
    _In_ STAR_TRANSFER_OPERATION *pTransferOp);

/**
 * \ingroup transfers
 *
 * Gets the stream item type for an item received by an ex receive operation.
 *
 * @param pTransferOp   A pointer to a STAR_TRANSFER_OPERATION.
 * @param item          The index of the item to get the stream item type for.
 *
 * @return      The stream item type for an item received by an ex receive
 *              operation.
 *
 * \versionAdded \ref changes_v4_00
 */
STAR_STREAM_ITEM_TYPE STAR_API_CC STAR_rxOperationExGetItemType(
    _In_ STAR_TRANSFER_OPERATION *pTransferOp, _In_ unsigned int item);

/**
 * \ingroup transfers
 *
 * Gets a data chunk from an item received by an ex receive
 * operation.
 *
 * @param pTransferOp       A pointer to a STAR_TRANSFER_OPERATION.
 * @param item              The index of the item to get the data chunk
 *                          from.
 * @param pDataChunk        A pointer to a data chunk that will be
 *                          populated with data from the item.
 *
 * @return      1 if a data chunk was successfully populated from
 *              the item, or -1 if an error occurred.
 *
 * \versionAdded \ref changes_v4_00
 */
int STAR_API_CC STAR_rxOperationExGetDataChunk(
    _In_ STAR_TRANSFER_OPERATION *pTransferOp, _In_ unsigned int item,
    _Out_ STAR_DATA_CHUNK *pDataChunk);

/**
 * \ingroup transfers
 *
 * Gets a broadcast message from an item received by an ex receive
 * operation.
 *
 * @param pTransferOp       A pointer to a STAR_TRANSFER_OPERATION.
 * @param item              The index of the item to get the broadcast message
 *                          from.
 * @param pBroadcastMessage A pointer to a broadcast message that will be
 *                          populated with data from the item.
 *
 * @return      1 if a broadcast message was successfully populated from
 *              the item, or -1 if an error occurred.
 *
 * \versionAdded \ref changes_v4_00
 */
int STAR_API_CC STAR_rxOperationExGetBroadcastMessage(
    _In_ STAR_TRANSFER_OPERATION *pTransferOp, _In_ unsigned int item,
    _Out_ STAR_BROADCAST_MESSAGE *pBroadcastMessage);

/**
 * \ingroup transfers
 *
 * Gets the status of an ex receive operation.
 *
 * @param pTransferOp    A pointer to a STAR_TRANSFER_OPERATION.
 *
 * @return       The status of the ex receive operation.
 *
 * \versionAdded \ref changes_v4_00
 */
STAR_TRANSFER_STATUS STAR_API_CC STAR_rxOperationExGetStatus(
    _In_ STAR_TRANSFER_OPERATION *pTransferOp);

#ifdef __cplusplus
}
#endif



#endif    /* STAR_API_TRANSFER_OPERATIONS_H */
