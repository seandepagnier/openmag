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
#include <data/databools.h>

#include <debug.h>

#include <avrdata.h>

#include <avrdrivers/power.h>

#include <calibration/cal.h>
#include <calibration/calc.h>

#include "menu.h"
#include "keys.h"
#include "internal.h"
#include "entry.h"
#include "readertext.h"

uint8_t contrast EEMEM = 135;
uint8_t reversevideo EEMEM;
uint8_t whitebacklight EEMEM;
uint8_t greenbacklight EEMEM = 1;

#ifndef __AVR_AT90USB1287__
DATA_ACCESSOR(name=contrast type=uint8_t mem=eeprom varname=contrast
              dir=settings applysettings)

/* so the contrast gets updated right when set */
DATA_OPERATOR(set, name=contrast type mem varname applysettings,
              extern type varname;
              data_set_##mem##_##type(&varname, data_param);
              settings_apply();
    )

uint8_t reversevideo EEMEM;
DATA_ACCESSOR(name=reversevideo type=bool mem=eeprom
              varname=reversevideo writable dir=settings applysettings)

uint8_t whitebacklight EEMEM;
DATA_ACCESSOR(name=whitebacklight type=bool mem=eeprom varname=whitebacklight
              writable dir=settings applysettings)
DATA_ACCESSOR(name=greenbacklight type=bool mem=eeprom varname=greenbacklight
              writable dir=settings applysettings)

DATA_OPERATOR(set, mem type=bool varname writable applysettings,
              extern uint8_t varname;
              uint8_t data_tmp = get_byte_##mem(&varname);
              data_setbool(&data_tmp, data_param);
              put_byte_##mem(&varname, data_tmp);
              settings_apply();
    )

#endif

extern void backlight_set(uint8_t white, uint8_t green);

void backlight_restore(void)
{
    backlight_set(eeprom_read_byte_safe(&whitebacklight),
                  eeprom_read_byte_safe(&greenbacklight));
}

void backlight_toggle(void)
{
    if(eeprom_read_byte_safe(&whitebacklight))
       eeprom_write_byte_safe(&greenbacklight, !eeprom_read_byte_safe(&greenbacklight));
    eeprom_write_byte_safe(&whitebacklight, !eeprom_read_byte_safe(&whitebacklight));
}

static int
magnetic_range_thread(void)
{
   static struct pt pt;

   PT_BEGIN(&pt);
 restart:

   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 11, "Magnetic Range Setup");

   /* draw selection */
   gfx_setfont(GFX_R08_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 12, LCD_HEIGHT-1,
		  "1 + 6.4 gauss\n2 + 3.2 gauss\n"
		  "3 + 1.6 gauss\n4 + 0.8 gauss\n"
		  "A About    D Done");

   /* draw - sign */
   int i;
   for(i = 0; i<4; i++)
      gfx_line(9, 22 + 10*i, 13, 22+10*i);

   extern uint8_t magrange_get(void);
   extern void magrange_set(uint8_t);

   /* draw in arrow */
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
   gfx_setpos(70, 12 + 10*magrange_get());
   GFX_PUTS("<-");
   
   for(;;) {
      switch(key_lastup()) {
      case KEY_1: magrange_set(0);  goto restart;
      case KEY_2: magrange_set(1);  goto restart;
      case KEY_3: magrange_set(2);  goto restart;
      case KEY_4: magrange_set(3);  goto restart;
      case KEY_A:
         PT_WAIT_THREAD(&pt, reader_thread(about_magnetic_range));
         goto restart;
      case KEY_D: /* menu */
         PT_EXIT(&pt);
      }         

      MENU_YIELD(&pt);
   }

   PT_END(&pt);
}

