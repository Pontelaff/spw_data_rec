/**
 * \file transfer_types.h
 *
 * \brief Types used to describe transmit and receive operations
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

#ifndef STAR_API_TRANSFER_TYPES_H
#define STAR_API_TRANSFER_TYPES_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup transfers
 *
 * Possible states a transfer operation can be in.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef enum star_api_transfer_status
{
    /**
     * Not yet started.  When a transfer operation is created, this will be its
     * status.
     */
    STAR_TRANSFER_STATUS_NOT_STARTED,

    /**
     * Transfer has begun.  When a transfer operation is submitted, this will be
     * its status, until it has completed. */
    STAR_TRANSFER_STATUS_STARTED,

    /**
     * Transfer has completed.  Once a transmit operation has successfully
     * transmitted all its traffic, or a receive operation has received all its
     * requested traffic, this will be its status.
     */
    STAR_TRANSFER_STATUS_COMPLETE,

    /**
     * Transfer was cancelled.  When a transfer is cancelled by calling
     * STAR_cancelTransferOperation(), this will be its status.
     */
    STAR_TRANSFER_STATUS_CANCELLED,

    /**
     * An error occurred while processing the transfer.  This will be the status
     * of a transfer operation if there was an error creating it, submitting it,
     * or there was an error while transmitting or receiving.
     */
    STAR_TRANSFER_STATUS_ERROR

} STAR_TRANSFER_STATUS;


/**
 * \ingroup transfers
 *
 * Flags used to determine what kind of stream items
 * to receive on a receive operation.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef enum
{
    /** #STAR_SPACEWIRE_PACKET  */
    STAR_RECEIVE_PACKETS            = 1U << 0,

    /** #STAR_DATA_CHUNK */
    STAR_RECEIVE_CHUNKS             = 1U << 1,

    /** #STAR_TIMECODE */
    STAR_RECEIVE_TIMECODES          = 1U << 2,

    /**
     * Flow control characters. Note that transmitting and receiving FCTs is not
     * currently supported.
     */
    STAR_RECEIVE_FCTS               = 1U << 3,

    /**
     * Null characters. Note that transmitting and receiving Nulls is not
     * currently supported.
     */
    STAR_RECEIVE_NULL               = 1U << 4,

    /**
     * #STAR_LINK_STATE_EVENT
     *
     * \versionAdded \ref changes_v2_0_beta_1
     */
    STAR_RECEIVE_LINK_STATE_EVENTS  = 1U << 5,

    /**
     * #STAR_LINK_SPEED_EVENT
     *
     * \versionAdded \ref changes_v2_0_beta_1
     */
    STAR_RECEIVE_LINK_SPEED_EVENTS  = 1U << 6,

    /**
     * #STAR_TIMESTAMP_EVENT
     *
     * \versionAdded \ref changes_v3_6
     */
    STAR_RECEIVE_TIMESTAMP_EVENTS   = 1U << 7,

    /**
     * #STAR_BROADCAST_MESSAGE
     *
     * \versionAdded \ref changes_v4_00
     */
    STAR_RECEIVE_BROADCAST_MESSAGES   = 1U << 8
} STAR_RECEIVE_MASK;


/**
 * \ingroup transfers
 * A structure used to identify a transfer operation.
 *
 * \note It is not intended for users of this library to access the members of
 *      this structure directly. Instead, use the provided accessor functions.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef void STAR_TRANSFER_OPERATION;

/**
 * \ingroup transfers
 * The function type for transfer operation listener functions which are called
 * when a transfer operation completes, is cancelled, or encounters an error.
 *
 * @param pOperation the operation which has completed
 * @param status the status of the operation
 * @param pContextInfo a pointer to user-specific data that is provided when the
 *                     completion listener is registered
 *
 * \note The calling convention for this function type is #STAR_API_CC
 *
 * \versionAdded \ref changes_v0_8
 */
typedef void (STAR_API_CC *STAR_TransferOperationListenerFunc)(STAR_TRANSFER_OPERATION *pOperation,
                            STAR_TRANSFER_STATUS status,
                            void *pContextInfo);

#ifdef __cplusplus
}
#endif

#endif  /* STAR_API_TRANSFER_TYPES_H */
