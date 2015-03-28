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

#include <avrdrivers/buzzer.h>

#include <data/data.h>
#include <data/databasictypes.h>
#include <data/databools.h>
#include <data/datadirectories.h>

#include <calibration/calc.h>
#include <calibration/compute.h>
#include <calibration/cal.h>
#include <calibration/sensor.h>
#include <calibration/fit.h>

#include <linalg/rotate.h>
#include <linalg/quaternion.h>
#include <linalg/vector.h>

#include "menu.h"
#include "keys.h"
#include "internal.h"
#include "readertext.h"
#include "entry.h"
#include "surveymeasurement.h"
#include "survey.h"

DATA_ACCESSOR(name=survey dirname=survey)
DATA_ACCESSOR(name=stats dir=survey)
DATA_ACCESSOR(name=measurements dir=survey)

DATA_OPERATOR(get, name=stats,
              survey_showstats();
              )

DATA_OPERATOR(get, name=measurements,
              survey_showmeasurements();
              )

DATA_OPERATOR(clear, name=measurements,
              surveymeasurements_clear();
    )

uint8_t rounding EEMEM = 10;

static const char survey_totaldistformat[] PROGMEM = "total distance:     %5.1f\n";
static const char survey_freememformat[] PROGMEM =   "free memory: %d bytes\n";

void survey_showmeasurements(void)
{
   DATA_OUTPUT("  Name     To  Dist    Azimuth      Incline"
               "       L    R    U    D\n");

   uint16_t pos = 0;
   struct surveymeasurement m;
   while((pos = surveymeasurement_read(pos, &m))) {
      DATA_OUTPUT("%c%c%c%03d ",
                  m.from.name[0], m.from.name[1], m.from.name[2], m.from.num);
      DATA_OUTPUT("%c%c%c%03d ",
                  m.to.name[0], m.to.name[1], m.to.name[2], m.to.num);
      if(m.header & DISTANCE)
         DATA_OUTPUT("%05.1f ", m.distance);
      else
         DATA_OUTPUT("  -   ");

      if(m.header & BACKSIGHT)
         DATA_OUTPUT("%05.1f/%05.1f  %c%05.2f/%c%05.2f",
                     m.azimuth, m.bazimuth,
                     m.incline < 0 ? '-' : '+', fabs(m.incline),
                     m.bincline < 0 ? '-' : '+', fabs(m.bincline));
      else
         DATA_OUTPUT("    %05.1f        %c%05.2f   ",
                     m.azimuth, m.incline < 0 ? '-' : '+', fabs(m.incline));


      if(m.header & LRUD)
         DATA_OUTPUT(" %04.1f %04.1f %04.1f %04.1f",
                     m.lrud[0], m.lrud[1], m.lrud[2], m.lrud[3]);
      DATA_OUTPUT_CHAR('\n');
   }
}

void
survey_showstats(void)
{
   float totaldist;
   int freespace;
   surveymeasurement_stats(&totaldist, &freespace);
   DATA_OUTPUT_P(survey_totaldistformat, totaldist);
   DATA_OUTPUT_P(survey_freememformat, freespace);
}

static uint8_t backsight;

static struct surveyname surveyname = {SURVEYMEASUREMENT_DEFAULTNAME, 1};

extern void laser_on(void);
extern void laser_off(void);

int
survey_enter_lrud_thread(const char *instructions, float *num)
{
   static struct pt pt;
   /* let user add extra data to a survey point */

   PT_BEGIN(&pt);

   static const char title[] PROGMEM = "LRUD";
   uint8_t accepted;
   *num = 0;
   PT_WAIT_THREAD(&pt, entry_number_thread_P(title, instructions,
                                             num, &accepted,
                                             0, 99, 1, 1, NULL));
   if(!accepted)
      *num = NAN;
   PT_END(&pt);
}

