/**
 * \file types.h
 *
 * \brief Types, structures and function types used by STAR-API
 *
 * Types used to describe drivers, devices and channels.
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

#ifndef STAR_API_TYPES_H
#define STAR_API_TYPES_H

#include "common.h"

#ifndef __KERNEL__
#include "star-dundee_types.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


/**
 * \todo The identifier type should be placed in star-dundee-types. This needs
 *       to be updated across STAR-System.
 */
#define CFG_INFO_ID_TYPE U32

/**
 * \ingroup devmgmt
 * Unique value used to identify an individual driver.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef  CFG_INFO_ID_TYPE STAR_DRIVER_ID;

/**
 * \ingroup devmgmt
 * Unique value used to identify an individual device.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef  CFG_INFO_ID_TYPE STAR_DEVICE_ID;

/**
 * \ingroup channel
 * Unique value used to identify an open channel.
 *
 * \versionAdded \ref changes_v1_1
 */
typedef  CFG_INFO_ID_TYPE STAR_CHANNEL_ID;

/**
 * \ingroup general
 * Unique value used to identify an application using STAR-System.
 *
 * \versionAdded \ref changes_v1_0
 */
typedef  CFG_INFO_ID_TYPE STAR_APP_ID;

/**
 * \ingroup devmgmt
 * Unique value used to identify an individual device listener.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef CFG_INFO_ID_TYPE STAR_DEVICE_LISTENER_ID;
/**
 * \ingroup devmgmt
 * Unique value used to identify an individual driver listener.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef CFG_INFO_ID_TYPE STAR_DRIVER_LISTENER_ID;
/**
 * \ingroup channel
 * Unique value used to identify an individual channel listener.
 *
 * \versionAdded \ref changes_v1_1
 */
typedef  CFG_INFO_ID_TYPE STAR_CHANNEL_LISTENER_ID;

#undef CFG_INFO_ID_TYPE

/**
 * \ingroup devmgmt
 * Type of device that a device is.
 *
 * \versionAdded \ref changes_v0_8
 */
#define STAR_DEVICE_TYPE U32


/**
 * \ingroup devmgmt
 * Size in bytes (including null terminator) of a device serial number.
 *
 * \versionAdded \ref changes_v1_0
 */
#define STAR_DEVICE_SERIAL_SIZE 16

/**
 * \ingroup devmgmt
 * The different types of bus that can be used to connect a SpaceWire device to
 * a PC.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef enum
{
    /** The bus type of a device is unknown. */
    STAR_BUS_UNKNOWN        = 0,

    /** The PCI bus type. */
    STAR_BUS_PCI        = 1,
    /** The USB bus type. */
    STAR_BUS_USB        = 2,
    /**
     * The PCI Express (PCIe) bus type.
     *
     * \versionAdded \ref changes_v1_2
     */
    STAR_BUS_PCIE       = 3,
    /** The TCP/IP bus type, used by Ethernet devices. */
    STAR_BUS_TCP        = 4,
    /**
     * The cPCI bus type.
     *
     * \versionAdded \ref changes_v1_6
     */
    STAR_BUS_CPCI       = 5,

    /** The bus type for virtual devices. */
    STAR_BUS_VIRTUAL    = 255

} STAR_BUS_TYPE;

/**
 * \ingroup devmgmt
 * Type used to represent channels that exist on a device.
 * (bit 0 set = channel 0 exists, bit 2 set = channel 2 exists etc...).
 *
 * \versionAdded \ref changes_v0_8
 */
#define STAR_CHANNEL_MASK U32

/**
 * \ingroup devmgmt
 *
 * Available driver types.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef enum
{
    /** Invalid*/
    STAR_DRIVER_INVALID     = 0,
    /** USB Driver*/
    STAR_DRIVER_TYPE_USB    = 1,
    /** PCI Driver*/
    STAR_DRIVER_TYPE_PCI    = 2,
    /** TCP/IP Driver*/
    STAR_DRIVER_TYPE_TCPIP  = 4,
     /** Virtual Driver*/
    STAR_DRIVER_TYPE_VIRTUAL= 5

}STAR_DRIVER_TYPE;

