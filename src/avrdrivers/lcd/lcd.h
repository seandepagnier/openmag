/* lcd driver Copyright (C) 2007 Sean D'Epagnier <sean@depagnier.com>
 *
 * This Driver is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * For more information on the GPL, please go to:
 * http://www.gnu.org/copyleft/gpl.html
 */

// driver for HD44780 lcd
#include <string.h>

#ifdef USE_LCD_CURSOR
static uint8_t lcd_cursorPos = 0;
#endif

/* cache contents on display */
static char lcd_curContents[2][LCD_NUM_COLS];
static uint8_t lcd_curAddress;

/* data that is not yet written that needs to be sent */
static char lcd_updateContents[2][LCD_NUM_COLS];
static uint8_t lcd_updatepos, lcd_updateline;

enum {LCD_IDLE, LCD_WRITE_ADDR1, LCD_WRITE_ADDR2, LCD_WRITE_DATA1, LCD_WRITE_DATA2};
static uint8_t lcd_state;

static inline void lcd_longDelay() {
   /* wait 6 ms.  in many cases we could wait much less, but this simplifies
   ** things */
    _delay_ms(6);
}

static inline void lcd_shortDelay() {
   /* NOTE: I have tested this at 3.3v data and this is the minimum wait
      a shorter wait works most of the time but sometimes has invalid
      data */
   uint8_t i;
   for(i = 0; i<150; i++) /* 8 for 12mhz 3.3v */
      _delay_us(1);
}

/* least significant 4 bits are what to send to LCD */
static inline void lcd_strobeNibble(int command, uint8_t nibble) {
  /* may need additional delays if run at over 12MHz */
  LCDRPORT &= ~LCDR;

  if (command)
    LCDRSPORT &= ~LCDRS;	/* clear register select */
  else
    LCDRSPORT |= LCDRS;		/* set register select (data) */

  /* set enable */
  LCDEPORT |= LCDE; 

  /* configure the DB lines as outputs */
  LCDDBDDR |= LCDDBMASK;

  /* set the DB lines to the requested values */
  LCDDBPORT = (LCDDBPORT & ~LCDDBMASK) | (nibble << LCDDBSHIFT);
}

/* blocking send word */
static inline void lcd_sendWord(int command, uint8_t dataWord)
{
   lcd_strobeNibble(command, dataWord >> 4);
   lcd_shortDelay();
   LCDEPORT &= ~LCDE;
   lcd_strobeNibble(command, dataWord & 0xf);
   lcd_shortDelay();
   LCDEPORT &= ~LCDE;
}

static inline uint8_t lcd_posToAddress(uint8_t line, uint8_t col) {
  uint8_t ret=0;
  if (line == 1)
     ret = 0x40;
  ret+=col;
  return ret;
}

void lcd_flush(void)
{
   int i, j;
   for(i = 0; i<2; i++)
      for(j = 0; j<LCD_NUM_COLS; j++)
         lcd_curContents[i][j] = 0;
}

void lcd_poll(void)
{
   LCDEPORT &= ~LCDE;       /* clear enable */

 reswitch:
   switch(lcd_state) {
   case LCD_IDLE:
      /* find next character to update */
      while(lcd_updatepos < LCD_NUM_COLS) {
         if(lcd_updateContents[lcd_updateline][lcd_updatepos]
            != lcd_curContents[lcd_updateline][lcd_updatepos]) {

            uint8_t address = lcd_posToAddress(lcd_updateline, lcd_updatepos);

            if (lcd_curAddress != address) {
               lcd_curAddress=address;
               lcd_state = LCD_WRITE_ADDR1;
            } else
               lcd_state = LCD_WRITE_DATA1;
            goto reswitch;
         }
         lcd_updatepos++;
      }
      lcd_updatepos = 0;
      lcd_updateline = !lcd_updateline;
      return;
   case LCD_WRITE_ADDR1:
      lcd_strobeNibble(1, (lcd_curAddress | 0x80) >> 4);
      break;
   case LCD_WRITE_ADDR2:
      lcd_strobeNibble(1, (lcd_curAddress | 0x80) & 0xf);
      break;
   case LCD_WRITE_DATA1:
      {
         uint8_t c = lcd_updateContents[lcd_updateline][lcd_updatepos];
         lcd_strobeNibble(0, c >> 4);
      } break;
   case LCD_WRITE_DATA2:
      {
         uint8_t c = lcd_updateContents[lcd_updateline][lcd_updatepos];
         lcd_strobeNibble(0, c & 0xf);
         lcd_curContents[lcd_updateline][lcd_updatepos]=c;
         lcd_updatepos++;
         lcd_curAddress++;
         lcd_state = LCD_IDLE;
      } return;
   }
   lcd_state++;
}

