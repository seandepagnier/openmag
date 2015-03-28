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

/* simple uart routines, opens up stdout as a stream, and
   runs bootloader if it gets a byte */

#if defined(__AVR_ATmega2561__)
#define UCSRA UCSR1A
#define UDRE UDRE1
#define UDR UDR1
#define RXC RXC1
#define UBRRH UBRR1H
#define UBRRL UBRR1L
#define UCSRB UCSR1B
#define RXEN RXEN1
#define TXEN TXEN1
#define RXCIE RXCIE1
#define UDRIE UDRIE1
#define USART_RX_vect USART1_RX_vect
#define USART_UDRE_vect USART1_UDRE_vect
#elif defined(__AVR_AT90USB1287__)
#define UCSRA UCSR1A
#define UDRE UDRE1
#define UDR UDR1
#define RXC RXC1
#define UBRRH UBRR1H
#define UBRRL UBRR1L
#define UCSRB UCSR1B
#define RXEN RXEN1
#define TXEN TXEN1
#define RXCIE RXCIE1
#define UDRIE UDRIE1
#define USART_RX_vect USART1_RX_vect
#define USART_UDRE_vect USART1_UDRE_vect
#elif defined(__AVR_ATmega644__)
#define UCSRA UCSR0A
#define UDRE UDRE0
#define UDR UDR0
#define RXC RXC0
#define UBRRH UBRR0H
#define UBRRL UBRR0L
#define UCSRB UCSR0B
#define RXEN RXEN0
#define TXEN TXEN0
#define RXCIE RXCIE0
#define UDRIE UDRIE0
#define USART_RX_vect USART0_RX_vect
#define USART_UDRE_vect USART0_UDRE_vect
#endif

int uart_read (FILE *stream)
{
    while(!(UCSRA & _BV(RXC)));
    return UDR;
}

int uart_write (char c, FILE *stream)
{
#ifdef USE_LINEFEED
   if(c == '\n')
      uart_write('\r', stream);
#endif
   /* Wait for empty transmit buffer */
   while (!(UCSRA & (1 << UDRE)));
   /* Start transmittion */
   UDR = c;
   return 0;
}

FILE uart_str = FDEV_SETUP_STREAM(uart_write, uart_read, _FDEV_SETUP_RW);

#ifdef BOOTLOADER_OFFSET
void uart_poll(void)
{
   if(UCSRA & (1 << RXC))
      ((void (*)(void))BOOTLOADER_OFFSET)();
}
#endif

/* to calculate baudrate, UBBR = F_CPU / (16 * BAUD) - 1
   F_OSC is the clock frequency, BAUD is the baudrate (ie 300)
  see calcbaud.scm */
void uart_init(uint16_t baudrate)
{
   /* Set the baud rate */
   UBRRH = baudrate >> 8;
   UBRRL = baudrate;

   /* Enable UART receiver and transmitter */
   UCSRB = (1 << RXEN) | (1 << TXEN)
#ifdef BOOTLOADER_OFFSET
       | (1 << RXCIE)
#endif
      ;

   stdout = stdin = stderr = &uart_str;
}
