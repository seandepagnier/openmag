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

#include <string.h>

#include <avrdata.h>

#include "gfx.h"
#include "font.h"
#include "lcd.h"
#include "internal.h"

void gfx_rectangle_abs(int left, int right, int top, int bottom, int fill)
{
   struct gfx_rect r = {left, right, top, bottom};
   gfx_rectangle(&r, fill);
}

static void cliprect(struct gfx_rect *rect, const struct gfx_rect *clip)
{
    if(rect->left < clip->left)  rect->left = clip->left;
    if(rect->right > clip->right)  rect->right = clip->right;
    if(rect->top < clip->top)  rect->top = clip->top;
    if(rect->bottom > clip->bottom)  rect->bottom = clip->bottom;
}

void gfx_rectangle(const struct gfx_rect *rect, int fill)
{
   int i, j;
   if(fill) {
#if 0
       /* slow simple version */
       for(i = rect->left; i<=rect->right; i++)
          for(j = rect->top; j<=rect->bottom; j++)
             gfx_putpixel(i, j);
#else
       /* Don't sit on top of putpixel for filling rectangles,
          it really slows things down. */

       /* clip the rectangle */
       struct gfx_rect screen = {0, LCD_WIDTH-1, 0, LCD_HEIGHT-1};
       struct gfx_rect crect = *rect;
       cliprect(&crect, &screen);
       if(clip_rect)
           cliprect(&crect, clip_rect);

       i = crect.left;
       while(i<=crect.right) {
           uint8_t mask = 0xff;
           uint8_t col = i>>3;

           /* mask off left part */
           mask >>= i&7;

           /* mask off right part */
           int diff = 7 - crect.right + (col<<3);
           if(diff > 0)
               mask &= 0xff<<diff;

           /* move to next row */
           i = (col+1)<<3;

           /* apply mask over this row */
           switch(drawtype) {
           case GFX_BLACK:
              for(j = crect.top; j<=crect.bottom; j++)
                 framebuffer[j*LCD_SPAN + col] |= mask;
              break;
           case GFX_WHITE:
              for(j = crect.top; j<=crect.bottom; j++)
                 framebuffer[j*LCD_SPAN + col] &= ~mask;
              break;
           case GFX_INVERT:
              for(j = crect.top; j<=crect.bottom; j++)
                 framebuffer[j*LCD_SPAN + col] ^= mask;
              break;
           }
       }
#endif
   } else {
      for(i = rect->left; i<=rect->right; i++) {
         gfx_putpixel(i, rect->top);
         gfx_putpixel(i, rect->bottom);
      }
      for(i = rect->top+1; i<rect->bottom; i++) {
         gfx_putpixel(rect->left, i);
         gfx_putpixel(rect->right, i);
      }
   }
}

void gfx_rectangle_P(const struct gfx_rect *rect, int fill)
{
   struct gfx_rect r;
   memcpy_P(&r, rect, sizeof r);
   gfx_rectangle(&r, fill);
}

void gfx_center_in_rect(struct gfx_rect *rect)
{
   gfx_setalignment(GFX_ALIGN_CENTER | GFX_ALIGN_MIDDLE);
   gfx_setpos((rect->left+rect->right)/2, (rect->top+rect->bottom)/2);
}
