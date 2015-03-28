#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 12000000UL	      // Sets up the default speed for delay.h
#include <util/delay.h>

#include <avr/wdt.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <uart.h>

#define	USBTINY_INT_VECT 
#define USBTINY_INT_NUM 0
#define BOOTLOADER_OFFSET 0xe000
#include <usbloadable.h>

int main(void) {
   uart_init(12);
   printf("init\n");

   usbloadable_init();

   sei();
   
   int ticks = 0, c = 0;
   for(;;) {
      if(ticks % 1000 == 0)
         printf("running %d\n", c++);
      ticks++;
      _delay_ms(10);
   }

   return 0;
}
