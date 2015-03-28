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

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <avr-pt.h>

#include <gfx/gfx.h>
#include <gfx/font.h>
#include <gfx/lcd.h>

#include <avrdata.h>

#include <ctype.h>

#include <debug.h>

#include "reader.h"
#include "keys.h"
#include "internal.h"

static int offset, speed;
static int atend;

static int8_t fontsize EEMEM;

static uint8_t attribs;

struct gfx_font *readerfonts[] = {GFX_4x6_FONT,
                                  GFX_5x8_FONT,
                                  GFX_R08_FONT,
                                  GFX_R12_FONT};

#define NUM_FONTS (sizeof readerfonts / sizeof *readerfonts)


#define PREV_STR "A"
#define NEXT_STR "B"
#define SIZE_STR "C size"
#define DONE_STR "D done"

static void
set_font(int size)
{
   size += (int8_t)eeprom_read_byte_safe((uint8_t*)&fontsize) - 2;
   if(size < 0)
      size = 0;
   if(size >= NUM_FONTS-1)
      size = NUM_FONTS-1;

   gfx_setfont(readerfonts[size]);
}

static void
draw_instructions(struct gfx_rect *area)
{
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);

   int x, y;
   gfx_setfont(GFX_4x6_FONT);
   gfx_setpos(area->left, area->bottom - 6);
   GFX_PUTS(PREV_STR);
   x = area->left+10;
   y = area->bottom-5;
   gfx_line(x, y+4, x, y);
   gfx_line(x-2, y+2, x, y);
   gfx_line(x+2, y+2, x, y);

   gfx_setpos(area->left + 18, area->bottom - 6);
   GFX_PUTS(NEXT_STR);
   x = area->left+28;
   y = area->bottom-1;
   gfx_line(x, y-4, x, y);
   gfx_line(x-2, y-2, x, y);
   gfx_line(x+2, y-2, x, y);

   gfx_setpos(area->left + 38, area->bottom - 6); 
   GFX_PUTS(SIZE_STR);
   gfx_setpos(area->left + 70, area->bottom - 6);
   GFX_PUTS(DONE_STR);

   gfx_line(area->left, area->top, area->right, area->top);
}

/* return 1 if a "vowel", otherwise 0 */
static int
isvowel(char c)
{
   switch(c) { case 'a': case 'e': case 'i': case 'o': case 'u': return 1; }
   return 0;
}

/* return 1 if a "consonant", otherwise 0 */
static int
isconsonant(char c)
{
   return isalpha(c) && !isvowel(c);
}

/* return the offset to hyphenate a word.
   return 0 if it should not be hyphenated */
static int
hyphenate_word(char *word, int off)
{
   int len = strlen(word);
   int letters = 0, i;
   for(i = 0; i<len; i++)
      if(isalpha(word[i]))
         letters++;

   if(letters < 5) /* don't hyphenate short words */
      return 0;

   char *c = strchr(word, '-');
   if(c && c - word < off) /* already have a hyphen, use that! */
      return c - word;

   /* work from end looking for vowel-consonant transition */
   int lastconsonant = 0;
   for(i = off-1; i>2; i--) {
      int consonant = isconsonant(word[i]);
      if(lastconsonant && consonant)
         if(len - i > 3)  /* make sure there is a long enough tail */
            return i + 1;
      lastconsonant = consonant;
   }
   return 0;
}

