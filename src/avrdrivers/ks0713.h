/* ks0713 driver Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
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

/* driver for ks0713 with icons
   relevant functions to call:
*/

void ks0713_init(void); /* initialize display (might need tweaking for your lcd) */
void ks0713_write_framebuffer(void);  /* send ram framebuffer to lcd */
void ks0713_set_contrast(uint8_t contrast); /* set contrast */
void ks0713_set_reverse_display(uint8_t reverse); /* set the display as inverted or not */
uint8_t ks0713_getstatus(void); /* read status byte */
void ks0713_seticon(uint8_t icon, uint8_t on); /* set an icon on or off, see list below */
void ks0713_setbattery(uint8_t val); /* set battery icon 0-4 */
void ks0713_standby_mode(uint8_t on); /* enter or exit standby mode */

/*
  you should define KS0713_RESETDDR, KS0713_RESETPORT, and KS0713_RESET before
  including this file eg:

  #define KS0713_RESETDDR DDRB
  #define KS0713_RESETPORT PORTB
  #define KS0713_RESET PB1

  next define either KS0713_USE_XMEM eg:
  #define KS0713_USE_XMEM
  or these eg:
  #define KS0713_WRDDR DDRE
  #define KS0713_WRPORT PORTE
  #define KS0713_WR PE0
  #define KS0713_RDDDR DDRE
  #define KS0713_RDPORT PORTE
  #define KS0713_RD PE1
  #define KS0713_RSDDR DDRC
  #define KS0713_RSPORT PORTC
  #define KS0713_RS PC0
  #define KS0713_DATADDR DDRA
  #define KS0713_DATAPORT PORTA
  #define KS0713_DATAPIN PINA

  Then include this file

  If using external memory, the first address line (A8) should go to RS, and
  A9 cannot be used for anything, leave it disconnected.

  Next call ks0713_init()

  call ks0713_write_framebuffer() to update the framebuffer to the lcd

  You can access the framebuffer directly with the global array framebuffer,
*/

char framebuffer[128*8];

enum {ICON_LEAF, ICON_PAPER, ICON_BATTERY0, ICON_BATTERY1, ICON_BATTERY2,
      ICON_BATTERY3, ICON_TERMINAL, ICON_ANTENNA, ICON_MUTE, ICON_PHONE,
      ICON_MAIL, ICON_BELL, ICON_CLOCK, ICON_CAUTION, ICON_ARROWS};

#ifndef KS0713_USE_XMEM
static void strobe_write()
{
    KS0713_WRPORT &= ~_BV(KS0713_WR);
    _delay_us(1);
    KS0713_WRPORT |= _BV(KS0713_WR);
}
#endif

static void send_command(uint8_t cmd)
{
#ifdef KS0713_USE_XMEM
    static volatile char *c = (char*)0x2200;
    *c = cmd;
#else
    KS0713_DATAPORT = cmd;
    KS0713_RSPORT &= ~_BV(KS0713_RS);
    strobe_write();
#endif
}

static inline void send_data(uint8_t data)
{
#ifdef KS0713_USE_XMEM
    static volatile char *d = (char*)0x2100;
    *d = data;
#else
    KS0713_DATAPORT = data;
    KS0713_RSPORT |= _BV(KS0713_RS);
    strobe_write();
#endif
}

static void set_display_on(uint8_t on)
{
    send_command(0xAE | (on & 1));
}

static void set_line(uint8_t line)
{
    send_command(0x40 | (0x3F & line));
}

static void ks0713_set_reference_voltage_register(uint8_t reg)
{
    send_command(0x81);
    send_command(0x3F & reg);
}

static void set_page_address(uint8_t addr)
{
    send_command(0xB0 | (0xF & addr));
}

static void set_column_address(uint8_t addr)
{
    send_command(0x10 | addr >> 4);
    send_command(addr & 0xF);
}

static void set_adc(uint8_t dir)
{
    send_command(0xA0 | (dir & 1));
}

void ks0713_set_reverse_display(uint8_t reverse)
{
    send_command(0xA6 | (reverse & 1));
}

static void set_entire_display_on(uint8_t on)
{
    send_command(0xA4 | (on & 1));
}

static void set_lcd_bias(uint8_t bias)
{
    send_command(0xA2 | (bias & 1));
}

static void set_modify_read_mode(uint8_t on)
{
    if(on)
        send_command(0xE0);
    else
        send_command(0xEE);
}

static void reset_lcd(void)
{
    send_command(0xE2);
}

static void set_shl_select(uint8_t shl)
{
    send_command(0xC0 | (shl ? 8 : 0));
}

static void set_power(uint8_t setting)
{
    send_command(0x28 | (setting & 7));
}

