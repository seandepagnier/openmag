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

#include "ConfigDescriptor.h"

uint8_t ProcessConfigurationDescriptor(void)
{
	uint8_t* ConfigDescriptorData;
	uint16_t ConfigDescriptorSize;
	
	/* Get Configuration Descriptor size from the device */
	if (USB_Host_GetDeviceConfigDescriptor(&ConfigDescriptorSize, NULL) != HOST_SENDCONTROL_Successful)
	  return ControlError;
	
	/* Ensure that the Configuration Descriptor isn't too large */
	if (ConfigDescriptorSize > MAX_CONFIG_DESCRIPTOR_SIZE)
	  return DescriptorTooLarge;
	  
	/* Allocate enough memory for the entire config descriptor */
	ConfigDescriptorData = alloca(ConfigDescriptorSize);

	/* Retrieve the entire configuration descriptor into the allocated buffer */
	USB_Host_GetDeviceConfigDescriptor(&ConfigDescriptorSize, ConfigDescriptorData);
	
	/* Validate returned data - ensure first entry is a configuration header descriptor */
	if (DESCRIPTOR_TYPE(ConfigDescriptorData) != DTYPE_Configuration)
	  return InvalidConfigDataReturned;
	
	/* Get the keyboard interface from the configuration descriptor */
	if (USB_Host_GetNextDescriptorComp(&ConfigDescriptorSize, &ConfigDescriptorData, NextKeyboardInterface))
	{
		/* Descriptor not found, error out */
		return NoHIDInterfaceFound;
	}

	/* Get the keyboard interface's data endpoint descriptor */
	if (USB_Host_GetNextDescriptorComp(&ConfigDescriptorSize, &ConfigDescriptorData,
	                                                NextInterfaceKeyboardDataEndpoint))
	{
		/* Descriptor not found, error out */
		return NoEndpointFound;
	}
	
	/* Retrieve the endpoint address from the endpoint descriptor */
	USB_Descriptor_Endpoint_t* EndpointData = DESCRIPTOR_PCAST(ConfigDescriptorData, USB_Descriptor_Endpoint_t);

	/* Configure the keyboard data pipe */
	Pipe_ConfigurePipe(KEYBOARD_DATAPIPE, EP_TYPE_INTERRUPT, PIPE_TOKEN_IN,
	                   EndpointData->EndpointAddress, EndpointData->EndpointSize, PIPE_BANK_SINGLE);

	Pipe_SetInfiniteINRequests();
	Pipe_SetInterruptPeriod(EndpointData->PollingIntervalMS);
	Pipe_Unfreeze();

	/* Enable the pipe IN interrupt for the data pipe */
	USB_INT_Enable(PIPE_INT_IN);

	/* Valid data found, return success */
	return SuccessfulConfigRead;
}

DESCRIPTOR_COMPARATOR(NextKeyboardInterface)
{
	/* PURPOSE: Find next keyboard class interface descriptor */

	if (DESCRIPTOR_TYPE(CurrentDescriptor) == DTYPE_Interface)
	{
		/* Check the HID descriptor class and protocol, break out if correct class/protocol interface found */
		if ((DESCRIPTOR_CAST(CurrentDescriptor, USB_Descriptor_Interface_t).Class    == KEYBOARD_CLASS) &&
		    (DESCRIPTOR_CAST(CurrentDescriptor, USB_Descriptor_Interface_t).Protocol == KEYBOARD_PROTOCOL))
		{
			return Descriptor_Search_Found;
		}
	}
	
	return Descriptor_Search_NotFound;
}

DESCRIPTOR_COMPARATOR(NextInterfaceKeyboardDataEndpoint)
{
	/* PURPOSE: Find next interface endpoint descriptor before next interface descriptor */

	if (DESCRIPTOR_TYPE(CurrentDescriptor) == DTYPE_Endpoint)
	{
		if (DESCRIPTOR_CAST(CurrentDescriptor, USB_Descriptor_Endpoint_t).EndpointAddress & ENDPOINT_DESCRIPTOR_DIR_IN)
		  return Descriptor_Search_Found;
	}
	else if (DESCRIPTOR_TYPE(CurrentDescriptor) == DTYPE_Interface)
	{
		return Descriptor_Search_Fail;
	}

	return Descriptor_Search_NotFound;
}
