/*
  MyUSB Library
  Copyright (C) Dean Camera, 2008.
              
  dean [at] fourwalledcubicle [dot] com
  www.fourwalledcubicle.com

  Released under the LGPL Licence, Version 3
*/

/* Modified by Sean D'Epagnier for openmag */

#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

/* Includes: */
#include <MyUSB/Drivers/USB/USB.h>

#include <avr/pgmspace.h>

/* Macros: */
#define CDC_FUNCTIONAL_DESCRIPTOR(size)                      \
     struct                                                  \
     {                                                       \
          USB_Descriptor_CDCFunctional_Header_t FuncHeader;  \
          uint8_t                               Data[size];  \
     }
#define CDC_NOTIFICATION_EPSIZE        8
#define CDC_TXRX_EPSIZE                64

#define MOUSE_EPSIZE                 8
#define JOYSTICK_EPSIZE              13

#define DTYPE_HID                 0x21
#define DTYPE_Report              0x22


enum {CONTROL_EPNUM = 1,
      CDC_NOTIFICATION_EPNUM, CDC_TX_EPNUM, CDC_RX_EPNUM,
      MOUSE_EPNUM, JOYSTICK_EPNUM,
};


/* Type Defines: */
typedef struct
{
   USB_Descriptor_Header_t               Header;
   uint8_t                               SubType;
} USB_Descriptor_CDCFunctional_Header_t;

#if defined(USE_MOUSE) || defined(USE_JOYSTICK)
typedef struct
{
   USB_Descriptor_Header_t               Header;
   uint16_t                              HIDSpec;
   uint8_t                               CountryCode;
   uint8_t                               TotalHIDDescriptors;
   uint8_t                               HIDReportType;
   uint16_t                              HIDReportLength;
} USB_Descriptor_HID_t;

typedef uint8_t USB_Descriptor_HIDReport_Datatype_t;
extern USB_Descriptor_HIDReport_Datatype_t MouseReport[];
extern USB_Descriptor_HIDReport_Datatype_t JoystickReport[];

#endif

#define CDC_PARTS \
   USB_Descriptor_Interface_t               CCI_Interface; \
   CDC_FUNCTIONAL_DESCRIPTOR(2)             CDC_Functional_IntHeader; \
   CDC_FUNCTIONAL_DESCRIPTOR(2)             CDC_Functional_CallManagement; \
   CDC_FUNCTIONAL_DESCRIPTOR(1)             CDC_Functional_AbstractControlManagement; \
   CDC_FUNCTIONAL_DESCRIPTOR(2)             CDC_Functional_Union; \
   USB_Descriptor_Endpoint_t                ManagementEndpoint; \
   USB_Descriptor_Interface_t               DCI_Interface; \
   USB_Descriptor_Endpoint_t                DataOutEndpoint; \
   USB_Descriptor_Endpoint_t                DataInEndpoint;

#define MOUSE_PARTS \
   USB_Descriptor_Interface_t            Mouse_Interface; \
   USB_Descriptor_HID_t                  MouseHID; \
   USB_Descriptor_Endpoint_t             MouseEndpoint;

#define JOYSTICK_PARTS \
   USB_Descriptor_Interface_t            Joystick_Interface; \
   USB_Descriptor_HID_t                  JoystickHID; \
   USB_Descriptor_Endpoint_t             JoystickEndpoint;

#ifdef USE_JOYSTICK
# ifdef USE_MOUSE
#  ifdef USE_CDC
typedef struct
{
   USB_Descriptor_Configuration_Header_t    Config;
   CDC_PARTS
   MOUSE_PARTS
   JOYSTICK_PARTS
} USB_Descriptor_Configuration_MousePlusCDCPlusJoystick_t;

#  endif
typedef struct
{
   USB_Descriptor_Configuration_Header_t    Config;
   MOUSE_PARTS
   JOYSTICK_PARTS
} USB_Descriptor_Configuration_MousePlusJoystick_t;
# endif
# ifdef USE_CDC
typedef struct
{
   USB_Descriptor_Configuration_Header_t    Config;
   CDC_PARTS
   JOYSTICK_PARTS
} USB_Descriptor_Configuration_CDCPlusJoystick_t;
# endif
typedef struct
{
   USB_Descriptor_Configuration_Header_t    Config;
   JOYSTICK_PARTS
} USB_Descriptor_Configuration_Joystick_t;
#endif

#ifdef USE_MOUSE
# ifdef USE_CDC
typedef struct
{
   USB_Descriptor_Configuration_Header_t    Config;
   CDC_PARTS
   MOUSE_PARTS
} USB_Descriptor_Configuration_MousePlusCDC_t;
# endif
typedef struct
{
   USB_Descriptor_Configuration_Header_t    Config;
   MOUSE_PARTS
} USB_Descriptor_Configuration_Mouse_t;
#endif

#ifdef USE_CDC
typedef struct
{
   USB_Descriptor_Configuration_Header_t    Config;
   CDC_PARTS
} USB_Descriptor_Configuration_CDC_t;
#endif

typedef struct
{
   USB_Descriptor_Configuration_Header_t    Config;
} USB_Descriptor_Configuration_None_t;

/* Function Prototypes: */
bool USB_GetDescriptor(const uint16_t wValue, const uint8_t wIndex,
                       void** const DescriptorAddress, uint16_t* const DescriptorSize)
   ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3, 4);

#undef bool

#endif
