/**
 * \file general.h
 *
 * \brief General functions provided by STAR-API.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains the declarations of the general functions provided by
 * STAR-API, the STAR-Dundee software API, relating to the API itself, along
 * with the functions used for managing channels.
 *
 * Copyright &copy; 2012 STAR-Dundee Ltd.
 */

#ifndef STAR_API_GENERAL_H
#define STAR_API_GENERAL_H

#include "common.h"
#include "types.h"
#include "stream_item_types.h"
#include "version.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup general
 *
 * Gets the Version of STAR-API.
 *
 * @return  Pointer to a version structure that will contain STAR-API's version info, or NULL
 *          if a memory allocation error occurred.
 *              \note This structure must be freed using
 *              STAR_destroyVersionInfo() when no longer required.
 *
 * \versionAdded \ref changes_v0_8
 */
_Ret_opt_cap_c_(1) STAR_VERSION_INFO* STAR_API_CC STAR_getApiVersion(void);

/**
 * \ingroup general
 * Frees a version information structure previously created by a call to
 * STAR_getApiVersion(), STAR_getDriverVersion() or
 * STAR_getDeviceFirmwareVersion().
 *
 * @param pVersionInfo the version information structure to be freed
 *
 * \versionAdded \ref changes_v1_1
 */
void STAR_API_CC STAR_destroyVersionInfo(
    _Post_ptr_invalid_ STAR_VERSION_INFO *pVersionInfo);

/**
 * \ingroup general
 *
 * Gets an array of all the version info structures provided by all modules of STAR-System.
 *
 * @param[out]  count   Count of version information structures held in array.
 *
 * @return  Array of version info structures containing STAR-System version info structures.
 *          \note This function returns a snapshot of the current state of the system
 *                and is not automatically updated.
 *                This array must be freed using
 *                STAR_destroyVersionInfoList() when no longer required.
 *
 * \versionAdded \ref changes_v0_8
 */
_Ret_opt_cap_(*count) STAR_VERSION_INFO* STAR_API_CC
STAR_getAllVersions( _Out_ U32* count);

/**
 * \ingroup general
 * Frees a version information list previously created by a call to
 * STAR_getAllVersions().
 *
 * @param pVersionInfoList the version information list to be freed
 *
 * \versionAdded \ref changes_v1_1
 */
void STAR_API_CC STAR_destroyVersionInfoList(
    _Post_ptr_invalid_ STAR_VERSION_INFO *pVersionInfoList);

/**
 * \ingroup general
 *
 * Sets the name of the application using STAR-API. This is the name provided to
 * other processes calling STAR_getApplicationNameForID().
 *
 * @param[in] name      Null terminated string containing the application name
 *
 * @return          1 if app name was successfully set, else 0
 *
 * \versionAdded \ref changes_v0_8
 */
int STAR_API_CC STAR_setApplicationName(_In_z_ char *name);

/**
 * \ingroup general
 *
 * Gets the name of the calling process (this process), set by
 * STAR_setApplicationName().  The string returned should be freed by
 * calling STAR_destroyString().
 *
 * @return the application name as a null terminated string
 *
 * \versionAdded \ref changes_v1_0
 * \versionChanged \ref changes_v1_1
 */
_Check_return_ _Ret_opt_z_ char * STAR_API_CC STAR_getApplicationName(void);

/**
 * \ingroup general
 *
 * Gets the application ID of the calling process.
 *
 * @return  The application ID for the calling process
 *
 * \versionAdded \ref changes_v1_0
 */
STAR_APP_ID STAR_API_CC STAR_getApplicationID(void);

/**
 * \ingroup general
 *
 * Gets the name of the application with the given ID.  The string returned
 * should be freed by calling STAR_destroyString().
 *
 * @param appId the application ID to get a corresponding name for
 *
 * @return the application name as a null terminated string
 *
 * \versionAdded \ref changes_v1_0
 * \versionChanged \ref changes_v1_1
 */
_Check_return_ _Ret_opt_z_ char * STAR_API_CC STAR_getApplicationNameForID(
    STAR_APP_ID appId);

/**
 * \ingroup general
 *
 * Destroy a string returned by STAR-API, such as from
 * STAR_getApplicationName() or STAR_getDeviceTypeAsString().
 *
 * @param[in] string the string to be destroyed
 *
 * \versionAdded \ref changes_v1_1
 */
void STAR_API_CC STAR_destroyString(_Post_ptr_invalid_ _In_z_ char *string);

