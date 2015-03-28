#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL	      // Sets up the default speed for delay.h
#include <util/delay.h>

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

#include <ad7799.h>

int main(void){

   uart_init(12);

   printf("init\n");
   sei();

   spi_init();

   DDRB |= _BV(DDB3);
   PORTB &= ~_BV(DDB3);

   ad7799_init();

   ad7799_write_config(0, 0, AD7799_128_GAIN, 0, 1,  AD7799_AIN1_CHAN); 
   ad7799_set_mode(AD7799_CONTINUOUS_CONVERSION_MODE, 1, AD7799_470_HZ);

   int c = 0;
   for(;;) {
       if(ad7799_data_ready()) {
           ad7799_request_data(0);
           int32_t val = ad7799_read_data();
           if(++c == 100) {
               printf("%ld\n", val);
               c = 0;
           }
           if(c%10 == 0) {
               int chans[] = { AD7799_AIN1_CHAN, AD7799_AIN2_CHAN, AD7799_AIN3_CHAN };
               static int chan;
               ad7799_write_config(0, 0, AD7799_128_GAIN, 0, 1, chans[chan]); 
               if(++chan == 3)
                   chan = 0;
           }
       }
   }
   
   return(0);
}
