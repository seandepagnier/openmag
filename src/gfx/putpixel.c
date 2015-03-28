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

#include "gfx.h"
#include "lcd.h"

#include "internal.h"

int gfx_getpixel(int x, int y)
{
    if(x < 0 || x >= LCD_WIDTH || y < 0 || y >= LCD_HEIGHT)
        return 0;

    unsigned int bit = 1<<(7^(x&7));
    int addr = y*17+(x>>3);

    return !!(framebuffer[addr] & bit);
}

void gfx_putpixel(int x, int y)
{
    if(x < 0 || x >= LCD_WIDTH
       || y < 0 || y >= LCD_HEIGHT)
        return;

    if(clip_rect) {
       /* test clipping */
       if(x < clip_rect->left)
          return;
       if(x > clip_rect->right)
          return;
       if(y < clip_rect->top)
          return;
       if(y > clip_rect->bottom)
          return;
    }

    int val;
    switch(drawtype) {
    case GFX_BLACK:
        val = 1;
        break;
    case GFX_WHITE:
        val = 0;
        break;
    case GFX_INVERT:
       val = !gfx_getpixel(x, y);
        break;
    }

    unsigned int bit = 1<<(7^(x&7));
    int addr = y*LCD_SPAN+(x>>3);

    if(val)
        framebuffer[addr] |= bit;
    else
        framebuffer[addr] &= ~bit;
}

void gfx_setdrawtype(enum gfxDrawType type)
{
   drawtype = type;
}
