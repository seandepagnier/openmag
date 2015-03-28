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

   DDRD |= _BV(PD7);
   PORTD &= ~_BV(PD7);

   printf("init\n");

   lcd_init();

   lcd_setCursor(0, 0);

   uint8_t data[] = {0, 0, 0, 0, 0, 0, 0, 0xff, 0, 0, 0, 0, 0, 0, 0};
   uint8_t i;
   for(i = 0; i<8; i++)
      lcd_setCG(i, data + i);

   char buf[] = {0, 1, 2, 3, 4, 5, 6, 7};
   char dir[] = {1, 1, 1, 1, 1, 1, 1, 0};
   if(0)
   for(;;) {
      for(i = 0; i<8; i++)
         if(dir[i]) {
            if(++buf[i] == 7)
               dir[i] = 0;
         } else
            if(--buf[i] == 1)
               dir[i] = 1;

      lcd_setLine(0, 0, buf, 8);
      _delay_ms(100); 
   }

   int c = 0;
   for(;;) {
      char buf[10];
      sprintf(buf, "count: %d", c);
      c++;

      lcd_setLine(0, 0, buf, strlen(buf));

      _delay_ms(50); 
   }
}
