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

/*
	Dual LUN Mass Storage demonstration application. This gives a simple
	reference application for implementing a multiple LUN USB Mass Storage
	device using the basic USB UFI drivers in all modern OSes (i.e. no
	special drivers required).
	
	On startup the system will automatically enumerate and function as an
	external mass storage device with two LUNs (seperate disks) which may
	be formatted and used in the same manner as commercial USB Mass Storage
	devices.
		
	You will need to format the mass storage drives upon first run of this
	demonstration.
	
	This demo is not restricted to only two LUNs; by changing the TOTAL_LUNS
	value in MassStorageDualLUN.h, any number of LUNs can be used (from 1 to
	255), with each LUN being allocated an equal portion of the available
	Dataflash memory.
*/

/*
	USB Mode:           Device
	USB Class:          Mass Storage Device
	USB Subclass:       Bulk Only
	Relevant Standards: USBIF Mass Storage Standard
	                    USB Bulk-Only Transport Standard
	                    SCSI Primary Commands Specification
	                    SCSI Block Commands Specification
	Usable Speeds:      Full Speed Mode
*/

#define INCLUDE_FROM_MASSSTORAGEDUALLUN_C
#include "MassStorage.h"

/* Project Tags, for reading out using the ButtLoad project */
BUTTLOADTAG(ProjName,     "MyUSB MassStore App");
BUTTLOADTAG(BuildTime,    __TIME__);
BUTTLOADTAG(BuildDate,    __DATE__);
BUTTLOADTAG(MyUSBVersion, "MyUSB V" MYUSB_VERSION_STRING);

/* Scheduler Task List */
TASK_LIST
{
	{ Task: USB_MassStorage      , TaskStatus: TASK_STOP },
};

/* Global Variables */
CommandBlockWrapper_t  CommandBlock;
CommandStatusWrapper_t CommandStatus = { Header: {Signature: CSW_SIGNATURE } };
volatile bool          IsMassStoreReset = false;


int main(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable Clock Division */
	SetSystemClockPrescaler(0);

	/* Hardware Initialization */
	LEDs_Init();
	Dataflash_Init(SPI_SPEED_FCPU_DIV_2);

	/* Clear Dataflash sector protections, if enabled */
	VirtualMemory_ResetDataflashProtections();
	
	/* Indicate USB not ready */
	LEDs_SetAllLEDs(LEDS_LED1 | LEDS_LED3);
	
	/* Initialize Scheduler so that it can be used */
	Scheduler_Init();

	/* Initialize USB Subsystem */
	USB_Init();

	/* Scheduling - routine never returns, so put this last in the main function */
	Scheduler_Start();
}

EVENT_HANDLER(USB_Reset)
{
	/* Select the control endpoint */
	Endpoint_SelectEndpoint(ENDPOINT_CONTROLEP);

	/* Enable the endpoint SETUP interrupt ISR for the control endpoint */
	USB_INT_Enable(ENDPOINT_INT_SETUP);
}

EVENT_HANDLER(USB_Connect)
{
	/* Indicate USB enumerating */
	LEDs_SetAllLEDs(LEDS_LED1 | LEDS_LED4);
	
	/* Reset the MSReset flag upon connection */
	IsMassStoreReset = false;
}

EVENT_HANDLER(USB_Disconnect)
{
	/* Stop running mass storage task */
	Scheduler_SetTaskMode(USB_MassStorage, TASK_STOP);

	/* Indicate USB not ready */
	LEDs_SetAllLEDs(LEDS_LED1 | LEDS_LED3);
}

EVENT_HANDLER(USB_ConfigurationChanged)
{
	/* Setup Mass Storage In and Out Endpoints */
	Endpoint_ConfigureEndpoint(MASS_STORAGE_IN_EPNUM, EP_TYPE_BULK,
		                       ENDPOINT_DIR_IN, MASS_STORAGE_IO_EPSIZE,
	                           ENDPOINT_BANK_SINGLE);

	Endpoint_ConfigureEndpoint(MASS_STORAGE_OUT_EPNUM, EP_TYPE_BULK,
		                       ENDPOINT_DIR_OUT, MASS_STORAGE_IO_EPSIZE,
	                           ENDPOINT_BANK_SINGLE);

	/* Indicate USB connected and ready */
	LEDs_SetAllLEDs(LEDS_LED2 | LEDS_LED4);
	
	/* Start mass storage task */
	Scheduler_SetTaskMode(USB_MassStorage, TASK_RUN);
}

EVENT_HANDLER(USB_UnhandledControlPacket)
{
	/* Process UFI specific control requests */
	switch (bRequest)
	{
		case MASS_STORAGE_RESET:
			if (bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				/* Indicate that the current transfer should be aborted */
				IsMassStoreReset = true;
			
				Endpoint_ClearSetupReceived();
				Endpoint_ClearSetupIN();
			}

			break;
		case GET_MAX_LUN:
			if (bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				/* Indicate to the host the number of supported LUNs (virtual disks) on the device */
				Endpoint_ClearSetupReceived();			
				Endpoint_Write_Byte(TOTAL_LUNS - 1);
				Endpoint_ClearSetupIN();
			}
			
			break;
	}
}
	
