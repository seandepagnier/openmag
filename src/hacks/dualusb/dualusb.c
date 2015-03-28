#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 12000000UL	      // Sets up the default speed for delay.h
#include <util/delay.h>

#include <avr/wdt.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <uart.h>

#include <usb.h>

#define USBASP_FUNC_CONNECT     1

byte_t usb_setup ( byte_t data[8] )
{
   printf("setup: \n");
   switch(data[1]) {
   case USBASP_FUNC_CONNECT:
      ((void (*)(void))BOOT_SECTION_START)();
      return 0;
   }
   return 0xff;
}

byte_t usb_in ( byte_t* data, byte_t len )
{
   printf("usb in(%d)\n", len);
   return len;
}

void usb_out ( byte_t* data, byte_t len )
{
   printf("usb out(%d)\n", len);
}

int main(void) {
   uart_init(12);
   usb_init();
   
   printf("init\n");

   for(;;)
      usb_poll();

   return 0;
}
