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
#include <gettime.h>

#include <linalg/vector.h>

#include <calibration/sensor.h>
#include <calibration/calc.h>

#include "menu.h"
#include "keys.h"
#include "internal.h"
#include "readertext.h"
#include "entry.h"
#include "eval.h"

#define CENTER 28
static char linebuffer[30];

static volatile uint8_t inplotsmode;

static volatile uint8_t historypos;
static int8_t history[LCD_WIDTH];
#define HISTORYLEN (sizeof history) / (sizeof *history)

static uint8_t showaxes EEMEM = 1;
static uint8_t scrollingplot EEMEM = 0;

static struct atom opqueue[16];
static int opqueuepos;

static float scale = 100, offset = 0, period = 4;
static int pixelticks;

static struct gfx_rect historyarea = {0, LCD_WIDTH-1, 0, 57};

void plots_setup(float s, float o, const char *l)
{
   scale = s;
   offset = o;
   strcpy_P(linebuffer, l);
   if(!strcmp_P(linebuffer, PSTR("ane"))) {
      opqueue[0].ops = VALUE;
      opqueue[0].value = ANEMOMETER;
      opqueuepos = 1;
   }
}

static void update_history(float val)
{
   float pos = (val - offset) * 29 / scale;
   if(pos > 127)
      pos = 127;
   if(pos < -128)
      pos = -128;
   history[historypos++] = pos;
   if(historypos == HISTORYLEN)
      historypos = 0;
}

static void dashed_horizontal_line(int y, int left, int right)
{
   int i, j;
   for(i = left&~7; i<=right; i+=8)
      for(j=0; j<3; j++)
	 gfx_putpixel(i+j, y);
}

static void dashed_vertical_line(int x, int left, int right)
{
   if(x < left || x > right)
      return;

   int i, j;
   for(i = 8; i<LCD_HEIGHT; i+=8)
      for(j=0; j<3; j++)
	 gfx_putpixel(x, i+j);
}

static void draw_axes(int left, int right)
{
   const int w = 20;

   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);

   dashed_horizontal_line(14, left, right);
   dashed_horizontal_line(28, left, right);
   dashed_horizontal_line(42, left, right);

   if(left < w) {
      gfx_setpos(0, 0);
      GFX_PRINTF("%4.1f", offset+scale);
      gfx_setpos(0, 14);
      GFX_PRINTF("%4.1f", offset+scale/2);
      gfx_setpos(0, 28);
      GFX_PRINTF("%4.1f", offset);
      gfx_setpos(0, 42);
      GFX_PRINTF("%4.1f", offset-scale/2);
      gfx_setpos(0, 52);
      GFX_PRINTF("%4.1f", offset-scale);
   }

   dashed_vertical_line(31, left, right);
   dashed_vertical_line(63, left, right);
   dashed_vertical_line(95, left, right);

   if(left <= 24+w && right >= 24) {
      gfx_setpos(24, 0);
      GFX_PRINTF("%4.1f", period*3/4);
   }

   if(left <= 56+w && right >= 56) {
      gfx_setpos(56, 0);
      GFX_PRINTF("%4.1f", period*2);
   }

   if(left <= 88+w && right >= 88) {
      gfx_setpos(88, 0);
      GFX_PRINTF("%4.1f", period/4);
   }

   if(left <= 128+w && right >= 128) {
      gfx_setpos(128, 0);
      GFX_PUTS("0");
   }
}

static void draw_scrolling_plot(void)
{
   uint8_t pos = historypos;
   /* draw the history */
   menu_preparearea(&historyarea);

   if(eeprom_read_byte_safe(&showaxes))
      draw_axes(0, LCD_WIDTH-1);

   gfx_setcliprect(&historyarea);

   uint8_t p = pos, x=0;
   for(;;) {
      int last = history[p];
      if(++p == HISTORYLEN)
	 p = 0;
      if(p == pos)
	 break;

      gfx_line(x, CENTER-last, x+1, CENTER-history[p]);
      x++;
   }
   gfx_setcliprect(NULL);
}