int
survey_wait_entry_surveyname_P(const char *title, struct surveyname *survey)
{
   static struct pt pt;

   PT_BEGIN(&pt);
   static const char nameinstructions[] PROGMEM = "Enter the survey name";
   uint8_t accepted;
   PT_WAIT_THREAD(&pt, entry_string_thread_P(title, nameinstructions,
                                             survey->name, &accepted, 3));
   if(accepted) {
      float fnum = survey->num;
      static const char numinstructions[] PROGMEM = "Enter the survey number";
      PT_WAIT_THREAD(&pt, entry_number_thread_P(title, numinstructions,
                                                &fnum, &accepted,
                                                0, 999, 0, 0, NULL));

      if(accepted)
         survey->num = fnum;
   }
   PT_END(&pt);
}

/* let user modifify and add extra data to a survey point */
static int
attributes_thread(uint8_t *saved, int candel,
                  struct surveymeasurement *m)
{
   static struct pt pt;

   static struct surveymeasurement a;

   PT_BEGIN(&pt);
   a = *m;

 restart:
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 11, "Survey Attributes");
   
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, LCD_WIDTH-1, 12, LCD_HEIGHT-1, 1);
   
   gfx_setdrawtype(GFX_BLACK);
   gfx_setfont(GFX_5x8_FONT);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
   
   gfx_setpos(0, 12);
   GFX_PRINTF("1 from survey  %c%c%c%03d",
              a.from.name[0], a.from.name[1], a.from.name[2], a.from.num);
   
   gfx_setpos(0, 19);
   GFX_PRINTF("2 to survey    %c%c%c%03d",
              a.to.name[0], a.to.name[1], a.to.name[2], a.to.num);
   
   gfx_setpos(0, 26);
   GFX_PUTS("3 Frontsight  ");
   GFX_PRINTF("%05.1f ", a.azimuth);
   NICE_NUM("%04.1f", a.incline);
   
   gfx_setpos(0, 33);
   if(a.header & BACKSIGHT) {
      GFX_PUTS("4 Clear Back  ");
      GFX_PRINTF("%05.1f ", a.bazimuth);
      NICE_NUM("%04.1f", a.bincline);
   } else
      GFX_PRINTF("4 Set backsight");

   gfx_setpos(0, 40);
   if(a.header & DISTANCE)
      GFX_PRINTF("5 Clear distance  %04.1f", a.distance);
   else
      GFX_PUTS("5 Set Distance");
   
   gfx_setpos(0, 47);
   if(a.header & LRUD)
      GFX_PRINTF("6 Clear lrud  %01.0f %01.0f %01.0f %01.0f",
                 a.lrud[0], a.lrud[1], a.lrud[2], a.lrud[3]);
   else
      GFX_PUTS("6 Set lrud");
   
   gfx_setdrawtype(GFX_BLACK);
   gfx_setpos(0, 57);
   if(candel) {
      gfx_setfont(GFX_4x6_FONT);
      GFX_PUTS("A about B delete C save D cancel");
   } else
      GFX_PUTS("A about  C accept D cancel");
   
   for(;;) {
      switch(key_lastup()) {
      case KEY_1: /* set from survey */
         WAIT_ENTRY_SURVEYNAME("From Survey", &a.from);
         goto restart;
      case KEY_2: /* set to survey */
         WAIT_ENTRY_SURVEYNAME("To Survey", &a.to);
         goto restart;
      case KEY_3: /* frontsight */
         WAIT_ENTRY_NUMBER("Frontsight", "Enter the azimuth",
                           &a.azimuth, NULL, 0, 359.9, 1, 0, NULL);
         WAIT_ENTRY_NUMBER("Frontsight", "Enter the incline",
                           &a.incline, NULL, -90, 90, 1, 0, NULL);
         goto restart;
      case KEY_4: /* backsight */
         if(a.header & BACKSIGHT)
            a.header &= ~BACKSIGHT;
         else {
            uint8_t accepted;
            a.bazimuth = compute_resolveheading(a.azimuth + 180);
            WAIT_ENTRY_NUMBER("Azimuth", "Enter the backsight azimuth",
                              &a.bazimuth, &accepted, 0, 359.9, 1, 0, NULL);
            if(accepted) {
               a.bincline = -a.incline;
               WAIT_ENTRY_NUMBER("Backsight", "Enter the backsight incline",
                                 &a.bincline, &accepted, -90, 90, 1, 0, NULL);
               if(accepted)
                  a.header |= BACKSIGHT;
            }
         }
         goto restart;
      case KEY_5: /* enter distance */
         if(a.header & DISTANCE)
            a.header &= ~DISTANCE;
         else {
            uint8_t accepted;
            WAIT_ENTRY_NUMBER("Distance", "Enter the Distance",
                              &a.distance, &accepted, 0, 999, 1, 1, NULL);
            if(accepted)
               a.header |= DISTANCE;
         }
         goto restart;
      case KEY_6: /* enter lrud */
         if(!(a.header & LRUD)) {
            WAIT_LRUD("Enter Left Distance", a.lrud+0);
            WAIT_LRUD("Enter Right Distance", a.lrud+1);
            WAIT_LRUD("Enter Up Distance", a.lrud+2);
            WAIT_LRUD("Enter Down Distance", a.lrud+3);
         }
         a.header ^= LRUD;
         goto restart;
      case KEY_A:
         PT_WAIT_THREAD(&pt, reader_thread(about_survey_attributes));
         goto restart;
      case KEY_B:
         if(candel) {
            surveymeasurement_delete(&a.from, &a.to);
            if(saved)
               *saved = 0;
            PT_EXIT(&pt);
         } break;
      case KEY_C:
         if(saved)
            *saved = 1;
         *m = a;
         PT_EXIT(&pt);         
      case KEY_D: /* done */
         if(saved)
            *saved = 0;
         PT_EXIT(&pt);
      }
      MENU_YIELD(&pt);
   }
   PT_END(&pt);
}

