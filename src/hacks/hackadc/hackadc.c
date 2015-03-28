#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 100000UL	      // Sets up the default speed for delay.h
#include <util/delay.h>

#include <avr/wdt.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include "avg.h"

#include "../include/uart.h"

/* LCD I/O definitions */

#define LCDDBPORT PORTD
#define LCDDBPIN PIND
#define LCDDBDDR DDRD
#define LCDDBMASK 0xf0
#define LCDDBSHIFT 4            /* how many bits left to shift
                                ** a DB nibble to get it to line up */
#define LCDRPORT PORTB
#define LCDRDDR DDRB
#define LCDR (_BV(PB7))

#define LCDRSPORT PORTB
#define LCDRSDDR DDRB
#define LCDRS (_BV(PB6))

#define LCDEPORT PORTD
#define LCDEDDR DDRD
#define LCDE (_BV(PD3))

#include "../include/lcd.h"
#include "../include/adc.h"


int main(void) {
   
   DDRD = _BV(PD6);
   uart_init(12);


   adc_init();
   lcd_init();
   lcd_setCursor(0, 0);

   lcd_setLine(0, 0, "booting         ", 16);

   _delay_ms(3000);

   int totaldev[3] = {0, 0, 0}, devcount[3] = {0, 0, 0};

   for(;;) {
      int dev[3], sensor;
      for(sensor = 0; sensor<3; sensor++) {
         int rb[16];
         const int rbs = (sizeof rb) / (sizeof *rb);
         int i;
         
         for(i = 0; i<rbs; i++) {
            //         printf("d:%d\n", getadc(0));
            rb[i] = getadc(sensor);
         }
         
         sortints(rb, rbs);

         dev[sensor] = rb[rbs-1] - rb[0];

         totaldev[sensor] += dev[sensor];
         devcount[sensor]++;
      }

      char buf[16];
      int len = snprintf(buf, sizeof buf, "%d %d %d          ", dev[0], dev[1], dev[2]);
      buf[len] = ' ';
      lcd_setLine(0, 0, buf, 16);
         
      len = snprintf(buf, sizeof buf, "%.1f %.1f %.1f         ",
                     ((double)totaldev[0] / devcount[0] + .5),
                     ((double)totaldev[1] / devcount[1] + .5),
                     ((double)totaldev[2] / devcount[2] + .5));
      buf[len] = ' ';
      lcd_setLine(1, 0, buf, 16);
   }
   return 0;
}
