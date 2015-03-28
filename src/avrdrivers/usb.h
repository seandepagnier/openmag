/* Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
 *
 * This Program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * * For more information on the GPL, please go to:
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <MyUSB/Version.h>                        // Library Version Information
#include <MyUSB/Common/ButtLoadTag.h>             // PROGMEM tags readable by the ButtLoad project
#include <MyUSB/Drivers/USB/USB.h>                // USB Functionality
#include <MyUSB/Drivers/USB/LowLevel/LowLevel.h>
#include <MyUSB/Scheduler/Scheduler.h>            // Simple scheduler for task management

/* Macros: */
#define REQ_GetReport   0x01
#define GET_LINE_CODING              0x21
#define SET_LINE_CODING              0x20
#define SET_CONTROL_LINE_STATE       0x22

/* Event Handlers: */
HANDLES_EVENT(USB_ConfigurationChanged);
HANDLES_EVENT(USB_UnhandledControlPacket);
		
/* Type Defines: */
typedef struct
{
   uint32_t BaudRateBPS;
   uint8_t  CharFormat;
   uint8_t  ParityType;
   uint8_t  DataBits;
} CDC_Line_Coding_t;
		
/* Enums: */
enum CDC_Line_Coding_Format_t
   {
      OneStopBit          = 0,
      OneAndAHalfStopBits = 1,
      TwoStopBits         = 2,
   };
		
enum CDC_Line_Codeing_Parity_t
   {
      Parity_None         = 0,
      Parity_Odd          = 1,
      Parity_Even         = 2,
      Parity_Mark         = 3,
      Parity_Space        = 4,
   };

/* Globals: */
CDC_Line_Coding_t LineCoding = { BaudRateBPS: 9600,
                                 CharFormat:  OneStopBit,
                                 ParityType:  Parity_None,
                                 DataBits:    8            };

uint8_t haveendpoints;

EVENT_HANDLER(USB_ConfigurationChanged)
{
#ifdef USE_CDC
    /* Setup CDC Notification, Rx and Tx Endpoints */
    Endpoint_ConfigureEndpoint(CDC_NOTIFICATION_EPNUM, EP_TYPE_INTERRUPT,
                               ENDPOINT_DIR_IN, CDC_NOTIFICATION_EPSIZE,
                               ENDPOINT_BANK_SINGLE);

    Endpoint_ConfigureEndpoint(CDC_TX_EPNUM, EP_TYPE_BULK,
                               ENDPOINT_DIR_IN, CDC_TXRX_EPSIZE,
                               ENDPOINT_BANK_DOUBLE);

    USB_INT_Enable(ENDPOINT_INT_IN);

    Endpoint_ConfigureEndpoint(CDC_RX_EPNUM, EP_TYPE_BULK,
                               ENDPOINT_DIR_OUT, CDC_TXRX_EPSIZE,
                               ENDPOINT_BANK_DOUBLE);
#endif
#ifdef USE_MOUSE
    Endpoint_ConfigureEndpoint(MOUSE_EPNUM, EP_TYPE_INTERRUPT,
                               ENDPOINT_DIR_IN, MOUSE_EPSIZE,
                               ENDPOINT_BANK_DOUBLE);
#endif
#ifdef USE_JOYSTICK
    Endpoint_ConfigureEndpoint(JOYSTICK_EPNUM, EP_TYPE_INTERRUPT,
                               ENDPOINT_DIR_IN, JOYSTICK_EPSIZE,
                               ENDPOINT_BANK_SINGLE);
#endif
    haveendpoints = 1;
}

EVENT_HANDLER(USB_UnhandledControlPacket)
{
   uint8_t* LineCodingData = (uint8_t*)&LineCoding;

   Endpoint_Ignore_Word();

   /* Process CDC specific control requests */
   switch (bRequest)
   {
   case GET_LINE_CODING:
      if (bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
      {
         Endpoint_ClearSetupReceived();
         
         for (uint8_t i = 0; i < sizeof(LineCoding); i++)
            Endpoint_Write_Byte(*(LineCodingData++));	
         
         Endpoint_ClearSetupIN();
         while (!(Endpoint_IsSetupINReady()));
         
         while (!(Endpoint_IsSetupOUTReceived()));
         Endpoint_ClearSetupOUT();
      }
      break;
   case SET_LINE_CODING:
      if (bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
      {
         Endpoint_ClearSetupReceived();
         
         while (!(Endpoint_IsSetupOUTReceived()));
         
         for (uint8_t i = 0; i < sizeof(LineCoding); i++)
            *(LineCodingData++) = Endpoint_Read_Byte();
         
         Endpoint_ClearSetupOUT();
         
         Endpoint_ClearSetupIN();
         while (!(Endpoint_IsSetupINReady()));
      } break;
   case SET_CONTROL_LINE_STATE:
      if (bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
      {
         Endpoint_ClearSetupReceived();
         
         Endpoint_ClearSetupIN();
         while (!(Endpoint_IsSetupINReady()));
      } break;
#ifdef USE_JOYSTICK 
   case REQ_GetReport:
      if (bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
      {
         /* Ignore report type and ID number value */
         Endpoint_Discard_Word();
         
         uint8_t wIndex = Endpoint_Read_Byte();
         
         /* Read in the number of bytes in the report to send to the host */
         uint16_t wLength = Endpoint_Read_Word_LE();
         
         /* If trying to send more bytes than exist to the host, clamp the value at the report size */
         if (wLength > 8)
            wLength = 8;
         
         Endpoint_ClearSetupReceived();
         
//         if(wIndex == 3) { /* only report for joystick */
            /* Write the report data to the control endpoint */
         uint8_t JoystickReportData[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
            Endpoint_Write_Control_Stream_LE(&JoystickReportData, wLength);
         
            /* Finalize the transfer, acknowedge the host error or success OUT transfer */
            Endpoint_ClearSetupOUT();
//         }

   DEBUG("wrote conf\n");
      } break;
#endif
   }
}

EVENT_HANDLER(USB_Disconnect)
{
    /* flush any incomming data */
    extern void usb_rxline_flush(void);
    usb_rxline_flush();

    haveendpoints = 0;
}

extern void usb_rxline_poll(void);
void usbhw_poll(void)
{
   /* disable pipe interrupts because the handler changes the endpoint
      and would confuse these routines */
   Endpoint_SelectEndpoint(CDC_TX_EPNUM);
   USB_INT_Disable(ENDPOINT_INT_IN);

   USB_USBTask();

   if (USB_IsConnected && haveendpoints) {
       usb_rxline_poll();
#ifdef USE_MOUSE
       mouse_transmit();
#endif
#ifdef USE_JOYSTICK
       joystick_transmit();
#endif
   }

    /* re-enable pipe interrupt */
   Endpoint_SelectEndpoint(CDC_TX_EPNUM);
   USB_INT_Enable(ENDPOINT_INT_IN);
}

extern void usb_rb_poll(void);
ISR(ENDPOINT_PIPE_vect)
{
    if (Endpoint_HasEndpointInterrupted(CDC_TX_EPNUM)) {
        USB_INT_Clear(ENDPOINT_INT_IN);
        Endpoint_ClearEndpointInterrupt(CDC_TX_EPNUM);
	usb_rb_poll();
    }
}