int
survey_stats_thread(void)
{
   static struct pt pt;
   PT_BEGIN(&pt);

restart:
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 11, "Survey Stats");

   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, LCD_WIDTH-1, 12, LCD_HEIGHT-1, 1);

   float totaldist;
   int freespace;
   surveymeasurement_stats(&totaldist, &freespace);

   gfx_setdrawtype(GFX_BLACK);
   gfx_setfont(GFX_R08_FONT);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
   gfx_setpos(0, 16);
   gfx_printf_P(survey_totaldistformat, totaldist);
   gfx_printf_P(survey_freememformat, freespace);

   gfx_setfont(GFX_5x8_FONT);
   gfx_setpos(0, 50);
   GFX_PUTS("5 Clear all measurements\n");
   GFX_PUTS("D Done\n");
   
   for(;;) {
      switch(key_lastup()) {
      case KEY_5:
         surveymeasurements_clear();
         surveyname.num = 1;
         backsight = 0;
	 goto restart;
      case KEY_D:
         PT_EXIT(&pt);
      }

      MENU_YIELD(&pt);
   }

   PT_END(&pt);
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
tight_1_decimal(float val, int places)
{
   static const char fmt_P[] PROGMEM = "%%0%dd";
   char format[6];
   snprintf_P(format, sizeof format, fmt_P, places);
   gfx_printf(format, (int)val);

   int x, y;
   gfx_getpos(&x, &y);
   gfx_putpixel(x, y+6);
   gfx_putpixel(x, y+7);
   gfx_setpos(x+1, y);
   GFX_PRINTF("%d", ((int)val)%10);
}

static void
nice_ned(float val)
{
   if(fabs(val) < 100)
      NICE_NUM("%04.1f ", val);
   else
      NICE_NUM("%03.0f  ", val);
}