static void set_regulator_resistor_ratio(uint8_t ratio)
{
    send_command(0x20 | (ratio & 7));
}

static void set_static_indicator(uint8_t sm, uint8_t s0, uint8_t s1)
{
    send_command(0xAC | (sm & 1));
    send_command((s1 << 1) | s0);
}

uint8_t ks0713_getstatus(void)
{
#ifdef KS0713_USE_XMEM
    static volatile char *c = (char*)0x2200;
    return *c;
#else
    KS0713_RSPORT &= ~_BV(KS0713_RS);
    KS0713_DATADDR = 0;
    KS0713_DATAPORT = 0;

    _delay_us(1);
    KS0713_RDPORT &= ~_BV(KS0713_RD);
    _delay_us(1);
    uint8_t status = KS0713_DATAPIN;
    KS0713_RDPORT |= _BV(KS0713_RD);
    KS0713_DATADDR = 0xff;
    return status;
#endif
}

void ks0713_write_framebuffer(void)
{
    int i, j;
    for(i = 0; i<8; i++) {
        set_page_address(i);
        set_column_address(4);
        for(j = 0; j<128; j++)
            send_data(framebuffer[i*128+j]);
    }
}

static void clear_display(void)
{
    int i, j;
    for(i = 0; i<9; i++) {
        set_page_address(i);
        set_column_address(4);
        for(j = 0; j<128; j++)
            send_data(0);
    }
}

void ks0713_standby_mode(uint8_t on)
{
    if(on) {
        set_display_on(0);
        set_entire_display_on(1);
    } else {
        set_entire_display_on(0);
        set_static_indicator(1, 1, 1);
        set_display_on(1);
    }
}

//#define CHECK_HARDWARE
void ks0713_init(void)
{
#ifdef CHECK_HARDWARE
#ifndef KS0713_USE_XMEM
   DATA_OUTPUT("Warning: not using external memory for ks0713 driver.\n"
               "not checking for for hardware errors.\n");
#else
   /* first make sure that the ALE pin isn't held low, this is likely because
      it is used to enter the bootloader */
   DDRE &= ~_BV(PE2);
   PORTE |= _BV(PE2);
//   _delay_us(10);
   _delay_ms(1);
   if(!(PINE & _BV(PE2))) {
       DEBUG("HWB held low, lcd cannot operate, skipping lcd init\n");
       return;
   }
   PORTE &= ~_BV(PE2);

#endif
#endif

#ifdef KS0713_USE_XMEM
   /* setup external memory interface */
   XMCRA = _BV(SRE);
   XMCRB = _BV(XMBK) | _BV(XMM1) | _BV(XMM2);
#else
   KS0713_DATADDR = 0xff;

   KS0713_RSDDR |= _BV(KS0713_RS);
   KS0713_RDDDR |= _BV(KS0713_RD);
   KS0713_WRDDR |= _BV(KS0713_WR);

   KS0713_RDPORT |= _BV(KS0713_RD);
   KS0713_WRPORT |= _BV(KS0713_WR);
#endif

   /* reset the lcd with io line */
   KS0713_RESETDDR |= _BV(KS0713_RESET);
   KS0713_RESETPORT &= ~_BV(KS0713_RESET);
   _delay_us(1);
   KS0713_RESETPORT |= _BV(KS0713_RESET);
   _delay_us(1);

   /* software reset */
   reset_lcd();

   /* need to delay a bit for the lcd to "start" */
   int i=0;
   for(i = 0; i<100; i++)
       _delay_ms(1);
 
   set_display_on(1);
   set_power(7);
   set_adc(1);

   clear_display();

   /* set contrast to something sane to start out with */
   ks0713_set_contrast(120);
}

void ks0713_seticon(uint8_t icon, uint8_t on)
{
#ifdef CRYSTALFONTZLCD
    const uint8_t iconcols[] = {6, 17, 21, 22, 23, 24, 39, 50, 59, 72, 83,
                                94, 105, 116, 126};
#else
    const uint8_t iconcols[] = {8, 19, 25, 26, 27, 28, 29, 45, 56, 64, 74,
                                84, 94, 104, 116, 131};
#endif

    set_page_address(8);
    set_column_address(iconcols[icon]);
    send_data(on);
}

/* takes a value 0-4 specifying battery */
void ks0713_setbattery(uint8_t val)
{
    ks0713_seticon(ICON_BATTERY0, val > 2);
    ks0713_seticon(ICON_BATTERY1, val > 1);
    ks0713_seticon(ICON_BATTERY2, val > 3);
    ks0713_seticon(ICON_BATTERY3, val > 0);
}

void ks0713_set_contrast(uint8_t contrast)
{
    set_regulator_resistor_ratio(4+(contrast>>6));
    ks0713_set_reference_voltage_register(contrast&0x3f);
}
