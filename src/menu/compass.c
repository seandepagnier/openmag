/* Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
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

#include <debug.h>

#include <avrdata.h>

#include <linalg/vector.h>
#include <linalg/rotate.h>

#include <data/data.h>

#include <calibration/sensor.h>
#include <calibration/calc.h>
#include <calibration/cal.h>

#include "menu.h"
#include "keys.h"

#include "internal.h"

static void
draw_compass_page1(void)
{
   struct gfx_rect compassarea = {60, LCD_WIDTH-1, 0, LCD_HEIGHT-1};

   /* draw pitch roll yaw */
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, 65, 10, 43, 1);

   gfx_setdrawtype(GFX_BLACK);
   gfx_setfont(GFX_R08_FONT);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);   

   gfx_setpos(0, 11);
   GFX_PUTS("pitch ");
   NICE_NUM("%05.2f", pitch);

   gfx_setpos(0, 21);
   GFX_PUTS("roll    ");
   NICE_NUM("%05.2f", roll);

   gfx_setpos(0, 31);
   GFX_PRINTF("yaw   %06.2f", yaw);

   /* draw compass */
   menu_preparearea(&compassarea);

   float m[3] = {boxalignedmag[0], boxalignedmag[1], boxalignedmag[2]};

   if(eeprom_read_byte_safe(&truenorth))
      rotatevecaroundaxis(m, boxalignedaccel,
                          deg2rad(eeprom_read_float_safe(&declination)));

   gfx_setcliprect(&compassarea);
   gfx_compass(boxalignedaccel, m,
               MIN((compassarea.right-compassarea.left+1)/2,
                   (compassarea.bottom-compassarea.top+1)/2));
   gfx_setcliprect(NULL);
}

static void
draw_compass_page2(void)
{
   /* draw stats */
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, 65, 10, 42, 1);
   gfx_rectangle_abs(66, LCD_WIDTH-1, 0, LCD_HEIGHT-1, 1);
   gfx_rectangle_abs(53, 65, 39, LCD_HEIGHT-1, 1);
   
   gfx_setdrawtype(GFX_BLACK);
   gfx_setfont(GFX_R08_FONT);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);   

   gfx_setpos(0, 29);
   GFX_PRINTF("force  %05.3fg", magnitude(boxalignedaccel));

   gfx_setpos(70, 29);
   extern float calibration_get_mag_magnitude(void);
   GFX_PRINTF("magm %05.3f", calibration_get_mag_magnitude());

   gfx_setpos(48, 40);
   GFX_PRINTF("inclination");
   NICE_NUM("%05.2f", dip);
   gfx_circle(1);

   gfx_setpos(48, 51);
   GFX_PRINTF("dip error    %05.2f", fabs(calibration_get_mag_diperror()));
   gfx_circle(1);

   gfx_setfont(GFX_R08_FONT);
   gfx_setpos(0, 9);
   GFX_PUTS("accel ");
   gfx_setpos(26, 9);
   NICE_NUM("%05.3f ", boxalignedaccel[0]);
   gfx_setpos(62, 9);
   NICE_NUM("%05.3f ", boxalignedaccel[1]);
   gfx_setpos(98, 9);
   NICE_NUM("%05.3f", boxalignedaccel[2]);

   gfx_setpos(0, 19);
   GFX_PUTS("mag  ");
   gfx_setpos(26, 19);
   NICE_NUM("%05.3f ", boxalignedmag[0]);
   gfx_setpos(62, 19);
   NICE_NUM("%05.3f ", boxalignedmag[1]);
   gfx_setpos(98, 19);
   NICE_NUM("%05.3f", boxalignedmag[2]);
}

int
compass_thread(void)
{
   static struct pt pt;
   static int curpage;
   const int numpages = 2;

   PT_BEGIN(&pt);
restart:
   /* draw title */
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, 65, -3, 9, "Compass");

   /* draw selection */
   gfx_setfont(GFX_R08_FONT);
   DRAW_SELECTION(0, 52, 43, LCD_HEIGHT-1, "1 page\nD menu");

   for(;;) {
      gfx_setdrawtype(GFX_WHITE);
      gfx_rectangle_abs(40, 65, 44, LCD_HEIGHT-1, 1);

      switch(curpage) {
      case 0: draw_compass_page1(); break;
      case 1: draw_compass_page2(); break;
      }

      switch(key_lastup()) { /* curpage */
      case KEY_1:
         curpage++;
         if(curpage == numpages)
            curpage = 0;
         break;
      case KEY_D:
         PT_EXIT(&pt);
      }
      MENU_YIELD(&pt);
   }

   PT_END(&pt);
}
