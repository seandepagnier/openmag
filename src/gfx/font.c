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
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include <avrdata.h>

#include <gfx/gfx.h>
#include <gfx/font.h>

#include "internal.h"

#if defined(__AVR_AT90USB1287__)
#include "at90usb1287.fonts.h"
#else
#include "default.fonts.h"
#endif

#include <debug.h>

static struct gfx_font curfont;
static struct gfx_font *curfontptr;

void gfx_setfont(struct gfx_font *font)
{
   if(font && font != curfontptr)
      memcpy_P(&curfont, font, sizeof curfont);
   curfontptr = font;
}

struct gfx_font *gfx_getfont(void)
{
   return curfontptr;
}

int gfx_font_height(void)
{
   /* if big use the height of '0' as font height times 5/4 */
   if(curfont.height > 10) {
        struct gfx_character d;
        memcpy_P(&d, curfont.characters['0'-32], sizeof d);
        return d.height * 5/4;
   }

   /* hack for 5x8 font to space closer */
   if(curfont.height == 8)
      return 7;

   return curfont.height;
}

static void draw_character_P(const struct gfx_character *c)
{
   struct gfx_character d;
   memcpy_P(&d, c, sizeof d);

   int i, j;
   int w = BYTESW(d.width);

   char data[d.height*w];
   memcpy_P(data, &c->data, sizeof data);

   for(i = 0; i<d.height; i++) {
      uint8_t m = 0x80;
      uint8_t p = 0;

      for(j = 0; j<d.width; j++) {
         if(data[i*w + p] & m)
            gfx_putpixel(curx + d.xoff + j, cury - d.height - d.yoff + i);

         m >>= 1;
         if(!m) {
            m = 0x80;
            p++;
         }
      }
   }
   curx += d.dwidth;
}

int
gfx_char_width(char c)
{
   if(!curfont.characters[(int)c-32])
      return 0;
   struct gfx_character d;
   memcpy_P(&d, curfont.characters[(int)c-32], sizeof d);
   return d.dwidth;
}

void
gfx_str_width_height(const char *str, int *wlen, int *hlen)
{
   int height = gfx_font_height();

   *wlen = 0;
   *hlen = height;

   if(!curfontptr)
      return;
   
   const char *c = str;
   int len = 0;
   while(*c) {
      if(*c == '\n') {
         *hlen += height;
         if(len > *wlen)
            *wlen = len;
         len = 0;
      } else
      if(*c > 0)
         len += gfx_char_width(*c);
      c++;
   }
   if(len > *wlen)
      *wlen = len;
}

void
gfx_str_width_height_P(const char *str, int *wlen, int *hlen)
{
   char buf[256];
   strncpy_P(buf, str, sizeof buf);
   gfx_str_width_height(buf, wlen, hlen);
}

int gfx_putc(char c)
{
   if(!curfont.characters[(int)c-32])
      return EOF;
   draw_character_P(curfont.characters[(int)c-32]);
   return c;
}

int gfx_puts(const char *str)
{
   int height = gfx_font_height();
   int wlen, hlen;
   gfx_str_width_height(str, &wlen, &hlen);

   int diffx = 0;
   if(gfx_getalignment() & GFX_ALIGN_CENTER)
      diffx = wlen/2;
   if(gfx_getalignment() & GFX_ALIGN_RIGHT)
      diffx = wlen;
   curx -= diffx;

   int diffy = 0;
   if(gfx_getalignment() & GFX_ALIGN_MIDDLE)
      diffy = height/2;
   if(gfx_getalignment() & GFX_ALIGN_TOP)
      diffy = height;
   cury += diffy;

   if(!curfontptr)
      return EOF;

   int x0 = curx;

   const char *c = str;
   while(*c) {
      if(*c == '\n') {
         curx = x0;
         cury += height;
      } else
      if(*c > 0)
         gfx_putc(*c);
      c++;
   }

   curx += diffx;
   cury -= diffy;

   return strlen(str);
}

int gfx_puts_P(const char *str)
{
   char buf[256];
   strncpy_P(buf, str, sizeof buf);
   return gfx_puts(buf);
}

int gfx_printf(const char *fmt, ...)
{
   int ret;
   va_list ap;
   va_start(ap, fmt);
   char buf[256];
   ret = vsnprintf(buf, sizeof buf, fmt, ap);
   va_end(ap);
   gfx_puts(buf);
   return ret;
}

int gfx_printf_P(const char *fmt, ...)
{
   int ret;
   va_list ap;
   va_start(ap, fmt);
   char buf[256];
   ret = vsnprintf_P(buf, sizeof buf, fmt, ap);
   va_end(ap);
   gfx_puts(buf);
   return ret;
}

/* set the max font size that can fit str in area */
static void
gfx_setmaxfont_internal(const char *str, struct gfx_rect *area,
                        void (*whfunc)(const char *, int*, int*))
{
   struct gfx_font *fonts[] = {GFX_4x6_FONT,
                               GFX_5x8_FONT,
                               GFX_R08_FONT,
                               GFX_R12_FONT};
   int f = -1;
   int width = area->right - area->left, height = area->bottom - area->top;
   int w = 0, h = 0;
   while(w < width && h < height) {
      f++;
      if(f >= (sizeof fonts) / (sizeof *fonts))
          break;
      gfx_setfont(fonts[f]);
      whfunc(str, &w, &h);
   }
   f--;

   gfx_setfont(f < 0 ? 0 : fonts[f]);
}

void gfx_setmaxfont(const char *str, struct gfx_rect *area)
{
   gfx_setmaxfont_internal(str, area, gfx_str_width_height);
}

void gfx_setmaxfont_P(const char *str, struct gfx_rect *area)
{
   gfx_setmaxfont_internal(str, area, gfx_str_width_height_P);
}
