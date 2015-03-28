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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

#include <math.h>

#include <gfx/gfx.h>
#include <gfx/font.h>
#include <gfx/lcd.h>

#include <avr-pt.h>

#include <debug.h>

#include <avrdata.h>

#include <linalg/vector.h>

#include <calibration/sensor.h>
#include <calibration/calc.h>

#include "menu.h"
#include "keys.h"

#include "internal.h"
#include "entry.h"

static uint8_t inpedometermode;

uint32_t pedometer_steps EEMEM;
static uint32_t steps;

uint8_t pedometer_thresh EEMEM = 45;
float pedometer_stepsmin = NAN;

/* count steps by g transitions past a thresh, this is very basic
   and could be greatly improved to detect human steps */
void
pedometer_poll(void)
{
   if(!inpedometermode)
      return;
   inpedometermode--;

   /* load in stored steps */
   if(steps == 0)
       eeprom_read_block_safe(&steps, &pedometer_steps, sizeof steps);

   enum state {PED_UP, PED_DOWN};
   static enum state trans;

   float a[3];
   sensor_get_calibrated(&accel, a);

   float g = magnitude(a);
   static float gg;
   gg = .5*g + .5*gg;

   float thresh = (float)eeprom_read_byte_safe(&pedometer_thresh)/200;
   if(trans == PED_UP) {
      if(gg < 1-thresh) {
         trans = PED_DOWN;
         steps++;

         /* only store steps every 100 so that we can walk up to
            5000 miles before wearing out the eeprom */
         if(steps % 100 == 0)
             eeprom_write_block_safe(&steps, &pedometer_steps, sizeof steps);

         static float laststeptime;
         float time = gettime();
         if(laststeptime)
            pedometer_stepsmin = 60/(time - laststeptime);
         laststeptime = time;
      }
   } else {
      if(gg > 1+thresh)
         trans = PED_UP;
   }
}

static void
draw_pedometer(void)
{
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, LCD_WIDTH-1, 11, 41, 1);

   const int y = 6;
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
   gfx_setdrawtype(GFX_BLACK);
   gfx_setfont(GFX_R24_FONT);
   gfx_setpos(0, y);
   GFX_PRINTF("%d", steps);

   gfx_setfont(GFX_5x8_FONT);
   gfx_setpos(0, y+28);
   GFX_PRINTF("thresh %d", eeprom_read_byte_safe(&pedometer_thresh));

   gfx_setpos(52, y+28);
   static float stepsminlp = 0.0 / 0.0;
   float ratio = stepsminlp / pedometer_stepsmin;
   if(ratio > 2 || ratio < .5 || isnan(ratio))
      stepsminlp = pedometer_stepsmin;
   else
      stepsminlp = .1*pedometer_stepsmin + .9*stepsminlp;
      
   GFX_PRINTF("steps/min %.1f", stepsminlp);
}

int
pedometer_thread(void)
{
   static struct pt pt;

   inpedometermode = 40;

   PT_BEGIN(&pt);
restart:;
   /* draw selection */
   gfx_setfont(GFX_R08_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 42, LCD_HEIGHT-1, "1 thresh+\n2 thresh-\n3 reset\nD menu");

   for(;;) {
       /* draw title */
       MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 10, "Pedometer");

   redraw:
       draw_pedometer();

       MENU_YIELD(&pt);

       uint16_t lastup = key_lastup();

       DO_ENTRY_SCROLL(lastup,
           uint8_t thresh = eeprom_read_byte_safe(&pedometer_thresh);
           eeprom_write_byte_safe(&pedometer_thresh, thresh+speed/3);
                       ,
           uint8_t thresh = eeprom_read_byte_safe(&pedometer_thresh);
           eeprom_write_byte_safe(&pedometer_thresh, thresh-speed/3);
		       ,
           goto redraw;
       )

       switch(lastup) {
       case KEY_3: /* reset */
	  steps = 0;
	  eeprom_write_block_safe(&steps, &pedometer_steps, sizeof steps);
	  
	  pedometer_stepsmin = 0.0 / 0.0;
	  break;
       case KEY_D: /* menu */
	  PT_EXIT(&pt);
       }
   }
   PT_END(&pt);
}