static uint8_t resetplot;
static void draw_sweep_plot(void)
{
   static uint8_t lastp;
   uint8_t pos = historypos;

   if(resetplot) {
      lastp = pos+1;
      if(lastp >= HISTORYLEN)
	 lastp -= HISTORYLEN;
      resetplot = 0;
   }

   uint8_t p = (lastp ? : HISTORYLEN) - 1;
   if(pos == lastp || pos == lastp+1)
      return;

   gfx_setdrawtype(GFX_WHITE);
   if(lastp < pos) {
      gfx_rectangle_abs(lastp, pos-1, historyarea.top, historyarea.bottom, 1);

      gfx_setdrawtype(GFX_BLACK);
      if(eeprom_read_byte_safe(&showaxes))
	 draw_axes(lastp, pos-2);
   } else {
      gfx_rectangle_abs(lastp, HISTORYLEN-1,
			historyarea.top, historyarea.bottom, 1);
      gfx_rectangle_abs(0, pos-1, historyarea.top, historyarea.bottom, 1);

      gfx_setdrawtype(GFX_BLACK);
      if(eeprom_read_byte_safe(&showaxes)) {
	 draw_axes(lastp, LCD_WIDTH-1);
	 draw_axes(0, pos-2);
      }
   }

   lastp = pos;
   if(pos)
      lastp--;

   do {
      int last = history[p];
      if(++p == HISTORYLEN)
	 p = 0;

      int16_t min, max;
      if(p == lastp || p == pos) {
 	 min = historyarea.top;
	 max = historyarea.bottom;
      } else {
	 if(history[p] > last) {
	    min = CENTER - history[p];
	    max = CENTER - last;
	 } else {
	    min = CENTER - last;
	    max = CENTER - history[p];
	 }
	 if(min < historyarea.top)
	    min = historyarea.top;
	 if(max > historyarea.bottom)
	    max = historyarea.bottom;
      }

      if(min != max)
	 min++;
      
      /* draw bits */
      uint8_t bit = 1<<(7^(p&7));
      int addr = p>>3;
      addr += min+(min<<4);
      int y;
      for(y=min; y<=max; y++) {
	 framebuffer[addr] |= bit;
	 addr += 17;
      }
   } while(p != pos);
}

void plots_poll(void)
{
   if(!inplotsmode)
      return;
   inplotsmode--;

  /* update the history at the right period */
   static uint32_t lastticks;
   uint32_t ticks = getticks();
   if(lastticks < ticks + pixelticks) {
      float val = eval(opqueue, opqueuepos);
      while(lastticks < ticks) {
	 update_history(val);
	 lastticks += pixelticks;
      }
   }
}

static int
graph_entry_thread(void)
{
   static struct pt pt;

   PT_BEGIN(&pt);
restart:

   /* draw title */
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 12, "Graph Entry");

   gfx_setalignment(GFX_ALIGN_CENTER | GFX_ALIGN_TOP);

   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, LCD_WIDTH-1, 13, 23, 1);

   gfx_setdrawtype(GFX_BLACK);
   gfx_setfont(GFX_B08_FONT);
   gfx_setpos(LCD_WIDTH/2, 12);
   gfx_puts(linebuffer);

   gfx_setfont(GFX_5x8_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 24, LCD_HEIGHT-1,
		  "1 pitch\n2 roll\n3 yaw\n4 dip\n5 force\n6 mfield\n"
		  "A About\nC Custom\nD done");

   for(;;) {
      switch(key_lastup()) {
      case KEY_1:
	 plots_setup(60, 0, PSTR("pitch"));
	 goto restart;
      case KEY_2:
	 plots_setup(60, 0, PSTR("roll"));
	 goto restart;
      case KEY_3:
	 plots_setup(60, 0, PSTR("yaw"));
	 goto restart;
      case KEY_4:
	 plots_setup(60, 0, PSTR("dip"));
	 goto restart;
      case KEY_5:
	 plots_setup(1, 1, PSTR("sqrt(ax^2+ay^2+az^2)"));
	 goto restart;
      case KEY_6:
	 plots_setup(1, 1, PSTR("sqrt(mx^2+my^2+mz^2)"));
	 goto restart;
      case KEY_A:
	 PT_WAIT_THREAD(&pt, reader_thread(about_plots_graph_entry));
	 goto restart;
      case KEY_C:
	 PT_WAIT_THREAD(&pt, graph_entry_custom_thread());
	 strcpy(linebuffer, calculator_linebuffer());
	 goto restart;
      case KEY_D:
	 PT_EXIT(&pt);
      }
      
      MENU_YIELD(&pt);
   }

  PT_END(&pt);
}

