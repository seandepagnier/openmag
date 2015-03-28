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
	Communications Device Class demonstration application.
	This gives a simple reference application for implementing
	a USB to Serial converter using the CDC class. Sent and
	recieved data on the serial port is communicated to the USB
	host.
	
	Before running, you will need to install the INF file that
	is located in the USBtoSerial project directory. This will enable
	Windows to use its inbuilt CDC drivers, negating the need
	for special Windows drivers for the device.
*/

/*
	USB Mode:           Device
	USB Class:          Communications Device Class (CDC)
	USB Subclass:       Abstract Control Model (ACM)
	Relevant Standards: USBIF CDC Class Standard
	Usable Speeds:      Full Speed Mode
*/

#include "USBtoSerial.h"

/* Project Tags, for reading out using the ButtLoad project */
BUTTLOADTAG(ProjName,     "MyUSB USB RS232 App");
BUTTLOADTAG(BuildTime,    __TIME__);
BUTTLOADTAG(BuildDate,    __DATE__);
BUTTLOADTAG(MyUSBVersion, "MyUSB V" MYUSB_VERSION_STRING);

/* Scheduler Task List */
TASK_LIST
{
	{ Task: USB_USBTask          , TaskStatus: TASK_STOP },
	{ Task: CDC_Task             , TaskStatus: TASK_STOP },
};

/* Globals: */
CDC_Line_Coding_t LineCoding = { BaudRateBPS: 9600,
                                 CharFormat:  OneStopBit,
                                 ParityType:  Parity_None,
                                 DataBits:    8            };

RingBuff_t        Rx_Buffer;
RingBuff_t        Tx_Buffer;

volatile bool     Transmitting = false;

int main(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable Clock Division */
	SetSystemClockPrescaler(0);

	/* Hardware Initialization */
	LEDs_Init();
	ReconfigureUSART();
	
	/* Ringbuffer Initialization */
	Buffer_Initialize(&Rx_Buffer);
	Buffer_Initialize(&Tx_Buffer);
	
	/* Indicate USB not ready */
	LEDs_SetAllLEDs(LEDS_LED1 | LEDS_LED3);
	
	/* Initialize Scheduler so that it can be used */
	Scheduler_Init();

	/* Initialize USB Subsystem */
	USB_Init();

	/* Scheduling - routine never returns, so put this last in the main function */
	Scheduler_Start();
}

EVENT_HANDLER(USB_Connect)
{
	/* Start USB management task */
	Scheduler_SetTaskMode(USB_USBTask, TASK_RUN);

	/* Indicate USB enumerating */
	LEDs_SetAllLEDs(LEDS_LED1 | LEDS_LED4);
}

EVENT_HANDLER(USB_Disconnect)
{
	/* Stop running CDC and USB management tasks */
	Scheduler_SetTaskMode(CDC_Task, TASK_STOP);
	Scheduler_SetTaskMode(USB_USBTask, TASK_STOP);

	/* Indicate USB not ready */
	LEDs_SetAllLEDs(LEDS_LED1 | LEDS_LED3);
}

EVENT_HANDLER(USB_ConfigurationChanged)
{
	/* Setup CDC Notification, Rx and Tx Endpoints */
	Endpoint_ConfigureEndpoint(CDC_NOTIFICATION_EPNUM, EP_TYPE_INTERRUPT,
		                       ENDPOINT_DIR_IN, CDC_NOTIFICATION_EPSIZE,
	                           ENDPOINT_BANK_SINGLE);

	Endpoint_ConfigureEndpoint(CDC_TX_EPNUM, EP_TYPE_BULK,
		                       ENDPOINT_DIR_IN, CDC_TXRX_EPSIZE,
	                           ENDPOINT_BANK_DOUBLE);

	Endpoint_ConfigureEndpoint(CDC_RX_EPNUM, EP_TYPE_BULK,
		                       ENDPOINT_DIR_OUT, CDC_TXRX_EPSIZE,
	                           ENDPOINT_BANK_DOUBLE);

	/* Indicate USB connected and ready */
	LEDs_SetAllLEDs(LEDS_LED2 | LEDS_LED4);

	/* Start CDC task */
	Scheduler_SetTaskMode(CDC_Task, TASK_RUN);
}

