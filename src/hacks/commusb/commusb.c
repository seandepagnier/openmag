#include <stdio.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#define F_CPU 12000000UL
#include <util/delay.h>

#include <uart.h>
#include <usb.h>

#include <string.h>

#include <usbfd.h>

/* warning, do not use above 9600bps for reliable data,
   the avr cannot service non-interrupt uart and usb faster */

int main(void) {
   uart_init(12);
   stderr = stdout;

   usb_init();
   
   printf("init\n");

   for(;;) {
      if (UCSR0A & _BV(RXC0))
         fputc(getc(stdin), &usb_str);

      if (usbfd_dataready())
         fputc(getc(&usb_str), stdout);

      usb_poll();
   }
}