static int
powersettings_thread(void)
{
   static struct pt pt;
   PT_BEGIN(&pt);

 restart:
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 11, "Power Settings");

      /* draw selection */
   gfx_setfont(GFX_5x8_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 12, LCD_HEIGHT-1, "");
   gfx_setpos(0, 12);
   static const char t[] PROGMEM = "true", f[] PROGMEM = "false";
   GFX_PRINTF("1 still sleep "PGM_STR_FMT"\n"
	      "2 still sleep timeout %u\n"
	      "3 button idle "PGM_STR_FMT"\n"
	      "4 button timeout %u\n"
	      "5 wake on motion "PGM_STR_FMT"\n"
	      "\n"
	      "A About   D done",
	      pgm_read_ptr(sleeptypes + eeprom_read_byte_safe(&stillsleeptype)),
	      eeprom_read_word_safe(&stillsleeptimeout),
	      pgm_read_ptr(sleeptypes + eeprom_read_byte_safe(&buttonidlesleeptype)),
	      eeprom_read_word_safe(&buttonidletimeout),
	      eeprom_read_byte_safe(&powerwakeonmotion) ? t : f);

   for(;;) {
      uint8_t accepted;
      switch(key_lastup()) {
      case KEY_1:
      {
	 int x = eeprom_read_byte_safe(&stillsleeptype) + 1;
	 if(pgm_read_ptr(sleeptypes + x) == 0)
	    x = 0;
	 eeprom_write_byte_safe(&stillsleeptype, x);
      } goto restart;
      case KEY_2:
      {
	 float x = eeprom_read_word_safe(&stillsleeptimeout);
         WAIT_ENTRY_NUMBER("Still Sleep Time", "Enter the number of seconds\n"
			   "before sleeping when still.",
			   &x, &accepted, 1, 65535, 0, 0, NULL);
	 if(accepted)
	    eeprom_write_word_safe(&stillsleeptimeout, x);
      } goto restart;
      case KEY_3:
      {
	 int x = eeprom_read_byte_safe(&buttonidlesleeptype) + 1;
	 if(pgm_read_ptr(sleeptypes + x) == 0)
	    x = 0;
	 eeprom_write_byte_safe(&buttonidlesleeptype, x);
      } goto restart;
      case KEY_4:
      {
	 float x = eeprom_read_word_safe(&buttonidletimeout);
         WAIT_ENTRY_NUMBER("Button Sleep Time", "Enter the number of seconds\n"		   
			   "to sleep after no button press.",
			   &x, &accepted, 1, 65535, 0, 0, NULL);
	 if(accepted)
	    eeprom_write_word_safe(&buttonidletimeout, x);
      } goto restart;
      case KEY_5:
          eeprom_write_byte_safe(&powerwakeonmotion,
				 !eeprom_read_byte_safe(&powerwakeonmotion));
          goto restart;
      case KEY_A:
	 PT_WAIT_THREAD(&pt, reader_thread(about_power_settings));
	 goto restart;
      case KEY_D:
          PT_EXIT(&pt);
      }
      MENU_YIELD(&pt);
   }
   
   PT_END(&pt);
}

int
settings_thread(void)
{
   static struct pt pt;
   PT_BEGIN(&pt);

 restart:
   settings_apply();

   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 11, "Settings");

      /* draw selection */
   gfx_setfont(GFX_5x8_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 12, LCD_HEIGHT-1,
		  "1 contrast +\n2 contrast -\n3 backlight\n"
		  "4 reverse video\n5 true north\n6 set magnetic range\n"
#ifndef __AVR_AT90USB1287__
		  "7 power settings    "
#endif
		  "D done");


   gfx_setfont(GFX_R08_FONT);
   gfx_setpos(80, 15);
   GFX_PRINTF("%d", eeprom_read_byte_safe(&contrast));

   gfx_setfont(GFX_5x8_FONT);
   gfx_setpos(90, 27);
   if(eeprom_read_byte_safe(&whitebacklight))
      if(eeprom_read_byte_safe(&greenbacklight)) {
         gfx_setpos(60, 27);
           GFX_PUTS("white & green");
      } else
           GFX_PUTS("white");
   else
       if(eeprom_read_byte_safe(&greenbacklight))
           GFX_PUTS("green");
       else
           GFX_PUTS("off");

   gfx_setpos(90, 34);
   if(eeprom_read_byte_safe(&reversevideo))
      GFX_PUTS("on");
   else
      GFX_PUTS("off");

   gfx_setpos(90, 41);
   if(eeprom_read_byte_safe(&truenorth))
      GFX_PUTS("true");
   else
      GFX_PUTS("false");

   for(;;) {
      MENU_YIELD(&pt);

      uint16_t lastup = key_lastup();

      DO_ENTRY_SCROLL(lastup,
                      eeprom_write_byte_safe(&contrast,
                                             eeprom_read_byte_safe(&contrast)
                                             +speed/3);
		      ,
                      eeprom_write_byte_safe(&contrast,
                                             eeprom_read_byte_safe(&contrast)
                                             -speed/3);
		      ,
	  goto restart;
      )

      switch(lastup) {
      case KEY_3:
          backlight_toggle();
          goto restart;
      case KEY_4:
          eeprom_write_byte_safe(&reversevideo, !eeprom_read_byte_safe(&reversevideo));
          goto restart;
      case KEY_5:
          eeprom_write_byte_safe(&truenorth, !eeprom_read_byte_safe(&truenorth));
          goto restart;
      case KEY_6:
          PT_WAIT_THREAD(&pt, magnetic_range_thread());
          goto restart;
#ifndef __AVR_AT90USB1287__
      case KEY_7:
          PT_WAIT_THREAD(&pt, powersettings_thread());
          goto restart;
#endif
      case KEY_D:
          PT_EXIT(&pt);
      }
   }
   
   PT_END(&pt);
}

void settings_apply(void)
{
#ifdef __AVR__
    extern void ra8816_set_contrast(uint8_t contrast);
    ra8816_set_contrast(eeprom_read_byte_safe(&contrast));
    extern void ra8816_set_reverse_display(uint8_t);
    ra8816_set_reverse_display(eeprom_read_byte_safe(&reversevideo));
#endif
    backlight_restore();
}
