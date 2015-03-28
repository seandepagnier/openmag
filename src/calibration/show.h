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

void calibration_show_sphere(int (*showfunc)(const char *, ...), const char *prefix, float X[3], float mag);
void calibration_show_rotatedellipsoid(int (*showfunc)(const char *, ...), const char *prefix, float X[9]);
void calibration_show_rotatedellipsoid3rdorder(int (*showfunc)(const char *, ...), const char *prefix, float X[15]);
void calibration_show_align(int (*showfunc)(const char *, ...), const char *prefix, float X[4]);
void calibration_show_transformation(int (*showfunc)(const char *, ...), const char *prefix, float X[13]);
