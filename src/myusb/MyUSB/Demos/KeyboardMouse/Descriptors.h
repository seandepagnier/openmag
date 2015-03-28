/*
             MyUSB Library
     Copyright (C) Dean Camera, 2008.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2008  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, and distribute this software
  and its documentation for any purpose and without fee is hereby
  granted, provided that the above copyright notice appear in all
  copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

	/* Includes: */
		#include <MyUSB/Drivers/USB/USB.h>

		#include <avr/pgmspace.h>

	/* Type Defines: */
		typedef struct
		{
			USB_Descriptor_Header_t               Header;
				
			uint16_t                              HIDSpec;
			uint8_t                               CountryCode;
		
			uint8_t                               TotalHIDReports;

			uint8_t                               HIDReportType;
			uint16_t                              HIDReportLength;
		} USB_Descriptor_HID_t;

		typedef uint8_t USB_Descriptor_HIDReport_Datatype_t;

		typedef struct
		{
			USB_Descriptor_Configuration_Header_t Config;
			USB_Descriptor_Interface_t            KeyboardInterface;
			USB_Descriptor_HID_t                  KeyboardHID;
	        USB_Descriptor_Endpoint_t             KeyboardInEndpoint;
	        USB_Descriptor_Endpoint_t             KeyboardOutEndpoint;
			USB_Descriptor_Interface_t            MouseInterface;
			USB_Descriptor_HID_t                  MouseHID;
	        USB_Descriptor_Endpoint_t             MouseInEndpoint;
		} USB_Descriptor_Configuration_t;
					
	/* Macros: */
		#define KEYBOARD_IN_EPNUM              1
		#define KEYBOARD_OUT_EPNUM             2
		#define MOUSE_IN_EPNUM                 3
		#define HID_EPSIZE                     8

		#define DTYPE_HID                      0x21
		#define DTYPE_Report                   0x22

	/* External Variables: */
		extern USB_Descriptor_HIDReport_Datatype_t MouseReport[];
		extern USB_Descriptor_HIDReport_Datatype_t KeyboardReport[];
		extern USB_Descriptor_Configuration_t      ConfigurationDescriptor;

	/* Function Prototypes: */
		bool USB_GetDescriptor(const uint16_t wValue, const uint8_t wIndex,
		                       void** const DescriptorAddress, uint16_t* const DescriptorSize)
		                       ATTR_WARN_UNUSED_RESULT ATTR_WEAK ATTR_NON_NULL_PTR_ARG(3, 4);

#endif
