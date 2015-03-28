// AT90USB/usb_spec.h
// USB datastructures as defined by www.usb.org
// S. Salewski 27-FEB-2007

#ifndef _USB_SPEC_H_
#define _USB_SPEC_H_
#include <stdint.h>

#define USB_Spec1_1	0x0110
#define USB_Spec2_0	0x0200

#define USB_ControlTransfer	0
#define USB_IsochronousTransfer	1
#define USB_BulkTransfer	2
#define USB_InterruptTransfer	3

#define USB_DeviceDescriptorType		0x01
#define USB_ConfigurationDescriptorType		0x02
#define USB_StringDescriptorType		0x03
#define USB_InterfaceDescriptorType		0x04
#define USB_EndpointDescriptorType		0x05
// #define USB_DeviceQualifierDescriptorType 0x06 // only used for high speed devices
// #define USB_OtherSpeedConfigurationDescriptorType 0x07 // only used for high speed devices

#define USB_DeviceDescriptorLength		0x12
#define USB_ConfigurationDescriptorLength	0x09
#define USB_InterfaceDescriptorLength		0x09
#define USB_EndpointDescriptorLength		0x07

#define USB_LanguageDescriptorIndex		0
#define USB_ManufacturerStringIndex		1
#define USB_ProductStringIndex			2
#define USB_SerialNumberStringIndex		3

// USB uses little endian format, avr-gcc too, so no byte-swap is necessary for 16 bit data

// USB-Device-Request, H.J. Kelm USB 2.0, section 2.9.1, page 107
typedef struct
{
  uint8_t	bmRequestType;
  uint8_t	bRequest;
  uint16_t	wValue;
  uint16_t	wIndex;
  uint16_t	wLength;
} USB_DeviceRequest;

// Device-Descriptor, H.J. Kelm, USB 2.0, section 2.8.2 (page 98) and section 9.4.7 (page 296)
typedef struct
{
  uint8_t	bLength;
  uint8_t	bDescriptorType;
  uint16_t	bcdUSB;
  uint8_t	bDeviceClass;
  uint8_t	bDeviceSubClass;
  uint8_t	bDeviceProtocoll;
  uint8_t	bMaxPacketSize0;
  uint16_t	idVendor;
  uint16_t	idProduct;
  uint16_t	bcdDevice;
  uint8_t	iManufacturer;
  uint8_t	iProduct;
  uint8_t	iSerialNumber;
  uint8_t	bNumConfigurations;
} USB_DeviceDescriptor;

// Configuration-Descriptor, H.J. Kelm, USB 2.0, section 2.8.3 (page 100) and section 9.4.7 (page 297)
typedef struct
{
  uint8_t	bLength;
  uint8_t	bDescriptorType;
  uint16_t	wTotalLength;
  uint8_t	bNumInterfaces;
  uint8_t	bConfigurationValue;
  uint8_t	iConfiguration;
  uint8_t	bmAttributes;
  uint8_t	MaxPower;
} USB_ConfigurationDescriptor;

// Interface-Descriptor, H.J. Kelm, USB 2.0, section 2.8.4 (page 101) and section 9.4.7 (page 297)
typedef struct
{
  uint8_t	bLength;
  uint8_t	bDescriptorType;
  uint8_t	bInterfaceNumber;
  uint8_t	bAlternateSetting;
  uint8_t	bNumEndpoints;
  uint8_t	bInterfaceClass;
  uint8_t	bInterfaceSubClass;
  uint8_t	bInterfaceProtocol;
  uint8_t	iInterface;
} USB_InterfaceDescriptor;

// Endpoint-Descriptor, H.J. Kelm, USB 2.0, section 2.8.5 (page 102) and section 9.4.7 (page 298)
typedef struct
{
  uint8_t	bLength;
  uint8_t	bDescriptorType;
  uint8_t	bEndpointAddress;
  uint8_t	bmAttributes;
  uint16_t	wMaxPacketSize;
  uint8_t	bInterval;
} USB_EndpointDescriptor;

/* not used
typedef char USB_StringDescriptor[USB_MaxStringDescriptorLength];
void InitDeviceDes(USB_DeviceDescriptor *d);
void InitConfigurationDes(USB_ConfigurationDescriptor *c);
void InitInterfaceDes(USB_InterfaceDescriptor *i);
void InitEndpointDes(USB_EndpointDescriptor *e);
*/

#if defined(DEBUG) && 0
void UsbDumpDeviceDescriptor(USB_DeviceDescriptor *d);
void UsbDumpConfigurationDescriptor(USB_ConfigurationDescriptor *c);
void UsbDumpInterfaceDescriptor(USB_InterfaceDescriptor *i);
void UsbDumpEndpointDescriptor(USB_EndpointDescriptor *e);
void UsbDumpStringDescriptor(char *s);
void UsbDumpSetupRequest(USB_DeviceRequest *req);
#else
#define UsbDumpDeviceDescriptor(d)
#define UsbDumpConfigurationDescriptor(c)
#define UsbDumpInterfaceDescriptor(i)
#define UsbDumpEndpointDescriptor(e)
#define UsbDumpStringDescriptor(s)
#define UsbDumpSetupRequest(req)
#endif

#endif
