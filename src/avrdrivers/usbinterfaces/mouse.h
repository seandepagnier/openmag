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

   Mouse_Interface:
   {
      Header:                 {Size: sizeof(USB_Descriptor_Interface_t), Type: DTYPE_Interface},

      InterfaceNumber:        2,
      AlternateSetting:       0,
			
      TotalEndpoints:         1,
				
      Class:                  0x03,
      SubClass:               0x00,
      Protocol:               0x02,
				
      InterfaceStrIndex:      NO_DESCRIPTOR_STRING
   },

   MouseHID:
   {
      Header:                 {Size: sizeof(USB_Descriptor_HID_t), Type: DTYPE_HID},
									 
      HIDSpec:          		VERSION_BCD(01.11),
      CountryCode:      		0x00,
      TotalHIDDescriptors:    0x01,
      HIDReportType:    		DTYPE_Report,
      HIDReportLength:        sizeof(MouseReport)
   },

   MouseEndpoint:
   {
      Header:                 {Size: sizeof(USB_Descriptor_Endpoint_t), Type: DTYPE_Endpoint},
      
      EndpointAddress:        (ENDPOINT_DESCRIPTOR_DIR_IN | MOUSE_EPNUM),
      Attributes:       		EP_TYPE_INTERRUPT,
      EndpointSize:           MOUSE_EPSIZE,
      PollingIntervalMS:		0x01
   },
