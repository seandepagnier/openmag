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

#include <debug.h>

#include <avrdata.h>

#include <data/data.h>

#include <avrdrivers/buzzer.h>

#include <linalg/vector.h>
#include <linalg/quaternion.h>
#include <calibration/calc.h>
#include <calibration/compute.h>

#include <calibration/sensor.h>

#include "menu.h"
#include "keys.h"
#include "internal.h"
#include "readertext.h"
#include "entry.h"
#include "surveymeasurement.h"
#include "survey.h"

static uint8_t havenextlrud;
static float nextlrud[4];

static int8_t screen = 1, screendir = 1;
static int screen1(void);
static int screen2(void);
static int screen3_5(void);
static int screen4_7(void);
static int screen6(void);
static int screen8(void);
static int (*screenthreads[])(void) = {0, screen1, screen2,
                                       screen3_5, screen4_7, screen3_5,
                                       screen6, screen4_7, screen8};
#define NUM_SCREENS (sizeof screenthreads) / (sizeof *screenthreads)

#define CASE_AB(CASEA, CASEB) \
      case KEY_A: \
         CASEA; \
         PT_EXIT(&pt); \
      case KEY_B: \
         CASEB; \
         PT_EXIT(&pt);

#define DEFAULT_CASE_AB CASE_AB(decrement_screen(), increment_screen())
#define DEFAULT_CASE_D \
  case KEY_D: \
     screen = 0; \
     PT_EXIT(&pt);

#define DEFAULT_CASE_ABD DEFAULT_CASE_AB DEFAULT_CASE_D


static struct surveymeasurement m = {0,
                                     {SURVEYMEASUREMENT_DEFAULTNAME, 1},
                                     {SURVEYMEASUREMENT_DEFAULTNAME, 1}};

static uint8_t defaultheader EEMEM = BACKSIGHT | DISTANCE | LRUD;
static float tolerance EEMEM = 1;
static float xyzscale EEMEM = 1;

static void last_survey(void)
{
   uint16_t pos = 0;
   while((pos = surveymeasurement_read(pos, &m)));
}

static void decrement_survey(void)
{
   struct surveymeasurement n;
   uint16_t pos = 0;
   while((pos = surveymeasurement_read(pos, &n))) {
      if(!surveymeasurement_namecmp(&n.to, &m.from)) {
         m = n;
         break;
      }
   }
}

static void
putsign(float val)
{
   if(val < 0)
      GFX_PUTS("-");
   else
      GFX_PUTS("+");
}

static void
putname(struct surveyname *n)
{
   GFX_PRINTF("%c%c%c%03d", n->name[0], n->name[1], n->name[2], n->num);
}

static void increment_survey(void)
{
   m.from = m.to;
   m.to.num++;
}

static void increment_screen(void)
{
   if(++screen == NUM_SCREENS) {
      increment_survey();
      screen = 1;
   }
   screendir=1;
}

static void decrement_screen(void)
{
   if(--screen == 0) {
      screen = NUM_SCREENS - 1;
      decrement_survey();
   }
   screendir=-1;
}

static void pass_screen(void)
{
   if(screendir == 1)
      increment_screen();
   else
      decrement_screen();
}

static void clearscreen(void)
{
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, LCD_WIDTH-1, 0, LCD_HEIGHT-1, 1);
   gfx_setdrawtype(GFX_BLACK);
}

static int enter_distance(void)
{
   static struct pt pt;

   PT_BEGIN(&pt);

   uint8_t accepted;
   WAIT_ENTRY_NUMBER("Distance", "Enter the Distance",
                     &m.distance, &accepted, 0, 999, 1, 1, NULL);
   if(accepted)
      m.header |= DISTANCE;
   else
      m.header &= ~DISTANCE;

   PT_END(&pt);
}

