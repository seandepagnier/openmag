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

#include <linalg/vector.h>

#include <avr-pt.h>

#include <debug.h>

#include <avrdata.h>

#include <data/data.h>

#include <avrdrivers/buzzer.h>

#include <calibration/sensor.h>
#include <calibration/calc.h>

#include "menu.h"
#include "keys.h"

#include "internal.h"
#include "readertext.h"

static float gausszero = 0;
static struct gfx_rect gaussarea = {38, LCD_WIDTH-1, 10, LCD_HEIGHT-1};

static void draw_num(float num, int showsign)
{
   int curx, cury;
   gfx_getpos(&curx, &cury);

   if(showsign) {
      gfx_line(curx-1, cury+11, curx+6, cury+11);
      gfx_line(curx-1, cury+12, curx+6, cury+12);
      if(num >= 0) {
         gfx_line(curx+2, cury+7, curx+2, cury+16);
         gfx_line(curx+3, cury+7, curx+3, cury+16);
      }
   }

   gfx_setpos(curx + 8, cury);
   GFX_PRINTF("%01d", (int)fabs(num));
   gfx_setpos(curx + 17, cury);
   GFX_PUTS(".");
   gfx_setpos(curx + 23, cury);
   GFX_PRINTF("%05ld", (long)(100000*fmod(fabs(num), 1)));
}

void draw_gaussmeter(float m[3])
{
   float absolutemag = magnitude(m);
   float relativemag = absolutemag - gausszero;

   const int x = gaussarea.left - 1, y = 16;

   /* clear area */
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle(&gaussarea, 1);

   /* display gauss */
   gfx_setdrawtype(GFX_BLACK);
   gfx_setfont(GFX_B18_FONT);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
   gfx_setpos(x, y);
   draw_num(absolutemag, 0);
   gfx_setpos(x, y+20);
   draw_num(relativemag, 1);

   const int end = 84;
   /* draw B stuff */
   gfx_setfont(GFX_B12_FONT);
   gfx_setpos(x+end, y-3);
   GFX_PUTS("^");
   gfx_setpos(x+end, y+6);
   GFX_PUTS("B");
   gfx_setpos(x+end, y+16);
   GFX_PUTS("~");
   gfx_setpos(x+end, y+26);
   GFX_PUTS("B");
}

static void draw_vectordisplay(float m[3])
{
   const int x = gaussarea.left+2, y=7;
   /* clear area */
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle(&gaussarea, 1);

   /* draw text */
   gfx_setdrawtype(GFX_BLACK);
   gfx_setfont(GFX_B18_FONT);
   gfx_setpos(x, y);

   draw_num(m[0], 1);
   gfx_setpos(x, y+18);
   draw_num(m[1], 1);
   gfx_setpos(x, y+36);
   draw_num(m[2], 1);

   gfx_setfont(GFX_B12_FONT);
   const int end = 82;
   const int l = 4;
   gfx_setpos(x+end, y+l);
   GFX_PUTS("X");
   gfx_setpos(x+end, y+l+18);
   GFX_PUTS("Y");
   gfx_setpos(x+end+1, y+l+36);
   GFX_PUTS("Z");
}

int
gaussmeter_thread(void)
{
   static struct pt pt;
   static uint8_t curpage, hold;
   const int numpages = 2;

   PT_BEGIN(&pt);
   hold = 0;
 restart:

   /* draw title */
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 9, "Gaussmeter");

   /* draw selection */
   static const struct gfx_rect selectionarea PROGMEM = {0, 41, 10, LCD_HEIGHT-1};
   gfx_setfont(GFX_R08_FONT);
   switch(curpage) {
   case 0: DRAW_SELECTION_A(&selectionarea, "1 page\n2 hold \n3 zero\nA about\nD menu"); break;
   case 1: DRAW_SELECTION_A(&selectionarea, "1 page\n2 hold\n\nA about\nD menu"); break;
   }

   for(;;) {
      static float gausszerotime = 0;

      float m[3];
      sensor_get_calibrated(&mag, m);

      if(!hold) {
         /* display the current gauss */
         switch(curpage) {
         case 0: draw_gaussmeter(m); break;
         case 1: draw_vectordisplay(m); break;
         }
      }

      /* zero if requested */
      if(gausszerotime && gettime() - gausszerotime > .5) {
         gausszero = magnitude(m);
         gausszerotime = 0;
	 buzzer_buzz(200);
      }

      switch(key_lastup()) {
      case KEY_1: /* page */
         hold = 0;
         curpage++;
         if(curpage == numpages)
            curpage = 0;
         goto restart;
      case KEY_2: /* hold */
         if((hold = !hold))
            gfx_rectangle_abs(7, 29, 20, 31, 0);
         else
            goto restart;
         break;
      case KEY_3: /* zero later */
         gausszerotime = gettime();
         break;
      case KEY_A: /* about */
      {
         PT_WAIT_THREAD(&pt, reader_thread(about_gaussmeter));
         goto restart;
      }
      case KEY_D: /* menu */
         PT_EXIT(&pt);
      }         

      MENU_YIELD(&pt);
   }
   
   PT_END(&pt);
}