/**
 * \ingroup devmgmt
 *
 * Gets an array of driver identifiers for all drivers of the requested type(s).
 *
 * @param physicalDeviceDrivers   Whether to include physical device drivers in
 *                                the list
 * @param virtualDeviceDrivers    Whether to include virtual device drivers in
 *                                the list
 * @param[out] count              Count of driver identifiers held in the array.
 *
 * @return   Array of identifiers for drivers.
 *          \note This function returns a snapshot of the current state of the
 *                system and is not automatically updated.
 *                This array must be freed using STAR_destroyDriverList()
 *                when no longer required.
 *
 * \versionAdded \ref changes_v0_8
 */
_Ret_opt_cap_(*count) STAR_DRIVER_ID* STAR_API_CC STAR_getDriverList(
    int physicalDeviceDrivers, int virtualDeviceDrivers, _Out_ U32* count);

/**
 * \ingroup devmgmt
 * Frees a driver list previously created by a call to STAR_getDriverList().
 *
 * @param pDriverList the driver list to be freed
 *
 * \versionAdded \ref changes_v0_8
 */
void STAR_API_CC STAR_destroyDriverList(
    _Post_ptr_invalid_ STAR_DRIVER_ID *pDriverList);

/**
 * \ingroup devmgmt
 *
 * Registers a Call-back function that will be called whenever a Driver is added or removed.
 *
 * @param listenerFunc          Call-back Function
 * @param[in] pContextInfo      Optional pointer to some context information that will be passed
 *                              to the listener function when it is called
 * @param notifyForCurrent      Whether the function should be called for all
 *                              existing drivers to indicate that they've been added
 *                              \bug notifyForCurrent parameter is currently not used
 *
 * @return                      1 if call-back was registered successfully, else 0
 *
 * \versionAdded \ref changes_v0_8
 */
_Check_return_ STAR_DRIVER_LISTENER_ID STAR_API_CC STAR_registerDriverListener(
    STAR_DriverEventFunc listenerFunc, _In_opt_ void *pContextInfo,
    int notifyForCurrent);

/**
 * \ingroup devmgmt
 *
 * Unregister a Call-back function that was previously registered to be called whenever a
 * driver is added or removed.
 *
 * @param listenerId            Driver Listener ID returned from a previous call to STAR_registerDriverListener
 *
 * @return                      1 if call-back was unregistered successfully
 *
 * \versionAdded \ref changes_v0_8
 */
int STAR_API_CC STAR_unregisterDriverListener(
    STAR_DRIVER_LISTENER_ID listenerId);

/**
 * \ingroup devmgmt
 *
 * Gets the Version of a given driver.
 *
 * @param driverId  ID of driver to get version for, obtained from a call to STAR_getDriverList()
 *
 * @return      Pointer to a version structure that will contain specific drivers  version info,
 *              or NULL if there was an error.
 *              \note This structure must be freed using
 *              STAR_destroyVersionInfo() when no longer required.
 *
 * \versionAdded \ref changes_v0_8
 */
_Ret_opt_cap_c_(1) STAR_VERSION_INFO* STAR_API_CC STAR_getDriverVersion(
    STAR_DRIVER_ID driverId);

/**
 * \ingroup devmgmt
 *
 * Gets the type (USB/PCI/specific virtual type identifier) of a given driver.
 *
 * @param driverId              Identifier for driver to be checked
 *
 * @return                      Type of the driver
 *
 * \versionAdded \ref changes_v0_8
 */
STAR_DRIVER_TYPE STAR_API_CC STAR_getDriverType(STAR_DRIVER_ID driverId);

/**
 * \ingroup devmgmt
 *
 * Gets an array of identifiers for all devices present for all drivers.
 *
 * @param[out] count Count of device identifiers held in the array.
 *
 * @return Array of identifiers for all devices present.
 *          \note This function returns a snapshot of the current state of the
 *                system and is not automatically updated.
 *                This array must be freed using STAR_destroyDeviceList()
 *                when no longer required.
 *
 * \versionAdded \ref changes_v0_8
 */
_Ret_opt_cap_(*count) STAR_DEVICE_ID * STAR_API_CC STAR_getDeviceList(
    _Out_ U32* count);

