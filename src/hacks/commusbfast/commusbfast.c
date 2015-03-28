#include <stdio.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#define F_CPU 12000000UL
#include <util/delay.h>

#include <uart.h>
#include <usb.h>

char buf_out[64];
volatile uint8_t buf_out_head, buf_out_tail;

ISR(USART0_RX_vect)
{
}

static inline void send_byte(void)
{
   UDR0 = buf_out[buf_out_tail++];
   buf_out_tail &= 63;
}

ISR(USART0_TX_vect)
{
   if(buf_out_tail != buf_out_head)
      send_byte();
}

byte_t usb_setup ( byte_t data[8] )
{
   /* set baud rate */
   if(data[1] == 1) {
      UBRR0H = data[3];
      UBRR0L = data[2];
      return 0;
   }
      
   return 0xff;
}

byte_t usb_in ( byte_t* data, byte_t len )
{
   return 0;
}

void usb_out ( byte_t* data, byte_t len )
{
   uint8_t i;

   uint8_t in = buf_out_head == buf_out_tail;
   
   for(i = 0; i<len; i++) {
      buf_out[buf_out_head++] = data[i];
      if(buf_out_head == sizeof buf_out)
         buf_out_head = 0;

      if(buf_out_head == buf_out_tail) {
         while (!(UCSR0A & (1 << UDRE0)));
         send_byte();
      }
   }

   if(in && len)
      send_byte();
}

int main(void) {
   uart_init(12);

   UCSR0B |= (1 << RXCIE0) | (1 << TXCIE0);

   usb_init();
   
   //   printf("init\n");

   buf_out_head = buf_out_tail = 0;

   for(;;)
      usb_poll();
}
