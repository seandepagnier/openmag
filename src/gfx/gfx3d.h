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

void gfx3d_reset_rotation(void);
void gfx3d_rotate(float angle, float x, float y, float z);
void gfx3d_rotate_after(float angle, float x, float y, float z);

void gfx3d_reset_translation(void);
void gfx3d_translate(float x, float y, float z);
void gfx3d_translate_after(float x, float y, float z);
void gfx3d_bound_translation(float minx, float miny, float minz,
			     float maxx, float maxy, float maxz);

void gfx3d_setpos(float pos[3]);
void gfx3d_screenpos(int x, int y);

void gfx3d_great_arc(float a[3], float b[3]);
void gfx3d_draw_axes(void);
void gfx3d_line(float a[3], float b[3]);
void gfx3d_point(float a[3]);