/**
 * \ingroup devmgmt
 *
 * Gets an array of identifiers for all devices present for the given device
 * type.  The device type may be the type of a specific device, e.g.
 * #STAR_DEVICE_BRICK_MK3, or it may be one of the special device types,
 * #STAR_DEVICE_TXRX_SUPPORTED, #STAR_DEVICE_TXRX_NOT_SUPPORTED,
 * #STAR_DEVICE_CONFIG_SUPPORTED or #STAR_DEVICE_CONFIG_NOT_SUPPORTED to get the
 * list of all devices which can or cannot transmit and receive packets or
 * be configured, or #STAR_DEVICE_ALL to get all devices.
 *
 * @param deviceType The type of device to get the list for
 * @param[out] pCount A pointer to a variable which will be updated to include
 *                   the count of device identifiers held in the returned array.
 *
 * @return Array of identifiers for all devices of the given type present.
 *          \note This function returns a snapshot of the current state of the
 *                system and is not automatically updated.
 *                This array must be freed using STAR_destroyDeviceList()
 *                when no longer required.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
_Ret_opt_cap_(*pCount) STAR_DEVICE_ID * STAR_API_CC STAR_getDeviceListForType(
    STAR_DEVICE_TYPE deviceType, _Out_ U32* pCount);

/**
 * \ingroup devmgmt
 *
 * Gets an array of identifiers for all devices present for the given device
 * types in an array.  Each device type may be the type of a specific device,
 * e.g. #STAR_DEVICE_BRICK_MK3, or it may be one of the special device types,
 * #STAR_DEVICE_TXRX_SUPPORTED, #STAR_DEVICE_TXRX_NOT_SUPPORTED,
 * #STAR_DEVICE_CONFIG_SUPPORTED or #STAR_DEVICE_CONFIG_NOT_SUPPORTED, to get
 * the list of all devices which can or cannot transmit and receive packets or
 * be configured, or #STAR_DEVICE_ALL to get all devices.
 *
 * @param deviceTypes An array of device types to get the list for
 * @param numRequestedTypes The number of entries in the deviceTypes array
 * @param[out] pCount A pointer to a variable which will be updated to include
 *                   the count of device identifiers held in the returned array.
 *
 * @return Array of identifiers for all devices of the given type present.
 *          \note This function returns a snapshot of the current state of the
 *                system and is not automatically updated.
 *                This array must be freed using STAR_destroyDeviceList()
 *                when no longer required.
 *
 * \versionAdded \ref changes_v3_0_beta3
 */
_Ret_opt_cap_(*pCount) STAR_DEVICE_ID* STAR_API_CC STAR_getDeviceListForTypes(
    STAR_DEVICE_TYPE deviceTypes[], U32 numRequestedTypes, _Out_ U32* pCount);

/**
 * \ingroup devmgmt
 *
 * Gets an array of device identifiers for all devices present for a specific driver.
 *
 * @param      driverId     ID of driver to get devices for, obtained from a call to STAR_getDriverList()
 * @param[out] count        Count of device identifiers held in the array.
 *
 * @return Array of identifiers for devices for the given driver.
 *          \note This function returns a snapshot of the current state of the system
 *                and is not automatically updated.
 *                This array must be freed using STAR_destroyDeviceList()
 *                when no longer required.
 *
 * \versionAdded \ref changes_v0_8
 */
_Ret_opt_cap_(*count) STAR_DEVICE_ID* STAR_API_CC STAR_getDeviceListForDriver(
    STAR_DRIVER_ID driverId, _Out_ U32* count);

/**
 * \ingroup devmgmt
 * Frees a device list previously created by a call to
 * STAR_getDeviceList() or STAR_getDeviceListForDriver().
 *
 * @param pDeviceList the device list to be freed
 *
 * \versionAdded \ref changes_v1_1
 */
void STAR_API_CC STAR_destroyDeviceList(
    _Post_ptr_invalid_ STAR_DEVICE_ID *pDeviceList);

/**
 * \ingroup devmgmt
 *
 * Registers a Call-back function that will be called whenever a device is added or removed.
 *
 * @param[in] listenerFunc          Call-back Function
 * @param[in] pContextInfo          Optional pointer to some context information that will be passed
 *                                  to the listener function when it is called
 * @param notifyForCurrent          Whether the function should be called for all
 *                                  existing devices to indicate that they've been added
 *                                  \bug notifyForCurrent parameter is currently not used
 *
 * @return                      Non-Zero if call-back was registered successfully
 *
 * \versionAdded \ref changes_v0_8
 */
_Check_return_ STAR_DEVICE_LISTENER_ID STAR_API_CC STAR_registerDeviceListener(
    _In_ STAR_DeviceEventFunc listenerFunc, _In_opt_ void *pContextInfo,
    int notifyForCurrent);

