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

// driver for HD44780 lcd, blocking version

#ifdef USE_LCD_CURSOR
static uint8_t lcd_cursorPos=0;
#endif

static uint8_t lcd_curAddress;

static void lcd_reset();

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
    for(i = 0; i<8; i++)
        _delay_us(5);
}

static uint8_t lcd_readStatus() {
  uint8_t retVal;

  LCDDBDDR &= ~LCDDBMASK;	/* DB pins are inputs now */

  LCDRSPORT &= ~LCDRS;		/* set RS negative */
  LCDRPORT |= LCDR;		/* assert R; we're reading */

  LCDEPORT |= LCDE;		/* assert E */

  lcd_shortDelay();

  retVal = (LCDDBPIN & LCDDBMASK) << (LCDDBSHIFT == 0 ? 4 : 0);

  LCDEPORT &= ~LCDE;		/* deassert E */

  lcd_shortDelay();

  LCDEPORT |= LCDE;

  lcd_shortDelay();

  retVal |= (LCDDBPIN & LCDDBMASK) >> (LCDDBSHIFT == 0 ? 0 : 4);

  LCDEPORT &= ~LCDE;		/* deassert E */

  lcd_shortDelay();

  return retVal;
}

/* least significant 4 bits are what to send to LCD */
static inline void lcd_strobeNibble(int command, uint8_t nibble) {
  /* may need additional delays if run at over 12MHz */
#if F_CPU > 12000000
#error strobeNibble is not verified over 12MHz
#endif

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

  lcd_shortDelay();

  /* clear enable */
  LCDEPORT &= ~LCDE;
}

static inline void lcd_waitBusy() {
  lcd_shortDelay();

  uint8_t timeout = 0;
  while (lcd_readStatus() & 0x80)		/* wait while busy */
      if(++timeout == 5) /* give up.. let other stuff run */
          return;
}

static inline void lcd_sendWord(int command, uint8_t dataWord) {
  lcd_waitBusy();

  lcd_strobeNibble(command, dataWord >> 4);
  lcd_strobeNibble(command, dataWord & 0xf);
}

static inline uint8_t lcd_posToAddress(uint8_t line, uint8_t col) {
  uint8_t ret=0;

  if (line == 1) ret = 0x40;

  ret+=col;

  return ret;
}

static inline void lcd_goToAddress(uint8_t address) {
  if (lcd_curAddress != address) {
    lcd_sendWord(1, address | 0x80);

#if 1 /* robust (can unplug and re-plug in lcd while running) */
    lcd_waitBusy();
    if(address != lcd_readStatus())
        lcd_reset(); /* bad status, reset */
#endif

    lcd_curAddress=address;
  }
}

static inline void lcd_sendChar(uint8_t c) {
  lcd_sendWord(0, c);
  lcd_curAddress++;
}

static inline void lcd_updatePos(uint8_t line, uint8_t pos, uint8_t c) {
  uint8_t address;

  address=lcd_posToAddress(line, pos);

  lcd_goToAddress(address);

  lcd_sendChar(c);
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
  uint8_t i;

  for (i=off; i<off+count; i++)
    lcd_updatePos(line, i, data[i-off]);

#ifdef USE_LCD_CURSOR
  lcd_goToAddress(lcd_cursorPos);
#endif
}

/* make it a macro so we can avoid having the fmt in sram */
#define lcd_printf(line, fmt, ...) \
  do { \
     static const char tmp[] PROGMEM = fmt; \
     char buf[NUMCHAR]; \
     int len = snprintf_P(buf, sizeof buf, tmp, ## __VA_ARGS__); \
     lcd_setLine(line, 0, buf, len); \
  } while(0)


static void lcd_reset() {
  /* sending the 0011 three times somehow magically resets the HD44780 */
  lcd_strobeNibble(1, 0x3);
  lcd_longDelay(); 				/* delay more than 4.1ms */
  lcd_strobeNibble(1, 0x3);
  lcd_longDelay();				/* delay more than 100uS */
  lcd_strobeNibble(1, 0x3);
  lcd_longDelay();		/* delay here, right?!  (???, but i assume so) */

  /* OK, now we're guaranteed to be in a sane startup mode (8 bit wide
  ** operation) */

  lcd_strobeNibble(1, 0x2);	/* set data width to 4 bits */

  /* for now on, we can lcd_send commands normally and check busy flag */

  lcd_sendWord(1, 0x28);		/* 2 lines, 5x8 characters, 1/16 duty */
  lcd_sendWord(1, 0x08);		/* turn display off */
  lcd_sendWord(1, 0x01);		/* clear display */
  lcd_sendWord(1, 0x06);		/* set mode: increment, don't shift display */
  lcd_sendWord(1, 0x0c);		/* turn display on */
  lcd_goToAddress(lcd_curAddress);	/* just for consistency */
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

  _delay_ms(50); 

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

/* the duty cycle of timer2 adjusts the negative voltage generator's
   average voltage for the contrast adjust of the lcd, right now is
   PD^ and OCR2B */
void lcd_setcontrast(uint8_t contrast)
{
   /* fast pwm mode, normal */
   TCCR2A = _BV(COM2B1) /* output to OCR2A */
       | _BV(WGM20) | _BV(WGM21); /* fast pwm */
   TCCR2B = _BV(CS20);

   DDRD |= _BV(PD6); /* output */

   OCR2B = contrast;
}