int
survey_stored_thread(void)
{
   static struct pt pt;
   static int offset;
   static int8_t atend, selected, page;

   PT_BEGIN(&pt);
 restart:
   gfx_setfont(GFX_5x8_FONT);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, LCD_WIDTH-1, 0, 7, 1);
   gfx_setdrawtype(GFX_BLACK);
   gfx_setpos(0, 0);
   GFX_PUTS("Survey");

   switch(page) {
   case 0:
      GFX_PUTS("  Azimuth  Incline");
      break;
   case 1:
      GFX_PUTS("     To     Dist");
      break;
   case 2:
      GFX_PUTS("  L    R    U    D");
      break;
   case 3:
      GFX_PUTS("  North  East   Up");
   }

   /* draw survey data */
   gfx_setdrawtype(GFX_WHITE);

   const struct gfx_rect measurementarea = {0, LCD_WIDTH-1, 8, LCD_HEIGHT-1};
   gfx_rectangle(&measurementarea, 1);

   gfx_setdrawtype(GFX_BLACK);
   gfx_setfont(GFX_5x8_FONT);

   uint16_t pos = 0;

   static const int maxpage = 5;

   atend = 0;

   static struct surveymeasurement s = {0,
                                        {SURVEYMEASUREMENT_DEFAULTNAME, 1},
                                        {SURVEYMEASUREMENT_DEFAULTNAME, 2}};

   int i = 0;
   struct surveymeasurement m;
   while((pos = surveymeasurement_read(pos, &m))) {
      if(i > offset + maxpage)
	 break;

      if(i >= offset) {
         gfx_setpos(0, measurementarea.top + (i - offset)*8);
         GFX_PRINTF("%c%c%c%03d",
                    m.from.name[0],
                    m.from.name[1],
                    m.from.name[2],
                    m.from.num);

         float bestincline, bestazimuth;
         if(m.header & BACKSIGHT) {
            bestincline = (m.incline - m.bincline) / 2;
            bestazimuth = compute_resolveheading((m.azimuth
                           + compute_resolveheading(m.bazimuth + 180)) / 2);
         } else {
            bestincline = incline;
            bestazimuth = azimuth;
         }

         switch(page) {
         case 0:
            if(m.header & BACKSIGHT) {
               int x, y;
               gfx_getpos(&x, &y);
               gfx_setpos(x+4, y);
               tight_1_decimal(fabs(m.azimuth), 3);
               GFX_PUTS("/");
               tight_1_decimal(fabs(m.bazimuth), 3);
               gfx_setpos(x+55, y);
               putsign(m.incline);
               tight_1_decimal(fabs(m.incline), 2);
               GFX_PUTS("/");
               putsign(m.bincline);
               tight_1_decimal(fabs(m.bincline), 2);
            } else {
               GFX_PRINTF("  %05.1f    ", m.azimuth);
               NICE_NUM("%04.1f ", m.incline);
            }
            break;
         case 1:
            GFX_PRINTF(" %c%c%c%03d     ",
                       m.to.name[0],
                       m.to.name[1],
                       m.to.name[2],
                       m.to.num);
            if(m.header & DISTANCE)
               GFX_PRINTF("%04.1f ", m.distance);
            else
               GFX_PUTS("     ");
            break;
         case 2:
            if(m.header & LRUD)
               GFX_PRINTF(" %04.1f %04.1f %04.1f %04.1f",
                          m.lrud[0], m.lrud[1], m.lrud[2], m.lrud[3]);
            break;
         case 3:
            if(m.header & DISTANCE) {
               GFX_PUTS("  ");
               float ned[3];
	       vector_from_polar(ned, m.distance, bestincline, bestazimuth);
               nice_ned(ned[0]);
               nice_ned(ned[1]);
               nice_ned(ned[2]);
            }
            break;
         }

         /* keep selected values */
         if(i - offset == selected)
            s = m;
      }

      i++;
   }

   if(!pos) {
      atend = i - offset;
      if(!atend)
	 atend = -1;
      else
	 if(offset >= i) {
	    /* if we got to the end before drawing anything, move up
	       so at least 1 entree is visible */
	    offset--;
	    goto restart;
	 }
   }

   while(atend > 0 && selected > atend - 1)
      selected--;

   /* highlight selected */
   if(atend != -1) {
      gfx_setdrawtype(GFX_INVERT);
      gfx_rectangle_abs(0, LCD_WIDTH-1, measurementarea.top + selected*8,
                        measurementarea.top + (selected+1)*8-1, 1);
   }

   /* draw instructions */
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_BOTTOM);
   gfx_setdrawtype(GFX_BLACK);
   gfx_setpos(0, LCD_HEIGHT);
   GFX_PRINTF("1 edit 2");
   gfx_setpos(54, LCD_HEIGHT);
   GFX_PRINTF("A  B  C  D back");

   gfx_rectangle_abs(42, 46, LCD_HEIGHT-6, LCD_HEIGHT-1, 0);
   gfx_setdrawtype(GFX_INVERT);
   gfx_putpixel(46, LCD_HEIGHT-6);
   gfx_putpixel(46, LCD_HEIGHT-5);
   gfx_putpixel(45, LCD_HEIGHT-6);
   gfx_putpixel(45, LCD_HEIGHT-5);

   gfx_setdrawtype(GFX_BLACK);

   const int upx = 62;
   gfx_line(upx, LCD_HEIGHT-1, upx, LCD_HEIGHT-5);
   gfx_line(upx-2, LCD_HEIGHT-3, upx, LCD_HEIGHT-5);
   gfx_line(upx+2, LCD_HEIGHT-3, upx, LCD_HEIGHT-5);

   const int downx = upx+15;
   gfx_line(downx, LCD_HEIGHT-1, downx, LCD_HEIGHT-5);
   gfx_line(downx-2, LCD_HEIGHT-3, downx, LCD_HEIGHT-1);
   gfx_line(downx+2, LCD_HEIGHT-3, downx, LCD_HEIGHT-1);

   const int sidex = downx+13;
   gfx_line(sidex, LCD_HEIGHT-3, sidex+4, LCD_HEIGHT-3);
   gfx_line(sidex+2, LCD_HEIGHT-5, sidex+4, LCD_HEIGHT-3);
   gfx_line(sidex+2, LCD_HEIGHT-1, sidex+4, LCD_HEIGHT-3);

   for(;;) {
      MENU_YIELD(&pt);

      static int speed;
      if(keys & KEY_A)
         speed++;
      else
      if(keys & KEY_B)
         speed--;
      else
         speed/=2;

      switch(key_lastup()) {
      case KEY_1:
         {
            uint8_t saved;
            PT_WAIT_THREAD(&pt, attributes_thread(&saved, 1, &s));
            if(saved)
               if(!surveymeasurement_store(&s))
                  PT_WAIT_THREAD(&pt, reader_thread(warning_survey_outofmemory));
            goto restart;
         }
      case KEY_2:
         PT_WAIT_THREAD(&pt, survey_stats_thread());
         goto restart;
      case KEY_A:
         speed = 2;
         break;
      case KEY_B:
         speed = -2;
         break;
      case KEY_C:
         if(++page == 4)
            page = 0;
         goto restart;
      case KEY_D: /* cancel */
         PT_EXIT(&pt);
      }

      if(speed > 0) {
         for(i = 0; i<speed/2; i++)
            if(selected > 0)
               selected--;
            else
               if(offset > 0)
                  offset--;
         goto restart;
      }

      if(speed < 0) {
         for(i = 0; i<-speed/2; i++)
            if(atend) {
               if(selected < atend - 1)
                  selected++;
            } else 
               if(selected < maxpage)
                  selected++;
               else
                  offset++;
         goto restart;
      }
   }

   PT_END(&pt);
}