/**
 * \ingroup devmgmt
 *
 * Registers a Call-back function that will be called whenever a device  for a specific driver is added or removed.
 *
 * @param driverId              Driver to register device listener for
 * @param listenerFunc          Call-back Function
 * @param[in] pContextInfo      Optional pointer to some context information that will be passed
 *                              to the listener function when it is called
 * @param notifyForCurrent      Whether the function should be called for all
 *                              existing devices to indicate that they've been added
 *                              \bug notifyForCurrent parameter is currently not used
 *
 * @return                      Non-Zero if call-back was registered successfully
 *
 * \versionAdded \ref changes_v0_8
 */
_Check_return_ STAR_DEVICE_LISTENER_ID STAR_API_CC
    STAR_registerDeviceListenerForDriver(STAR_DRIVER_ID driverId,
                                        _In_ STAR_DeviceEventFunc listenerFunc,
                                        _In_opt_ void *pContextInfo,
                                        int notifyForCurrent);

/**
 * \ingroup devmgmt
 *
 * Registers a Call-back function that will be called whenever a specific device is removed.
 *
 * @param deviceId              Device to register device listener for
 * @param listenerFunc          Call-back Function
 * @param[in] pContextInfo      Optional A pointer to some context information that will be passed
 *                              to the listener function when it is called
 *
 * @return                      Non-Zero if call-back was registered successfully
 */
_Check_return_ STAR_DEVICE_LISTENER_ID STAR_API_CC
    STAR_registerDeviceListenerForDevice(STAR_DEVICE_ID deviceId,
                                         _In_ STAR_DeviceEventFunc listenerFunc,
                                         _In_opt_ void *pContextInfo);

/**
 * \ingroup devmgmt
 *
 * Unregister a Call-back function that was previously registered to be called whenever a
 * device is added or removed.
 *
 * @param listenerId            Device Listener ID returned from a previous call to STAR_registerDeviceListener
 *
 * @return                      1 if call-back was unregistered successfully, else 0
 *
 * \versionAdded \ref changes_v0_8
 */
int STAR_API_CC STAR_unregisterDeviceListener(
    STAR_DEVICE_LISTENER_ID listenerId);

/**
 * \ingroup devmgmt
 *
 * Resets a device. This function will return an error if called with a Virtual Device's
 * identifier.
 *
 * \note Calling this function while packets are being transmitted and/or
 *       received can cause the device to get in to a bad state.
 *
 *
 * @param deviceId              Device to be reset
 *
 * @return                      1 if the device was successfully reset, else 0
 *
 * \versionAdded \ref changes_v1_0
 */
int STAR_API_CC STAR_resetDevice(STAR_DEVICE_ID deviceId);

/**
 * \ingroup devmgmt
 *
 * Gets the version of a device's firmware.
 *
 * @param deviceId              Identifier for device to be checked
 *
 * @return  Pointer to a version structure that will contain the device's version info, or NULL
 *          if a memory allocation error occurred.
 *              \note This structure must be freed using
 *              STAR_destroyVersionInfo() when no longer required.
 *
 * \versionAdded \ref changes_v1_0
 */
_Ret_opt_cap_c_(1) STAR_VERSION_INFO* STAR_API_CC STAR_getDeviceFirmwareVersion(
    STAR_DEVICE_ID deviceId);

/**
 * \ingroup devmgmt
 *
 * Gets the bus type (PCI,USB, Virtual, etc) for the device.
 *
 * @param deviceId the identifier of the device to get the bus type of
 *
 * @return                      1 if the driver is virtual, otherwise 0
 *
 * \versionAdded \ref changes_v0_8
 */
STAR_BUS_TYPE STAR_API_CC STAR_getDeviceBusType(STAR_DEVICE_ID deviceId);

/**
 * \ingroup devmgmt
 *
 * Gets a string representation of a device's bus type.  The string returned
 * should be freed by calling STAR_destroyString().
 *
 * @param deviceId the identifier of the device to get the bus type of
 *
 * @return the device's bus type as a null terminated string
 *
 * \versionAdded \ref changes_v1_0
 * \versionChanged \ref changes_v1_1
 */
_Check_return_ _Ret_opt_z_ char * STAR_API_CC STAR_getDeviceBusTypeAsString(
    STAR_DEVICE_ID deviceId);

/**
 * \ingroup devmgmt
 *
 * Gets the device index by Driver.
 * Provided for backwards compatibility reasons, not for typical usage.
 *
 * @param deviceId          The device to check
 *
 * @return                  The index number of the device, or -1 if the function call was unsuccessful
 *
 * \versionAdded \ref changes_v0_8
 */
int STAR_API_CC STAR_getDeviceIndex(STAR_DEVICE_ID deviceId);

