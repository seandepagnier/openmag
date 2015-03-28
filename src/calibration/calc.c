/* Copyright (C) 2008, 2009 Sean D'Epagnier <sean@depagnier.com>
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

#include <string.h>

#include <math.h>

#include <avrdata.h>

#include <data/data.h>
#include <data/datadirectories.h>
#include <data/dataquaternions.h>

#include <linalg/quaternion.h>
#include <linalg/rotate.h>
#include <linalg/vector.h>
#include <linalg/matrix.h>

#include <calibration/fit.h>
#include <calibration/compute.h>
#include <calibration/sensor.h>

DATA_ACCESSOR(name=calibration dirname=calibration)

float pitch, roll, yaw, dip;

float boxalignedaccel[3] = {0, 0, 1}, boxalignedmag[3] = {0, 1, 0};

float incline, azimuth;

float inclination EEMEM, declination EEMEM, fieldstrength EEMEM = .4;

uint8_t truenorth EEMEM;

float boxalignment[4] EEMEM = {1, 0, 0, 0};
DATA_ACCESSOR(name=boxalignment mem=eeprom type=quaternion
              varname=boxalignment dir=calibration)

DATA_OPERATOR(set, name=boxalignment type=quaternion,
	      calc_boxalignment(data_param[0] - '0');
	      )

DATA_OPERATOR(clear, name type=quaternion,
	      calc_boxalignment(0);
	      )

float accellaseralignment[4] EEMEM = {1, 0, 0, 0};
float maglaseralignment[4] EEMEM = {1, 0, 0, 0};

DATA_ACCESSOR(name=laseralignment mem=eeprom type=quaternion dir=calibration)

DATA_OPERATOR(set, name=laseralignment type=quaternion,
	      calc_accellaseralignment(data_param[0] - '0');
	      calc_maglaseralignment(data_param[0] - '0');
   )

DATA_OPERATOR(get, name=laseralignment type=quaternion mem,
	      DATA_OUTPUT("accel: ");
              extern float accel##name[4];
              data_get_quaternion_##mem(accel##name);
	      DATA_OUTPUT("mag: ");
              extern float mag##name[4];
              data_get_quaternion_##mem(mag##name);
   )

DATA_OPERATOR(clear, name type=quaternion,
	      calc_accellaseralignment(0);
	      calc_maglaseralignment(0);
   )

/* the filtered accel and mag get updated in timer2
   so lock here so we don't get data that is partially updated */
#ifdef __AVR__
#include <avr/interrupt.h>
#endif

static void
reset_rotation(float *alignment)
{
   float q[4] = {1, 0, 0, 0};
   eeprom_write_block_safe(q, alignment, sizeof q);
}

void calc_boxalignment(int mode)
{
   float boxq[4], z[3] = {0, 0, 1};
   /* load old alignment */
   eeprom_read_block_safe(boxq, boxalignment, sizeof boxq);

   if(mode == 1 || mode == 2) {
      /* align box in pitch and roll (rotate accels to z) */
      float a[3];
      sensor_get_calibrated(&accel, a);
      vec2vec2quat(boxq, a, z);
   }

   switch(mode) {
   case 0:
      reset_rotation(boxalignment);
      break;
   case 1:
      {
	 /* apply the pitch and roll alignment to the mag */
	 float nmag[3];
         sensor_get_calibrated(&mag, nmag);
	 rotvecquat(nmag, boxq);

	 /* calculate the heading alignment rotation */
	 float magalignment[4];
	 angvec2quat(magalignment, -atan2(nmag[1], nmag[0]), z);

	 /* combine the rotations */
	 MultQuat2(magalignment, boxq);
      } /* fallthrough */
   case 2:
      /* store to eeprom */
      eeprom_write_block_safe(boxq, boxalignment, sizeof boxq);
      break;
   case 3:
      {
	 /* align to vertical */
	 float boxq2[4], ny[3] = {-1, 0, 0};
	 vec2vec2quat(boxq2, boxalignedaccel, ny);

	 /* combine the rotations */
	 MultQuat2(boxq2, boxq);

	 /* store to eeprom */
	 eeprom_write_block_safe(boxq, boxalignment, sizeof boxq);
      } break;
   default:
      DATA_OUTPUT("Invalid box alignment parameter: %d\n", mode);
      break;
   }
}

