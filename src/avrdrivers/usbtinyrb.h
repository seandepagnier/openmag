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

#include "usbaspdefs.h"

#include <ringbuffer.h>
RB_DECLARE(usbtiny_out_rb, char, volatile uint8_t, USBTINY_TXSIZE);

/* dump as much as we can */
byte_t usb_in (byte_t* data, byte_t len )
{
   byte_t i;
   for(i = 0; i<len; i++) {
      if(RB_EMPTY(usbtiny_out_rb))
         break;
      RB_GET(usbtiny_out_rb, data[i]);
   }

   return i;
}

static void usbtiny_write (char c)
{
    if(!usbtiny_polled)
       /* if full, we aren't being polled, drop on the floor */
       return;

    int d = 0;
    TIMSK0 &= ~_BV(TOIE1);
    while(RB_FULL(usbtiny_out_rb)) {
       if(d++ == 64) { /* WARNING: will block 64ms, watch out watch dog! */
          usbtiny_polled = 0;
          RB_SET_EMPTY(usbtiny_out_rb);
          TIMSK0 |= _BV(TOIE1);
          return;
       }
       TIMSK0 |= _BV(TOIE1);
       _delay_ms(1);
       TIMSK0 &= ~_BV(TOIE1);
    }
    RB_PUT(usbtiny_out_rb, c);
    TIMSK0 |= _BV(TOIE1);
}
