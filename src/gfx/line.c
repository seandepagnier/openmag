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
#include <gfx/gfx.h>

#define SWAP(X, Y) (X ^= Y, Y ^= X, X ^= Y)
#define DRAW_LINE(X, Y) \
   do { \
       if(Y##2 < Y##1) { \
          SWAP(y1, y2); \
          SWAP(x1, x2); \
       } \
       int dp = 2*d##X-d##Y, i; \
       int x = x1, y = y1; \
       int s = X##1 > X##2 ? -1 : 1; \
       for(i = 0; i <= d##Y; i++) { \
          gfx_putpixel(x, y); \
          if(dp > 0) { \
             X += s; \
             dp -= 2*d##Y; \
          } \
          Y++; \
          dp += 2*d##X; \
       } \
   } while(0)

void gfx_line(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2-x1), dy = abs(y2-y1);

    if(dy > dx)
       DRAW_LINE(x, y);
    else
       DRAW_LINE(y, x);
}
