#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL	      // Sets up the default speed for delay.h
#include <util/delay.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include "../include/uart.h"

/* the pins are set to:
   SCLK  (1) to SCLK (19)
   MISO  (2) to MISO (18)
   MOSI  (3) to MOSI (17)
   SS    (4) to SS   (16)
   DRDY  (5) to PB0  (15)
   RESET (6) to PB1  (14)
*/

#define DDR_SPI DDRB
#define DD_MISO DDB4
#define DD_MOSI DDB3
#define DD_SCK  DDB5
#define DD_SS   DDB2
#define DD_RESET DDB0

#include "../include/spi.h"

#define MAGDRDYPORT PINB
#define MAGDRDYDDR DDRB
#define MAGDRDY (_BV(PB1))
#define MAGDRDYINT (_BV(PCINT1))

#define MAGRESETPORT PORTB
#define MAGRESETDDR DDRB
#define MAGRESET (_BV(PB0))
#include "../include/mag.h"

int main(void) {
   uart_init(12);
   printf("init\n");
   sei();

   for(;;);

   spi_init();

   mag_init();
   _delay_ms(100);

   for(;;) {
#if 0
   static int lastval[3];
   float d[3];
   for(i = 0; i<3; i++)
      d[i] = (float)val[i] - (float)lastval[i];
   
   float dist = d[0]*d[0] + d[1]*d[1] + d[2]*d[2];
   if(dist > 500000) {
      memcpy(lastval, val, sizeof val);
      /* good point! */
      printf("%ld %ld %ld\n", magdata[0], magdata[1], magdata[2]);
   }
#else
   printf("%ld %ld %ld\n", magdata[0], magdata[1], magdata[2]);
#endif
   
   _delay_ms(100);
   }
   return 0;
}
