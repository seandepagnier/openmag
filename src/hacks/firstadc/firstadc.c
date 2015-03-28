#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 100000UL	      // Sets up the default speed for delay.h
#include <util/delay.h>

#include <avr/wdt.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include "avg.h"

#include <uart.h>

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

static volatile uint16_t val;

ISR(ADC_vect)
{
   val = ADCW;
}

#define SAMPLES 6

int main(void) {
   //   DIDR = 0; /* disable digital input buffer */
   
   DDRD = _BV(PD6);
   uart_init(12);
   printf("init\n");
   sei();

   adc_init();
   lcd_init();
   lcd_setCursor(0, 0);

   _delay_ms(1000);

   int sensor = 0;
 
   for(;;) {
#if 0
      static int rb[16];
      const int rbs = (sizeof rb) / (sizeof *rb);
      int i;
#ifdef RUNNING_UPDATE
      static int rbp;
      rb[rbp] = getadc(0);
      if(++rbp == rbs)
         rbp = 0;
#else
      for(i = 0; i<rbs; i++) {
         //         printf("d:%d\n", getadc(0));
         rb[i] = getadc(sensor);
      }
#endif

      sortints(rb, rbs);

      //      printf("%d, vals: ", sensor);

      //      for(i = 0; i<rbs; i++)
         //         printf("%d ", rb[i]);
      //      printf("\n");

      //      printf("avg: ");
      int min, max;
      //      for(min = 0, max = rbs - 1; min <= max; min++, max--)
         //         printf("%d ", getavg(rb, min, max, rbs));
      //      printf("\n");

      //      printf("deviation: ");

      int dev = 0;
      for(min = 0, max = rbs - 1; min <= max; min++, max--) {
         int d = rb[max] - rb[min];
         if(min == 0)
            dev = d;
         //         printf("%d ", d);
      }
      //      printf("\n");

      static int totaldev[3], devcount[3];
      totaldev[sensor] += dev;
      devcount[sensor]++;
#if 0
      printf("dev: %d avgdev: %f, count %d\n", dev,
             ((double)totaldev[sensor] / devcount[sensor] + .5), devcount[sensor]);

      printf("\n");
      printf("\n");
#else
      char buf[16];
      int len = snprintf(buf, sizeof buf, "%d %d %d", devcount[0], devcount[1], devcount[2]);
      buf[len] = ' ';
      lcd_setLine(0, 0, buf, 16);

      len = snprintf(buf, sizeof buf, "%.2f %.2f %.2f",
                     ((double)totaldev[0] / devcount[0] + .5),
                     ((double)totaldev[1] / devcount[1] + .5),
                     ((double)totaldev[2] / devcount[2] + .5));
      buf[len] = ' ';
      lcd_setLine(1, 0, buf, 16);
#endif

#else
      int32_t v[3] = {0};
      int i, j;
      const int count = 128;
      for(i = 0; i<count; i++)
         for(j = 0; j<3; j++)
            v[j] += getadc(j);

      for(j = 0; j<3; j++)
         v[j] /= count;

      printf("%ld %ld %ld\n", v[0], v[1], v[2]);
#endif

      if(++sensor == 3)
         sensor = 0;
   }
   return 0;
}