static int enter_lrud(float walls[4], uint8_t *header)
{
   static struct pt pt;

   PT_BEGIN(&pt);

   WAIT_LRUD("Enter Left Distance", walls+0);
   WAIT_LRUD("Enter Right Distance", walls+1);
   WAIT_LRUD("Enter Up Distance", walls+2);
   WAIT_LRUD("Enter Down Distance", walls+3);
   *header |= LRUD;
restart:

   PT_END(&pt);
}

static void xordefaultheader(int mask)
{
   eeprom_write_byte_safe(&defaultheader,
                          eeprom_read_byte_safe(&defaultheader) ^ mask);
}

static const char *onoff(int mask)
{
   static const char on[] PROGMEM = "on", off[] PROGMEM = "off";
   if(eeprom_read_byte_safe(&defaultheader) & mask)
      return on;
   return off;
}

int options_thread(void)
{
   static struct pt pt;

   PT_BEGIN(&pt);

restart:
   clearscreen();
   gfx_setpos(0, 0);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
   gfx_setfont(GFX_R12_FONT);
   GFX_PUTS("Survey Options");
   
   /* draw selection */
   gfx_setpos(0, 18);
   gfx_setfont(GFX_5x8_FONT);
   GFX_PRINTF("1 FS/BS tolerance %.1f\n", eeprom_read_float_safe(&tolerance));
   GFX_PRINTF("2 xyz scale factor %.2f\n", eeprom_read_float_safe(&xyzscale));
   GFX_PRINTF("3 distance "PGM_STR_FMT"\n", onoff(DISTANCE));
   GFX_PRINTF("4 backsight "PGM_STR_FMT"\n", onoff(BACKSIGHT));
   GFX_PRINTF("5 LRUD "PGM_STR_FMT"\n", onoff(LRUD));
   GFX_PRINTF("6 Rounding 1/%d  ", eeprom_read_byte_safe(&rounding));
   gfx_setpos(90, 53);
   GFX_PUTS("D Done");
   
   for(;;) {
      uint8_t accepted;
      float val;
      switch(key_lastup()) {
      case KEY_1:
         val = eeprom_read_float_safe(&tolerance);
         WAIT_ENTRY_NUMBER("Tolerance", "Enter the FS/BS Tolerance",
                           &val, &accepted, 0, 9.9, 1, 1, NULL);
         if(accepted)
            eeprom_write_float_safe(&tolerance, val);
         goto restart;
      case KEY_2:
         val = eeprom_read_float_safe(&xyzscale);
         WAIT_ENTRY_NUMBER("xyz scale", "Enter the xyz scale factor",
                           &val, &accepted, 0, 9, 1, 1, NULL);
         if(accepted)
            eeprom_write_float_safe(&xyzscale, val);
         goto restart;
      case KEY_3: xordefaultheader(DISTANCE); goto restart;
      case KEY_4: xordefaultheader(BACKSIGHT); goto restart;
      case KEY_5: xordefaultheader(LRUD); goto restart;
      case KEY_6:
      {
        uint8_t nextrounding = eeprom_read_byte_safe(&rounding);
        switch(nextrounding) {
        case 4: nextrounding = 5; break;
        case 20: nextrounding = 100; break;
        case 100: nextrounding = 1; break;
        default: nextrounding <<= 1; break;
        }
        eeprom_write_byte_safe(&rounding, nextrounding);
      } goto restart;
      case KEY_D:
         PT_EXIT(&pt);
      }
      MENU_YIELD(&pt);
   }
   PT_END(&pt);
}

