/* Copyright (C) 2007, 2009 Sean D'Epagnier <sean@depagnier.com>
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

/* use interrupt driven ringbuffer for transmission.
   with this set, it is not possible to send bytes in interrupts,
   so it should be disabled for debugging */
#define USE_UART_RB

#include "ringbuffer.h"

/* write a char to ring buffer which is interrupt emptied waiting if full */
#ifdef USE_UART_RB
#define MAKE_UART_RB(NUM)						\
   RB_DECLARE(uart##NUM##_out_rb, char, volatile uint8_t, UART_TXSIZE);	\
   static int uartrb##NUM##_write(char c, FILE *str)			\
   { \
    while(RB_FULL(uart##NUM##_out_rb)); \
    UCSR##NUM##B &= ~_BV(UDRIE##NUM); /* stop interrupt */ \
    RB_PUT(uart##NUM##_out_rb, c); \
    UCSR##NUM##B |= _BV(UDRIE##NUM); /* start interrupt */ \
    return 0; \
    } \
    ISR(USART##NUM##_UDRE_vect) \
    { \
      if(RB_EMPTY(uart##NUM##_out_rb)) \
        UCSR##NUM##B &= ~_BV(UDRIE##NUM); /* stop interrupt */ \
      else \
        RB_GET(uart##NUM##_out_rb, UDR##NUM); \
    }
#else
#define MAKE_UART_RB(NUM) \
   static int uartrb##NUM##_write(char c, FILE *str) \
   { \
    /* Wait for empty transmit buffer */ \
    while (!(UCSR##NUM##A & (1 << UDRE##NUM))); \
    /* Start transmission */ \
    UDR##NUM = c; \
    return 0; \
   }
#endif
