/*
  MyUSB Library
  Copyright (C) Dean Camera, 2008.
              
  dean [at] fourwalledcubicle [dot] com
  www.fourwalledcubicle.com

  Released under the LGPL Licence, Version 3
*/

/* Modified for openmag by Sean D'Epagnier (C) 2008 */

#include "descriptors.h"
extern uint8_t useusbcdc, useusbmouse, useusbjoystick;
#define MAKE_DEVICE_DESCRIPTOR(PID) \
   Header:                 {Size: sizeof(USB_Descriptor_Device_t), Type: DTYPE_Device}, \
   USBSpecification:       VERSION_BCD(01.10), \
   Class:                  0x02, \
   SubClass:               0x00, \
   Protocol:               0x00, \
   Endpoint0Size:          8, \
   VendorID:               0x0000, \
   ProductID:              PID, \
   ReleaseNumber:          0x0000, \
   ManufacturerStrIndex:   0x01, \
   ProductStrIndex:        0x02, \
   SerialNumStrIndex:      0x03, \
   NumberOfConfigurations: 1

#define MAKE_DESCRIPTOR_CONFIGURATION(SIZE, INTERFACES) \
   Config: \
   { \
      Header:        {Size: sizeof(USB_Descriptor_Configuration_Header_t), \
                      Type: DTYPE_Configuration}, \
      TotalConfigurationSize: SIZE,  \
      TotalInterfaces:        INTERFACES, \
      ConfigurationNumber:    1, \
      ConfigurationStrIndex:  NO_DESCRIPTOR_STRING, \
      ConfigAttributes:       (USB_CONFIG_ATTR_BUSPOWERED | USB_CONFIG_ATTR_SELFPOWERED), \
      MaxPowerConsumption:    USB_CONFIG_POWER_MA(100) \
   },