/**
 * \ingroup devmgmt
 * The "unknown" device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_UNKNOWN                 0U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire PCI-2 and cPCI device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_PCI                     1U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire Router-USB device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_ROUTER_USB              2U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire-USB Brick device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_USB_BRICK               3U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire Link Analyser device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_LINK_ANALYSER           4U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire Conformance Tester device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_CONFORMANCE_TESTER      5U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire IP-Tunnel device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_IP_TUNNEL               6U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire Router Mk2S device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_ROUTER_MK2S             7U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire PCI Mk2 device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_PCI_MK2                 8U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire Brick Mk2 device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_BRICK_MK2               9U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire Link Analyser Mk2 device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_LINK_ANALYSER_MK2       10U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire PCI Express device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_PCIE                    11U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire RTC device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_RTC                     12U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire EGSE device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_EGSE                    13U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire cPCI Mk2 device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_CPCI_MK2                14U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire Physical Layer Tester device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_SPLT                    15U

/**
 * \ingroup devmgmt
 * The STAR-Dundee STAR Fire device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_STAR_FIRE               16U

/**
 * \ingroup devmgmt
 * The STAR-Dundee Wide Band Spectrometer II device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_WBS_II                  17U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire Recorder device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_RECORDER                18U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire Brick Mk3 device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_BRICK_MK3               19U

/**
 * \ingroup devmgmt
 * The Shimafuji Traffic Generator device type.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_TRAFFIC_GENERATOR       20U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire PXI Interface device type.
 *
 * \versionAdded \ref changes_v3_0_beta4
 */
#define STAR_DEVICE_PXI_INTERFACE           21U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire PXI RMAP device type.
 *
 * \versionAdded \ref changes_v3_0_beta4
 */
#define STAR_DEVICE_PXI_RMAP                22U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire PXI 8 port Router device type.
 *
 * \versionAdded \ref changes_v3_0_beta4
 */
#define STAR_DEVICE_PXI_ROUTER_8            23U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire PXI 12 port Router device type.
 *
 * \versionAdded \ref changes_v3_5
 */
#define STAR_DEVICE_PXI_ROUTER_12           24U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceFibre PCI Express device type.
 */
#define STAR_DEVICE_SPFIPCIE                25U

/**
 * \ingroup devmgmt
 * The STAR-Dundee STAR Fire Mk3 device type.
 *
 * \versionAdded \ref changes_v3_2
 */
#define STAR_DEVICE_STAR_FIRE_MK3           26U

 /**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire PCI Mk3 device type.
 */
#define STAR_DEVICE_PCI_MK3                 27U

 /**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire Link Analyser Mk3 device type.
 *
 * \versionAdded \ref changes_v4_0_beta3
 */
#define STAR_DEVICE_LINK_ANALYSER_MK3       28U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire Conformance Tester Mk2 device type.
 *
 * \versionAdded \ref changes_v4_0_beta3
 */
#define STAR_DEVICE_CONFORMANCE_TESTER_MK2  29U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire EGSE Mk2 device type.
 *
 * \versionAdded \ref changes_v4_0_beta3
 */
#define STAR_DEVICE_EGSE_MK2                30U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire GbE Brick device type.
 */
#define STAR_DEVICE_GBE_BRICK               31U

/**
 * \ingroup devmgmt
 * The STAR-Dundee STAR-Ultra PCIe device type.
 *
 * \versionAdded \ref changes_v4_00
 */
#define STAR_DEVICE_STAR_ULTRA_PCIE         32U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire PXI Interface Mk2 device type.
 *
 * \versionAdded \ref changes_v4_00_beta4
 */
#define STAR_DEVICE_PXI_INTERFACE_MK2       33U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire PXI RMAP Mk2 device type.
 *
 * \versionAdded \ref changes_v4_00_beta4
 */
#define STAR_DEVICE_PXI_RMAP_MK2            34U

/**
 * \ingroup devmgmt
 *  The STAR-Dundee SpaceWire PXI Router Mk2 device type.
 *
 * \versionAdded \ref changes_v4_00_beta4
 */
#define STAR_DEVICE_PXI_ROUTER_MK2          35U

/**
 * \ingroup devmgmt
 *  The STAR-Dundee SpaceWire Brick Mk4 device type.
 *
 * \versionAdded \ref changes_v4_03
 */
#define STAR_DEVICE_BRICK_MK4               36U

/**
 * \ingroup devmgmt
 * The STAR-Dundee SpaceWire Recorder Mk2 device type.
 *
 * \versionAdded \ref changes_v4_03
 */
#define STAR_DEVICE_RECORDER_MK2            37U

/**
 * \ingroup devmgmt
 * A device which is capable of being configured.
 *
 * \versionAdded \ref changes_v3_0_beta9
 */
#define STAR_DEVICE_CONFIG_SUPPORTED      0x00fffffbU

 /**
 * \ingroup devmgmt
 * A device which is not capable of being configured.
 *
 * \versionAdded \ref changes_v3_0_beta9
 */
#define STAR_DEVICE_CONFIG_NOT_SUPPORTED  0x00fffffcU

