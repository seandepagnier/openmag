static uint8_t lcd_cursorPos=0;

#define NUMLINE 2
#define NUMCHAR 16

static uint8_t lcd_curContents[NUMLINE][NUMCHAR];
static uint8_t lcd_curAddress;

static inline void lcd_longDelay() {
  /* wait 6 ms.  in many cases we could wait much less, but this simplifies
  ** things */

  _delay_ms(6);
}

static inline void lcd_shortDelay() {
  _delay_us(15);
}

static uint8_t lcd_readStatus() {
  uint8_t retVal;

  LCDDBDDR &= ~LCDDBMASK;	/* DB pins are inputs now */

  LCDRSPORT &= ~LCDRS;		/* set RS negative */
  LCDRPORT |= LCDR;		/* assert R; we're reading */

  LCDEPORT |= LCDE;		/* assert E */

  lcd_shortDelay();

  retVal = (LCDDBPIN & LCDDBMASK) << (LCDDBSHIFT-4);

  LCDEPORT &= ~LCDE;		/* deassert E */

  lcd_shortDelay();

  LCDEPORT |= LCDE;

  lcd_shortDelay();

  retVal |= (LCDDBPIN & LCDDBMASK) >> LCDDBSHIFT;

  LCDEPORT &= ~LCDE;		/* deassert E */

  lcd_shortDelay();

  return retVal;
}

/* least significant 4 bits are what to send to LCD */
static inline void lcd_strobeNibble(int command, uint8_t nibble) {
  /* may need additional delays if run at over 4MHz */
#if F_CPU > 4000000
#error strobeNibble is not verified over 4MHz
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

  while (lcd_readStatus() & 0x80);		/* wait while busy */
}

static inline void lcd_sendWord(int command, uint8_t dataWord) {
  lcd_waitBusy();

  lcd_strobeNibble(command, dataWord >> 4);
  lcd_strobeNibble(command, dataWord & 0xf);
}

static inline uint8_t lcd_posToAddress(uint8_t line,
			uint8_t col) {
  uint8_t ret=0;

  if (line == 1) ret = 0x40;

  ret+=col;

  return ret;
}

static inline void lcd_goToAddress(uint8_t address) {
  if (lcd_curAddress != address) {
    lcd_sendWord(1, address | 0x80);
    lcd_curAddress=address;
  }
}

static inline void lcd_sendChar(uint8_t c) {
  lcd_sendWord(0, c);
  lcd_curAddress++;
}

static inline void lcd_updatePos(uint8_t line, uint8_t pos, uint8_t c) {
  uint8_t address;

  if (lcd_curContents[line][pos] == c) return;

  address=lcd_posToAddress(line, pos);

  lcd_goToAddress(address);

  lcd_sendChar(c);

  lcd_curContents[line][pos]=c;

  lcd_shortDelay();
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

  for (i=off; i<count; i++)
    lcd_updatePos(line, i, data[i]);

  lcd_goToAddress(lcd_cursorPos);
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

  lcd_goToAddress(0);		/* just for consistency */
}

void lcd_setCursor(uint8_t enabled, uint8_t blink)
{
   lcd_sendWord(1, 0x0c | enabled*2 | blink);
}

void lcd_moveCursor(uint8_t line, uint8_t row)
{
   lcd_cursorPos=lcd_posToAddress(line, row);
   lcd_goToAddress(lcd_cursorPos);
}