#ifdef USE_MOUSE
USB_Descriptor_HIDReport_Datatype_t MouseReport[] PROGMEM =
{
#if 1
   0x05, 0x01,     /* Usage Page (Generic Desktop)             */
   0x09, 0x02,     /* Usage (Mouse)                            */
   0xA1, 0x01,     /* Collection (Application)                 */
   0x09, 0x01,     /*   Usage (Pointer)                        */
   0xA1, 0x00,     /*   Collection (Application)               */
   0x05, 0x09,     /*     Usage Page (Button)                  */
   0x19, 0x01,     /*     Usage Minimum (1)                    */
   0x29, 0x02,     /*     Usage Maximum (2)                    */
   0x15, 0x00,     /*     Logical Minimum (0)                  */
   0x25, 0x01,     /*     Logical Maximum (1)                  */
   0x75, 0x01,     /*     Report Size (1)                      */
   0x95, 0x03,     /*     Report Count (3)                     */
   0x81, 0x02,     /*     Input (Data, Variable, Absolute)     */
   0x75, 0x05,     /*     Report Size (5)                      */
   0x95, 0x01,     /*     Report Count (1)                     */
   0x81, 0x01,     /*     Input (Constant)                     */
   0x05, 0x01,     /*     Usage Page (Generic Desktop Control) */
   0x09, 0x30,     /*     Usage X                              */
   0x09, 0x31,     /*     Usage Y                              */
   0x09, 0x38,     /*     Usage Wheel                          */
   0x15, 0x81,     /*     Logical Minimum (-127)               */
   0x25, 0x7F,     /*     Logical Maximum (127)                */
   0x35, 0x81,     /*     Physical Minimum (-127)              */
   0x45, 0x7F,     /*     Physical Maximum (127)               */
   0x55, 0x00,     /*     Unit Exponent (0)                    */
   0x65, 0x13,     /*     Unit (inch)                          */
   0x75, 0x08,     /*     Report Size (8)                      */
   0x95, 0x03,     /*     Report Count (3)                     */
   0x81, 0x06,     /*     Input (Data, Variable, Relative)     */
   0xC0,           /*   End Collection                         */
   0xC0            /* End Collection                           */
#else
   0x05, 0x0C, 0x09, 0x01, 0xA1, 0x01, 0x05, 0x01, 0x09, 0x02, 0xA1, 0x02, 0x85, 0x13, 0x05, 0x0C,
   0x0A, 0x38, 0x02, 0x95, 0x01, 0x75, 0x08, 0x15, 0x81, 0x25, 0x7F, 0x81, 0x06, 0x85, 0x17, 0x06,
   0x00, 0xFF, 0x0A, 0x06, 0xFF, 0x15, 0x00, 0x25, 0x01, 0x35, 0x01, 0x45, 0x04, 0x95, 0x01, 0x75,
   0x02, 0xB1, 0x02, 0x35, 0x00, 0x45, 0x00, 0xB1, 0x01, 0x0A, 0x04, 0xFF, 0x75, 0x01, 0xB1, 0x02,
   0x75, 0x03, 0xB1, 0x01, 0x85, 0x18, 0x0A, 0x08, 0xFF, 0x75, 0x01, 0xB1, 0x02, 0x75, 0x07, 0xB1,
   0x01, 0xC0, 0xC0, 0x05, 0x01, 0x09, 0x02, 0xA1, 0x01, 0x05, 0x01, 0x09, 0x02, 0xA1, 0x02, 0x85,
   0x11, 0x09, 0x01, 0xA1, 0x00, 0x05, 0x09, 0x19, 0x01, 0x29, 0x03, 0x95, 0x03, 0x75, 0x01, 0x25,
   0x01, 0x81, 0x02, 0x95, 0x01, 0x81, 0x01, 0x09, 0x05, 0x81, 0x02, 0x95, 0x03, 0x81, 0x01, 0x05,
   0x01, 0x09, 0x30, 0x09, 0x31, 0x95, 0x02, 0x75, 0x08, 0x15, 0x81, 0x25, 0x7F, 0x81, 0x06, 0xA1,
   0x02, 0x85, 0x12, 0x09, 0x48, 0x95, 0x01, 0x75, 0x02, 0x15, 0x00, 0x25, 0x01, 0x35, 0x01, 0x45,
   0x04, 0xB1, 0x02, 0x35, 0x00, 0x45, 0x00, 0x75, 0x06, 0xB1, 0x01, 0x85, 0x11, 0x09, 0x38, 0x15,
   0x81, 0x25, 0x7F, 0x75, 0x08, 0x81, 0x06, 0xC0, 0x05, 0x0C, 0x75, 0x08, 0x0A, 0x38, 0x02, 0x81,
 0x06, 0xC0, 0xC0, 0xC0
#endif
};
#endif

#ifdef USE_JOYSTICK
USB_Descriptor_HIDReport_Datatype_t JoystickReport[] PROGMEM =
{
	0x05, 0x01,          /* Usage Page (Generic Desktop)                       */
	0x09, 0x04,          /* Usage (Joystick)                                   */
	0xa1, 0x01,          /* Collection (Application)                           */
	0x09, 0x01,          /*   Usage (Pointer)                                  */
	0xa1, 0x00,          /*   Collection (Physical)                            */
	0x05, 0x01,          /*     Usage Page (Generic Desktop)                   */
	0x09, 0x30,          /*     Usage (X)                                      */
	0x09, 0x31,          /*     Usage (Y)                                      */
	0x09, 0x32,          /*     Usage (Z)                                      */
	0x09, 0x33,          /*     Usage (Rx)                                     */
	0x09, 0x34,          /*     Usage (Ry)                                     */
	0x09, 0x35,          /*     Usage (Rz)                                     */
        0x15, 0x81,          /*     Logical Minimum (-127)               */
        0x25, 0x7F,          /*     Logical Maximum (127)                */
        0x35, 0x81,     /*     Physical Minimum (-127)              */
        0x45, 0x7F,     /*     Physical Maximum (127)               */
        0x55, 0x00,     /*     Unit Exponent (0)                    */
        0x65, 0x13,     /*     Unit (inch)                          */
	0x75, 0x10,          /*     Report Size (16)                                */
	0x95, 0x06,          /*     Report Count (6)                               */
	0x81, 0x82,          /*     Input (Data, Variable, Absolute, Volatile)     */
	0x05, 0x09,          /*     Usage Page (Button)                            */
	0x09, 0x01,          /*     Usage (Button 1)                               */
	0x09, 0x02,          /*     Usage (Button 2)                               */
	0x09, 0x03,          /*     Usage (Button 3)                               */
	0x15, 0x00,          /*     Logical Minimum (0)                            */
	0x25, 0x01,          /*     Logical Maximum (1)                            */
	0x75, 0x01,          /*     Report Size (1)                                */
	0x95, 0x03,          /*     Report Count (3)                               */
	0x81, 0x02,          /*     Input (Data, Variable, Absolute)               */
	0x75, 0x05,          /*     Report Size (5)                                */
	0x95, 0x01,          /*     Report Count (1)                               */
	0x81, 0x01,          /*     Input (Constant)                               */
	0xc0,                /*   End Collection                                   */
	0xc0                 /* End Collection                                     */
};
#endif