/* t is input for where to start, and output for where to end
   y is the y offset to draw to, and output to the new y offset,
   width is in pixels

   return 0 if we hit the end of the text, 1 otherwise
*/
static int
draw_line(const char **t, int *y, int width)
{
   /* enough space for max on a line smallest font, current word */
   struct {
      char c;
      unsigned char attribs;
      uint8_t w, h;
   } buf[LCD_WIDTH/2 + 1];

   uint8_t w = 0, h = 0;
   int ret = 1;

   /* eat bytes until end of line setting stuff up in buf */
   int hitend = 0;
   int pos, pos2, widthout = 0;
   for(pos=0; pos<(sizeof buf)/(sizeof *buf);) {
      char c = pgm_read_byte(*t);
      (*t)++;

      if(c & CONTROL) {
         attribs = c;
         continue;
      }

      switch(c) {
      case '\0':
         if(widthout)
            goto done;
         ret = 0;
         goto line;
      case '\n': /* break on newline */
         if(widthout)
            goto done;
         if(h < gfx_font_height())
            h = gfx_font_height();
         goto line;
      case ' ':
         if(widthout)
            goto done;
         break;
      }

      /* update width and heights */
      set_font(attribs&SIZE_MASK);
      w += gfx_char_width(c);
      if(!widthout && w >= width)
         widthout = pos;

      if(h < gfx_font_height())
         h = gfx_font_height();

      buf[pos].c = c;
      buf[pos].attribs = attribs;
      buf[pos].w = w;
      buf[pos].h = h;

      pos++;
   }

 done:

   hitend = 1;

   pos2 = pos;
   while((buf[pos2].c == ' ' || buf[pos2].c == '\n') && pos2 >= 0)
      pos2--;
   while(buf[pos2].c != ' ' && buf[pos2].c != '\n' && pos2 >= 0)
      pos2--;

   /* give up if we run out of space */
   if(pos2 < 6)
      pos2 = pos;
   int wordlen = pos - pos2;

   /* copy the word to hyphenate into a buffer */
   char wordbuf[LCD_WIDTH/2 + 1];
   int i, j;
   for(i=pos2+1, j=0; i<pos; i++, j++)
      wordbuf[j] = buf[i].c;
   wordbuf[j] = '\0';

   /* find how far we actually can hyphenate in */
   int p = pos2;
   while(buf[p].w < width)
      p++;

   int hypoff = 0;
   if((buf[0].attribs & ALIGNMENT_MASK) == JUSTIFY)
      hypoff = hyphenate_word(wordbuf, p - pos2);

   if(!hypoff) {
      /* don't hyphenate */
      pos = pos2;
      *t -= wordlen;
   } else {
      pos = pos2 + hypoff + 2;
      *t -= wordlen - hypoff - 1;

      /* insert the hyphen if it doesn't already exist */
      if(buf[pos-1].c != '-') {
         buf[pos-1].c = '-';
         buf[pos-1].w = buf[pos-2].w + gfx_char_width('-');
         (*t)--;
      }
   }

   while(buf[pos-1].c == ' ')
      pos--;

   w = buf[pos-1].w;
   h = buf[pos-1].h;

 line:

   *y += h;
   
   /* align it nice, add 2 to space, one is since the rect bounds
      are 1 off, and the other is because all the fonts have a blank
      column at the end of each character which we don't need on the right
      of the screen */
   int space = width - w + 2, left = 0, spaces = 0;

   switch(buf[0].attribs & ALIGNMENT_MASK) {
   case CENTER:
      left = space/2;
      break;
   case RIGHT:
      left = space;
      break;
   case JUSTIFY:
      for(pos2 = 1; pos2<pos; pos2++)
         if(buf[pos2].c == ' ')
            spaces++;
      break;
   }
   gfx_setpos(left, *y);

   pos2 = 0;

   if(buf[0].c == ' ') {
      space+=buf[0].w;
      pos2 = 1;
   }

   /* now all we have to do is print the pretty bytes */
   for(; pos2<pos; pos2++) {
      /* set the font */
      set_font(buf[pos2].attribs&SIZE_MASK);

      /* draw the character */
      int x, y;
      gfx_getpos(&x, &y);
      gfx_putc(buf[pos2].c);

      /* if justify set and a space, then adjust position */
      if(spaces && hitend && (buf[pos2].attribs&ALIGNMENT_MASK) == JUSTIFY && buf[pos2].c == ' ') {
         int adj = space / spaces;

         int x, y;
         gfx_getpos(&x, &y);
         x+=adj;

         space-=adj;
         spaces--;
         gfx_setpos(x, y);
      }
   }
   return ret;
}

static void
draw_page(struct gfx_rect *area, const char *text)
{
   /* clear out area */
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle(area, 1);

   /* draw the text */
   gfx_setdrawtype(GFX_BLACK);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_BOTTOM);

   /* clip it */
   struct gfx_rect readerarea = {area->left, area->right,
                                 area->top, area->bottom-8};
   gfx_setcliprect(&readerarea);

   const char *t=text;
   int y=-offset;
   attribs = DEFAULT_ATTRIBS;
   while(draw_line(&t, &y, readerarea.right - readerarea.left))
      if(y > readerarea.bottom+8)
         break;

   if(y > readerarea.bottom-8)
      atend = 0;
   else
      atend = 1;

   gfx_setcliprect(NULL);
   
   /* draw instructions */
   struct gfx_rect instrect = {area->left, area->right,
                               area->bottom-7, area->bottom};
   draw_instructions(&instrect);
}

int
reader_thread(const char *text)
{
   struct gfx_rect area = {0, LCD_WIDTH-1, 0, LCD_HEIGHT-1};
   const int maxspeed = 50;

   static struct pt pt;
   PT_BEGIN(&pt);

   /* set offset to top */
   offset = 0;

restart:
   /* main loop: draw pages, and scroll with acceleration */
   for(;;) {
      draw_page(&area, text);
   again:
      MENU_YIELD(&pt);

      int lastup = key_lastup();
      if(keys & KEY_A || lastup == KEY_A) {
         if(speed > 0)
            speed = 0;
         if(speed > -maxspeed)
            speed -= 2;
      } else if(keys & KEY_B || lastup == KEY_B) {
         if(speed < 0)
            speed = 0;
         if(speed < maxspeed)
            speed += 2;
      } else {
         speed /= 2;

         switch(lastup) {
         case 0: /* no key */
            goto again;
         case KEY_C:
         {
             int8_t size = (int8_t)eeprom_read_byte_safe((uint8_t*)&fontsize);
             if(size < 1)
                 size++;
             else
                 size = -1;
             eeprom_write_byte_safe((uint8_t*)&fontsize, size);
            offset = 0;
         } break;
         case KEY_D:
            PT_EXIT(&pt);
         }
      }

      if(speed > 0) {
         if(atend)
            speed = 0;
      } else if(speed < 0) {
         if(speed + offset < 0)
            speed = -offset;
      }
      offset += speed;
   }

   PT_END(&pt);
}
