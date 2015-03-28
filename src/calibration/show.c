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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <linalg/rotate.h>
#include <linalg/quaternion.h>

#include <avrdata.h>

#include <data/data.h>

void calibration_show_sphere(int (*showfunc)(const char *, ...),
			     const char *prefix, float X[3], float mag)
{
   showfunc(PSTR(PGM_STR_FMT"bias: (%.5g %.5g %.5g)\n"
		 PGM_STR_FMT"magnitude: %.5g\n"),
	    prefix, X[0], X[1], X[2],
	    prefix, mag);
}

void calibration_show_rotatedellipsoid(int (*showfunc)(const char *, ...),
				       const char *prefix, float X[9])
{
   calibration_show_sphere(showfunc, prefix, X, 1/X[3]);
   showfunc(PSTR(PGM_STR_FMT"magnitude ratios: [%.3f %.3f]\n"
		 PGM_STR_FMT"cross coupling: {%.4g %.4g %.4g}\n"),
	    prefix, X[4], X[5],
	    prefix, X[6], X[7], X[8]);
}

void calibration_show_rotatedellipsoid3rdorder(int (*showfunc)(const char *, ...),
					       const char *prefix, float X[15])
{
   calibration_show_rotatedellipsoid(showfunc, prefix, X);
   showfunc(PSTR(PGM_STR_FMT"second order terms: <%.5g %.5g %.5g>\n"
		 PGM_STR_FMT"third order terms: <%.5g %.5g %.5g>\n"),
	      prefix, X[9], X[11], X[13],
	      prefix, X[10], X[12], X[14]);
}

void calibration_show_align(int (*showfunc)(const char *, ...),
			    const char *prefix, float X[4])
{
    showfunc(PSTR(PGM_STR_FMT"rotation: <%.5g %.5g %.5g> %.3g degrees\n"
		  PGM_STR_FMT"dip angle: %.5g\n"),
		prefix, X[0], X[1], X[2], rad2deg(subquatangle(X)),
                prefix, rad2deg(asin(X[3])));
}

void calibration_show_transformation(int (*showfunc)(const char *, ...),
				     const char *prefix, float X[13])
{
   calibration_show_sphere(showfunc, prefix, X, 1/X[3]);
   showfunc(PSTR(PGM_STR_FMT"matrix: [%.5g %.5g %.5g]\n"
		 PGM_STR_FMT"        [%.5g %.5g %.5g]\n"
		 PGM_STR_FMT"        [%.5g %.5g %.5g]\n"
	         PGM_STR_FMT"dip angle: %.5g\n"),
	    prefix, 1.0,    X[6], X[9],
	    prefix, X[4], X[7], X[10],
	    prefix, X[5], X[8], X[11],
	    prefix, rad2deg(asin(X[12])));
}