/**
 * \ingroup devmgmt
 * A device which is capable of transmitting and receiving packets.
 *
 * \versionAdded \ref changes_v3_0_beta1
 * \versionChanged \ref changes_v3_0_beta2
 */
#define STAR_DEVICE_TXRX_SUPPORTED      0x00fffffdU

/**
 * \ingroup devmgmt
 * A device which is not capable of transmitting and receiving packets.
 *
 * \versionAdded \ref changes_v3_0_beta1
 */
#define STAR_DEVICE_TXRX_NOT_SUPPORTED  0x00fffffeU

/**
 * \ingroup devmgmt
 * All devices.
 *
 * \versionAdded \ref changes_v3_0_beta2
 */
#define STAR_DEVICE_ALL                 0x00ffffffU


/**
 * \ingroup channel
 *
 * Channel types
 *
 * \versionAdded \ref changes_v1_1
 */
typedef enum
{
    /** Not open */
    STAR_CHANNEL_TYPE_NOT_OPEN,
    /** Attached to a device */
    STAR_CHANNEL_TYPE_DEVICE,
    /** Attached to an application*/
    STAR_CHANNEL_TYPE_APPLICATION
}STAR_CHANNEL_TYPE;

/**
 * \ingroup channel
 *
 * Channel directions
 *
 * \versionAdded \ref changes_v1_1
 */
typedef enum
{
    /** Channel may be used to receive traffic */
    STAR_CHANNEL_DIRECTION_IN    = 1,
    /** Channel may be used to transmit traffic */
    STAR_CHANNEL_DIRECTION_OUT   = 2,
    /** Channel may be used to both receive and transmit traffic */
    STAR_CHANNEL_DIRECTION_INOUT = STAR_CHANNEL_DIRECTION_IN |
                                      STAR_CHANNEL_DIRECTION_OUT
}STAR_CHANNEL_DIRECTION;


/**
 * \ingroup devmgmt
 *
 * The function type for driver listener functions which are called when a new
 * driver is added, or an existing one removed.
 *
 * @param     driverListenerIdentifier  the driver listener that this event corresponds to
 * @param     driverIdentifier          The driver which has been added or removed
 * @param     driverAdded               Whether the driver was added (1) or removed (0)
 * @param[in] pContextInfo      A pointer to optional context information that can be passed to
 *                              the function
 *
 * \versionAdded \ref changes_v0_8
 */
typedef void (STAR_API_CC *STAR_DriverEventFunc)(
    STAR_DRIVER_LISTENER_ID driverListenerIdentifier,
    STAR_DRIVER_ID driverIdentifier, int driverAdded,
    void *pContextInfo);

/**
 * \ingroup devmgmt
 *
 * The function type for device listener functions which are called when a
 * device is added or removed.
 *
 * @param     deviceListenerIdentifier  the device listener that this event corresponds to
 * @param     driverIdentifier          the driver on which the device was added or removed
 * @param     deviceIdentifier          the device which was added or removed
 * @param     deviceAdded               whether the device has been added (1) or removed (0)
 * @param[in] pContextInfo              a pointer to context information that can be passed to
 *                                      the function
 *
 * \versionAdded \ref changes_v0_8
 */
typedef void (STAR_API_CC *STAR_DeviceEventFunc)(
    STAR_DEVICE_LISTENER_ID deviceListenerIdentifier,
    STAR_DRIVER_ID driverIdentifier, STAR_DEVICE_ID deviceIdentifier,
    int deviceAdded, void *pContextInfo);

/**
 * \ingroup channel
 *
 * The function type for channel listener functions which are called when a
 * device channel is opened or closed.
 *
 * @param     channelListenerIdentifier the channel listener that this event corresponds to
 * @param     driverIdentifier          the driver of the device on which the channel was opened or closed
 * @param     deviceIdentifier          the device on which the channel was opened or closed
 * @param     channelIdentifier         the identifier of the channel which has been opened or closed
 * @param     channelOpened             whether the channel was opened (1) or closed (0)
 * @param     channelNumber             the channel number on the device of the channel which was opened or closed
 * @param[in] pContextInfo              a pointer to context information that can be passed to the function
 *
 * \note The calling convention for this function type is #STAR_API_CC
 *
 * \versionAdded \ref changes_v1_1
 */
typedef void (STAR_API_CC *STAR_ChannelEventFunc)(
    STAR_CHANNEL_LISTENER_ID channelListenerIdentifier,
    STAR_DRIVER_ID driverIdentifier, STAR_DEVICE_ID deviceIdentifier,
    STAR_CHANNEL_ID channelIdentifier, int channelOpened, U8 channelNumber,
    void *pContextInfo);



#ifdef __cplusplus
}
#endif

#endif  /* STAR_API_TYPES_H */
