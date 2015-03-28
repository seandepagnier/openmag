/* Copyright (C) 2008, 2009 Sean D'Epagnier <sean@depagnier.com>
 *
 * This Program is free software; you can redistribute it and/or
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include <gfx/gfx.h>
#include <gfx/font.h>
#include <gfx/lcd.h>

#include <avr-pt.h>

#include <data/data.h>
#include <data/databasictypes.h>
#include <data/datadirectories.h>

#include <debug.h>

#include <avrdata.h>

#include <avrdrivers/avrdrivers.h>

#include <linalg/vector.h>

#include <calibration/sensor.h>
#include <calibration/cal.h>
#include <calibration/calc.h>

#include <avrdrivers/battery.h>

#include "menu.h"
#include "keys.h"
#include "internal.h"

static const char *wordforcal(float dev)
{
   if(isnan(dev) || dev < 0)
      return PSTR("corrupted");
   if(dev == 0)
      return PSTR("basic");
   if(dev > .05)
      return PSTR("bad");
   if(dev > .01)
      return PSTR("poor");
   if(dev > .002)
      return PSTR("good");
   return PSTR("excellent");
}

static const char *yesno(int val) {
   return val ? PSTR("yes") : PSTR("no");
}

int getfreeram(void);

/* emulation stubs */
#ifndef __AVR__
int getfreeram(void)
{
   return 0;
}
uint16_t watchdog_resets;
uint8_t hmc_saturated[3];
#endif

int
stats_thread(void)
{
   static struct pt pt;

   PT_BEGIN(&pt);
 restart:

   /* clear out whole screen */
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, LCD_WIDTH-1, 0, LCD_HEIGHT-1, 1);

   /* draw title */
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 11, "Stats");

   for(;;) {
      static struct gfx_rect statsarea = {0, LCD_WIDTH-1, 10, LCD_HEIGHT-1};
      menu_preparearea(&statsarea);

      gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
      gfx_setfont(GFX_4x6_FONT);
      extern char *_end;
      gfx_setpos(0, 10);

      char temps[8];
      extern void temp_format(char *buf, int len, char usedeg);
      temp_format(temps, sizeof temps, 9);

      GFX_PRINTF("freeram: %d      temp %s\n", getfreeram(), temps);
      GFX_PRINTF("runtime: %.2f    batt %03.2f%%\n", gettime(), battery_charge()*100);
      extern uint16_t watchdog_resets;
      GFX_PRINTF("sw crashes: %d\n", eeprom_read_word_safe(&watchdog_resets));
      GFX_PRINTF("sw version: "PGM_STR_FMT, softwareversion + 13);

      extern float acceldev, magdev, magaligndev;
      GFX_PRINTF("accel calibration: "PGM_STR_FMT"\n", wordforcal(acceldev));
      GFX_PRINTF("mag calibration: "PGM_STR_FMT"\n", wordforcal(magdev+magaligndev));

      GFX_PRINTF("mag distortions detected: "PGM_STR_FMT"\n",
                 yesno(calibration_mag_detectdistortions()));
      extern uint8_t hmc_saturated[3];
      GFX_PRINTF("mag sensor saturation: "PGM_STR_FMT"\n",
                 yesno(hmc_saturated[0] || hmc_saturated[1] || hmc_saturated[2]));
      GFX_PRINTF("D menu");
                 
      switch(key_lastup()) {
      case KEY_D: /* menu */
         PT_EXIT(&pt);
      }         

      MENU_YIELD(&pt);
   }
   
   PT_END(&pt);
}
