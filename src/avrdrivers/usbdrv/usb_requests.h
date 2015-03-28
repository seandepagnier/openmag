// AT90USB/usb_requests.h
// Handling of USB (standard) requests
// S. Salewski 22-MAR-2007

#ifndef _USB_REQUESTS_H_
#define _USB_REQUESTS_H_

#include <stdint.h>
#include <stdbool.h>
#include "usb_spec.h"
#include "usb_api.h" // USB_MaxInterfaces

// USB-Standard-Device-Requests, H.J. Kelm USB 2.0, section 2.9.1, page 108
#define USB_StdDevReqGET_STATUS		0x00
#define USB_StdDevReqCLEAR_FEATURE	0x01
#define USB_StdDevReqSET_FEATURE	0x03
#define USB_StdDevReqSET_ADDRESS	0x05
#define USB_StdDevReqGET_DESCRIPTOR	0x06
#define USB_StdDevReqSET_DESCRIPTOR	0x07
#define USB_StdDevReqGET_CONFIGURATION	0x08
#define USB_StdDevReqSET_CONFIGURATION	0x09
#define USB_StdDevReqGET_INTERFACE	0x0A
#define USB_StdDevReqSET_INTERFACE	0x0B
#define USB_StdDevReqSYNCH_FRAME	0x0C

// Meaning of bmRequestType, H.J. Kelm USB 2.0, section 2.9.1, page 107
#define UsbIsDataHostToDevice(bm)	(!(bm & (1<<7)))
#define UsbIsDataDeviceToHost(bm)	(bm & (1<<7))

#define UsbIsStandardRequest(bm)	(bm & (3<<5)) == 0
#define UsbIsClassRequest(bm)		(bm & (3<<5)) == (1<<5)
#define UsbIsVendorRequest(bm)		(bm & (3<<5)) == (1<<6)

#define UsbIsRequestForDevice(bm)	(bm & (31) == 0
#define UsbIsRequestForInterface(bm)	(bm & (31) == 1
#define UsbIsRequestForEndpoint(bm)	(bm & (31) == 2
#define UsbIsRequestForOther(bm)	(bm & (31) == 3

extern uint8_t UsbDevConfValue; // current configuration of our device; 0 is unconfigured state
extern uint8_t AltSettingOfInterface [USB_MaxInterfaces]; // current alternate setting of active interfaces

void UsbProcessSetupRequest(void);

//void UsbDevWriteDescriptor(void *d, uint8_t *written, uint8_t requested);
//void UsbDevReadBytesN(void *c, uint8_t n);
//void WaitZLP_FromHost(void);
//void UsbProcessSetupRequest(void);
//bool UsbSendDescriptors(uint8_t cdi, uint8_t *written, uint8_t requested);
//bool UsbDevSetInterface(uint8_t inf, uint8_t as);
//bool UsbDevSetConfiguration(uint8_t c);

#endif
