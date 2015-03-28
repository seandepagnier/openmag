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
 *
 * For more information on the GPL, please go to:
 * http://www.gnu.org/copyleft/gpl.html
 */

   CCI_Interface:
   {
      Header:                 {Size: sizeof(USB_Descriptor_Interface_t), Type: DTYPE_Interface},

      InterfaceNumber:        0,
      AlternateSetting:       0,
			
      TotalEndpoints:         1,
				
      Class:                  0x02,
      SubClass:               0x02,
      Protocol:               0x01,
				
      InterfaceStrIndex:      NO_DESCRIPTOR_STRING
   },

   CDC_Functional_IntHeader:
   {
      FuncHeader:             {Header:
			       {Size: sizeof(CDC_FUNCTIONAL_DESCRIPTOR(2)),
				Type: 0x24},
			       SubType: 0x00},
			
      Data:                   {0x10, 0x01}
   },

   CDC_Functional_CallManagement:
   {
      FuncHeader:             {Header:
			       {Size: sizeof(CDC_FUNCTIONAL_DESCRIPTOR(2)),
				Type: 0x24},
			       SubType: 0x01},
			
      Data:                   {0x03, 0x01}
   },

   CDC_Functional_AbstractControlManagement:
   {
      FuncHeader:             {Header:
			       {Size: sizeof(CDC_FUNCTIONAL_DESCRIPTOR(1)),
				Type: 0x24},
			       SubType: 0x02},
			
      Data:                   {0x06}
   },
		
   CDC_Functional_Union:
   {
      FuncHeader:             {Header:
			       {Size: sizeof(CDC_FUNCTIONAL_DESCRIPTOR(2)),
				Type: 0x24},
			       SubType: 0x06},
			
      Data:                   {0x00, 0x01}
   },	

   ManagementEndpoint:
   {
      Header:                 {Size: sizeof(USB_Descriptor_Endpoint_t), Type: DTYPE_Endpoint},
										 
      EndpointAddress:        (ENDPOINT_DESCRIPTOR_DIR_IN | CDC_NOTIFICATION_EPNUM),
      Attributes:       		EP_TYPE_INTERRUPT,
      EndpointSize:           CDC_NOTIFICATION_EPSIZE,
      PollingIntervalMS:		0x02
   },

   DCI_Interface:
   {
      Header:                 {Size: sizeof(USB_Descriptor_Interface_t), Type: DTYPE_Interface},

      InterfaceNumber:        1,
      AlternateSetting:       0,
			
      TotalEndpoints:         2,
				
      Class:                  0x0A,
      SubClass:               0,
      Protocol:               0,
				
      InterfaceStrIndex:      NO_DESCRIPTOR_STRING
   },

   DataOutEndpoint:
   {
      Header:                 {Size: sizeof(USB_Descriptor_Endpoint_t), Type: DTYPE_Endpoint},
										 
      EndpointAddress:        (ENDPOINT_DESCRIPTOR_DIR_OUT | CDC_RX_EPNUM),
      Attributes:       		EP_TYPE_BULK,
      EndpointSize:           CDC_TXRX_EPSIZE,
      PollingIntervalMS:		0x00
   },
		
   DataInEndpoint:
   {
      Header:                 {Size: sizeof(USB_Descriptor_Endpoint_t), Type: DTYPE_Endpoint},
										 
      EndpointAddress:        (ENDPOINT_DESCRIPTOR_DIR_IN | CDC_TX_EPNUM),
      Attributes:       		EP_TYPE_BULK,
      EndpointSize:           CDC_TXRX_EPSIZE,
      PollingIntervalMS:		0x00
   },