static int screen1(void)
{
   static struct pt pt;

   PT_BEGIN(&pt);

restart:
   clearscreen();

   gfx_setpos(0, 0);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
   gfx_setfont(GFX_R12_FONT);
   survey_print_from_to(&m);
   
   /* draw selection */
   gfx_setfont(GFX_R08_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 22, LCD_HEIGHT-1,
		  "1 change From station\n2 change To station\n"
		  "3 stored 4 plot 5 options\nA Prev B Next    D menu");
   
   for(;;) {       
      switch(key_lastup()) {
      case KEY_1:
         WAIT_ENTRY_SURVEYNAME("From Survey", &m.from);
         goto restart;
      case KEY_2:
         WAIT_ENTRY_SURVEYNAME("To Survey", &m.to);
         goto restart;
         break;
      case KEY_3:
         PT_WAIT_THREAD(&pt, survey_stored_thread());
         goto restart;
      case KEY_4:
         PT_WAIT_THREAD(&pt, surveyplot_thread());
         goto restart;
      case KEY_5:
         PT_WAIT_THREAD(&pt, options_thread());
         goto restart;
      case KEY_A:
         decrement_screen();
         PT_EXIT(&pt);
      case KEY_B:
         /* load in the stored measurement if we have it, or clear it out */
         if(!surveymeasurement_find(&m)) {
            m.header = 0;
            m.incline = m.azimuth = NAN;
            m.bincline = m.bazimuth = NAN;
	    m.distance = 0;
            
            if(havenextlrud) {
               havenextlrud = 0;
               m.header |= LRUD;
               memcpy(m.lrud, nextlrud, sizeof nextlrud);
               memset(nextlrud, 0, sizeof nextlrud);
            }
         }
         
         /* try to read in a measurement that has a from of the current to */
         uint16_t pos = 0;
         struct surveymeasurement n;
         while((pos = surveymeasurement_read(pos, &n)))
            if(n.header & LRUD && !surveymeasurement_namecmp(&n.from, &m.to)) {
               havenextlrud = LRUD;
               memcpy(nextlrud, n.lrud, sizeof nextlrud);
            }

         increment_screen();
         PT_EXIT(&pt);
      DEFAULT_CASE_D
      }      
      
      MENU_YIELD(&pt);
   }
   PT_END(&pt);
}

int screen8()
{
   static struct pt pt;

   PT_BEGIN(&pt);
restart:
   clearscreen();

   gfx_setpos(0, 0);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
   gfx_setfont(GFX_5x8_FONT);
   GFX_PUTS("Summary ");
   survey_print_from_to(&m);

   int y = 8;

   gfx_setpos(0, y);
   GFX_PRINTF("1 distance");
   if(m.header & DISTANCE)
      GFX_PRINTF("  %4.1f", m.distance);

   gfx_setpos(0, y+7);
   GFX_PUTS("2 foresight  ");
   GFX_PRINTF("%05.1f/", m.azimuth);
   putsign(m.incline);
   GFX_PRINTF("%04.1f", fabs(m.incline));

   gfx_setpos(0, y+14);
   GFX_PUTS("3 LRUD ");
   putname(&m.from);
   if(m.header & LRUD)
      GFX_PRINTF(" %.0f %.0f %.0f %.0f",
                 m.lrud[0], m.lrud[1], m.lrud[2], m.lrud[3]);

   gfx_setpos(0, y+21);
   GFX_PUTS("4 backsight  ");
   if(m.header & BACKSIGHT) {
      GFX_PRINTF("%05.1f/", m.bazimuth);
      putsign(m.incline);
      GFX_PRINTF("%04.1f", fabs(m.bincline));
   }

   gfx_setpos(0, y+28);
   GFX_PUTS("5 LRUD ");
   putname(&m.to);
   if(havenextlrud)
      GFX_PRINTF(" %.0f %.0f %.0f %.0f",
                 nextlrud[0], nextlrud[1], nextlrud[2], nextlrud[3]);

   gfx_setpos(0, y+35);
   if(m.header & DISTANCE) {
      float ned[3];
      vector_from_polar(ned, m.distance, m.incline, m.azimuth);
      vector_scale(eeprom_read_float_safe(&xyzscale), ned, 3);
      GFX_PRINTF("xyz %.2f %.2f %.2f", ned[0], ned[1], ned[2]);
   }
   
   gfx_setpos(0, y+44);
   GFX_PUTS("A Prev  B Store  D Exit");

   uint8_t accepted;
   for(;;) {
      switch(key_lastup()) {
      case KEY_1:
         m.header ^= DISTANCE;
         if(m.header & DISTANCE)
            PT_WAIT_THREAD(&pt, enter_distance());
         goto restart;
      case KEY_2:
         WAIT_ENTRY_NUMBER("Frontsight", "Enter the azimuth",
                           &m.azimuth, &accepted, 0, 359.9, 1, 0, NULL);
         if(!accepted)
            goto restart;
         WAIT_ENTRY_NUMBER("Frontsight", "Enter the incline",
                           &m.incline, NULL, -90, 90, 1, 0, NULL);
         goto restart;
      case KEY_3:
         m.header ^= LRUD;
         if(m.header & LRUD)
            PT_WAIT_THREAD(&pt, enter_lrud(m.lrud, &m.header));
         goto restart;
      case KEY_4:
         if(m.header & BACKSIGHT) {
            m.header &= ~BACKSIGHT;
            goto restart;
         }

         WAIT_ENTRY_NUMBER("Backsight", "Enter the azimuth",
                           &m.bazimuth, &accepted, 0, 359.9, 1, 0, NULL);
         if(!accepted)
            goto restart;
         WAIT_ENTRY_NUMBER("Backsight", "Enter the incline",
                           &m.bincline, &accepted, -90, 90, 1, 0, NULL);
         if(accepted)
            m.header |= BACKSIGHT;
         goto restart;
      case KEY_5:
         PT_WAIT_THREAD(&pt, enter_lrud(nextlrud, &havenextlrud));
         break;
         CASE_AB(decrement_screen(),
                 if(!surveymeasurement_store(&m))
                   PT_WAIT_THREAD(&pt, reader_thread(warning_survey_outofmemory));
                 increment_screen());
      case KEY_D:
         screen = 0;
         PT_EXIT(&pt);
      }
      MENU_YIELD(&pt);
   }

   PT_END(&pt);  
}

