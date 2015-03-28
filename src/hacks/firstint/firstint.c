#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL	      // Sets up the default speed for delay.h
#include <util/delay.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <string.h>

#include <uart.h>

ISR(INT0_vect)
{
   static int c;
   printf("int0: %d\n", c++);
}

int main(void) {

   cli();
   uart_init(12);

   EIMSK = _BV(INT0);
   EICRA = _BV(ISC01); /* falling edge */
   
   sei();

   printf("init\n");

   for(;;);
   
   return 1;
}
