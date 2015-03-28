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

/* This file calibrates magnetometers. */

#include <stdio.h>
#include <stdlib.h>
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
#include <linalg/quaternion.h>

#include "fit.h"
#include "calc.h"
#include "sensor.h"
#include "cal.h"
#include "still.h"
#include "show.h"

MAKE_AUTOCAL_STATE(magfast, 4, ad7799_init_status)
MAKE_AUTOCAL_COVARIANCE(magfast, 4)

MAKE_AUTOCAL_STATE(mag, 9, ad7799_init_status)
MAKE_AUTOCAL_STILL(mag)

MAKE_AUTOCAL_STATE(magalign, 4, ad7799_init_status)
MAKE_AUTOCAL_STILL(magalign)

#ifndef __AVR_AT90USB1287__
uint8_t usemagdip EEMEM;
DATA_ACCESSOR(name=usemagdip dir=magsettings type=boolean mem=eeprom writable
	      varname=usemagdip)

MAKE_AUTOCAL_STATE(magdip, 13, ad7799_init_status)
MAKE_AUTOCAL_STILL(magdip)
#endif

#define SCALEM 50000.0 /* nominal scale factor */

void calibration_clear_mag(void)
{
   memset(magX, 0, sizeof magX);
   magX[0] = magfastX[0];
   magX[1] = magfastX[1];
   magX[2] = magfastX[2];

   magX[3] = 1/SCALEM;
   magX[4] = 1;
   magX[5] = 1;
   magdev = 0;
}

static float bestinc(void)
{
   /* if we have a better estimate of inclination, start with that */
   float inc = eeprom_read_float_safe(&inclination);
   if(inc)
     return sin(deg2rad(inc));
   return 0;
}

#ifndef __AVR_AT90USB1287__
void calibration_clear_magdip(void)
{
   memset(magdipX, 0, sizeof magdipX);
   magdipX[0] = magfastX[0];
   magdipX[1] = magfastX[1];
   magdipX[2] = magfastX[2];
   magdipX[3] = 1/SCALEM;
   magdipX[4] = 1;
   magdipX[5] = 1;
   magdipX[12] = bestinc();
   magdipdev = 0;
}
#endif

void calibration_clear_magalign(void)
{
   memset(magalignX, 0, sizeof magalignX);
   magalignX[3] = bestinc();
   magaligndev = 0;
}

void calibration_clear_magfast(void)
{
   const float X[4] = {0, 0, 0, SCALEM};
   memcpy(magfastX, X, sizeof X);

#define SCF 1e-4
#define BIF 1e-5
   const float PDiag[4] = {BIF, BIF, BIF, SCF};
   matrix_diag(magfastP, PDiag, 4);
   calibration_store_magfast();
}

void calibration_show_magfast(void)
{
  calibration_show_sphere(DATA_OUTPUT_FUNC, PSTR("magfast "), magfastX, magfastX[3]);
}

void calibration_show_mag(void)
{
   calibration_show_rotatedellipsoid(DATA_OUTPUT_FUNC, PSTR("mag "), magX);
  DATA_OUTPUT("mag dev: %f\n", magdev);
}

void calibration_show_magalign(void)
{
  calibration_show_align(DATA_OUTPUT_FUNC, PSTR("magalign "), magalignX);
  DATA_OUTPUT("magalign dev: %f\n", magaligndev);
}

#ifndef __AVR_AT90USB1287__
void calibration_show_magdip(void)
{
  calibration_show_transformation(DATA_OUTPUT_FUNC, PSTR("magdip "), magdipX);
  DATA_OUTPUT("magdip dev: %f\n", magdipdev);
}
#endif

static void apply_magonly(float data[3])
{
  /* If the dev is 0, we don't have valid data yet.
     Use fast calibration as backup.
  */
   if(magdev) {
      ApplyRotatedEllipsoid(magX, data, 0);
      vector_scale(magX[3], data, 3);
   } else {
      vector_sub(data, data, magfastX, 3);
      vector_scale(1/magfastX[3], data, 3);
  }
}

float calibration_get_mag_magnitude(void)
{
   float m[3] = {mag.filtered[0] >> mag.shift,
                 mag.filtered[1] >> mag.shift,
                 mag.filtered[2] >> mag.shift};

   apply_magonly(m);
   return magnitude(m);
}

float calibration_get_mag_diperror(void)
{
   return dip - rad2deg(asin(magalignX[3]));
}