TASK(USB_MassStorage)
{
	/* Check if the USB System is connected to a Host */
	if (USB_IsConnected)
	{
		/* Select the Data Out Endpoint */
		Endpoint_SelectEndpoint(MASS_STORAGE_OUT_EPNUM);
		
		/* Check to see if a command from the host has been issued */
		if (Endpoint_ReadWriteAllowed())
		{	
			/* Indicate busy */
			LEDs_TurnOnLEDs(LEDS_LED3 | LEDS_LED4);

			/* Process sent command block from the host */
			if (ReadInCommandBlock())
			{
				/* Check direction of command, select Data IN endpoint if data is from the device */
				if (CommandBlock.Header.Flags & COMMAND_DIRECTION_DATA_IN)
				  Endpoint_SelectEndpoint(MASS_STORAGE_IN_EPNUM);

				/* Decode the recieved SCSI command */
				SCSI_DecodeSCSICommand();

				/* Load in the CBW tag into the CSW to link them together */
				CommandStatus.Header.Tag = CommandBlock.Header.Tag;

				/* Load in the Command Data residue into the CSW */
				CommandStatus.Header.SCSICommandResidue = CommandBlock.Header.DataTransferLength;

				/* Stall the selected data pipe if command failed (if data is still to be transferred) */
				if ((CommandStatus.Header.Status == Command_Fail) && (CommandStatus.Header.SCSICommandResidue))
				  Endpoint_StallTransaction();

				/* Return command status block to the host */
				ReturnCommandStatus();
				
				/* Clear the abort transfer flag */
				IsMassStoreReset = false;

				/* Indicate ready */
				LEDs_SetAllLEDs(LEDS_LED2 | LEDS_LED4);
			}
			else
			{
				/* Indicate error reading in the command block from the host */
				LEDs_SetAllLEDs(LEDS_LED1);
			}
		}
	}
}

static bool ReadInCommandBlock(void)
{
	/* Select the Data Out endpoint */
	Endpoint_SelectEndpoint(MASS_STORAGE_OUT_EPNUM);

	/* Read in command block header */
	Endpoint_Read_Stream_LE(&CommandBlock.Header, sizeof(CommandBlock.Header),
	                        AbortOnMassStoreReset);

	/* Verify the command block - abort if invalid */
	if ((CommandBlock.Header.Signature != CBW_SIGNATURE) ||
	    (CommandBlock.Header.LUN >= TOTAL_LUNS) ||
		(CommandBlock.Header.SCSICommandLength > MAX_SCSI_COMMAND_LENGTH))
	{
		/* Stall both data pipes until reset by host */
		Endpoint_StallTransaction();
		Endpoint_SelectEndpoint(MASS_STORAGE_IN_EPNUM);
		Endpoint_StallTransaction();
		
		return false;
	}

	/* Read in command block command data */
	Endpoint_Read_Stream_LE(&CommandBlock.SCSICommandData,
	                        CommandBlock.Header.SCSICommandLength,
	                        AbortOnMassStoreReset);
	  
	/* Clear the endpoint */
	Endpoint_ClearCurrentBank();
	
	return true;
}

static void ReturnCommandStatus(void)
{
	/* Select the Data Out endpoint */
	Endpoint_SelectEndpoint(MASS_STORAGE_OUT_EPNUM);

	/* While data pipe is stalled, process control requests */
	while (Endpoint_IsStalled())
	{
		/* Run the USB task manually to process any received control requests */
		USB_USBTask();
		
		/* Check if the current command is being aborted by the host */
		if (IsMassStoreReset)
		  return;
	}

	/* Select the Data In endpoint */
	Endpoint_SelectEndpoint(MASS_STORAGE_IN_EPNUM);

	/* While data pipe is stalled, process control requests */
	while (Endpoint_IsStalled())
	{
		/* Run the USB task manually to process any received control requests */
		USB_USBTask();

		/* Check if the current command is being aborted by the host */
		if (IsMassStoreReset)
		  return;
	}

	/* Write the CSW to the endpoint */
	Endpoint_Write_Stream_LE(&CommandStatus, sizeof(CommandStatus),
	                          AbortOnMassStoreReset);
	
	/* Send the CSW */
	Endpoint_ClearCurrentBank();
}

STREAM_CALLBACK(AbortOnMassStoreReset)
{	
	/* Abort if a Mass Storage reset command was received */
	if (IsMassStoreReset)
	  return STREAMCALLBACK_Abort;
	
	/* Continue with the current stream operation */
	return STREAMCALLBACK_Continue;
}

ISR(ENDPOINT_PIPE_vect)
{
	/* Check if the control endpoint has recieved a request */
	if (Endpoint_HasEndpointInterrupted(ENDPOINT_CONTROLEP))
	{
		/* Clear the endpoint interrupt */
		Endpoint_ClearEndpointInterrupt(ENDPOINT_CONTROLEP);

		/* Process the control request */
		USB_USBTask();

		/* Handshake the endpoint setup interrupt - must be after the call to USB_USBTask() */
		USB_INT_Clear(ENDPOINT_INT_SETUP);
	}
}