EVENT_HANDLER(USB_UnhandledControlPacket)
{
	uint8_t* LineCodingData = (uint8_t*)&LineCoding;

	/* Process CDC specific control requests */
	switch (bRequest)
	{
		case GET_LINE_CODING:
			if (bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{	
				/* Acknowedge the SETUP packet, ready for data transfer */
				Endpoint_ClearSetupReceived();

				/* Write the line coding data to the control endpoint */
				Endpoint_Write_Control_Stream_LE(LineCodingData, sizeof(LineCoding));
				
				/* Send the line coding data to the host and clear the control endpoint */
				Endpoint_ClearSetupOUT();
			}
			
			break;
		case SET_LINE_CODING:
			if (bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				/* Acknowedge the SETUP packet, ready for data transfer */
				Endpoint_ClearSetupReceived();

				/* Read the line coding data in from the host into the global struct */
				Endpoint_Read_Control_Stream_LE(LineCodingData, sizeof(LineCoding));

				/* Send the line coding data to the host and clear the control endpoint */
				Endpoint_ClearSetupIN();
				
				/* Reconfigure the USART with the new settings */
				ReconfigureUSART();
			}
	
			break;
		case SET_CONTROL_LINE_STATE:
			if (bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				/* Acknowedge the SETUP packet, ready for data transfer */
				Endpoint_ClearSetupReceived();
				
				/* Send an empty packet to acknowedge the command (currently unused) */
				Endpoint_ClearSetupIN();
			}
	
			break;
	}
}

TASK(CDC_Task)
{
	if (USB_IsConnected)
	{
		/* Select the Serial Rx Endpoint */
		Endpoint_SelectEndpoint(CDC_RX_EPNUM);
		
		if (Endpoint_ReadWriteAllowed())
		{
			/* Read the recieved data endpoint into the transmission buffer */
			while (Endpoint_BytesInEndpoint())
			{
				/* Wait until the buffer has space for a new character */
				while (!((BUFF_STATICSIZE - Rx_Buffer.Elements)));
			
				/* Store each character from the endpoint */
				Buffer_StoreElement(&Rx_Buffer, Endpoint_Read_Byte());
			}
			
			/* Clear the endpoint buffer */
			Endpoint_ClearCurrentBank();
		}
		
		/* Check if Rx buffer contains data */
		if (Rx_Buffer.Elements)
		{
			/* Initiate the transmission of the buffer contents if USART idle */
			if (!(Transmitting))
			{
				Transmitting = true;
				Serial_TxByte(Buffer_GetElement(&Rx_Buffer));
			}
		}

		/* Select the Serial Tx Endpoint */
		Endpoint_SelectEndpoint(CDC_TX_EPNUM);

		/* Check if the Tx buffer contains anything to be sent to the host */
		if (Tx_Buffer.Elements)
		{
			/* Wait until Serial Tx Endpoint Ready for Read/Write */
			while (!(Endpoint_ReadWriteAllowed()));
			
			/* Check before sending the data if the endpoint is completely full */
			bool IsFull = (Endpoint_BytesInEndpoint() == CDC_TXRX_EPSIZE);
			
			/* Write the transmission buffer contents to the recieved data endpoint */
			while (Tx_Buffer.Elements && (Endpoint_BytesInEndpoint() < CDC_TXRX_EPSIZE))
			  Endpoint_Write_Byte(Buffer_GetElement(&Tx_Buffer));
			
			/* Send the data */
			Endpoint_ClearCurrentBank();

			/* If a full endpoint was sent, we need to send an empty packet afterwards to terminate the transfer */
			if (IsFull)
			{
				/* Wait until Serial Tx Endpoint Ready for Read/Write */
				while (!(Endpoint_ReadWriteAllowed()));

				/* Send an empty packet to terminate the transfer */
				Endpoint_ClearCurrentBank();
			}
		}
	}
}

ISR(USART1_TX_vect)
{
	/* Send next character if avaliable */
	if (Rx_Buffer.Elements)
	  UDR1 = Buffer_GetElement(&Rx_Buffer);
	else
	  Transmitting = false;
}

ISR(USART1_RX_vect)
{
	/* Character recieved, store it into the buffer */
	Buffer_StoreElement(&Tx_Buffer, UDR1);
}

void ReconfigureUSART(void)
{
	uint8_t ConfigMask = 0;

	/* Determine parity - non odd/even parity mode defaults to no parity */
	if (LineCoding.ParityType == Parity_Odd)
	  ConfigMask = ((1 << UPM11) | (1 << UPM10));
	else if (LineCoding.ParityType == Parity_Even)
	  ConfigMask = (1 << UPM11);

	/* Determine stop bits - 1.5 stop bits is set as 1 stop bit due to hardware limitations */
	if (LineCoding.CharFormat == TwoStopBits)
	  ConfigMask |= (1 << USBS1);

	/* Determine data size - 5, 6, 7, or 8 bits are supported */
	if (LineCoding.DataBits == 6)
	  ConfigMask |= (1 << UCSZ10);
	else if (LineCoding.DataBits == 7)
	  ConfigMask |= (1 << UCSZ11);
	else if (LineCoding.DataBits == 8)
	  ConfigMask |= ((1 << UCSZ11) | (1 << UCSZ10));
	
	/* Enable double speed, gives better error percentages at 8MHz */
	UCSR1A = (1 << U2X1);
	
	/* Enable transmit and receive modules and interrupts */
	UCSR1B = ((1 << TXCIE1) | (1 << RXCIE1) | (1 << TXEN1) | (1 << RXEN1));

	/* Set the USART mode to the mask generated by the Line Coding options */
	UCSR1C = ConfigMask;
	
	/* Set the USART baud rate register to the desired baud rate value */
	UBRR1  = SERIAL_2X_UBBRVAL((uint16_t)LineCoding.BaudRateBPS);
}
