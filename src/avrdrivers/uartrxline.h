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

#define MAKE_UART_RXLINE(NUM) \
   char uart##NUM##_buf_in[UART_RXSIZE]; \
   volatile uint8_t uart##NUM##_buf_len; \
   volatile uint8_t uart##NUM##_newline; \
   \
ISR(USART##NUM##_RX_vect)				\
{ \
   char c = UDR##NUM; \
   if(c == '\033') { \
     gotobootloader = 1; \
     return; \
   } \
 \
    /* ignore this character */ \
    if(c == '\n') \
        return; \
 \
    if(uart##NUM##_newline) \
        return; \
 \
    if(uart##NUM##_buf_len == sizeof uart##NUM##_buf_in) { \
       uart##NUM##_buf_in[uart##NUM##_buf_len-1] = 0; \
       DEBUG("uart"#NUM" buffer overflow!!\n"); \
       uart##NUM##_buf_len = 0; \
       return; \
    } \
 \
    if(c == '\r') { \
        uart##NUM##_newline = 1; \
        c = '\0'; \
    } \
 \
    uart##NUM##_buf_in[uart##NUM##_buf_len++] = c; \
} \
 \
static void uart##NUM##_rxline_init(void) \
{ \
   /* enable receiving interrupt */ \
   UCSR##NUM##B |= (1 << RXCIE##NUM); \
} \
 \
static char *uart##NUM##_getline(void) \
{ \
    if(uart##NUM##_newline) \
        return uart##NUM##_buf_in; \
    return NULL; \
} \
 \
static void uart##NUM##_ackline(void) \
{ \
    cli(); \
    uart##NUM##_buf_len = 0; \
    uart##NUM##_newline = 0; \
    sei(); \
}
