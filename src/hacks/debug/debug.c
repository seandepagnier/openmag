#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 12000000UL	      // Sets up the default speed for delay.h
#include <util/delay.h>

#include <avr/wdt.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <uart.h>

#define USBTINY_INT_NUM 0
#define BOOTLOADER_OFFSET 0xe000
#include <usbloadable.h>

int uart_read (FILE *stream)
{
   while (!(UCSR0A & _BV(RXC0)));
    /* return received byte */
   return UDR0;
}

ISR(USART0_RX_vect)
{
   //   static char cmd[16];
   //   static uint8_t len;

   char in = UDR0;
   putchar(in);
   if(in == '\r')
      putchar('\n');
}

static FILE input_str = FDEV_SETUP_STREAM(NULL, uart_read, _FDEV_SETUP_RW);

ISR(TIMER0_OVF_vect) {
   uint8_t x;
    __asm__ __volatile__
    (
        "mov %0, r24\n\t"
        : "=r" (x)
        );

    //   printf("the value is: %d %d\n", x, TCNT0);
}

/* try to have this timer go off after every instruction */
void starttimer(void) {
   cli();

   //   TCCR0A |= _BV(WGM10) | _BV(WGM11); | _BV(COM1A1) | _BV(COM1A0);
      TCCR0B = _BV(CS01) | _BV(CS00);
      TIMSK0 |= _BV(TOIE0);

   sei();
}

void testasm(void);

int main(void) {
   uart_init(12);
   usbloadable_init();
   sei();
   stdin = &input_str;

   UCSR0B |= (1 << RXCIE0);
   
   printf("init\n");

   starttimer();

   testasm();

   for(;;);

   return 0;
}
