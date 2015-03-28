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

/* uart driver made to transmit bytes using an optional software
   fifo in addition to a hardware fifo.

   For receiving, whole lines are taken and must be read by the
   application before new data arrives
*/

#include <LPC214x.h>
#include <irq.h>
#include <uart.h>

#include <debug.h>

#include "fcpu.h"

#define USE_UART_ISR

#include "ringbuffer.h"

/* optional additional software fifo for transmission */
#define USE_UART_RB

#ifdef USE_UART_RB
#define UART_TXSIZE 64
RB_DECLARE(uart_out_rb, char, volatile int, UART_TXSIZE);
#endif

#ifdef USE_UART_ISR
#define UART_RXSIZE 256
RB_DECLARE(uart_in_rb, char, volatile int, UART_RXSIZE);

static void uart0_store(char c)
{
   /* Receive Data Available */
   if(RB_FULL(uart_in_rb)) {
      DEBUG("uart buffer full!\n");
      return;
   }

   RB_PUT(uart_in_rb, c);
}


static void USBIntHandler(void) __attribute__ ((interrupt("IRQ")));

static void uart0_handler (void)
{
   char IIRValue;

   //   IENABLE;				/* handles nested interrupt */	
   IIRValue = U0IIR;
   
   IIRValue >>= 1;			/* skip pending bit in IIR */
   IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
   switch(IIRValue) {
      case IIR_RLS:       /* line status */
         /* Receive Line Status */
         if ( U0LSR & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
         {
            /* There are errors or break interrupt */
            char Dummy = U0RBR; /* clear interrupt */
         }
         break;
      case IIR_RDA:  /* Receive Data Available */
      case IIR_CTI:  /* Character Timeout Interrupt */
         while (U0LSR & LSR_RDR )            
            uart0_store(U0RBR);
         break;
#ifdef USE_UART_RB
      case IIR_THRE: /* transmit empty */
      {
         if(U0LSR & LSR_THRE && !RB_EMPTY(uart_out_rb))
            RB_GET(uart_out_rb, U0THR);
      } break;
#endif
   }
    
   //   IDISABLE;
   VICVectAddr = 0;		/* Acknowledge Interrupt */
}
#endif

void uart0_init( long baudrate )
{
    long Fdiv;

    PINSEL0 |= 0x00000005;       /* Enable RxD0 and TxD0 */

    U0LCR = 0x83;               /* 8 bits, no Parity, 1 Stop bit    */
    //        Fdiv = Fpclk / (16 * baudrate) ;	/*baud rate */
        Fdiv = 60000000 / (64 * 9600) ;	/*baud rate */
        //Fdiv = 12000000 / (64 * 9600) ;	/*baud rate */

    U0DLM = Fdiv >> 8;							
    U0DLL = Fdiv & 0xFF;	
    U0LCR = 0x03;               /* DLAB = 0                         */

    U0FCR = 0x87;		/* Enable and reset TX and RX FIFO. */
    //    U0FCR = 0x06;		/* Enable and reset TX and RX FIFO. */

#ifdef USE_UART_ISR
    install_irq(UART0_INT, (void *)uart0_handler);
   
    /* enable interrupt */
    U0IER = IER_RBR | IER_RLS
#ifdef USE_UART_RB
       | IER_THRE
#endif
       ;
#endif
}

int uart0_read(void)
{
#if defined(USE_UART_ISR)
   if(RB_EMPTY(uart_in_rb))
      return EOF;
   char c;
   RB_GET(uart_in_rb, c);
   return c;
#else
   if (!(U0LSR & 0x01))
      return EOF;

  return (U0RBR);
#endif
}

void uart0_write(char c)
{
#if defined(USE_UART_ISR) && defined(USE_UART_RB)
   while(RB_FULL(uart_out_rb));
   RB_PUT(uart_out_rb, c);

   if(U0LSR & 0x20)
      RB_GET(uart_out_rb, U0THR);
#else
   while(!(U0LSR & 0x20));
   U0THR = c;
#endif
}
