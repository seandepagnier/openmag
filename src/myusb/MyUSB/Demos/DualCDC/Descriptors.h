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

	/* Macros: */
		#define CDC_FUNCTIONAL_DESCRIPTOR(size)                      \
		     struct                                                  \
		     {                                                       \
		          USB_Descriptor_CDCFunctional_Header_t FuncHeader;  \
		          uint8_t                               Data[size];  \
		     }

		#define CDC1_NOTIFICATION_EPNUM        3
		#define CDC1_TX_EPNUM                  1	
		#define CDC1_RX_EPNUM                  2	

		#define CDC2_NOTIFICATION_EPNUM        4
		#define CDC2_TX_EPNUM                  5	
		#define CDC2_RX_EPNUM                  6	

		#define CDC_NOTIFICATION_EPSIZE        8
		#define CDC_TXRX_EPSIZE                16	

	/* Type Defines: */	
		typedef struct
		{
			USB_Descriptor_Header_t                  Header;
			uint8_t                                  SubType;
		} USB_Descriptor_CDCFunctional_Header_t;

		typedef struct
		{
			USB_Descriptor_Configuration_Header_t    Config;
			USB_Descriptor_Interface_Association_t   IAD1;
			USB_Descriptor_Interface_t               CDC1_CCI_Interface;
			CDC_FUNCTIONAL_DESCRIPTOR(2)             CDC1_Functional_IntHeader;
			CDC_FUNCTIONAL_DESCRIPTOR(2)             CDC1_Functional_CallManagement;
			CDC_FUNCTIONAL_DESCRIPTOR(1)             CDC1_Functional_AbstractControlManagement;
			CDC_FUNCTIONAL_DESCRIPTOR(2)             CDC1_Functional_Union;
			USB_Descriptor_Endpoint_t                CDC1_ManagementEndpoint;
			USB_Descriptor_Interface_t               CDC1_DCI_Interface;
			USB_Descriptor_Endpoint_t                CDC1_DataOutEndpoint;
			USB_Descriptor_Endpoint_t                CDC1_DataInEndpoint;
			USB_Descriptor_Interface_Association_t   IAD2;
			USB_Descriptor_Interface_t               CDC2_CCI_Interface;
			CDC_FUNCTIONAL_DESCRIPTOR(2)             CDC2_Functional_IntHeader;
			CDC_FUNCTIONAL_DESCRIPTOR(2)             CDC2_Functional_CallManagement;
			CDC_FUNCTIONAL_DESCRIPTOR(1)             CDC2_Functional_AbstractControlManagement;
			CDC_FUNCTIONAL_DESCRIPTOR(2)             CDC2_Functional_Union;
			USB_Descriptor_Endpoint_t                CDC2_ManagementEndpoint;
			USB_Descriptor_Interface_t               CDC2_DCI_Interface;
			USB_Descriptor_Endpoint_t                CDC2_DataOutEndpoint;
			USB_Descriptor_Endpoint_t                CDC2_DataInEndpoint;
		} USB_Descriptor_Configuration_t;

	/* Function Prototypes: */
		bool USB_GetDescriptor(const uint16_t wValue, const uint8_t wIndex,
		                       void** const DescriptorAddress, uint16_t* const DescriptorSize)
		                       ATTR_WARN_UNUSED_RESULT ATTR_WEAK ATTR_NON_NULL_PTR_ARG(3, 4);

#endif