/**
 * \ingroup devmgmt
 *
 * Gets the device's type identifier.
 *
 * @param deviceId the identifier of the device to get the type of
 *
 * @return the type of the device
 *
 * \versionAdded \ref changes_v0_8
 */
STAR_DEVICE_TYPE STAR_API_CC STAR_getDeviceType(STAR_DEVICE_ID deviceId);

/**
 * \ingroup devmgmt
 *
 * Gets a string representation of a device's type.  The string returned should
 * be freed by calling STAR_destroyString().
 *
 * @param deviceId the identifier of the device to get the type of
 *
 * @return the device's type as a null terminated string
 *
 * \versionAdded \ref changes_v0_8
 * \versionChanged \ref changes_v1_1
 */
_Check_return_ _Ret_opt_z_ char * STAR_API_CC STAR_getDeviceTypeAsString(
    STAR_DEVICE_ID deviceId);

/**
 * \ingroup devmgmt
 *
 * Gets the identifier of the device's driver.
 *
 * @param deviceId the identifier of the device to get the driver of
 *
 * @return the driver for the device
 *
 * \versionAdded \ref changes_v1_12
 */
STAR_DRIVER_ID STAR_API_CC STAR_getDeviceDriver(STAR_DEVICE_ID deviceId);

/**
 * \ingroup devmgmt
 *
 * Gets the name of the device identified by a given identifier.  The string
 * returned should be freed by calling STAR_destroyString().
 *
 * @param deviceId the identifier of the device to get the type of
 *
 * @return the device's name as a null terminated string
 *
 * \versionAdded \ref changes_v0_8
 * \versionChanged \ref changes_v1_1
 */
_Check_return_ _Ret_opt_z_ char * STAR_API_CC STAR_getDeviceName(
    STAR_DEVICE_ID deviceId);

/**
 * \ingroup devmgmt
 *
 * Sets the name of the device (friendly name).
 *
 * @param     deviceId     The device to set a name for
 * @param[in] newName      The new name for the device (newName must be null
 *                         terminated and shorter than 256 characters).
 *                         If newName is NULL then the device name is reset to
 *                         the default.
 *
 * @return                  1 if the name could be set, otherwise 0
 *
 * \versionAdded \ref changes_v0_8
 */
int STAR_API_CC STAR_setDeviceName(STAR_DEVICE_ID deviceId,
    _In_z_ char* newName);

/**
 * \ingroup devmgmt
 *
 * Resets a device's name to its default value.
 *
 * @param deviceId              Device to have it's name reset
 *
 * @return                      1 if the device's name was successfully reset, else 0
 *
 * \versionAdded \ref changes_v4_0_beta1
 */
int STAR_API_CC STAR_resetDeviceName(STAR_DEVICE_ID deviceId);

/**
 * \ingroup devmgmt
 *
 * Gets the serial number (unique alphanumeric identifier) of the device
 * identified by a given identifier.  The string returned should be freed by
 * calling STAR_destroyString().
 *
 * @param deviceId the identifier of the device to get the serial number of
 *
 * @return the device's serial number as a null terminated string
 *
 * \versionAdded \ref changes_v1_0
 * \versionChanged \ref changes_v1_1
 */
_Check_return_ _Ret_opt_z_ char* STAR_API_CC STAR_getDeviceSerialNumber(
    STAR_DEVICE_ID deviceId);

/**
 * \ingroup devmgmt
 *
 * Determine whether the device is capable of transmitting and receiving
 * packets on channels.  This function can be used to determine whether a device
 * can be used to route packets over channels and in and out of the device's
 * SpaceWire links (#STAR_DEVICE_TXRX_SUPPORTED), or whether it is a special
 * device such as a Link Analyser or Conformance Tester
 * (#STAR_DEVICE_TXRX_NOT_SUPPORTED) which cannot route packets.  Note that some
 * devices, such as the EGSE may be capable of transmitting and receiving
 * packets, but a value of #STAR_DEVICE_TXRX_NOT_SUPPORTED will be returned, as
 * these devices do not simply route any packets transmitted or received on
 * channels, and must be specially configured to transmit or receive packets.
 *
 * @param deviceId the identifier of the device to get whether the device can
 *                 transmit and receive
 *
 * @return a value indicating whether the device is capable of transmitting and
 *         receiving packets (#STAR_DEVICE_TXRX_SUPPORTED), not capable
 *         (#STAR_DEVICE_TXRX_NOT_SUPPORTED), or 0 if an error occurred
 *
 * \versionAdded \ref changes_v3_0_beta2
 */
