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

#include "menu.h"
#include "keys.h"
#include "internal.h"

void
draw_selection(const struct gfx_rect *a, const char *t)
{
   struct gfx_rect area;
   memcpy_P(&area, a, sizeof area);

   char text[256];
   strncpy_P(text, t, sizeof text);

   /* the font set when we are called is what we use */
   ASSERT(gfx_getfont());

   /* figure out how to draw stuff */
   int w, h;
   gfx_str_width_height(text, &w, &h);

   int width = area.right - area.left;
   int height = area.bottom - area.top;

   int lineheight = gfx_font_height();

   menu_preparearea(&area);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);

   if(!w || !h)
      return;

   int cols = width / w;
   int rows = height / lineheight;

   if(rows == 0)
      rows = 1;
   if(cols == 0)
      cols = 1;

   int linewidth = width / cols;
   int i, j;
   char *p = text;
   for(i = 0; i<cols; i++)
      for(j = 0; j<rows; j++) {
         gfx_setpos(linewidth*i + area.left, lineheight*j + area.top);
         char *c = strchr(p, '\n');
         if(c) {
            *c = '\0';
            gfx_puts(p);
            p = c+1;
         } else {
            gfx_puts(p);
            return;
         }
      }
}

void nice_num_P(const char *str, float val)
{
   int curx, cury;
   gfx_getpos(&curx, &cury);
   if(val < 0)
      GFX_PUTS("-");
   else
      GFX_PUTS("+");
   gfx_setpos(curx + gfx_char_width('+'), cury);
   gfx_printf_P(str, fabs(val));
}