void lcd_setCG(uint8_t addr, uint8_t data[8])
{
   if(addr >= 8)
      return;

   lcd_sendWord(1, 0x40 | addr*8);

   int i;
   for(i=0; i<8; i++)
      lcd_sendWord(0, data[i]);
}

void lcd_setLine(uint8_t line, uint8_t off, char *data, uint8_t count) {
   if(off + count > LCD_NUM_COLS)
      count = LCD_NUM_COLS - off;
   memcpy(lcd_updateContents[line] + off, data, count);

#ifdef USE_LCD_CURSOR
  lcd_goToAddress(lcd_cursorPos);
#endif
}

static void lcd_reset() {
  /* sending the 0011 three times somehow magically resets the HD44780 */
  lcd_strobeNibble(1, 0x3);
  lcd_longDelay(); 				/* delay more than 4.1ms */
  LCDEPORT &= ~LCDE;       /* clear enable */
  lcd_strobeNibble(1, 0x3);
  lcd_longDelay();				/* delay more than 100uS */
  LCDEPORT &= ~LCDE;       /* clear enable */
  lcd_strobeNibble(1, 0x3);
  lcd_longDelay();		/* delay here, right?!  (???, but i assume so) */
  LCDEPORT &= ~LCDE;       /* clear enable */

  /* OK, now we're guaranteed to be in a sane startup mode (8 bit wide
  ** operation) */

  lcd_strobeNibble(1, 0x2);	/* set data width to 4 bits */
  lcd_longDelay();		/* delay here, right?!  (???, but i assume so) */
  lcd_shortDelay();
  LCDEPORT &= ~LCDE;       /* clear enable */

  /* for now on, we can lcd_send lcd_commands */
  lcd_longDelay();		/* delay here, right?!  (???, but i assume so) */
  lcd_sendWord(1, 0x28);		/* 2 lines, 5x8 characters, 1/16 duty */
  lcd_longDelay();		/* delay here, right?!  (???, but i assume so) */
  lcd_sendWord(1, 0x08);		/* turn display off */
  lcd_longDelay();		/* delay here, right?!  (???, but i assume so) */
  lcd_sendWord(1, 0x01);		/* clear display */
  lcd_longDelay();		/* delay here, right?!  (???, but i assume so) */
  lcd_sendWord(1, 0x06);		/* set mode: increment, don't shift display */
  lcd_longDelay();		/* delay here, right?!  (???, but i assume so) */
  lcd_sendWord(1, 0x0c);		/* turn display on */
}

void lcd_init(void) {
  /* set control lines low */
  LCDRPORT &= ~LCDR;
  LCDRSPORT &= ~LCDRS;
  LCDEPORT &= ~LCDE;

  /* and to outputs... */
  LCDRDDR |= LCDR;
  LCDRSDDR |= LCDRS;
  LCDEDDR |= LCDE;

  lcd_reset();
}

#ifdef USE_LCD_CURSOR
void lcd_setCursor(uint8_t enabled, uint8_t blink)
{
   lcd_sendWord(1, 0x0c | enabled*2 | blink);
}

void lcd_moveCursor(uint8_t line, uint8_t col)
{
   lcd_cursorPos=lcd_posToAddress(line, col);
   lcd_goToAddress(lcd_cursorPos);
}
#endif

/* This is to setup the negative voltage driver for the lcd display.
   It is comprised of a single diode and capacitor attached to the output
   of PD6 on an atmega644

   The duty cycle of timer2 adjusts the negative voltage generator's
   average voltage for the contrast adjust of the lcd, right now is
   PD6 and OCR2B */
void lcd_setcontrast(uint8_t contrast)
{
#if defined(__AVR_ATmega644__)
   /* fast pwm mode, normal */
   TCCR2A = _BV(COM2B1) /* output to OCR2A */
       | _BV(WGM20) | _BV(WGM21); /* fast pwm */
   TCCR2B = _BV(CS20);
   DDRD |= _BV(PD6); /* output */
   OCR2B = contrast;
#endif
}