float rounded(float value)
{
  uint8_t r = eeprom_read_byte_safe(&rounding);

  /* for 100ths and 10ths, the truncation takes care of rounding,
     floating point numbers are binary and have decimal issues,
     this works best */
  if(r == 100)
    return value + .005;

  if(r == 10)
    return value + .05;

  float mod = r;
  value = round(mod*value) / mod;

  /* binary floats get converted to string later,
     add a small value so truncation works right later */
  mod = 0;
  if(r == 5)
    mod = .01;
  if(r == 20)
    mod = .005;
  if(value < 0)
    mod = -mod;

  return value + mod;
}

static void
print_number(float number, uint8_t digits)
{
  char fmt[] = "%0xd";
  fmt[2] = '0' + digits;
  gfx_printf(fmt, (int)number);
}

void
survey_draw_incline_azimuth(float incline, float azimuth)
{
   uint8_t r = eeprom_read_byte_safe(&rounding);
   uint8_t decimals = r == 4 || r == 20 || r == 100 ? 2 : r == 1 ? 0 : 1;
   uint8_t xoffd[3] = {56, 52, 42}, xoff = xoffd[decimals], yoff = 8;
   uint8_t dmult = decimals == 2 ? 100 : 10;

   gfx_setfont(GFX_R24_FONT);
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(xoff, LCD_WIDTH-1, 11, LCD_HEIGHT-1, 1);
   
   gfx_setdrawtype(GFX_BLACK);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
      
   /* display azimuth */
   float razimuth = rounded(azimuth);
   gfx_setpos(xoff, yoff);
   print_number(razimuth, 3);

   if(decimals) {
     gfx_setpos(xoff+51, yoff);
     GFX_PUTS(".");
     gfx_setpos(xoff+56, yoff);
     print_number(dmult*fmod(razimuth, 1.0), decimals);
   }

   /* display incline */
   gfx_line(xoff+4, 41+yoff, xoff+13, 41+yoff);
   gfx_line(xoff+4, 42+yoff, xoff+13, 42+yoff);
   if(incline >= 0) {
      gfx_line(xoff+8, 36+yoff, xoff+8, 47+yoff);
      gfx_line(xoff+9, 36+yoff, xoff+9, 47+yoff);
   }

   float rincline = fabs(rounded(incline));
   gfx_setpos(xoff+18, 26+yoff);
   print_number(rincline, 2);

   if(decimals) {
     gfx_setpos(xoff+51, 26+yoff);
     GFX_PUTS(".");
     gfx_setpos(xoff+56, 26+yoff);
     print_number(dmult*fmod(rincline, 1.0), decimals);
   }
}

