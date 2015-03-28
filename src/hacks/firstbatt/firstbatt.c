#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL	      // Sets up the default speed for delay.h
#include <util/delay.h>

#include <avr/wdt.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

/* read battery voltage using adc with internal compare */


#include "../include/uart.h"
#include "../include/adc.h"

int main(void) {
   uart_init(12);

   adc_init();
 
   double nl = 0;
   for(;;) {
      int ref;
      double voltage;
      int i;
      for(i = 0; i<1000; i++) {
         ref = getadcref();
         voltage = 1/(double)ref*1023.0;
         nl = nl*.999 + voltage*.001;
      }
      printf("val: %d  voltage: %f nl: %f\n", ref, voltage, nl);
      //      _delay_ms(1000);
   }

   return 0;
}
