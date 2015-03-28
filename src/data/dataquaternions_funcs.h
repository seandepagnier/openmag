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

#include <math.h>
#include <linalg/quaternion.h>
#include <linalg/rotate.h>

#define MAKE_QUATERNION_GETS(mem) \
    void data_get_quaternion_##mem(float quat[4]) \
    { \
      float q[4];                        \
      get_bytes_##mem(q, quat, sizeof q);\
      data_show_quaternion(q);           \
    }

void data_show_quaternion(float q[4])
{
  DATA_OUTPUT("<%f %f %f %f> %f degrees\n",
              q[0], q[1], q[2], q[3], rad2deg(quatangle(q)));
}

MAKE_QUATERNION_GETS(sram)
MAKE_QUATERNION_GETS(eeprom)
MAKE_QUATERNION_GETS(flash)
