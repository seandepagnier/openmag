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

char usb_buf_in[64];
volatile uint8_t usb_in_len;
volatile uint8_t usb_newline;

void usb_rxline_flush(void)
{
    usb_in_len = 0;
    usb_newline = 0;
}

void usb_rxline_poll(void)
{
   Endpoint_SelectEndpoint(CDC_RX_EPNUM);
   if(!Endpoint_ReadWriteAllowed())
      return;

   while (Endpoint_BytesInEndpoint()) {
      char c = Endpoint_Read_Byte();
         
      if(c == '\033') {
         gotobootloader=1;
         continue;
      }
         
      /* ignore this character */
      if(c == '\n')
         continue;
      
      if(usb_newline) {
         DEBUG("usb buffer not ready for new data\n");
         break;
      }
      
      if(c == '\r') {
         usb_newline = 1;
         c = '\0';
      }
      
      usb_buf_in[usb_in_len++] = c;
      
   }

   Endpoint_ClearCurrentBank();
}

static char *usb_getline(void)
{
    if(usb_newline)
       return usb_buf_in;
    return NULL;
}

static void usb_ackline(void)
{
    cli();
    usb_in_len = 0;
    usb_newline = 0;
    sei();
}