#ifdef USE_JOYSTICK
# ifdef USE_MOUSE
#  ifdef USE_CDC
USB_Descriptor_Device_t DeviceDescriptorMousePlusCDCPlusJoystick PROGMEM = {
   MAKE_DEVICE_DESCRIPTOR(0x3037)
};

USB_Descriptor_Configuration_MousePlusCDCPlusJoystick_t ConfigurationDescriptorMousePlusCDCPlusJoystick PROGMEM = {
   MAKE_DESCRIPTOR_CONFIGURATION(sizeof(USB_Descriptor_Configuration_MousePlusCDCPlusJoystick_t), 4)
#include "usbinterfaces/cdc.h"
#include "usbinterfaces/mouse.h"
#include "usbinterfaces/joystick.h"
};

#  endif

USB_Descriptor_Device_t DeviceDescriptorMousePlusJoystick PROGMEM = {
   MAKE_DEVICE_DESCRIPTOR(0x3035)
};

USB_Descriptor_Configuration_MousePlusJoystick_t ConfigurationDescriptorMousePlusJoystick PROGMEM = {
   MAKE_DESCRIPTOR_CONFIGURATION(sizeof(USB_Descriptor_Configuration_MousePlusJoystick_t), 2)
#include "usbinterfaces/mouse.h"
#include "usbinterfaces/joystick.h"
};
# endif

# ifdef USE_CDC
USB_Descriptor_Device_t DeviceDescriptorCDCPlusJoystick PROGMEM = {
   MAKE_DEVICE_DESCRIPTOR(0x3036)
};

USB_Descriptor_Configuration_CDCPlusJoystick_t ConfigurationDescriptorCDCPlusJoystick PROGMEM = {
   MAKE_DESCRIPTOR_CONFIGURATION(sizeof(USB_Descriptor_Configuration_CDCPlusJoystick_t), 3)
#include "usbinterfaces/cdc.h"
#include "usbinterfaces/joystick.h"
};
# endif

USB_Descriptor_Device_t DeviceDescriptorJoystick PROGMEM = {
   MAKE_DEVICE_DESCRIPTOR(0x3034)
};

USB_Descriptor_Configuration_Joystick_t ConfigurationDescriptorJoystick PROGMEM = {
   MAKE_DESCRIPTOR_CONFIGURATION(sizeof(USB_Descriptor_Configuration_Joystick_t), 1)
#include "usbinterfaces/joystick.h"
};
#endif

#ifdef USE_MOUSE
# ifdef USE_CDC
USB_Descriptor_Device_t DeviceDescriptorMousePlusCDC PROGMEM = {
   MAKE_DEVICE_DESCRIPTOR(0x3033)
};

USB_Descriptor_Configuration_MousePlusCDC_t ConfigurationDescriptorMousePlusCDC PROGMEM = {
   MAKE_DESCRIPTOR_CONFIGURATION(sizeof(USB_Descriptor_Configuration_MousePlusCDC_t), 3)
#include "usbinterfaces/cdc.h"
#include "usbinterfaces/mouse.h"
};
# endif
USB_Descriptor_Device_t DeviceDescriptorMouse PROGMEM = {
   MAKE_DEVICE_DESCRIPTOR(0x3031)
};

USB_Descriptor_Configuration_Mouse_t ConfigurationDescriptorMouse PROGMEM = {
   MAKE_DESCRIPTOR_CONFIGURATION(sizeof(USB_Descriptor_Configuration_Mouse_t), 1)
#include "usbinterfaces/mouse.h"
};
#endif

