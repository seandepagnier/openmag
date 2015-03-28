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

#ifndef _HAVE_GFX_H
#define _HAVE_GFX_H

extern char framebuffer[];

int gfx_getpixel(int x, int y);
void gfx_putpixel(int x, int y);

void gfx_line(int x1, int y1, int x2, int y2);
void gfx_circle(int r);

void gfx_ellipse(float a, float b, int r, float Q);
void gfx_ellipse_1(float a, float b, int r);

void gfx_compass(float accel[3], float mag[3], int radius);
void gfx_pendulum(float roll, int radius);

enum gfxDrawType {
   GFX_BLACK, GFX_WHITE, GFX_INVERT
};

void gfx_setdrawtype(enum gfxDrawType type);

struct gfx_rect {
   int left, right, top, bottom;
};

void gfx_rectangle_abs(int left, int right, int top, int bottom, int fill);
void gfx_rectangle(const struct gfx_rect *rect, int fill);
void gfx_rectangle_P(const struct gfx_rect *rect, int fill);

void gfx_setcliprect(struct gfx_rect *rect);

void gfx_center_in_rect(struct gfx_rect *rect);

void gfx_setpos(int x, int y);
void gfx_getpos(int *x, int *y);

enum gfxAlignmentType {GFX_ALIGN_LEFT = 0, GFX_ALIGN_RIGHT = 1,
                       GFX_ALIGN_CENTER = 2, GFX_ALIGN_BOTTOM = 0,
                       GFX_ALIGN_TOP = 16, GFX_ALIGN_MIDDLE = 32};

#define MAKE_TYPICAL_STATE_SET_GET_DEF(TYPE, NAME) \
    void gfx_set##NAME(TYPE NAME); \
    TYPE gfx_get##NAME(void);

#endif