void calibration_apply_mag(float data[3])
{
   float scale;
  /* If the dev is 0, we don't have valid data yet.
     Use fast calibration as backup.  */
#ifdef __AVR_AT90USB1287__
   if(
#else
   if(eeprom_read_byte_safe(&usemagdip) && magdipdev) {
      ApplyTransformationMatrix(magdipX, data, 0);
      scale = magdipX[3]*eeprom_read_float_safe(&fieldstrength);
   } else
   if(!eeprom_read_byte_safe(&usemagdip) &&
#endif
      magdev && magaligndev) {
      apply_magonly(data);
      ApplyQuaternion(data, magalignX, 0);
      scale = eeprom_read_float_safe(&fieldstrength);
   } else {
      vector_sub(data, data, magfastX, 3);
      scale = eeprom_read_float_safe(&fieldstrength) / magfastX[3];
   }
   vector_scale(scale, data, 3);
}

/* apply accel fast with integer math */
void calibration_apply_magfast(int32_t data[3])
{
   int i;
   for(i = 0; i<3; i++)
      data[i] -= magfastX[i];
}

void calibration_mag_scale(float data[3])
{
   vector_scale(1/magX[3], data, 3);
}

static int MakeRowMag(float *H, float *Z, const float *X, int i)
{
   if(isnan(stillpoints[i][0]))
      return 0;
   SetupFitRotatedEllipsoid(H, Z, X, stillpoints[i]);
   return 1;
}

static int MakeRowMagAlign(float *H, float *Z, const float *X, int i)
{
   float *x = stillpoints[i];
   if(isnan(x[0]) || isnan(x[3]))
      return 0;
   float m[3] = {x[0], x[1], x[2]};
   apply_magonly(m);
   float g[3] = {x[3], x[4], x[5]};
   /* match the next accel type with the mag type hence +1 */
   calibration_apply_accel(g);

   SetupFitQuaternion(H, Z, X, g, m);
   return 1;
}

#ifndef __AVR_AT90USB1287__
static int MakeRowMagDip(float *H, float *Z, const float *X, int i)
{
  if(i < numstillpoints) {
    if(isnan(stillpoints[i][0]))
      return 0;
    SetupFitTransformationMatrix_m(H, Z, X, stillpoints[i]);
  } else {
    i -= numstillpoints;
    if(isnan(stillpoints[i][0]) || isnan(stillpoints[i][3]))
      return 0;

    float g[3] = {stillpoints[i][3], stillpoints[i][4], stillpoints[i][5]};
    calibration_apply_accel(g);
    normalize(g);
    SetupFitTransformationMatrix_g(H, Z, X, stillpoints[i], g);
  }
  return 1;
}
#endif

/* this is called whenever we get a new stillpoint,
   update the calibration using the stillpoints */
int calibration_mag_stillcalibrate(void)
{
   float olddev = magdev;
   magdev = still_runpoints(MakeRowMag, magX, 9, numstillpoints);
   if(eeprom_read_byte_safe(&calibrationdebugging))
     calibration_show_mag();
   if(still_cal_done(olddev, magdev)) {
      calibration_store_mag();
      return 1;
   }
   return 0;
}
      
int calibration_magalign_stillcalibrate(void)
{
   float olddev = magaligndev;
   magaligndev = still_runpoints(MakeRowMagAlign, magalignX, 4, numstillpoints);
   if(eeprom_read_byte_safe(&calibrationdebugging))
     calibration_show_magalign();

   if(still_cal_done(olddev, magaligndev)) {
      calibration_store_magalign();
      return 1;
   }
   return 0;
}

#ifndef __AVR_AT90USB1287__
int calibration_magdip_stillcalibrate(void)
{
   float olddev = magdipdev;
   magdipdev = still_runpoints(MakeRowMagDip, magdipX, 13, numstillpoints*2);
   if(eeprom_read_byte_safe(&calibrationdebugging))
     calibration_show_magdip();

   if(still_cal_done(olddev, magdipdev)) {
      calibration_store_magdip();
      return 1;
   }
   return 0;
}
#endif

void calibration_magfast_poll(void)
{
   if(!*mag.filtered)
      return;

   float x[3] = {mag.filtered[0] >> mag.shift,
                 mag.filtered[1] >> mag.shift,
                 mag.filtered[2] >> mag.shift};

   /* don't run the calibration algorithms unless we move enough */
   static float lastpos[3];
   float d[3];
   vector_sub(d, x, lastpos, 3);
   if(magnitude2(d) < 10000000)
      return;

   memcpy(lastpos, x, sizeof lastpos);

   float H[4], Z[1], R[1] = {5e3};
   SetupFitSphere(H, Z, magfastX, x);

   /* don't get too confident,
      this lets us deal with magnetic distortions and recover */
   int i;
   for(i=0; i<3; i++)
      if(magfastP[i][i] < 1e-6)
         magfastP[i][i] = 1e-6;
   if(magfastP[3][3] < 1e-6)
      magfastP[3][3] = 1e-6;

   if(ApplyRecursiveLeastSquares(magfastP, H, R, Z, magfastX, 1, 4))
      return;

   /* safety.. should remove this, but I haven't had find to track down why
      it's possible to get nans */
   if(isnan(magfastX[0]) || isnan(magfastX[1]) ||
      isnan(magfastX[2]) || isnan(magfastX[3])) {
      calibration_restore_magfast();
      return;
   }

   if(eeprom_read_byte_safe(&calibrationfastdebugging))
      calibration_show_magfast();


   float storedx[3];
   eeprom_read_block_safe(storedx, magfastStoredX, sizeof magfastStoredX);

   if(dist2(storedx, magfastX) > 1e7)
      calibration_store_magfast();
}

int calibration_mag_detectdistortions(void)
{
   /* see if we are likely in a distortion,
      calculate all the time so we can warn the user */
   float x = calibration_get_mag_magnitude();
   static float magmag, magdip, magc, lastmagX;
   if(magX[3] != lastmagX) {
      magc = 0;
      lastmagX = magX[3];
   }

   magmag = (magmag*magc+x)/(magc+1);
   magdip = (magdip*magc+dip)/(magc+1);
   magc++;
   return fabs(magmag - x) > .02 || fabs(magdip - dip) > 2;
}