void
survey_print_from_to(struct surveymeasurement *m)
{
   GFX_PRINTF("%c%c%c%03d to %c%c%c%03d",
              m->from.name[0], m->from.name[1], m->from.name[2], m->from.num,
              m->to.name[0], m->to.name[1], m->to.name[2], m->to.num);
}

static void
draw_survey_title(struct surveymeasurement *m)
{
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, LCD_WIDTH-1, 0, 11, 1);
   gfx_setdrawtype(GFX_BLACK);
   gfx_setfont(GFX_R08_FONT);
   gfx_setpos(66, 0);
   gfx_setalignment(GFX_ALIGN_CENTER | GFX_ALIGN_TOP);

   survey_print_from_to(m);
}

static int
survey_measured_thread(void)
{
   static struct pt pt;

   PT_BEGIN(&pt);

   static struct surveymeasurement m;
   static struct surveyname lastname;
   static uint8_t showmagcalwarning;
   static float mincline, mazimuth;
   static uint8_t canbacksight;

   if(!lastname.num) {
      lastname = surveyname;
      lastname = surveyname;
      if(lastname.num > 1)
         lastname.num--;
   }

   showmagcalwarning = 0;

reset:
   /* see if we have data for this shot, and fill it in */
   m.from = lastname;
   m.to = surveyname;
   if(surveymeasurement_find(&m) && !(m.header&BACKSIGHT))
      canbacksight = 1;
   else {
      m.header = 0;
      canbacksight = 0;
      backsight = 0;
   }

   if(backsight) {
      m.header |= BACKSIGHT;
      m.bincline = mincline;
      m.bazimuth = mazimuth;
   } else {
      m.from = surveyname;
      m.to = surveyname;
      m.to.num++;

      m.header &= ~BACKSIGHT;
      m.incline = mincline;
      m.azimuth = mazimuth;

   }

 restart:
   /* update display */
   draw_survey_title(&m);

   gfx_setfont(GFX_R08_FONT);
   DRAW_SELECTION(0, 55, 12, LCD_HEIGHT-1,
		  "1 remeasure\n2 store\n3 attributes\n\nD cancel");

   if(canbacksight) {
      gfx_setpos(0, 42);
      gfx_setdrawtype(GFX_BLACK);
      GFX_PUTS("4 FS / BS");

      gfx_setdrawtype(GFX_INVERT);
      if(backsight)
         gfx_rectangle_abs(31, 44, 43, 52, 1);
      else
         gfx_rectangle_abs(9, 21, 43, 52, 1);
   }

   for(;;) {
      static float laserofftime;

      /* turn laser on if '1' is pressed, off 1 second after '1' is released */
      if(keys & KEY_1) {
         laser_on();
         laserofftime = 0;
	 mincline = incline;
	 mazimuth = azimuth;
      }

      survey_draw_incline_azimuth(mincline, mazimuth);

      if(laserofftime) {
	 if(laserofftime + 1 > gettime()) {
	    mincline = incline;
	    mazimuth = azimuth;
	    MENU_YIELD(&pt);
	    goto reset;
	 } else {
	    laser_off();
	    laserofftime = 0;
	    buzzer_buzz(200);
	 }
      }

      switch(key_lastup()) {
      case KEY_1: /* remeasure */
         laserofftime = gettime();
         if(calibration_mag_detectdistortions()) {
            if(showmagcalwarning > 15)
               showmagcalwarning = 20 - showmagcalwarning;
            else
               if(showmagcalwarning > 7)
                  showmagcalwarning = 8;
               else
                  showmagcalwarning = 1;
            }
         goto restart;
      case KEY_2: /* store the measurement */
         if(surveymeasurement_store(&m)) {
            backsight = 0;
            lastname = m.from;
            surveyname = m.to;
         } else
            PT_WAIT_THREAD(&pt, reader_thread(warning_survey_outofmemory));
         PT_EXIT(&pt);
      case KEY_3: /* attributes */
         laser_off();
         PT_WAIT_THREAD(&pt, attributes_thread(0, 0, &m));
         if(backsight && !(m.header & BACKSIGHT))
            backsight = 0;
         goto restart;
      case KEY_4: /* FS / BS */
         if(canbacksight)
            backsight = !backsight;
         goto reset;
      case KEY_D: /* cancel */
         PT_EXIT(&pt);
      }

      if(showmagcalwarning) {
         int off = 20 - abs(20 - showmagcalwarning*2);
         if(off > 15)
            off = 15;

         gfx_setpos(0, LCD_HEIGHT - off);

         gfx_setdrawtype(GFX_WHITE);
         gfx_rectangle_abs(0, LCD_WIDTH-1, LCD_HEIGHT - off, LCD_HEIGHT-1, 1);
         gfx_setdrawtype(GFX_BLACK);
         gfx_setfont(GFX_5x8_FONT);
         GFX_PUTS("Warning: azimuth inaccurate\ndistortion or bad mag cal");

         if(++showmagcalwarning == 20)
            showmagcalwarning = 0;
         MENU_YIELD(&pt);
         goto restart;
      }

      MENU_YIELD(&pt);
   }
   PT_END(&pt);
}

