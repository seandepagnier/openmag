/* Copyright (C) 2009 Sean D'Epagnier <sean@depagnier.com>
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

/* this file contains routines to render 3d graphics */

#include <stdint.h>
#include <string.h>
#include <math.h>

#include <linalg/quaternion.h>
#include <linalg/vector.h>
#include <linalg/rotate.h>

#include <gfx/gfx.h>
#include <gfx/font.h>
#include <gfx/lcd.h>

static float translation[3];
static float rotation[4];

static int gfx3d_screenx, gfx3d_screeny;

void gfx3d_screenpos(int x, int y)
{
   gfx3d_screenx = x;
   gfx3d_screeny = y;
}

void gfx3d_rotate(float angle, float x, float y, float z)
{
   float q[4], v[3] = {x, y, z};
   angvec2quat(q, deg2rad(angle), v);
   MultQuat2(q, rotation);
   quatnormalize(rotation);
}

void gfx3d_rotate_after(float angle, float x, float y, float z)
{
   float q[4], v[3] = {x, y, z};
   angvec2quat(q, deg2rad(angle), v);
   MultQuat1(rotation, q);
   quatnormalize(rotation);
}

void gfx3d_reset_rotation(void)
{
   rotation[0] = 1;
   rotation[1] = rotation[2] = rotation[3] = 0;
}

void gfx3d_reset_translation(void)
{
   translation[0] = translation[1] = translation[2] = 0;
}

void gfx3d_translate(float x, float y, float z)
{
   float vec[3] = {x, y, z};
   vector_add(translation, translation, vec, 3);
}

void gfx3d_translate_after(float x, float y, float z)
{
   float vec[3] = {x, y, z};
   float q[4] = {rotation[0], -rotation[1], -rotation[2], -rotation[3]};
   rotvecquat(vec, q);
   vec[2] = z;
   vector_add(translation, translation, vec, 3);
}

void gfx3d_bound_translation(float minx, float miny, float minz,
			     float maxx, float maxy, float maxz)
{
   float min[3] = {minx, miny, minz};
   float max[3] = {maxx, maxy, maxz};
   int i;
   for(i=0; i<3; i++)
      if(translation[i] < min[i])
	 translation[i] = min[i];
      else if(translation[i] > max[i])
	 translation[i] = max[i];
}

/* take a point in 3d coordinates and give its location on screen */
static int
gfx3d_project(float x[2], float p[3])
{
   float y[3] = {p[0], p[1], p[2]};

   /* translate x and y */
   y[0] += translation[0];
   y[1] += translation[1];

   /* rotate */
   rotvecquat(y, rotation);

   /* translate z */
   y[2] += translation[2];

   if(y[2] <= 0)
      return 0;

   /* project */
   x[0] = y[0] / y[2];
   x[1] = y[1] / y[2];
   return 1;
}

/* take a 2d position and convert to screen coordinates for drawing */
static int16_t screencoordx(float pos)
{
   return pos * LCD_WIDTH/2 + gfx3d_screenx;
}

static int16_t screencoordy(float pos)
{
   return pos * LCD_WIDTH/2 + gfx3d_screeny;
}

void gfx3d_setpos(float pos[3])
{
   float x[2];
   if(gfx3d_project(x, pos))
      gfx_setpos(screencoordx(x[0]), screencoordy(-x[1]));
   else
      gfx_setpos(1000, 1000); /* offscreen */
}

/* helper to draw lines when 2d coordinates are known */
static void
draw_line(float pa[2], float pb[2])
{
   gfx_line(screencoordx(pa[0]), screencoordy(-pa[1]),
	    screencoordx(pb[0]), screencoordy(-pb[1]));
}

static void
great_arc_1(float a[3], float b[3], float pa[2], float pb[2], uint8_t depth)
{
   if(depth > 3) {
      draw_line(pa, pb);
      return;
   }

   float c[3];
   vector_add(c, a, b, 3);
   normalize(c);

   float pc[2];
   if(gfx3d_project(pc, c)) {
      float v1[2], v2[2];
      vector_sub(v1, pc, pa, 2);
      vector_sub(v2, pc, pb, 2);
      if(fabs(v1[0]*v2[0] + v1[1]*v2[1])
	 /(vector_length(v1, 2)*vector_length(v2, 2)) < .95) {
	 great_arc_1(a, c, pa, pc, depth+1);
	 great_arc_1(c, b, pc, pb, depth+1);
	 return;
      }
   }

   draw_line(pa, pb);
}

/* draw lines along a draw_great arc from spherical coordinates a to b
   this function projects points onto the screen subdividing
   as needed at highly curved areas. */
void
gfx3d_great_arc(float a[3], float b[3])
{
   float pa[2], pb[2];
   if(gfx3d_project(pa, a) && gfx3d_project(pb, b))
      great_arc_1(a, b, pa, pb, 0);
}

void gfx3d_line(float a[3], float b[3])
{
   float pa[2], pb[2];
   if(gfx3d_project(pa, a) && gfx3d_project(pb, b))
      draw_line(pa, pb);
}

void gfx3d_point(float a[3])
{
   float pa[2];
   if(gfx3d_project(pa, a))
      gfx_putpixel(screencoordx(pa[0]), screencoordy(-pa[1]));
}

static void
line_with_character(float x[3], char c)
{
   int xc, yc;
   gfx_getpos(&xc, &yc);
   gfx_line(xc, yc, x[0] + xc, x[1] + yc);
   gfx_setpos(1.5*x[0] + xc, 1.5*x[1] + yc);
   gfx_putc(c);
   gfx_setpos(xc, yc);
}

void gfx3d_draw_axes(void)
{
   float len = 7;
   float x1[3] = {len, 0, 0}, y1[3] = {0, len, 0}, z1[3] = {0, 0, len};
   rotvecquat(x1, rotation);
   rotvecquat(y1, rotation);
   rotvecquat(z1, rotation);

   line_with_character(x1, 'x');
   line_with_character(y1, 'y');
   line_with_character(z1, 'z');
}