STAR_DEVICE_TYPE STAR_API_CC STAR_getDeviceTxRxCapabilities(
    STAR_DEVICE_ID deviceId);

/**
 * \ingroup devmgmt
 *
 * Determine whether the device is capable of being configured.  This function
 * can be used to determine whether a device can be configured using the
 * Configuration APIs (#STAR_DEVICE_CONFIG_SUPPORTED), or whether it is a
 * special device such as a Link Analyser or Conformance Tester
 * (#STAR_DEVICE_CONFIG_NOT_SUPPORTED) which does not have a router with a
 * configuration port.
 *
 * @param deviceId the identifier of the device to get whether the device can be
 *                 configured
 *
 * @return a value indicating whether the device is capable of being configured
 *         (#STAR_DEVICE_CONFIG_SUPPORTED), not capable
 *         (#STAR_DEVICE_CONFIG_NOT_SUPPORTED), or 0 if an error occurred
 *
 * \versionAdded \ref changes_v3_0_beta9
 */
STAR_DEVICE_TYPE STAR_API_CC STAR_getDeviceConfigCapabilities(
    STAR_DEVICE_ID deviceId);

/**
 * \ingroup devmgmt
 *
 * Gets the channels on a device.
 *
 * @param deviceId          The device to check
 *
 * @return                  A bit mask representing the channels on the device
 *
 * \versionAdded \ref changes_v0_8
 */
STAR_CHANNEL_MASK STAR_API_CC STAR_getDeviceChannels(STAR_DEVICE_ID deviceId);

/**
 * \ingroup channel
 *
 *  Open a channel between two devices on the specified channel numbers.
 *
 * @param deviceA       First device to attach to
 * @param channelA          Channel number on first device to attach to
 * @param deviceB       Second device to attach to
 * @param channelB          Channel number on second device to attach to
 *
 * @return      Channel identifier of new channel or 0 if an error occurred
 *
 * \versionAdded \ref changes_v1_1
 */
_Check_return_ STAR_CHANNEL_ID STAR_API_CC STAR_openChannelBetweenLocalDevices(
                             STAR_DEVICE_ID deviceA,
                             unsigned char channelA,
                             STAR_DEVICE_ID deviceB,
                             unsigned char channelB);

/**
 * \ingroup channel
 *
 * Open a channel to a device on the specified channel number.  When the channel
 * is no longer required it should be closed by calling STAR_closeChannel().
 * Note that a channel can only be opened once in a particular direction, but it
 * is possible, for example, to open a channel to receive, then open it again
 * to transmit.
 *
 * The isQueued parameter determines whether received traffic is buffered when
 * it is received.  As well as buffering in the device, there is also buffering
 * in the device driver, and if an application is open with a channel open to
 * receive data, there is also buffering in STAR-API for this channel.  As these
 * buffers become full, the device will stop issuing credit, so the transmitting
 * device can no longer transmit.
 *
 * The isQueued parameter can be thought of as a way to override this behaviour.
 * When it is set to FALSE, packets will be received as normal.  However, if the
 * STAR-API buffer becomes full, the API will keep reading data from the driver.
 * If another packet is received from the driver before the application reads a
 * packet from the channel, the API will drop a packet from its buffer and read
 * in the new packet.
 *
 * The benefit of passing FALSE for the isQueued parameter is that the link
 * shouldn't become blocked, as the device should always be issuing credit.  The
 * obvious disadvantage is that packets can be dropped, so this isn't a mode
 * that is often used.  In most circumstances it is recommended that TRUE is
 * provided for this argument, although note that the argument will be ignored
 * for transmit-only channels.
 *
 * @param device        the device to attach to
 * @param direction     whether this channel is for transmitting data from this
 *                      application or receiving data into it, or both.  Note
 *                      that there is no performance penalty in opening a
 *                      channel in both directions when only receiving, for
 *                      example.  It simply stops the channel from being opened
 *                      for transmitting in another process or thread
 * @param channelNumber the channel number on the device to attach to
 * @param isQueued      whether traffic received on this channel should be
 *                      buffered if there is no receive op waiting to receive it
 *
 * @return      Channel identifier of new channel or 0 if an error occurred
 *
 * \versionAdded \ref changes_v1_1
 */
_Check_return_ STAR_CHANNEL_ID STAR_API_CC STAR_openChannelToLocalDevice(
                        STAR_DEVICE_ID device,
                        STAR_CHANNEL_DIRECTION direction,
                        unsigned char channelNumber,
                        int isQueued);

