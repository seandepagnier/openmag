#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL	      // Sets up the default speed for delay.h
#include <util/delay.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <string.h>

#include <debug.h>

#define DDR_SPI DDRB
#define DD_MOSI DDB5
#define DD_MISO DDB6
#define DD_SCK  DDB7
#define DD_SS   DDB4

#include <spi.h>

#define BOOTLOADER_OFFSET 0xf800
#include <uart.h>

#include <avr/eeprom.h>

#include <data/data.h>

#define HMC_SRDDR  DDRB
#define HMC_SRPORT PORTB

#define HMC_SSDDR  DDRB
#define HMC_SSPORT PORTB
#ifdef FIRSTBOARD
# define HMC_SR     DDB1
# define HMC_SS     DDB3
#else
# define HMC_SR     DDB3
# define HMC_SS     DDB1
#endif
#include <hmc105x.h>

#define SCA_SSDDR DDRB
#define SCA_SSPORT PORTB
#define SCA_SS DDB0

#define SCA_INTDDR DDRB
#define SCA_INTPORT PORTB
#define SCA_INTBV DDB2
#define SCA_BEFORE_SS   PORTB |= _BV(DDB1);  /* turn off ad7799 */
#define SCA_AFTER_SS    PORTB &= ~_BV(DDB1); /* turn on ad7799 */
//#define SCA_INT INT2
#define SCA_INT_vect INT2_vect
#include <sca3000.h>

int main(void) {

   uart_init(12);
   sei();

   printf("init\n");

   spi_init();

   sca3000_init(SCA3000_MODE_BUFFERED);

   for(;;) {
#if 0 /* for non-interrupt mode (raw) */
       printf("%d %d %d %d\n",
              sca3000_readaxis(0),
              sca3000_readaxis(1),
              sca3000_readaxis(2),
              sca3000_readtemp());

#else
       int32_t data[3];
       uint16_t count[3];
      if(sca3000_filter_data(data, count))
         printf("point: %ld %ld %ld\n", data[0], data[1], data[2]);

      //     sca3000_temp_farenheight(sca_tempf));
#endif
   }

   return 0;
}
