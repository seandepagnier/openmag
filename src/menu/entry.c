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
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include <gfx/gfx.h>
#include <gfx/font.h>
#include <gfx/lcd.h>

#include <avr-pt.h>

#include <debug.h>

#include <avrdata.h>

#include "menu.h"
#include "keys.h"

#include "internal.h"
#include "readertext.h"
#include "entry.h"

static void drawtitleandinstructions_P(const char *title, const char *instructions)
{
   static const struct gfx_rect titlearea PROGMEM = {0, LCD_WIDTH-1, 0, 11};
   gfx_setfont(GFX_R08_FONT);
   menu_draw_title_P(&titlearea, title);

   gfx_setdrawtype(GFX_WHITE);
   struct gfx_rect instructionarea = {0, LCD_WIDTH-1, 12, 28};
   gfx_rectangle(&instructionarea, 1);
   gfx_setdrawtype(GFX_BLACK);
   gfx_setpos(instructionarea.left, instructionarea.top);
   gfx_setmaxfont_P(instructions, &instructionarea);
   gfx_setalignment(GFX_ALIGN_CENTER | GFX_ALIGN_TOP);
   gfx_setpos((instructionarea.left+instructionarea.right)/2,
              instructionarea.top);
   gfx_puts_P(instructions);
}


int
entry_number_thread_P(const char *title, const char *instructions,
                      float *value, uint8_t *accepted, float min, float max,
                      int decimalplaces, int startpos, const char *signs)
{
   static struct pt pt;
   static uint8_t curpos, signlen, intlen, maxpos;

   PT_BEGIN(&pt);

   curpos = startpos;

   /* setup for number drawing */
   signlen = min < 0; /* if min is less than 0, then we have sign */

   float flen = MAX(fabs(max), fabs(min));
   for(intlen = 0; flen >= 1 && intlen < 8; intlen++)
      flen /= 10;

   maxpos = signlen + intlen + decimalplaces;

   static float curvalue;
   curvalue = *value;

restart:
   drawtitleandinstructions_P(title, instructions);

   /* draw selection */
   gfx_setfont(GFX_5x8_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 48, LCD_HEIGHT-1,
		  "     <- *       # ->\n"
		  "   B done       D cancel");

 numchanged:;
   static char intbuf[8], fracbuf[8];
   static uint8_t sign;

   sign = curvalue < 0;

   float shift = pow(10, decimalplaces);
   double round(double);
   float roundvalue = round(fabs(curvalue)*shift)/shift;
   double intpart;
   float fracpart = modf(roundvalue, &intpart);
   char fmt[8];
   snprintf(fmt, sizeof fmt, "%%0%d.0f", intlen);
   snprintf(intbuf, sizeof intbuf, fmt, intpart);
   snprintf(fmt, sizeof fmt, "%%0.0%df", decimalplaces);
   snprintf(fracbuf, sizeof fracbuf, fmt, fracpart);
   /* get rid of "0." in fractbuf */
   memmove(fracbuf, fracbuf+2, 6);

 redraw:
   /* clear space for number */
   gfx_setdrawtype(GFX_WHITE);
   struct gfx_rect numberarea = {0, LCD_WIDTH-1, 29, 47};
   gfx_rectangle(&numberarea, 1);

   char numbuf[16];
   snprintf(numbuf, sizeof numbuf, "%c%s.%s", sign ? '-' : '+', intbuf,
	    fracbuf);

   sscanf(numbuf, "%f", &curvalue);

   if(curvalue < min) {
      curvalue = min;
      goto numchanged;
   }

   if(curvalue > max) {
      curvalue = max;
      goto numchanged;
   }

   /* draw number */
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_BOTTOM);
   int i;
   int po = 0;
   for(i = 0; i < maxpos; i++) {
      if(i - signlen == intlen)
         po = 5;

      const int pitch = 14, centeroff = (LCD_WIDTH - pitch*maxpos)/2;
      struct gfx_rect rect = {centeroff + i*pitch+po, centeroff + (i+1)*pitch+po,
                              numberarea.top, numberarea.bottom};

      gfx_setdrawtype(i == curpos ? GFX_BLACK : GFX_WHITE);
      gfx_rectangle(&rect, 1);

      gfx_setpos(rect.left+1, rect.bottom);
      gfx_setdrawtype(GFX_INVERT);
      if(i == signlen - 1) {
	 if(signs) {
	    gfx_setfont(GFX_R12_FONT);
	    gfx_putc(signs[sign]);
	 }
	 else {
	    if(!sign) {
	       gfx_rectangle_abs(rect.left+6, rect.left+7,
				 rect.top+4, rect.top+8, 0);
	       gfx_rectangle_abs(rect.left+6, rect.left+7,
				 rect.top+11, rect.bottom-3, 0);
	    }
	    gfx_rectangle_abs(rect.left+2, rect.right-3,
			      rect.top+9, rect.top+10, 0);
	 }
      } else {
	 gfx_setfont(GFX_B18_FONT);
	 if(i - signlen < intlen)
	    gfx_putc(intbuf[i - signlen]);
	 else
	    gfx_putc(fracbuf[i - intlen - signlen]);
      }

      if(i - signlen == intlen) {
         gfx_setpos(rect.left - 6, rect.bottom);
         gfx_putc('.'); /* draw the decimal */
      }
   }

   for(;;) {
      int key = key_lastup();
      if(key & KEYS_NUMERIC) {
         if(curpos == signlen - 1) {
            /* toggle sign */
	    sign = !sign;
         } else {
            /* enter current num */
            char keychar = key_todigit(key) + '0';
            if(curpos - signlen < intlen)
               intbuf[curpos - signlen] = keychar;
            else
               fracbuf[curpos - intlen - signlen] = keychar;
         }
         if(curpos < maxpos - 1)
            curpos++;
         goto redraw;
      }

      switch(key) {
      case KEY_STAR:
         if(curpos > 0)
            curpos--;
         else
            curpos = maxpos - 1;
         goto redraw;
      case KEY_POUND:
         if(curpos < maxpos - 1)
            curpos++;
         else
            curpos = 0;
         goto redraw;
      case KEY_B:
         if(accepted)
            *accepted = 1;
         *value = curvalue;
         PT_EXIT(&pt);
      case KEY_D:
         if(accepted)
            *accepted = 0;
         PT_EXIT(&pt);
      }

      MENU_YIELD(&pt);
   }
   PT_END(&pt);
}