/**
 * \ingroup channel
 *
 * This function is an alternative to STAR_openChannelToLocalDevice that uses
 * an optimised receive strategy. Instead of creating dynamically allocated
 * packets, data is copied directly into pre-allocated buffers.
 *
 * To utilise the optimised receive strategy, the channel should be used with
 * receive transfer operations created with the STAR_createRxOperationEx
 * function.
 *
 * When the channel is no longer required it should be closed by calling
 * STAR_closeChannel(). Note that a channel can only be opened once in a
 * particular direction, but it is possible, for example, to open a channel to
 * receive, then open it again to transmit.
 *
 * The isQueued parameter determines whether received traffic is buffered when
 * it is received.  As well as buffering in the device, there is also buffering
 * in the device driver, and if an application is open with a channel open to
 * receive data, there is also buffering in STAR-API for this channel.  As these
 * buffers become full, the device will stop issuing credit, so the transmitting
 * device can no longer transmit.
 *
 * The isQueued parameter can be thought of as a way to override this behaviour.
 * When it is set to FALSE, data will be received as normal.  However, if the
 * STAR-API buffer becomes full, the API will keep reading data from the driver.
 * If more data is received from the driver before the application reads it
 * from the channel, the API will drop the data from its buffer and read
 * in the new data.
 *
 * The benefit of passing FALSE for the isQueued parameter is that the link
 * shouldn't become blocked, as the device should always be issuing credit.  The
 * obvious disadvantage is that data can be dropped, so this isn't a mode
 * that is often used.  In most circumstances it is recommended that TRUE is
 * provided for this argument, although note that the argument will be ignored
 * for transmit-only channels.
 *
 * @param device        the device to attach to
 * @param direction     whether this channel is for transmitting data from this
 *                      application or receiving data into it, or both.  Note
 *                      that there is no performance penalty in opening a
 *                      channel in both directions when only receiving, for
 *                      example.  It simply stops the channel from being opened
 *                      for transmitting in another process or thread
 * @param channelNumber the channel number on the device to attach to
 * @param isQueued      whether traffic received on this channel should be
 *                      buffered if there is no receive op waiting to receive it
 *
 * @return      Channel identifier of new channel or 0 if an error occurred
 *
 * \versionAdded \ref changes_v4_00
 */
_Check_return_ STAR_CHANNEL_ID STAR_API_CC STAR_openChannelToLocalDeviceEx(
    _In_ STAR_DEVICE_ID device,
    _In_ STAR_CHANNEL_DIRECTION direction,
    _In_ unsigned char channelNumber,
    _In_ int isQueued);

/**
 * \ingroup channel
 *
 * Close a previously opened channel.
 *
 * @param channelId Channel to close
 *
 * @return      1 if the channel was successfully closed, else 0.  If a channel
 *              could not be closed, it was not open or has already been closed
 *
 * \versionAdded \ref changes_v1_1
 */
int STAR_API_CC STAR_closeChannel(STAR_CHANNEL_ID channelId);

/**
 * \ingroup devmgmt
 *
 * Returns whether a given channel on a device is open and, if it is open,
 * whether it is connected to a device or an application.
 *
 * @param deviceId          The device to check
 * @param channelNumber     The channel number to check
 *
 * @return                  Whether the channel is connected a device
 *                          (#STAR_CHANNEL_TYPE_DEVICE) or application
 *                          (#STAR_CHANNEL_TYPE_APPLICATION) if the channel is
 *                          open, or not open (#STAR_CHANNEL_TYPE_NOT_OPEN) if
 *                          the channel is not open
 *
 * \versionAdded \ref changes_v1_1
 */
STAR_CHANNEL_TYPE STAR_API_CC STAR_isChannelOpen(STAR_DEVICE_ID deviceId,
                     unsigned int channelNumber);

/**
 * \ingroup channel
 *
 * Get the direction information of an open channel.
 *
 * @param channelID Channel to get the direction information for
 *
 * @return      Direction information of the channel,
 *              or 0 if an error occurred
 *
 * \versionAdded \ref changes_v3_10
 */
_Check_return_ STAR_CHANNEL_DIRECTION STAR_API_CC STAR_getChannelDirection(
                        STAR_CHANNEL_ID channelID);

/**
 * \ingroup devmgmt
 *
 * Returns the id of a device attached to a given channel on a given device.
 *
 * @param deviceId          The device to check
 * @param channelNumber         The channel number to check
 *
 * @return                  The attached device, or 0 if no device is attached
 *
 * \versionAdded \ref changes_v1_1
 */
STAR_DEVICE_ID STAR_API_CC STAR_getLocalDeviceAttachedToChannel(
    STAR_DEVICE_ID deviceId, unsigned int channelNumber);

