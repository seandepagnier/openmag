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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

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

#include <avrdrivers/hotwire.h>
#include <avrdrivers/battery.h>

#include "menu.h"
#include "keys.h"
#include "internal.h"
#include "readertext.h"
#include "entry.h"

#ifndef __AVR_AT90USB1287__
DATA_ACCESSOR(name=anemometer dirname=anemometer dir=calibration)

DATA_ACCESSOR(name=scale type=float mem=eeprom writable
              varname=anemometer_scale dir=anemometer);

DATA_ACCESSOR(name=offset type=float mem=eeprom writable
              varname=anemometer_offset dir=anemometer);

DATA_ACCESSOR(name=exponent type=float mem=eeprom writable
              varname=anemometer_exponent dir=anemometer);
#endif

float anemometer_scale EEMEM = 15000.0;
float anemometer_offset EEMEM = 30000;
float anemometer_exponent EEMEM = .45;

float
calibrated_anemometer(void)
{
   float v = eeprom_read_float_safe(&anemometer_offset) - hotwire_value();
   if(v < -400) {
      eeprom_write_float_safe(&anemometer_offset, hotwire_value());
      v = 0;
   }
   float sign = v > 0 ? 1 : -1;
   float s = eeprom_read_float_safe(&anemometer_scale);
   return sign*pow(fabs(v/s), 1/eeprom_read_float_safe(&anemometer_exponent));
}

static int
setup_thread(void)
{
 static struct pt pt;
  PT_BEGIN(&pt);
restart:

   /* draw title */
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 12, "Anemometer Setup");

   /* draw selection */
   gfx_setfont(GFX_R08_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 12, LCD_HEIGHT-1,
		  "1 range\n2 offset\n3 scale\n4 exponent\nD done");

   gfx_setpos(60, 12);

   switch(eeprom_read_byte_safe(&hotwiremode)) {
   case 0: GFX_PUTS("2.54"); break;
   case 1: GFX_PUTS("1.1"); break;
   case 2: GFX_PUTS(".3"); break;
   case 3: GFX_PUTS(".254"); break;
   case 4: GFX_PUTS(".11"); break;
   }

   gfx_setpos(60, 22);
   GFX_PRINTF("%4.1f\n%4.1f\n%5.2f",
	      eeprom_read_float_safe(&anemometer_scale),
	      eeprom_read_float_safe(&anemometer_offset),
	      eeprom_read_float_safe(&anemometer_exponent));
      
   for(;;) {
      uint8_t accepted;
      switch(key_lastup()) {
      case KEY_1:
      {
	 uint8_t mode = eeprom_read_byte_safe(&hotwiremode);
	 if(++mode > 4)
	    mode = 0;
	 eeprom_write_byte_safe(&hotwiremode, mode);
      } goto restart;
      case KEY_2:
      {
	 float x = eeprom_read_float_safe(&anemometer_offset);
	 WAIT_ENTRY_NUMBER("Anemometer Offset",
			   "Enter the probe offset value\n",
			   &x, &accepted, -99999.9, 99999.9, 1, 2, NULL);
	 if(accepted)
	    eeprom_write_float_safe(&anemometer_offset, x);
      } goto restart;
      case KEY_3:
      {
	 float x = eeprom_read_float_safe(&anemometer_scale);
	 WAIT_ENTRY_NUMBER("Anemometer Scale",
			   "Enter the probe scale value\n",
			   &x, &accepted, .1, 99999.9, 1, 2, NULL);
	 if(accepted)
	    eeprom_write_float_safe(&anemometer_scale, x);
      } goto restart;
      case KEY_4:
      {
	 float x = eeprom_read_float_safe(&anemometer_exponent);
	 WAIT_ENTRY_NUMBER("Anemometer Exponent",
			   "Enter the probe exponent value\n(nominally 0.45)",
			   &x, &accepted, .01, .99, 2, 0, NULL);
	 if(accepted)
	    eeprom_write_float_safe(&anemometer_exponent, x);
      } goto restart;
      case KEY_D: PT_EXIT(&pt);
      }

     MENU_YIELD(&pt);
   }

  PT_END(&pt);
}

int
anemometer_thread(void)
{
   static struct pt pt;

   hotwire_poll();

   PT_BEGIN(&pt);
 restart:

   /* clear out whole screen */
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, LCD_WIDTH-1, 0, LCD_HEIGHT-1, 1);

   /* draw title */
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH, 0, 11, "Anemometer");

   /* draw selection */
   gfx_setfont(GFX_R08_FONT);
   DRAW_SELECTION(0, 36, 12, LCD_HEIGHT-1, "1 setup\n2 plot\n3 zero\n4 one\n"
		  "A about  B info  D menu");

   for(;;) {
     static const struct gfx_rect area = {37, LCD_WIDTH-1,
					  12, LCD_HEIGHT-12};

     gfx_setfont(GFX_B08_FONT);

     if(hotwirestate != SATURATED) {
	menu_preparearea(&area);
	gfx_setalignment(GFX_ALIGN_LEFT);
     }

     gfx_setpos(40, 24);
     switch(hotwirestate) {
     case DISABLED:
     case SATURATED:
	GFX_PUTS("the anemometer\nis disabled");
	break;
     case POWERFAIL:
	GFX_PUTS("the anemometer\nis disabled when\nthe battery is\ndrained");
	break;
     case WAITING:
	GFX_PUTS("waiting for\nanemometer probe\nto be inserted");
	break;
     case ENABLED:
	gfx_setpos(LCD_WIDTH-1, 34);
	gfx_setalignment(GFX_ALIGN_RIGHT);
	gfx_setfont(GFX_B18_FONT);
	GFX_PRINTF("%6.3f\n", calibrated_anemometer());

	gfx_setpos(42, 50);
	gfx_setfont(GFX_B12_FONT);
	gfx_setalignment(GFX_ALIGN_LEFT);
	GFX_PUTS("raw: ");
	int32_t val = hotwire_value();
	if(val > 32500) {
	   gfx_setfont(GFX_5x8_FONT);
	   GFX_PRINTF("saturated +\n");
	} else if(val < -32500) {
	   gfx_setfont(GFX_5x8_FONT);
	   GFX_PRINTF("saturated -\n");
	} else
	   GFX_PRINTF("%ld\n", val);
     }

     switch(key_lastup()) {
     case KEY_1:
	PT_WAIT_THREAD(&pt, setup_thread());
	goto restart;
     case KEY_2:
	plots_setup(2, 1, PSTR("ane"));
	PT_WAIT_THREAD(&pt, plots_thread());
	plots_setup(1, 0, PSTR(""));
	goto restart;
     case KEY_3:
	eeprom_write_float_safe(&anemometer_offset, hotwire_value());
	goto restart;
     case KEY_4:
	eeprom_write_float_safe(&anemometer_scale,
				hotwire_value()
				- eeprom_read_float_safe(&anemometer_offset));
	goto restart;
     case KEY_A: /* about */
	PT_WAIT_THREAD(&pt, reader_thread(about_anemometer));
	goto restart;
     case KEY_B: /* info */
	PT_WAIT_THREAD(&pt, reader_thread(info_anemometer));
	goto restart;
     case KEY_D: /* menu */
	hotwire_disable();
	PT_EXIT(&pt);
     }         

     static float lastruntime;
     while(gettime() - lastruntime < .2)
	MENU_YIELD(&pt);
     lastruntime = gettime();
   }

   PT_END(&pt);
}
