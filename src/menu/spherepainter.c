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
#include <gfx/gfx3d.h>

#include <avr-pt.h>

#include <data/data.h>
#include <data/databasictypes.h>
#include <data/datadirectories.h>

#include <debug.h>

#include <avrdata.h>

#include <linalg/vector.h>
#include <linalg/rotate.h>

#include <calibration/sensor.h>
#include <calibration/calc.h>

#include "menu.h"
#include "keys.h"
#include "internal.h"
#include "readertext.h"

uint8_t input EEMEM;

static float ang[3];

static float history[10][6];
static int historylen;

static uint8_t drawing;

static int
setup_thread(void)
{
 static struct pt pt;
  PT_BEGIN(&pt);
restart:

   /* draw title */
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 12, "Sphere Painter Setup");

   gfx_setfont(GFX_R08_FONT);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
   DRAW_SELECTION(0, LCD_WIDTH-1, 12, LCD_HEIGHT-1,
		  "1 input\n2 clear history\nD done");

   static const char laser[] PROGMEM = "laser";
   static const char accel[] PROGMEM = "accel";
   static const char mag[] PROGMEM = "mag";
   static const char *inputstrs[3] PROGMEM = {laser, accel, mag};

   gfx_setpos(65, 12);
   GFX_PRINTF(PGM_STR_FMT"\n",
	      pgm_read_ptr(inputstrs+eeprom_read_byte_safe(&input)));

   for(;;) {
      switch(key_lastup()) {
      case KEY_1:
      {
	 int i = eeprom_read_byte_safe(&input);
	 if(++i == 3)
	    i = 0;
	 eeprom_write_byte_safe(&input, i);
      } goto restart;
      case KEY_2:
	 historylen = 0;
	 drawing = 0;
      case KEY_D: PT_EXIT(&pt);
      }

     MENU_YIELD(&pt);
   }

  PT_END(&pt);
}

static void setang(float x, float y, float z)
{
   ang[0] = x;
   ang[1] = y;
   ang[2] = z;
}

int
spherepainter_thread(void)
{
   static struct pt pt;

   PT_BEGIN(&pt);

   static uint8_t ango;

   gfx3d_reset_rotation();
   gfx3d_reset_translation();
   gfx3d_translate(0, 0, 3);
   gfx3d_screenpos(LCD_WIDTH/2, LCD_HEIGHT/2);

 restart:
   /* clear out whole screen */
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, LCD_WIDTH-1, 0, LCD_HEIGHT-1, 1);

   /* draw selection */
   gfx_setfont(GFX_4x6_FONT);
   DRAW_SELECTION(0, 90, 50, LCD_HEIGHT-1, "A about\nB setup\nC draw\nD menu");

   static float point[3], movo;
      
   for(;;) {
      float curpos[3];

      if((keys & KEY_C || drawing) && !eeprom_read_byte_safe(&input))
	 laser_on();
      else
	 laser_off();

      float pos[3];
      switch(eeprom_read_byte_safe(&input)) {
      case 0:
	 calc_laser_incline_azimuth();
	 vector_from_polar(curpos, 1, incline, azimuth);
	 curpos[1] = -curpos[1];
	 break;
      case 1:
	 sensor_get_calibrated(&accel, pos);
	 goto swapxy;
      case 2:
	 sensor_get_calibrated(&mag, pos);
      swapxy:
	 curpos[0] = -pos[1];
	 curpos[1] = pos[0];
	 curpos[2] = pos[2];
	 break;
      }
      normalize(curpos);
      
      /* draw lines */
      static struct gfx_rect paintingarea = {0, LCD_WIDTH-1, 0, 50};

      gfx_setdrawtype(GFX_WHITE);
      gfx_rectangle(&paintingarea, 1);
      gfx_rectangle_abs(91, LCD_WIDTH-1, 49, LCD_HEIGHT-1, 1);

      gfx_setdrawtype(GFX_BLACK);
      gfx_setcliprect(&paintingarea);

      /* draw current vector */
      float origin[3] = {0, 0, 0};
      gfx3d_line(origin, curpos);

      /* draw current arc */
      if(drawing)
	 gfx3d_great_arc(point, curpos);

      /* draw history */
      int i;
      for(i=0; i<historylen; i++)
	 gfx3d_great_arc(history[i], history[i] + 3);

      gfx_setcliprect(NULL);

      gfx_setfont(GFX_4x6_FONT);
      gfx_setpos(110, 50);
      gfx3d_draw_axes();

      if(keys & KEY_1) setang(1, 1, 0);
      if(keys & KEY_2) setang(1, 0, 0);
      if(keys & KEY_3) setang(1, -1, 0);
      if(keys & KEY_4) setang(0, 1, 0);
      if(keys & KEY_6) setang(0, -1, 0);
      if(keys & KEY_7) setang(-1, 1, 0);
      if(keys & KEY_8) setang(-1, 0, 0);
      if(keys & KEY_9) setang(-1, -1, 0);
      if(keys & KEY_POUND) setang(0, 0, 1);
      if(keys & KEY_STAR) setang(0, 0, -1);

      if(keys & KEY_0)
	 movo += .1;
      if(keys & KEY_5)
	 movo -= .1;

      if(!keys) setang(0, 0, 0), movo = 0, ango = 0;

      gfx3d_rotate(ango++, ang[0], ang[1], ang[2]);
      gfx3d_translate(0, 0, movo);

      gfx3d_bound_translation(0, 0, 0, 0, 0, 4);

      switch(key_lastup()) {
      case KEY_A: /* about */
         PT_WAIT_THREAD(&pt, reader_thread(about_spherepainter));
	 goto restart;
      case KEY_B: 
         PT_WAIT_THREAD(&pt, setup_thread());
	 goto restart;
      case KEY_C:
	 drawing = !drawing;
	 if(drawing)
	    memcpy(point, curpos, sizeof curpos);
	 else {
	    if(historylen < (sizeof history) / (sizeof *history))
	       historylen++;
	    memcpy(history[historylen-1], point, sizeof point);
	    memcpy(history[historylen-1] + 3, curpos, sizeof curpos);
	 }
	 break;
      case KEY_D: /* menu */
	 laser_off();
         PT_EXIT(&pt);
      }         

      MENU_YIELD(&pt);
   }
   
   PT_END(&pt);
}