#define MAX_LASERALIGN_POINTS 12
static float calc_laseralignment(float (*alignment)[4], float (*points)[3],
                                 int *count, float x[3], int mode)
{
   if(mode == 0) {
      /* reset to identity quaternion */
      reset_rotation(*alignment);
      *count = 0;
      return 0;
   }

   if(*count < MAX_LASERALIGN_POINTS) {
      normalize(x);
      memcpy(points[*count], x, 3*(sizeof *x));
      (*count)++;
   }

   float q[4];
   float error = AlignAxis(q, points, *count);

   eeprom_write_block_safe(q, alignment, sizeof q);
   if(mode == 1)
      DATA_OUTPUT("error: %.2f\n", error);

   return error;
}

#define MAKE_CALC_LASERALIGNMENT(NAME)             \
   float calc_##NAME##laseralignment(int mode)  \
   { \
      static float points[MAX_LASERALIGN_POINTS][3]; \
      static int count; \
      float x[3]; \
      sensor_get_calibrated(&NAME, x); \
      return calc_laseralignment(&NAME##laseralignment, points, \
                                 &count, x, mode); \
   }

MAKE_CALC_LASERALIGNMENT(accel)
MAKE_CALC_LASERALIGNMENT(mag)

void calc_box_pitch_roll(void)
{
   /* start with calibrated accel */
   sensor_get_calibrated(&accel, boxalignedaccel);

   /* apply box alignment to accel */
   float boxq[4];
   eeprom_read_block_safe(boxq, boxalignment, sizeof boxq);
   rotvecquat(boxalignedaccel, boxq);
   
   pitch = compute_pitch(boxalignedaccel);
   roll = compute_roll(boxalignedaccel);
}

void calc_box_yaw_dip(void)
{
   /* start with calibrated mag */
   sensor_get_calibrated(&mag, boxalignedmag);

   /* apply box alignment to mag */
   float boxq[4];
   eeprom_read_block_safe(boxq, boxalignment, sizeof boxq);
   rotvecquat(boxalignedmag, boxq);

   /* compute yaw and dip */
   yaw = compute_yaw(boxalignedaccel, boxalignedmag, &dip);
   if(eeprom_read_byte_safe(&truenorth))
      yaw += eeprom_read_float_safe(&declination);
   yaw = compute_resolveheading(yaw);
}

void calc_laser_incline_azimuth(void)
{
   /* start with calibrated accel */
   float laseralignedaccel[3];
   sensor_get_calibrated(&accel, laseralignedaccel);

   /* apply laser alignment */
   float q[4];
   eeprom_read_block_safe(q, accellaseralignment, sizeof q);
   rotvecquat(laseralignedaccel, q);
   
   /* compute incline */
   incline = compute_pitch(laseralignedaccel);

   /* start with calibrated mag */
   float laseralignedmag[3];
   sensor_get_calibrated(&mag, laseralignedmag);
   
   /* apply laser alignment to mag */
   eeprom_read_block_safe(q, maglaseralignment, sizeof q);
   rotvecquat(laseralignedmag, q);

   /* calculate azimuth, don't bother with dip */
   azimuth = compute_yaw(laseralignedaccel, laseralignedmag, NULL);
   if(eeprom_read_byte_safe(&truenorth))
      azimuth += eeprom_read_float_safe(&declination);
   azimuth = compute_resolveheading(azimuth);

   if(isnan(azimuth))
      azimuth = 0;
}
