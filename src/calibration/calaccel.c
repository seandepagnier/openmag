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

/* This file calibrates accelerometers. */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include <avrdata.h>

#include <data/data.h>
#include <data/databasictypes.h>
#include <data/datadirectories.h>
#include <data/databools.h>
#include <data/dataarrays.h>

#include <linalg/vector.h>
#include <linalg/matrix.h>
#include <linalg/rotate.h>

#include "fit.h"
#include "calc.h"
#include "sensor.h"
#include "cal.h"
#include "still.h"
#include "show.h"

#define SCALEA 1333.0  /* nominal scale factor for sca3000-d01 accel */

#define NUMACCELFASTSTATES 4 /* spherical */
MAKE_AUTOCAL_STATE(accelfast, NUMACCELFASTSTATES, sca3000_detected)
MAKE_AUTOCAL_COVARIANCE(accelfast, NUMACCELFASTSTATES)

#define NUMACCELSTATES 15 /* 3rd order ellipsoid */
MAKE_AUTOCAL_STATE(accel, NUMACCELSTATES, sca3000_detected)
MAKE_AUTOCAL_STILL(accel)

void calibration_show_accelfast(void)
{
   calibration_show_sphere(DATA_OUTPUT_FUNC, PSTR("accelfast "), accelfastX,
			   accelfastX[3]);
}

void calibration_show_accel(void)
{
   calibration_show_rotatedellipsoid3rdorder(DATA_OUTPUT_FUNC, PSTR("accel "), accelX);
   DATA_OUTPUT("accel dev: %f\n", acceldev);
}

/* apply accel fast with integer math */
void calibration_apply_accelfast(int32_t data[3])
{
   int i;
   for(i = 0; i<3; i++)
      data[i] -= accelfastX[i];
}
/* tweaked for accels, fit for rotated ellipsoid */
void calibration_accelfast_poll(void)
{
   /* only take data if we are still */
   if(accel.stillstate != STILL)
     return;

   float x[3] = {accel.filtered[0] >> accel.shift,
                 accel.filtered[1] >> accel.shift,
                 accel.filtered[2] >> accel.shift};

   /* don't run the calibration algorithms unless we move enough */
   static float lastpos[3];
   float d[3];
   vector_sub(d, x, lastpos, 3);
   if(magnitude2(d) < 30000)
      return;
   memcpy(lastpos, x, sizeof lastpos);

   float H[NUMACCELFASTSTATES], Z[1], R[1] = {1};
   SetupFitSphere(H, Z, accelfastX, x);

   if(ApplyRecursiveLeastSquares(accelfastP, H, R, Z, accelfastX, 1, NUMACCELFASTSTATES))
      return;

   /* set a floor on the bias covariance */
   int i;
   for(i = 0; i<3; i++)
      if(accelfastP[i][i] < 5e-6)
         accelfastP[i][i] = 5e-6;

   /* set a floor on the scalefactor covariance */
   if(accelfastP[3][3] < 5e-7)
      accelfastP[3][3] = 5e-7;

   if(eeprom_read_byte_safe(&calibrationfastdebugging))
      calibration_show_accelfast();

   float storedx[3];
   eeprom_read_block_safe(storedx, accelfastStoredX, sizeof storedx);

   /* if the bias changed by more than 2 counts, store the calibration again */
   if(dist2(storedx, accelfastX) > 2)
      calibration_store_accelfast();
}

void calibration_clear_accelfast(void)
{
  const float X[NUMACCELFASTSTATES] = {0, 0, 0, SCALEA};
  memcpy(accelfastX, X, sizeof X);
#define B 1e-3  /* bias */
#define W 1e-5  /* scale factor */
  const float PDiag[NUMACCELFASTSTATES] = {B, B, B, W};
  matrix_diag(accelfastP, PDiag, NUMACCELFASTSTATES);
}

void calibration_apply_accel(float data[3])
{
  /* If the dev is 0, we don't have valid data yet.
     Use fast calibration as backup.  */
  if(acceldev) {
    ApplyRotatedEllipsoid3rdOrder(accelX, data, 0, 0);
    vector_scale(accelX[3], data, 3);
  } else {
    vector_sub(data, data, accelfastX, 3);
    vector_scale(1/accelfastX[3], data, 3);
  }
}

/* 3rd order ellipsoid */
static int MakeRowAccel(float *H, float *Z, const float *X, int i)
{
   if(isnan(stillpoints[i][3]))
      return 0;
   SetupFitRotatedEllipsoid3rdOrder(H, Z, X, stillpoints[i]+3);
   return 1;
}

int calibration_accel_stillcalibrate(void)
{
   float olddev = acceldev;
   acceldev = still_runpoints(MakeRowAccel, accelX,
			      (sizeof accelX) / (sizeof *accelX),
			      numstillpoints);
   if(eeprom_read_byte_safe(&calibrationdebugging))
      calibration_show_accel();
   if(still_cal_done(olddev, acceldev)) {
      calibration_store_accel();
      return 1;
   }
   return 0;
}

void calibration_clear_accel(void)
{
  memset(accelX, 0, sizeof accelX);
  accelX[3] = 1/SCALEA;
  accelX[4] = 1;
  accelX[5] = 1;
  acceldev = 0;
}