static int
setup_thread(void)
{
 static struct pt pt;
  PT_BEGIN(&pt);
restart:

   /* draw title */
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 12, "Plots Setup");

   gfx_setfont(GFX_R08_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 11, LCD_HEIGHT-1,
		  "1 scale\n2 offset\n3 period\n4 graph\n5 show axes\n"
		  "      6\n      D done");

   gfx_setpos(35, 11);
   GFX_PRINTF("%5.2f\n", scale);
   gfx_setpos(40, 21);
   GFX_PRINTF("%5.2f\n %4.1f\n", offset, period);

   if(strlen(linebuffer) > 15) {
      gfx_setfont(GFX_4x6_FONT);
      gfx_setpos(40, 44);
   } else
      gfx_setfont(GFX_B08_FONT);
   gfx_puts(linebuffer);

   gfx_setfont(GFX_R08_FONT);
   gfx_setpos(64, 51);
   if(eeprom_read_byte_safe(&showaxes))
      GFX_PUTS("true");
   else
      GFX_PUTS("false");

   gfx_setpos(93, 11);
   if(eeprom_read_byte_safe(&scrollingplot))
      GFX_PUTS("scroll");
   else
      GFX_PUTS("sweep");

   for(;;) {
      switch(key_lastup()) {
      case KEY_1:
          WAIT_ENTRY_NUMBER("Plot Scale",
                            "Enter the scale value to use",
                            &scale, 0, .01, 99999.99, 2, 2, NULL);
	 goto restart;
      case KEY_2:
          WAIT_ENTRY_NUMBER("Plot Offset",
                            "Enter the offset value to use",
                            &offset, 0, .01, 99999.99, 2, 1, NULL);
	 goto restart;
      case KEY_3:
          WAIT_ENTRY_NUMBER("Plot Period",
                            "Enter the period value to use",
                            &period, 0, .1, 999.9, 1, 1, NULL);
	 goto restart;
      case KEY_4:
         PT_WAIT_THREAD(&pt, graph_entry_thread());
	 goto restart;
      case KEY_5:
	 eeprom_write_byte_safe(&showaxes, !eeprom_read_byte_safe(&showaxes));
	 goto restart;
      case KEY_6:
	 eeprom_write_byte_safe(&scrollingplot,
				!eeprom_read_byte_safe(&scrollingplot));
	 goto restart;
      case KEY_D:
	 PT_EXIT(&pt);
      }

     MENU_YIELD(&pt);
   }

  PT_END(&pt);
}

int
plots_thread(void)
{
   static struct pt pt;

   inplotsmode = 40;

   if(opqueuepos == 0) {
      opqueue[0].value = 12;
      opqueue[0].ops = VALUE;
      opqueuepos++;
   }

   PT_BEGIN(&pt);
   sensor_disable_calc = 1;
reparse:
   if(strlen(linebuffer) == 0)
      plots_setup(60, 0, PSTR("pitch"));

   inplotsmode = 0;
   char tempbuf[30];
   strcpy(tempbuf, linebuffer);
   if(shunting_yard(tempbuf, opqueue, &opqueuepos) < 0)
      PT_WAIT_THREAD(&pt, reader_thread(warning_calculator_bad_exp));
   inplotsmode = 40;
restart:

   resetplot=1;

   /* recompute pixel ticks */
   pixelticks = seconds2ticks(period / HISTORYLEN);
   if(pixelticks < 1)
      pixelticks = 1;

   /* clear out whole screen */
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, LCD_WIDTH-1, 0, LCD_HEIGHT-1, 1);

   /* draw selection */
   gfx_setfont(GFX_4x6_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 58, LCD_HEIGHT-1,
		  "1 setup\n2 zero\nA about\nD menu");

   /* draw the history */
   menu_preparearea(&historyarea);

   for(;;) {
      (eeprom_read_byte_safe(&scrollingplot) ?
       draw_scrolling_plot : draw_sweep_plot)();

      switch(key_lastup()) {
      case KEY_1:
         PT_WAIT_THREAD(&pt, setup_thread());
	 goto reparse;
      case KEY_2:
      {
	 float newoffset = history[historypos-1 >= 0 ? historypos-1
				   : HISTORYLEN - 1] * scale / 29;
	 int i;
	 for(i=0; i<HISTORYLEN; i++)
	    history[i] -= newoffset * 29 / scale;
	 offset += newoffset;
      } break;
      case KEY_A: /* about */
         PT_WAIT_THREAD(&pt, reader_thread(about_plots));
         goto restart;
      case KEY_D: /* menu */
	 sensor_disable_calc = 0;
         PT_EXIT(&pt);
      }

      MENU_YIELD(&pt);
   }
   
   PT_END(&pt);
}
