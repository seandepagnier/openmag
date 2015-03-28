/* Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
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

/* ra8816 driver */

#include <gfx/lcd.h>

void ra8816_init(void); /* initialize display (might need tweaking for your lcd) */
void ra8816_write_framebuffer(void);  /* send ram framebuffer to lcd */
void ra8816_set_contrast(uint8_t contrast); /* set contrast */
void ra8816_set_reverse_display(uint8_t reverse); /* set the display as inverted or not */
void ra8816_standby_mode(uint8_t on); /* enter or exit standby mode */

/*
  you should define RA8816_RESETDDR, RA8816_RESETPORT, and RA8816_RESET before
  including this file eg:

  #define RA8816_RESETDDR DDRB
  #define RA8816_RESETPORT PORTB
  #define RA8816_RESET PB1

  Then include this file

  If using external memory, the first address line (A8) should go to RS, and
  A9 cannot be used for anything, leave it disconnected.

  Next call ra8816_init()

  call ra8816_write_framebuffer() to update the framebuffer to the lcd

  You can access the framebuffer directly with the global array framebuffer,
*/

char framebuffer[LCD_SPAN*64];

static uint8_t ra8816_reverse_display;

enum {DWFR, PWRR, SYSR, MWMR, CURCR, XCUR, YCUR, KEYR, SWSXR, SWSYR, SWRXR,
      SWRYR, SCOR, ASCR, SCCR, ISR, CSTR, DRCR_A, DRCR_B, BLTR, IODR, IODAR,
      ELCR, CGMI, CGMD};

static inline void send_command(uint8_t reg, uint8_t cmd)
{
    static volatile char *c = (char*)0x2200;
    *c = reg;
    *c = cmd;
}

static inline void send_data(uint8_t data)
{
    static volatile char *d =
#if defined(__AVR_AT90USB1287__)
      (char*)0x2100;
#elif defined(__AVR_ATmega2561__)
      (char*)0x2300;
#else
# error "memory location for lcd data not specified"
#endif
    *d = data;
}

void ra8816_set_contrast(uint8_t contrast)
{
   /* first 3 bits are backwards! */
   contrast ^= 0xE0;
   send_command(CSTR, contrast);
}

void ra8816_set_reverse_display(uint8_t reverse)
{
   ra8816_reverse_display = reverse;
}

void ra8816_standby_mode(uint8_t on)
{
   send_command(PWRR, on ? 1 : 2);
}

void ra8816_write_framebuffer(void)
{
    int i, j;
    for(i = 0; i<64; i++) {
        send_command(XCUR, 0);
        send_command(YCUR, i);
        if(ra8816_reverse_display)
           for(j = 0; j<LCD_SPAN; j++)
              send_data(~framebuffer[i*LCD_SPAN+j]);
        else
           for(j = 0; j<LCD_SPAN; j++)
              send_data(framebuffer[i*LCD_SPAN+j]);
    }
}

void ra8816_init(void)
{
   /* setup external memory interface */
   XMCRA = _BV(SRE);
   XMCRB = _BV(XMBK) | _BV(XMM1) | _BV(XMM2);

   /* reset the lcd with io line */
   RA8816_RESETDDR |= _BV(RA8816_RESET);
   RA8816_RESETPORT &= ~_BV(RA8816_RESET);

   _delay_ms(50);
      
   RA8816_RESETPORT |= _BV(RA8816_RESET);
   _delay_ms(1);

   send_command(PWRR, 0x02);
   _delay_ms(1);

   send_command(SYSR, 0x83);

   send_command(DRCR_A, 0xf0);
   send_command(DRCR_B, 0x3f);

   ra8816_set_contrast(0x40);
}