/**
 * \ingroup general
 *
 * Returns the ID of an application attached to a given channel on a given device.

 *
 * @param      deviceId         The device to check
 * @param      channelNumber        The channel number to check

 *
 * @return   Requested application ID, or 0 if no application is attached
 *
 * \versionAdded \ref changes_v1_1
 */
STAR_APP_ID STAR_API_CC STAR_getApplicationAttachedToChannel(
    STAR_DEVICE_ID deviceId, unsigned int channelNumber);

/**
 * \ingroup channel
 *
 * Registers a Call-back function that will be called whenever any channel is opened or closed.
 *
 * @param     listenerFunc          Call-back Function
 * @param[in] pContextInfo          A pointer to some context information that will be passed
 *                                  to the listener function when it is called
 * @param     notifyForCurrent      Whether the function should be called for all
 *                                  existing channels to indicate that they've been opened
 *
 * @return                      Non-Zero if call-back was registered successfully
 *
 * \versionAdded \ref changes_v1_1
 */
_Check_return_ STAR_CHANNEL_LISTENER_ID STAR_API_CC
    STAR_registerChannelListener( _In_ STAR_ChannelEventFunc listenerFunc,
                                 _In_opt_ void *pContextInfo,
                                 int notifyForCurrent);

/**
 * \ingroup channel
 *
 * Registers a Call-back function that will be called whenever a channel is
 * opened or closed on a device which has the specific driver.
 *
 * @param     driverId              Driver to register channel listener for
 * @param     listenerFunc          Call-back Function
 * @param[in] pContextInfo          A pointer to some context information that will be passed
 *                                  to the listener function when it is called
 * @param     notifyForCurrent      Whether the function should be called for all
 *                                  existing channels to indicate that they've been opened
 *
 * @return                      Non-Zero if call-back was registered successfully
 *
 * \versionAdded \ref changes_v1_1
 */
_Check_return_ STAR_CHANNEL_LISTENER_ID STAR_API_CC
STAR_registerChannelListenerForDriver( STAR_DRIVER_ID driverId,
                                          _In_ STAR_ChannelEventFunc listenerFunc,
                                          _In_opt_ void *pContextInfo,
                                          int notifyForCurrent);

/**
 * \ingroup channel
 *
 * Registers a Call-back function that will be called whenever a channel on the specific device
 * is opened or closed.
 *
 * @param     deviceId              Device to register channel listener for
 * @param     listenerFunc          Call-back Function
 * @param[in] pContextInfo          A pointer to some context information that will be passed
 *                                  to the listener function when it is called
 * @param     notifyForCurrent      Whether the function should be called for all
 *                                  existing channels to indicate that they've been opened
 *
 * @return                      Non-Zero if call-back was registered successfully
 *
 * \versionAdded \ref changes_v1_1
 */
_Check_return_ STAR_CHANNEL_LISTENER_ID STAR_API_CC
STAR_registerChannelListenerForDevice( STAR_DEVICE_ID deviceId,
                                          _In_ STAR_ChannelEventFunc listenerFunc,
                                          _In_opt_ void *pContextInfo,
                                          int notifyForCurrent);

/**
 * \ingroup channel
 *
 * Unregister a Call-back function that was previously registered to be called whenever a
 * channel is opened or closed.
 *
 * @param listenerId            Channel Listener ID returned from a previous call to a
 *                              registerChannelListener function
 *
 * @return                      1 if call-back was unregistered successfully, else 0
 *
 * \versionAdded \ref changes_v1_1
 */
int STAR_API_CC STAR_unregisterChannelListener(
    STAR_CHANNEL_LISTENER_ID listenerId);

/**
 * \ingroup virtual
 *
 * Gets whether or not a given driver is virtual.
 *
 * @param driverId              Identifier for driver to be checked
 *
 * @return                      1 if the driver is virtual, otherwise 0
 *
 * \versionAdded \ref changes_v0_8
 */
int STAR_API_CC STAR_isDriverVirtual(STAR_DRIVER_ID driverId);

/**
 * \ingroup virtual
 *
 * Gets whether or not a given device is virtual.
 *
 * @param deviceId              Identifier for device to be checked
 *
 * @return                      1 if the device is virtual, otherwise 0
 *
 * \versionAdded \ref changes_v1_12
 */
int STAR_API_CC STAR_isDeviceVirtual(STAR_DEVICE_ID deviceId);

#ifdef __cplusplus
}
#endif

#endif  /* STAR_API_GENERAL_H */
