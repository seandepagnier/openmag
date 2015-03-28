#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL	      // Sets up the default speed for delay.h
#include <util/delay.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#define BOOTLOADER_OFFSET 0xf800
#include <uart.h>

#define	USBTINY_INT_VECT 
#define USBTINY_INT_NUM 0
//#define BOOTLOADER_OFFSET 0xe000

//#include <usbloadable.h>

int main(void){

   uart_init(12);

   printf("init\n");
   // usbloadable_init();

   sei();

   /* fast pwm mode, normal */
   TCCR2A = _BV(COM2A1) /* output to OCR2A */
      | _BV(WGM20) | _BV(WGM21); /* fast pwm */
   TCCR2B = /* _BV(CS21) | */_BV(CS20); /* divide by 32 */

   DDRD |= _BV(PD7); /* output */

   OCR2A = 118; /* duty */

   for(;;);

   return(0);
}
