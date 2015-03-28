// AT90USB/usb_spec.c
// Initialization of USB datastructures
// S. Salewski 27-FEB-2007

#include "usb_spec.h"
#include "usb_api.h"

// USB uses little endian format, avr-gcc too, so no byte-swap is necessary for 16 bit data

/* set defaults, we don't really need this

void
InitDeviceDes(USB_DeviceDescriptor *d)
{
  d->bLength = USB_DeviceDescriptorLength;
  d->bDescriptorType = USB_DeviceDescriptorType;
  d->bcdUSB = USB_Spec1_1;
  d->bDeviceClass = 0;
  d->bDeviceSubClass = 0;
  d->bDeviceProtocoll = 0;
  d->bMaxPacketSize0 = EP0_FIFO_Size;
  d->idVendor = MyUSB_VendorID;
  d->idProduct = MyUSB_ProductID;
  d->bcdDevice = MyUSB_DeviceBCD;
  d->iManufacturer = USB_ManufacturerStringIndex;
  d->iProduct = USB_ProductStringIndex;
  d->iSerialNumber = USB_SerialNumberStringIndex;
  d->bNumConfigurations = 1;
}

void
InitConfigurationDes(USB_ConfigurationDescriptor *c)
{
  c->bLength = USB_ConfigurationDescriptorLength;
  c->bDescriptorType = USB_ConfigurationDescriptorType;
  c->wTotalLength = 0; // this and the following fields must be filled with correct values!
  c->bNumInterfaces = 0;
  c->bConfigurationValue = 0;
  c->iConfiguration = 0;
  c->bmAttributes = 0;
  c->MaxPower = 0;
}

void
InitInterfaceDes(USB_InterfaceDescriptor *i)
{
  i->bLength = USB_InterfaceDescriptorLength;
  i->bDescriptorType = USB_InterfaceDescriptorType;
  i->bInterfaceNumber = 0; // this and the following fields must be filled with correct values!
  i->bAlternateSetting = 0;
  i->bNumEndpoints = 0;
  i->bInterfaceClass = 0;
  i->bInterfaceSubClass = 0;
  i->bInterfaceProtocol = 0;
  i->iInterface = 0;
}

void
InitEndpointDes(USB_EndpointDescriptor *e)
{
  e->bLength = USB_EndpointDescriptorLength;
  e->bDescriptorType = USB_EndpointDescriptorType;
  e->bEndpointAddress = 0; // this and the following fields must be filled with correct values!
  e->bmAttributes = 0;
  e->wMaxPacketSize = 0;
  e->bInterval = 0;
}

*/