#ifdef USE_CDC
USB_Descriptor_Device_t DeviceDescriptorCDC PROGMEM = {
   MAKE_DEVICE_DESCRIPTOR(0x3032)
};

USB_Descriptor_Configuration_CDC_t ConfigurationDescriptorCDC PROGMEM = {
   MAKE_DESCRIPTOR_CONFIGURATION(sizeof(USB_Descriptor_Configuration_CDC_t), 2)
#include "usbinterfaces/cdc.h"
};

# if defined(USE_MOUSE) || defined(USE_JOYSTICK)
USB_Descriptor_Device_t DeviceDescriptorNone PROGMEM = {
   MAKE_DEVICE_DESCRIPTOR(0x3030)
};

USB_Descriptor_Configuration_None_t ConfigurationDescriptorNone PROGMEM = {
   MAKE_DESCRIPTOR_CONFIGURATION(sizeof(USB_Descriptor_Configuration_None_t), 0)
};
# endif
#endif

USB_Descriptor_String_t LanguageString PROGMEM =
{
	Header:                 {Size: USB_STRING_LEN(1), Type: DTYPE_String},
	UnicodeString:          {LANGUAGE_ID_ENG}
};

USB_Descriptor_String_t ManafacturerString PROGMEM = {
   Header:                 {Size: USB_STRING_LEN(15), Type: DTYPE_String},
   UnicodeString:          L"Sean D'Epagnier"
};

USB_Descriptor_String_t ProductString PROGMEM = {
   Header:                 {Size: USB_STRING_LEN(8), Type: DTYPE_String},
   UnicodeString:          L"Open Mag"
};

USB_Descriptor_String_t SerialNumberString PROGMEM = {
   Header:                 {Size: USB_STRING_LEN(13), Type: DTYPE_String},
   UnicodeString:          L"000000000000"
};

