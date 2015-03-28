#include <stdio.h>
#include <string.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#define F_CPU 8000000UL
#include <util/delay.h>

#define BOOTLOADER_OFFSET 0xf800
#include <uart.h>

/* LCD I/O definitions */

#define LCDDBPORT PORTA
#define LCDDBPIN PINA
#define LCDDBDDR DDRA
#define LCDDBMASK 0x0f
#define LCDDBSHIFT 0            /* how many bits left to shift
                                ** a DB nibble to get it to line up */
#define LCDRPORT PORTA
#define LCDRDDR DDRA
#define LCDR (_BV(PA5))

#define LCDRSPORT PORTA
#define LCDRSDDR DDRA
#define LCDRS (_BV(PA6))

#define LCDEPORT PORTA
#define LCDEDDR DDRA
#define LCDE (_BV(PA4))

#include <lcd.h>

int main(void) {
   uart_init(12);
   sei();

   printf("init\n");

   lcd_setcontrast(80);

   lcd_init();

   lcd_setCursor(0, 0);

   DDRA |= _BV(PD7);
   PORTA &= ~_BV(PD7);

   int c = 0;
   for(;;) {
       lcd_printf(0, "%d    ", c++);
   }
}