static int screen2(void)
{
   static struct pt pt;

   PT_BEGIN(&pt);

   if(!(eeprom_read_byte_safe(&defaultheader) & DISTANCE)) {
      pass_screen();
      PT_EXIT(&pt);
   }

restart:
   clearscreen();
   
   gfx_setfont(GFX_R12_FONT);
   gfx_setpos(0, 0);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
   survey_print_from_to(&m);
   gfx_setpos(0, 16);
   GFX_PUTS("Distance  ");
   if(m.header & DISTANCE)
      GFX_PRINTF("%.1f", m.distance);
   else
      GFX_PUTS("None");

   /* draw selection */
   gfx_setfont(GFX_R08_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 32, LCD_HEIGHT-1,
		  "1 enter distance\n2 clear distance\nA prev   B next");
   
   for(;;) {
      switch(key_lastup()) {
      case KEY_1:
         PT_WAIT_THREAD(&pt, enter_distance());
         goto restart;
      case KEY_2:
         m.header &= ~DISTANCE;
         m.distance = 0;
         goto restart;
      DEFAULT_CASE_ABD
      }
      MENU_YIELD(&pt);
   }

   PT_END(&pt);
}

/* look at the accelerometer x axis, and if there is a lot of movement, switch
   the laser off to reduce the risk of shooting someone's eye with a laser */
static float shakeincline, shakeazimuth;
static int test_shakedetector(void)
{
   if(isnan(shakeincline)) {
      shakeincline = incline;
      shakeazimuth = azimuth;
   }

   float diff = fabs(shakeincline - incline) + fabs(shakeazimuth - azimuth);
   shakeincline += (incline - shakeincline)*.5;
   shakeazimuth += (azimuth - shakeazimuth)*.5;
   if(diff > 10) {
      shakeincline = NAN;
      return 1;
   }
   return 0;
}

