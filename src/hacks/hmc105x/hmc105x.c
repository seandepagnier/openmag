#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL	      // Sets up the default speed for delay.h
#include <util/delay.h>

#include <avr/eeprom.h>

#include <stdio.h>
#include <stdlib.h>

#define BOOTLOADER_OFFSET 0xf800
#include <uart.h>

#define DDR_SPI DDRB
#define DD_MOSI DDB5
#define DD_MISO DDB6
#define DD_SCK  DDB7
#define DD_SS   DDB4
#include <spi.h>

#define HMC_SRDDR  DDRB
#define HMC_SRPORT PORTB
#define HMC_SR     DDB1

#define HMC_SSDDR  DDRB
#define HMC_SSPORT PORTB
#define HMC_SS     DDB3
#define HMC_SAMPLES 1 /* samples before switching channel */
#define HMC_LOWPASS .3
#include <hmc105x.h>

int main(void){

   uart_init(12);

   printf("init\n");
   sei();

   spi_init();
   hmc105x_init();

   int ret;
   int32_t data[3];
   for(;;)
       if((ret = hmc105x_filter_data(data)))
           printf("%ld %ld %ld %d\n", data[0], data[1], data[2], ret);
   
   return(0);
}
