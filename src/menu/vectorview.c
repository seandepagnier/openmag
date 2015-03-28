/* Copyright (C) 2009 Sean D'Epagnier <sean@depagnier.com>
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

#include <linalg/vector.h>

#include <calibration/sensor.h>
#include <calibration/calc.h>

#include "menu.h"
#include "keys.h"
#include "internal.h"
#include "readertext.h"

#define CX 100
#define CY 31
#define R 30

static void
show_vector(float pos[3], int arrows2)
{
   float x = -R*pos[1], y = -R*pos[0];
   gfx_line(CX, CY, CX+x, CY+y);
   float dx = round(x*.15), dy = round(y*.15);
   gfx_line(CX+x*.85-dy, CY+y*.85+dx, CX+x, CY+y);
   gfx_line(CX+x*.85+dy, CY+y*.85-dx, CX+x, CY+y);

   if(arrows2) {
      gfx_line(CX+x*.7-dy, CY+y*.7+dx, CX+x*.85, CY+y*.85);
      gfx_line(CX+x*.7+dy, CY+y*.7-dx, CX+x*.85, CY+y*.85);
   }
}

int
vectorview_thread(void)
{
   static struct pt pt;

   PT_BEGIN(&pt);
 restart:

   /* clear out whole screen */
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, LCD_WIDTH-1, 0, LCD_HEIGHT-1, 1);

   /* draw title */
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH/2, 0, 11, "Vector View");

   /* draw selection */
   gfx_setfont(GFX_R08_FONT);
   DRAW_SELECTION(0, 40, 42, LCD_HEIGHT-1, "A About\nD menu");

   gfx_setpos(30, 38);
   gfx_setfont(GFX_B08_FONT);

   for(;;) {
      gfx_setdrawtype(GFX_WHITE);
      gfx_rectangle_abs(63, LCD_WIDTH-1, 0, LCD_HEIGHT-1, 1);

      gfx_setdrawtype(GFX_BLACK);
      gfx_setpos(CX, CY);
      gfx_circle(1);
      gfx_circle(31);

      float pos[3];
      sensor_get_calibrated(&accel, pos);
      show_vector(pos, 0);

      sensor_get_calibrated(&mag, pos);
      /* we want the unit vector version, so cancel out field strength */
      vector_scale(1/eeprom_read_float_safe(&fieldstrength), pos, 3);
      show_vector(pos, 1);

      switch(key_lastup()) {
      case KEY_A: /* about */
         PT_WAIT_THREAD(&pt, reader_thread(about_vector_view));
         goto restart;
      case KEY_D: /* menu */
         PT_EXIT(&pt);
      }         

      MENU_YIELD(&pt);
   }
   
   PT_END(&pt);
}