static int screen3_5(void)
{
   static struct pt pt;

   PT_BEGIN(&pt);

   if(screen == 5 && !(eeprom_read_byte_safe(&defaultheader) & BACKSIGHT)) {
      pass_screen();
      PT_EXIT(&pt);
   }

   /* start with not measure state */
   static uint8_t measure;
   measure = 0;

restart:
   clearscreen();
   gfx_setfont(GFX_R08_FONT);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
   gfx_setpos(0, 0);
   survey_print_from_to(&m);

   gfx_setfont(GFX_R08_FONT);
   gfx_setpos(0, 10);
   if(screen == 3)
      GFX_PUTS("foresight");
   else
      GFX_PUTS("backsight");

   /* draw selection */
   gfx_setfont(GFX_R08_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 43, LCD_HEIGHT-1, "A Prev\nB Next");

   int i;

   for(;;) {
      gfx_setfont(GFX_R08_FONT);
      gfx_setdrawtype(GFX_BLACK);
      gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);

      if(screen == 5) {
	 gfx_setpos(0, 33);
	 GFX_PUTS("2 clear");
      }

      gfx_setpos(0, 23);

      if(measure) {
	 GFX_PUTS("1 measure");
	 laser_on();
#if 1
	 calc_laser_incline_azimuth();
	 survey_draw_incline_azimuth(incline, azimuth);
#else
	 gfx_setdrawtype(GFX_WHITE);
	 gfx_rectangle_abs(26, LCD_WIDTH-1, 10, LCD_HEIGHT-1, 1);
	 gfx_setdrawtype(GFX_BLACK);
	 gfx_setfont(GFX_R12_FONT);
	 gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
	 calc_laser_incline_azimuth(0, 0);
	 gfx_setpos(26, 10);
	 NICE_NUM("%05.2f ", incline+.005);
	 GFX_PRINTF("%06.2f", azimuth+.005);
	 calc_laser_incline_azimuth(0, 1);
	 gfx_setpos(26, 28);
	 NICE_NUM("%05.2f ", incline+.005);
	 GFX_PRINTF("%06.2f", azimuth+.005);
	 calc_laser_incline_azimuth(0, 2);
	 gfx_setpos(26, 46);
	 NICE_NUM("%05.2f ", incline+.005);
	 GFX_PRINTF("%06.2f", azimuth+.005);
#endif
	 if(test_shakedetector())
	    goto laseroff;
      } else {
	 GFX_PUTS("1 reset");
	 if((screen == 3 && isnan(m.incline))
	    || (screen == 5 && isnan(m.bincline))) {
	    gfx_setalignment(GFX_ALIGN_CENTER | GFX_ALIGN_TOP);
	    gfx_setpos(96, 24);
	    GFX_PUTS("No");
	    gfx_setpos(96, 32);
	    GFX_PUTS("Measurement");
	 } else {
	    if(screen == 3)
	       survey_draw_incline_azimuth(m.incline, m.azimuth);
	    else
	       survey_draw_incline_azimuth(m.bincline, m.bazimuth);
	 }
      }

      static float laserofftime;
      if(laserofftime) {
         if(laserofftime + 1 < gettime()) {
            if(screen == 3) {
               m.incline = incline;
               m.azimuth = azimuth;
            } else {
               m.bincline = incline;
               m.bazimuth = azimuth;
               m.header |= BACKSIGHT;
            }

            buzzer_buzz(200);

	 laseroff:
            laserofftime = 0;
            laser_off();
	    measure = 0;
	    goto restart;
         }
      }

      switch(key_lastup()) {
      case KEY_1:
	 if(measure)
	    laserofftime = gettime();
	 else {
	    shakeincline = NAN;
	    measure = 1;
	 }
	 goto restart;

      case KEY_2:
	 if(screen == 5) {
	    m.header &= ~BACKSIGHT;
	    m.bincline = m.bazimuth = NAN;
	    measure = 0;
	    goto restart;
	 }
	 break;

	 DEFAULT_CASE_ABD
      }
      MENU_YIELD(&pt);
   }
   PT_END(&pt);
}

