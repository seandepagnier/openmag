/* bootloader activation methods */

/* 1) activation via jumper */
#define BOOTLOADER_JUMPER

/* jumper configuration */
#define BOOTLOADER_DDR DDRD
#define BOOTLOADER_PORT PORTD
#define BOOTLOADER_PIN PIND
#define BOOTLOADER_PINNUM PIND7
#define BOOTLOADER_MASK _BV(BOOTLOADER_PINNUM)

/* use LCD,
   #define BOOTLOADER_USE_LCD
   and implement:
   void lcd_setup(void)
   void lcd_clear(void)
   void lcd_setLine(int line, int off, char *data, uint8_t count)
*/
#define BOOTLOADER_USE_LCD

/* LCD I/O definitions */
#define LCDDBPORT PORTA
#define LCDDBPIN PINA
#define LCDDBDDR DDRA
#define LCDDBMASK 0xf0
#define LCDDBSHIFT 4            /* how many bits left to shift
                                ** a DB nibble to get it to line up */
#define LCDRPORT PORTC
#define LCDRDDR DDRC
#define LCDR (_BV(PC5))

#define LCDRSPORT PORTC
#define LCDRSDDR DDRC
#define LCDRS (_BV(PC4))

#define LCDEPORT PORTA
#define LCDEDDR DDRA
#define LCDE (_BV(PA3))

#define LCD_NUM_COLS 20
#include <avr/lcd/lcd-blocking.h>

void lcd_setup(void)
{
   lcd_init();
   lcd_setcontrast(85);
}

void lcd_clear(void)
{
   lcd_reset();
}
