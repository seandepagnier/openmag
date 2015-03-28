/* Copyright (C) 2007, 2008 Sean D'Epagnier <sean@depagnier.com>
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

#include <stdint.h>
#include <math.h>

#include <linalg/vector.h>
#include <linalg/quaternion.h>
#include <linalg/rotate.h>

#include <avrdata.h>

#include <string.h>

#include "gfx.h"
#include "lcd.h"
#include "font.h"
#include "internal.h"

static void scale(int p[2], int n, int d)
{
   p[0]*=n, p[0]/=d;
   p[1]*=n, p[1]/=d;
} 

static void linev(int a[2], int b[2])
{
   gfx_line(a[0]+curx, a[1]+cury, b[0]+curx, b[1]+cury);
}

static void setposoff(int x, int y, int p[2])
{
   gfx_setpos(x+p[0], y+p[1]);
}

void gfx_compass(float accel[3], float mag[3], int radius)
{
   /* draw ring */
   float a[3] = {accel[0], accel[1], accel[2]};
   normalize(a);
   gfx_ellipse(a[1]/a[2], a[0]/a[2], radius, LCD_RATIO);

   /* draw cardinal directions */
   float m[3] = {mag[0], mag[1], mag[2]};
   normalize(m);

   /* project magnetic vector onto accel plane */
   float d = dot(a, m);
   float proj1[3] = {m[0] - d*a[0], m[1] - d*a[1], m[2] - d*a[2]};
   normalize(proj1);

   float proj2[3] = {proj1[0], proj1[1], proj1[2]};
   float q[4];
   angvec2quat(q, M_PI/4, a);
   rotvecquat(proj2, q);
   float proj3[3] = {proj2[0], proj2[1], proj2[2]};
   rotvecquat(proj3, q);
   float proj4[3] = {proj3[0], proj3[1], proj3[2]};
   rotvecquat(proj4, q);

   int r2 = radius*.8;
   int n[2] = {-r2*proj1[1]/LCD_RATIO, -r2*proj1[0]};
   int s[2] = { r2*proj1[1]/LCD_RATIO,  r2*proj1[0]};
   int e[2] = { r2*proj3[1]/LCD_RATIO,  r2*proj3[0]};
   int w[2] = {-r2*proj3[1]/LCD_RATIO, -r2*proj3[0]};

   gfx_setfont(GFX_R12_FONT);

   gfx_setalignment(GFX_ALIGN_CENTER | GFX_ALIGN_MIDDLE);

   int x0, y0;
   gfx_getpos(&x0, &y0);

   setposoff(x0, y0, n);
   GFX_PUTS("N");
   setposoff(x0, y0, s);
   GFX_PUTS("S");
   setposoff(x0, y0, e);
   GFX_PUTS("E");
   setposoff(x0, y0, w);
   GFX_PUTS("W");

   gfx_setpos(x0, y0);

   int r3 = r2*.6;
   if(r2) {
      scale(n, r3, r2);
      scale(s, r3, r2);
      scale(e, r3, r2);
      scale(w, r3, r2);
   }

   linev(n, s);
   linev(e, w);
}