static int screen4_7(void)
{
   static struct pt pt;

   float *walls;
   uint8_t *havelrud = 0;
   if(screen == 4) {
      walls = m.lrud;
      havelrud = &m.header;
   } else {
      walls = nextlrud;
      havelrud = &havenextlrud;
   }

   PT_BEGIN(&pt);

   if(!(eeprom_read_byte_safe(&defaultheader) & LRUD)) {
      pass_screen();
      PT_EXIT(&pt);
   }

restart:
   clearscreen();

   gfx_setfont(GFX_R12_FONT);
   gfx_setpos(0, 0);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
   GFX_PUTS("Walls at ");
   if(screen == 4)
      putname(&m.from);
   else
      putname(&m.to);

   gfx_setpos(0, 20);
   gfx_setfont(GFX_R08_FONT);
   if(*havelrud)
      GFX_PRINTF("  left   %.1f     right %.1f\n"
                 "  up    %.1f    down %.1f\n",
                 walls[0], walls[1], walls[2], walls[3]);
   else
      GFX_PUTS("No wall data");

   /* draw selection */
   gfx_setfont(GFX_R08_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 42, LCD_HEIGHT-1, "1 enter data\n2 clear\nA Prev\nB Next");

   for(;;) {
      switch(key_lastup()) {
      case KEY_1:
         PT_WAIT_THREAD(&pt, enter_lrud(walls, havelrud));
         goto restart;
      case KEY_2:
         *havelrud &= ~LRUD;
         goto restart;
      DEFAULT_CASE_ABD
      }

      MENU_YIELD(&pt);
   }
   PT_END(&pt);
}

static int screen6(void)
{
   static struct pt pt;

   float azerr = fabs(180 - compute_resolveheading(m.azimuth - m.bazimuth));
   float incerr = fabs(m.incline + m.bincline);

   PT_BEGIN(&pt);

   if(!(m.header & BACKSIGHT)) {
      pass_screen();
      PT_EXIT(&pt);
   }

restart:
   clearscreen();

   gfx_setfont(GFX_R08_FONT);
   gfx_setpos(0, 0);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
   survey_print_from_to(&m);
   gfx_setpos(0, 10);
   GFX_PUTS("FS/BS Errors");

   gfx_setpos(0, 20);
   gfx_setfont(GFX_R08_FONT);
   GFX_PRINTF("Az  %05.1f     El  %04.1f", azerr, incerr);

   /* draw selection */
   gfx_setfont(GFX_R08_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 30, LCD_HEIGHT-1, "1 redo FS\n2 redo BS\nA Prev   B Next");

   static float lastflip;
   for(;;) {
      float time = gettime();
      if(time - lastflip > .3) {
         float t = eeprom_read_float_safe(&tolerance);
         gfx_setdrawtype(GFX_INVERT);
         if(isnan(azerr) || azerr > t)
            gfx_rectangle_abs(0, 48, 21, 30, 1);
         if(isnan(incerr) || incerr > t)
            gfx_rectangle_abs(58, 100, 21, 30, 1);
         lastflip = time;
      }

      switch(key_lastup()) {
      case KEY_1:
         screen = 3;
         PT_EXIT(&pt);
      case KEY_2:
         screen = 5;
         PT_EXIT(&pt);
      DEFAULT_CASE_ABD
      }
      MENU_YIELD(&pt);
   }

   PT_END(&pt);
}

int
lynnsurvey_thread(void)
{
   static struct pt pt;

   static uint8_t once;
   if(!once) {
      once = 1;
      last_survey();
      increment_survey();
   }

   PT_BEGIN(&pt);

   screen = 1;
   while(screen) {
      PT_WAIT_THREAD(&pt, (screenthreads[screen]()));
      laser_off();
   }

   PT_END(&pt);
}
