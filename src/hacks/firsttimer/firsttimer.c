#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL	      // Sets up the default speed for delay.h
#include <util/delay.h>

#include <avr/wdt.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <avr/uart.h>
#include <avr/timer.h>

static volatile int ticks;

ISR(TIMER0_OVF_vect) {
   static int r;
   if(r++ > 100) {
      ticks++;
      r = 0;
   }
}

int main(void) {
   uart_init(12);

   timer_init();

   for(;;)
      printf("%d\n", ticks);

   return 0;
}