int
entry_string_thread_P(const char *title, const char *instructions,
                      char *str, uint8_t *accepted, int len)
{
   static struct pt pt;

   PT_BEGIN(&pt);

   /* setup for string drawing */
   static uint8_t curpos;
   static char buf[16];
   ASSERT(len < sizeof buf);
   curpos = 0;
   memcpy(buf, str, len);

restart:
   drawtitleandinstructions_P(title, instructions);

   /* draw selection */
   gfx_setfont(GFX_5x8_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 48, LCD_HEIGHT-1,
		  "1 next 2 prev  <- *  # ->\n"
		  "B done   D cancel");

 redraw:;
   /* clear space for string */
   gfx_setdrawtype(GFX_WHITE);
   struct gfx_rect numberarea = {0, LCD_WIDTH-1, 29, 47};
   gfx_rectangle(&numberarea, 1);

   /* draw string */
   gfx_setfont(GFX_R12_FONT);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_BOTTOM);
   int i;
   for(i = 0; i < len; i++) {
      const int pitch = 14, centeroff = (LCD_WIDTH - pitch*len)/2;
      struct gfx_rect rect = {centeroff + i*pitch, centeroff + (i+1)*pitch,
                              numberarea.top, numberarea.bottom};

      gfx_setdrawtype(i == curpos ? GFX_BLACK : GFX_WHITE);
      gfx_rectangle(&rect, 1);

      gfx_setpos(rect.left+1, rect.bottom-4);
      gfx_setdrawtype(GFX_INVERT);
      gfx_putc(buf[i]);
   }

   for(;;) {
      MENU_YIELD(&pt);

      uint16_t lastup = key_lastup();

      DO_ENTRY_SCROLL(lastup,
          /* use temp variable because of limited range of buf */
          int newval = buf[curpos]+speed/3;
          if(newval > 126)
             newval = 32;
          buf[curpos] = newval;
                      ,
          if((buf[curpos]-=speed/3) < 32)
             buf[curpos] = 126;
                      ,
          goto redraw;
      )

      switch(lastup) {
      case KEY_STAR:
         if(curpos > 0)
            curpos--;
         else
            curpos = len - 1;
         goto redraw;
      case KEY_POUND:
         if(curpos < len - 1)
            curpos++;
         else
            curpos = 0;
         goto redraw;
      case KEY_B:
         if(accepted)
            *accepted = 1;
         memcpy(str, buf, len);
         PT_EXIT(&pt);
      case KEY_D:
         if(accepted)
            *accepted = 0;
         PT_EXIT(&pt);
      }
   }
   PT_END(&pt);
}
