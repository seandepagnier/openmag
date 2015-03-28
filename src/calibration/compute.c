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

#include <math.h>

#include <linalg/rotate.h>

float compute_resolveheading(float heading)
{
   while(heading < 0)
      heading += 360;
   while(heading >= 360)
      heading -= 360;
   return heading;
}

float compute_pitch(float accel[3])
{
   return -rad2deg(atan2(accel[0], sqrt(accel[1]*accel[1]
                                        +accel[2]*accel[2])));
}

float compute_roll(float accel[3])
{
   return rad2deg(atan2(accel[1], accel[2]));
}

float compute_yaw(float accel[3], float mag[3], float *dip)
{
   float m[3] = {mag[0], mag[1], mag[2]};
   rotateoutmag(m, accel);
   float yaw = rad2deg(atan2(m[1], m[0]));
   if(yaw < 0)
     yaw += 360;

   /* calculate inclination */
   if(dip)
      *dip = rad2deg(atan2(m[2], sqrt(m[0]*m[0] + m[1]*m[1])));

   return yaw;
}
