// AT90USB/usb_api.h
// Usb_api contains constants and functions which are project specific.
// You may modify this file (and usb_api.c) for your application.
// S. Salewski 21-MAR-2007

// The current version of this file is adapted to a simple Data-Acquisition-Device.
// Our DAQ device uses endpoints 0, 1, 2 and 3.
// Commands are send to our device by vendor-requests using endpoint 0.
// Endpoint 1 is configured as Bulk-IN-endpoint and used to transfer status information
// from device to the host. Whenever the host tries to read data from empty endpoint 1,
// a NAK-interrupt is generated, causing firmware to write status information to this ep.
// Endpoint 2 is configured an Bulk-IN-Endpoint and used to send DAQ-data to the host.
// DAQ operation starts when host sends a vendor request. Controlled by a periodically
// activated timer interrupt, daq data is put to the FIFO of endpoint 2.
// Endpoint 3 is configured as Bulk-OUT-Endpoint and used to send a single byte from the
// host to the device. This byte sets the pins of digital port B of our AT90USB. 

#ifndef _USB_API_H_
#define _USB_API_H_

#include <stdint.h>
#include <stdbool.h>
#include "usb_spec.h"

// USB Vendor ID is assigned by www.usb.org
// Vendor- and product id is defined in com_def.h, because we need it in our host program too!
#define MyUSB_DeviceBCD	0x0001

// modify these values for your application!
#define USB_NumConfigurations		1 // 1 or more
#define USB_MaxConfigurations		1 // our device can have multiple configuration with different number of interfaces
#define USB_MaxInterfaces		1 // each interface of a configuration has a number of endpoints
#define USB_MaxStringDescriptorLength	60

// An interface can have multiple alternate settings, but number of endpoints of this interface is fix.
// To ensure that reallocation of endpoints with new FIFO size of one interface will not interfere with other interfaces,
// only the interface with highest number should use multiple alternate settings with different endpoint FIFO sizes

#define EP0_FIFO_Size 64 // 8, 16, 32 or 64 byte
// FIFO size of EP1 upto EP6 is defined in com_def.h

// These macros are called if an endpoint interrupt is triggered (if enabled)
// and may be used to fill (IN-Endpoint) or read (OUT-Endpoint) the FIFO.
#define UsbDevEP1IntAction() usb_rxline_poll()
#define UsbDevEP2IntAction() usb_flush_rb()
#define UsbDevEP3IntAction()
#define UsbDevEP4IntAction()
#define UsbDevEP5IntAction()
#define UsbDevEP6IntAction()

// These functions provides the host with device specific USB descriptors during the enumeration process
void UsbGetDeviceDescriptor(USB_DeviceDescriptor *d);
bool UsbGetConfigurationDescriptor(USB_ConfigurationDescriptor *c, uint8_t confIndex);
bool UsbGetInterfaceDescriptor(USB_InterfaceDescriptor *i, uint8_t confIndex, uint8_t intIndex, uint8_t altSetting);
bool UsbGetEndpointDescriptor(USB_EndpointDescriptor *e, uint8_t confIndex, uint8_t intIndex, uint8_t altSetting, uint8_t endIndex);
void UsbGetStringDescriptor(char s[], uint8_t index);

// These functions allocate FIFO memory and setup all used endpoints
bool UsbDevSetConfiguration(uint8_t c);
bool UsbDevSetInterface(uint8_t conf, uint8_t inf, uint8_t as);

// User defined function, used in our application to start data acquisition
void UsbDevProcessVendorRequest(USB_DeviceRequest *req);

#endif
