#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 100000UL	      // Sets up the default speed for delay.h
#include <util/delay.h>

#include <avr/wdt.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include "../include/uart.h"

void (*bootloader)(void) = 0xf800;

int main(void) {
   uart_init(12);
   printf("echo on\n");
   int i;
   for(i=0; i<5; i++)
      putchar(getchar());
   printf("ready to boot\n");
   bootloader();

   return 0;
}
