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

/* usb uses a software fifo followed by dual 64byte
   hardware fifos in the at90usb */

#include "ringbuffer.h"
RB_DECLARE(usb_out_rb, char, volatile uint8_t, 128);

void usb_rb_poll()
{
    Endpoint_SelectEndpoint(CDC_TX_EPNUM);
    uint8_t count = 0;
    while(!RB_EMPTY(usb_out_rb) && count++ < CDC_TXRX_EPSIZE) {
        char c;
        RB_GET(usb_out_rb, c);
        Endpoint_Write_Byte(c);
    }
    Endpoint_ClearCurrentBank();
}

static void usbrb_write (char c)
{
    extern uint8_t haveendpoints;
    if (!USB_IsConnected || !haveendpoints)
        return;

    cli();
    if(RB_FULL(usb_out_rb))
        RB_FREE(usb_out_rb);
    RB_PUT(usb_out_rb, c);
    sei();
}
