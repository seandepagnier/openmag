/* Copyright (C) 2007 Sean D'Epagnier <sean@depagnier.com>
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

char usbtiny_buf_in[USBTINY_RXSIZE];
volatile uint8_t usbtiny_in_len;
volatile uint8_t usbtiny_newline;

void usb_out ( byte_t* data, byte_t len )
{
    byte_t i;
    for(i = 0; i<len; i++) {
        if(usbtiny_newline) {
           //           DEBUG("usb buffer not ready for new data\n");
           break;
        }

        if(data[i] == 64 || data[i] == 192) { /* it is actually a setup packet */
           usb_setup(data);
           return;
        }

        if(data[i] == '\n' || data[i] == '\r') {
            usbtiny_newline = 1;
            usbtiny_buf_in[usbtiny_in_len++] = '\0';
        } else
           usbtiny_buf_in[usbtiny_in_len++] = data[i];
    }
}

static char *usbtiny_getline(void)
{
    if(usbtiny_newline)
       return usbtiny_buf_in;
    return NULL;
}

static void usbtiny_ackline(void)
{
    usbtiny_in_len = 0;
    usbtiny_newline = 0;
}