int
survey_thread(void)
{
   static struct pt pt;

   static uint8_t once;
   if(!once) {
      once = 1;
      uint16_t pos = 0;
      /* set the survey name to the "to" of the last stored measurement */
      struct surveymeasurement m;
      int one = 0;
      while((pos = surveymeasurement_read(pos, &m)))
         one = 1;
      if(one)
         surveyname = m.to;
   }

   calc_laser_incline_azimuth();

   PT_BEGIN(&pt);

 restart:
   /* draw title */
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, -2, 11, "Survey");

   /* draw selection */
   gfx_setfont(GFX_R08_FONT);
   DRAW_SELECTION(0, 55, 12, LCD_HEIGHT-1,
		  "1 measure\n2 stored\n"
#ifndef __AVR_AT90USB1287__
		  "3 plot"
#endif
		  "\nA about\nD menu");

   for(;;) {       
       /* go to measured thread when down */
       if(keys & KEY_1) {
           PT_WAIT_THREAD(&pt, survey_measured_thread());
           goto restart;
       }

      /* update display */
      survey_draw_incline_azimuth(incline, azimuth);

      laser_off();
      switch(key_lastup()) {
      case KEY_2:
         PT_WAIT_THREAD(&pt, survey_stored_thread());
         goto restart;
#ifndef __AVR_AT90USB1287__
      case KEY_3:
         PT_WAIT_THREAD(&pt, surveyplot_thread());
         goto restart;
#endif
      case KEY_A:
         PT_WAIT_THREAD(&pt, reader_thread(about_survey));
         goto restart;
      case KEY_D:
         PT_EXIT(&pt);
      }
      MENU_YIELD(&pt);
   }
   PT_END(&pt);
}
