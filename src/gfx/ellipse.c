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
#include <stdio.h>
#include <math.h>

#include <gfx/gfx.h>

#include "internal.h"

/* in curx, cury, x1, y1, r form:
   (curx - x)^2 + (cury - y)^2 + (x1 - x)^2 + (y1 - y)^2 = r^2

   x = -(sqrt(-2*y1^2+4*y*y1-2*cury^2+4*y*cury-4*y^2-x1^2+2*curx*x1-curx^2+2*r^2)-x1-curx) / 2
   x =  (sqrt(-2*y1^2+4*y*y1-2*cury^2+4*y*cury-4*y^2-x1^2+2*curx*x1-curx^2+2*r^2)+x1+curx) / 2

   for circle
   (curx - x)^2 + (cury - y)^2 = r
*/

void gfx_ellipse_1(float a, float b, int r)
{
   /*
     x^2 + y^2 + (a*x + b*y)^2 = r^2
     x^2 + y^2 + a^2*x^2 + b^2*y^2 + 2*a*b*x*y - r^2 = 0

     2*x + a^2*2*x + 2*a*b*y
     2*y + b^2*2*y + 2*a*b*x
   */

   float x, y;
   for(x = -r; x<=r; x++)
      for(y = -r; y<=r; y++) {
         float f = x*x + y*y + a*a*x*x + b*b*y*y + 2*a*b*x*y - r*r;
         if(fabs(f) < sqrt(x*x + y*y))
            gfx_putpixel(curx + (int)x, cury + (int)y);
      }
}

static void comp(float *yp1, float *xp1,
                 float A, float B, float E, float R, float P)
{
   *yp1 = -sqrt(-4*P*R) / (2*P);
   *xp1 = (sqrt(fabs(-R+*yp1**yp1*E*E-4**yp1**yp1*A*B))-*yp1*E) / (2*A);
}

void gfx_ellipse(float a, float b, int r, float Q)
{
   /*
     Q^2*x^2 + y^2 + (a*x + b*y)^2 = r^2
     Q^2*x^2 + y^2 + a^2*x^2 + b^2*y^2 + 2*a*b*x*y - r^2 = 0

     A*x^2 + B*y^2 + C*x + D*y + E*x*y + F = 0

     A = Q^2 + a^2
     B = 1 + b^2
     C = 0
     D = 0
     E = 2*a*b
     F = -r^2
   */

   if(isnan(a))
      return;

   const float fac = 10;

   float A = (Q*Q + a*a) * fac;
   float B = (1 + b*b) * fac;
   float E = (2*a*b) * fac;
   float F = (-r*r) * fac;

   float P = 4*A*B - E*E;
   float R = 4*A*F;
   float S = 4*B*F;

   float yp1, xp1;
   comp(&yp1, &xp1, A, B, E, R, P);

   float xp2, yp2;
   comp(&xp2, &yp2, B, A, E, S, P);

   if(isnan(xp1))
      return;

   int16_t d1 = A*xp1*xp1 + B*yp1*yp1 + E*xp1*yp1 + F;
   int16_t dx1 = 2*A*xp1 + E*yp1;
   int16_t dy1 = 2*B*yp1 + E*xp1;

   int16_t ddxx = 2*A;
   int16_t ddyy = 2*B;
   int16_t ddxy = E;
   int16_t ddyx = E;

   int16_t d2 = A*xp2*xp2 + B*yp2*yp2 + E*xp2*yp2 + F;
   int16_t dx2 = 2*A*xp2 + E*yp2;
   int16_t dy2 = 2*B*yp2 + E*xp2;

   int16_t xp, yp;
   int16_t d, dx, dy;

#define incx xp++, d += dx, dx += ddxx, dy += ddxy
#define incy yp++, d += dy, dx += ddyx, dy += ddyy
#define decx xp--, dx -= ddxx, dy -= ddxy, d -= dx
#define decy yp--, dx -= ddyx, dy -= ddyy, d -= dy

   d = d1;
   yp = yp1;
   xp = xp1;
   dx = dx1;
   dy = dy1;

   while ( dx < -dy && yp < -yp1)
   {
      gfx_putpixel( curx + xp, cury + yp);
      gfx_putpixel( curx - xp, cury - yp);

      incx;
      if ( d > 0 )
         incy;
   }

   d = d2;
   yp = yp2;
   xp = xp2;
   dx = dx2;
   dy = dy2;

   while ( dx < dy)
   {
      gfx_putpixel( curx + xp, cury + yp);
      gfx_putpixel( curx - xp, cury - yp);

      decy;
      if ( d > 0 )
         incx;
   }

   d = d1;
   yp = -yp1;
   xp = -xp1;
   dx = dx1;
   dy = -dy1;

   while ( dy > dx && yp > yp1 )
   {
      gfx_putpixel( curx + xp, cury + yp);
      gfx_putpixel( curx - xp, cury - yp);

      incx;
      if ( d > 0 )
         decy;
   }

   d = d2;
   yp = -yp2;
   xp = -xp2;
   dx = -dx2;
   dy = dy2;

   while ( dy > -dx)
   {
      gfx_putpixel( curx + xp, cury + yp);
      gfx_putpixel( curx - xp, cury - yp);

      decy;
      if ( d > 0 )
         decx;
   }
}