_Bool USB_GetDescriptor(const uint16_t wValue, const uint8_t wIndex,
                       void** const DescriptorAddress, uint16_t* const DescriptorSize)
{
   void *Address = NULL, *DeviceAddress = NULL, *ConfigurationAddress = NULL;
   uint16_t Size    = 0;

#ifdef USE_CDC
# if defined(USE_MOUSE) || defined(USE_JOYSTICK)
#  ifdef USE_JOYSTICK
   if(useusbjoystick)
#   ifdef USE_MOUSE
      if(useusbmouse)
         if(useusbcdc) {
            DeviceAddress = DESCRIPTOR_ADDRESS(DeviceDescriptorMousePlusCDCPlusJoystick);
            ConfigurationAddress = DESCRIPTOR_ADDRESS(ConfigurationDescriptorMousePlusCDCPlusJoystick);
            Size    = sizeof(USB_Descriptor_Configuration_MousePlusCDCPlusJoystick_t);
         } else {
            DeviceAddress = DESCRIPTOR_ADDRESS(DeviceDescriptorMousePlusJoystick);
            ConfigurationAddress = DESCRIPTOR_ADDRESS(ConfigurationDescriptorMousePlusJoystick);
            Size    = sizeof(USB_Descriptor_Configuration_MousePlusJoystick_t);
         }
      else
#   endif
         if(useusbcdc) {
            DeviceAddress = DESCRIPTOR_ADDRESS(DeviceDescriptorCDCPlusJoystick);
            ConfigurationAddress = DESCRIPTOR_ADDRESS(ConfigurationDescriptorCDCPlusJoystick);
            Size    = sizeof(USB_Descriptor_Configuration_CDCPlusJoystick_t);
         }
         else {
            DeviceAddress = DESCRIPTOR_ADDRESS(DeviceDescriptorJoystick);
            ConfigurationAddress = DESCRIPTOR_ADDRESS(ConfigurationDescriptorJoystick);
            Size    = sizeof(USB_Descriptor_Configuration_Joystick_t);

         }
   else
#  endif
#  ifdef USE_MOUSE
      if(useusbmouse)
         if(useusbcdc) {
            DeviceAddress = DESCRIPTOR_ADDRESS(DeviceDescriptorMousePlusCDC);
            ConfigurationAddress = DESCRIPTOR_ADDRESS(ConfigurationDescriptorMousePlusCDC);
            Size    = sizeof(USB_Descriptor_Configuration_MousePlusCDC_t);
         } else {
            DeviceAddress = DESCRIPTOR_ADDRESS(DeviceDescriptorMouse);
            ConfigurationAddress = DESCRIPTOR_ADDRESS(ConfigurationDescriptorMouse);
               Size    = sizeof(USB_Descriptor_Configuration_Mouse_t);
         }
      else
#  endif
         if(useusbcdc) {
            DeviceAddress = DESCRIPTOR_ADDRESS(DeviceDescriptorCDC);
               ConfigurationAddress = DESCRIPTOR_ADDRESS(ConfigurationDescriptorCDC);
               Size    = sizeof(USB_Descriptor_Configuration_CDC_t);
         } else {
            DeviceAddress = DESCRIPTOR_ADDRESS(DeviceDescriptorNone);
            ConfigurationAddress = DESCRIPTOR_ADDRESS(ConfigurationDescriptorNone);
            Size    = sizeof(USB_Descriptor_Configuration_None_t);
         }
# else
   DeviceAddress = DESCRIPTOR_ADDRESS(DeviceDescriptorCDC);
   ConfigurationAddress = DESCRIPTOR_ADDRESS(ConfigurationDescriptorCDC);
   Size    = sizeof(USB_Descriptor_Configuration_CDC_t);
# endif
#endif

   switch (wValue >> 8) {
   case DTYPE_Device:
       Address = DeviceAddress;
       Size    = sizeof(USB_Descriptor_Device_t);
       break;
   case DTYPE_Configuration:
      Address = ConfigurationAddress;
      break;
   case DTYPE_String:
      switch (wValue & 0xFF) {
      case 0x00:
	 Address = DESCRIPTOR_ADDRESS(LanguageString);
         Size    = pgm_read_byte(&LanguageString.Header.Size);
	 break;
      case 0x01:
	 Address = DESCRIPTOR_ADDRESS(ManafacturerString);
	 Size    = pgm_read_byte(&ManafacturerString.Header.Size);
	 break;
      case 0x02:
	 Address = DESCRIPTOR_ADDRESS(ProductString);
	 Size    = pgm_read_byte(&ProductString.Header.Size);
	 break;
      case 0x03:
	 Address = DESCRIPTOR_ADDRESS(SerialNumberString);
	 Size    = pgm_read_byte(&SerialNumberString.Header.Size);
	 break;
      }
      break;
   case DTYPE_HID:
#ifdef USE_JOYSTICK
# ifdef USE_MOUSE
      if(wIndex == 2)
         Address = DESCRIPTOR_ADDRESS(ConfigurationDescriptorMouse.MouseHID);
      else
         Address = DESCRIPTOR_ADDRESS(ConfigurationDescriptorJoystick.JoystickHID);
# else
       Address = DESCRIPTOR_ADDRESS(ConfigurationDescriptorJoystick.JoystickHID);
# endif
#else
# ifdef USE_MOUSE
       Address = DESCRIPTOR_ADDRESS(ConfigurationDescriptorMouse.MouseHID);
# endif
#endif
#if defined(USE_MOUSE) || defined(USE_JOYSTICK)
       Size    = sizeof(USB_Descriptor_HID_t);
#endif
       break;
   case DTYPE_Report:
#ifdef USE_JOYSTICK
# ifdef USE_MOUSE
      if(wIndex == 2) {
         Address = DESCRIPTOR_ADDRESS(MouseReport);
         Size    = sizeof(MouseReport);
      } else {
         Address = DESCRIPTOR_ADDRESS(JoystickReport);
         Size    = sizeof(JoystickReport);
      }
# else
      Address = DESCRIPTOR_ADDRESS(JoystickReport);
      Size    = sizeof(JoystickReport);
# endif
#else
# ifdef USE_MOUSE
      Address = DESCRIPTOR_ADDRESS(MouseReport);
      Size    = sizeof(MouseReport);
# endif
#endif
      break;
   }
	
   if (Address != NULL) {
      *DescriptorAddress = Address;
      *DescriptorSize    = Size;

      return true;
   }

   return false;
}
