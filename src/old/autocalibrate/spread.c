/* Copyright (C) 2007 Sean D'Epagnier <sean@depagnier.com>
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

#include <data/data.h>
#include <data/databasictypes.h>
#include <data/dataarrays.h>
#include <data/datadirectories.h>
#include <data/databools.h>
#include <data/dataenums.h>

#include <avrdata.h>

#include <linalg/fit.h>
#include <linalg/quaternion.h>
#include <linalg/vector.h>

#include "sensor.h"
#include "spread.h"
#include "itertimer.h"

#define MIN_VALID_ACCEL_SCALE 500
#define MAX_VALID_ACCEL_SCALE 800
#define MIN_VALID_MAG_SCALE 2000
#define MAX_VALID_MAG_SCALE 30000

DATA_ACCESSOR(name=spread dirname=SPREAD)

enum {START_FITTING, ACCEL_FITTING, MAG_FITTING, ALIGN_FITTING, FAILED_FITTING,
      DONE_FITTING};
uint8_t spreadfitstate = START_FITTING;

/* stores both accel and mag raw sensor data as well as noise for COUNT still points */
float stillpoints[NUM_STILL_POINTS][7] EEMEM = {{NAN}};
DATA_ACCESSOR(name=stillpoints dir=SPREAD);

DATA_OPERATOR(get, name=stillpoints,
              stillpoints_get();
    )

DATA_OPERATOR(clear, name=stillpoints,
              spread_clear();
    )

/* stuff we calculate from the spread */
#define MAKE_SCALE_BIAS_DEV(NAME, DIR) \
    DATA_ACCESSOR(name=NAME dirname=DIR dir=SPREAD) \
    float NAME##bias[3]; \
    DATA_ACCESSOR(name=bias type=float mem=sram arraylen=3 \
                  varname=NAME##bias dir=DIR) \
    float NAME##scale[3]; \
    DATA_ACCESSOR(name=scale type=float mem=sram arraylen=3 \
                  varname=NAME##scale dir=DIR) \
    float NAME##deviation = 1; \
    DATA_ACCESSOR(name=deviation type=float mem=sram varname=NAME##deviation \
                  dir=DIR)

MAKE_SCALE_BIAS_DEV(accel, ACCELSPREAD)
MAKE_SCALE_BIAS_DEV(mag, MAGSPREAD)
float magalignment[3]; /* crosscoupling values */
DATA_ACCESSOR(name=alignment type=float mem=sram varname=magalignment
              arraylen=3 dir=MAGSPREAD)

float biasquat[4] = {1, 0, 0, 0}; /* bias quaternion */
DATA_ACCESSOR(name=biasquat type=float mem=sram varname=biasquat
              arraylen=4 dir=MAGSPREAD)

float magrebias[3];

DATA_ACCESSOR(name=alignment dirname=ALIGNMENT dir=SPREAD)

float alignmentquat[4];
DATA_ACCESSOR(name=alignmentquat type=float mem=sram varname=alignmentquat
              arraylen=4 dir=ALIGNMENT)

float alignmentdeviation;
DATA_ACCESSOR(name=deviation type=float mem=sram varname=alignmentdeviation
              dir=ALIGNMENT)

float magneticdip;
DATA_ACCESSOR(name=magneticdip type=float mem=sram varname=magneticdip
              dir=ALIGNMENT)

uint8_t hemisphere EEMEM;
static const char hemiN[] PROGMEM = "N";
static const char hemiS[] PROGMEM = "S";
const char *const hemispheres[] PROGMEM = {hemiN, hemiS, 0};
DATA_ACCESSOR(name=hemisphere mem=eeprom type=enumerant varname=hemisphere
              enumvalues=hemispheres dir=ALIGNMENT)

/* this is set to 1 to indicate we can automatically accept new points and
   begin the calibration process if the sensor data is still */
uint8_t spread_autocal;
DATA_ACCESSOR(name=autocal type=bool mem=sram
              varname=spread_autocal writable dir=SPREAD)

/* sensors */
static int FitAccel(int start)
{
   const int count = NUM_STILL_POINTS;
   float points[count][3];

   int i;
   for(i=0; i<count; i++)
      eeprom_read_block_safe(points[i], stillpoints[i], sizeof *points);

   if(start) {
      InitialEstimateSphere(points, count, accelbias, accelscale);
      acceldeviation = 1;
   }

   if(CalcBestFitEllipse(points, count, accelbias, accelscale)) {
      float olddev = acceldeviation;
      acceldeviation = CalcAvgDeviation(points, count, accelbias, accelscale, 0);

      if(acceldeviation >= olddev*.99) {
         /* look for reasonable scale factors */
         if(accelscale[0] > MIN_VALID_ACCEL_SCALE
            && accelscale[0] < MAX_VALID_ACCEL_SCALE)
            return 1;
      }
   }

   if(acceldeviation > .3)
      spreadfitstate = FAILED_FITTING;

   return 0;
}

static int FitMag(int start)
{
   const int count = NUM_STILL_POINTS;
   float points[count][3];

   int i;
   for(i=0; i<count; i++)
      eeprom_read_block_safe(points[i], stillpoints[i] + 3, sizeof *points);

   int ret;
   if(start || magdeviation > .5) {
      InitialEstimateSphere(points, count, magbias, magscale);
      magalignment[0] = magalignment[1] = magalignment[2] = 0;
      magdeviation = 1;
   }

   /* if our deviation is high, don't bother with cross-coupling calibration */
   if(magdeviation > .1) {
      ret = CalcBestFitEllipse(points, count, magbias, magscale);
      magalignment[0] = magalignment[1] = magalignment[2] = 0;
   } else
      ret = CalcBestFitRotatedEllipse(points, count, magbias,
                                      magalignment, magscale);

   if(ret) {
      float olddev = magdeviation;
      magdeviation = CalcAvgDeviation(points, count, magbias, magscale,
                                      magalignment);
      if(magdeviation < .1 && magdeviation >= olddev*.99) {
         /* reasonable scale factors */
         if(magscale[0] > MIN_VALID_MAG_SCALE
            && magscale[0] < MAX_VALID_MAG_SCALE)
            return 1;
      }
   }

   if(magdeviation > .1)
      spreadfitstate = FAILED_FITTING;

   return 0;
}

static int FitAlign(int start)
{
   const int count = NUM_STILL_POINTS;
   float apoints[count][3], mpoints[count][3];

   int i;
   for(i=0; i<count; i++) {
      eeprom_read_block_safe(apoints[i], stillpoints[i], sizeof *apoints);
      eeprom_read_block_safe(mpoints[i], stillpoints[i] + 3, sizeof *mpoints);
   }

   /* apply calibration */
   for(int i=0; i<NUM_STILL_POINTS; i++) {
      ApplyBestFit(apoints[i], apoints[i], accelbias, accelscale, 0);
      ApplyBestFit(mpoints[i], mpoints[i], magbias, magscale, magalignment);
   }

   if(start) {
      alignmentquat[0] = alignmentquat[1] =
         alignmentquat[2] = alignmentquat[3] = .5;
      alignmentdeviation = 1;
   }

   int hemi = eeprom_read_byte_safe(&hemisphere);
   if(CalcBestFitQuaternion(apoints, mpoints, count, alignmentquat, hemi)) {
      float olddev = alignmentdeviation;
      alignmentdeviation = CalcAvgDeviationQuaternion(apoints, mpoints, count,
                                                      alignmentquat, hemi);

      float n = quatmagnitude(alignmentquat);
      magneticdip = 90 - 180/M_PI*acos(1/(n*n));

      /* try other hemisphere */
      if(alignmentdeviation > 1)
         eeprom_write_byte_safe(&hemisphere, !hemi);
      else
      if(alignmentdeviation >= olddev*.99)
         return 1;
   }

   if(alignmentdeviation > .3)
      spreadfitstate = FAILED_FITTING;

   return 0;
}

void stillpoints_get(void)
{
    int i;
    for(i=0; i < NUM_STILL_POINTS; i++) {
       float a[3], ao[3], m[3], mo[3], devg, devm, deva;
       eeprom_read_block_safe(a, stillpoints[i], sizeof a);
       if(isnan(a[0]))
          break;

       devg = CalcDeviation(a, accelbias, accelscale, 0); 

       eeprom_read_block_safe(m, stillpoints[i]+3, sizeof m);
       devm = CalcDeviation(m, magbias, magscale, magalignment); 

       int hemi = eeprom_read_byte_safe(&hemisphere);
       ApplyBestFit(a, ao, accelbias, accelscale, 0);
       ApplyBestFit(m, mo, magbias, magscale, magalignment);
       deva = CalcDeviationQuaternion(ao, mo, alignmentquat, hemi);

       float noise;
       eeprom_read_block_safe(&noise, stillpoints[i]+6, sizeof noise);
       DATA_OUTPUT("%.2f %.2f %.2f, %.2f %.2f %.2f [%.2f] (%.3g %.3g %.3g)\n",
                   a[0], a[1], a[2],
                   m[0], m[1], m[2],
                   noise,
                   devg, devm, deva);
    }
}

void spread_clear(void)
{
    int i;
    const float nan = NAN;
    eeprom_write_block_safe(&nan, stillpoints[0], sizeof nan);
}

static int read_cal_data(float apoints[NUM_STILL_POINTS][3],
                         float mpoints[NUM_STILL_POINTS][3],
                         float npoints[NUM_STILL_POINTS])
{
   int count;

   for(count = 0; count < NUM_STILL_POINTS; count++) {
      eeprom_read_block_safe(apoints[count], stillpoints[count], sizeof *apoints);
      eeprom_read_block_safe(mpoints[count], stillpoints[count] + 3, sizeof *mpoints);
      eeprom_read_block_safe(npoints+count, stillpoints[count] + 6, sizeof *npoints);

      if(isnan(apoints[count][0]))
         break;
   }
   return count;
}

static int mini;
static float mind;

/* calculate the "worst" still point which should be replaced by a better
   calibration when it arrives */
static void refresh_minpoint(void)
{
   mini=0;
   mind=1.0/0.0;

   int count;
   float apoints[NUM_STILL_POINTS][3];
   float mpoints[NUM_STILL_POINTS][3];
   float npoints[NUM_STILL_POINTS];

   count = read_cal_data(apoints, mpoints, npoints);

   int i, j;
   for(i=0; i<NUM_STILL_POINTS; i++) {
      for(j=0; j<NUM_STILL_POINTS; j++) {
         if(i == j)
            continue;

         float d = dist2(mpoints[i], mpoints[j]) - npoints[i];
         if(d < mind) {
            mind = d;
            mini = i;
         }
      }
   }
}

/* called if we should consider the current data for a stillpoint calibration
   will add the point, or replace the worst point and return 1, or return 0
   if the current data is not helpful */
static int spread_addpoint(void)
{ 
   float noise = accelnoise + magnoise;
   int i;

   /* read in the cal data from eeprom */
   int count = NUM_STILL_POINTS;
   float apoints[NUM_STILL_POINTS][3];
   float mpoints[NUM_STILL_POINTS][3];
   float npoints[NUM_STILL_POINTS];

   count = read_cal_data(apoints, mpoints, npoints);

   /* check for empty slots */
   if(count < NUM_STILL_POINTS) {
      /* always take the still point if there is free space */
      eeprom_write_block_safe(accelnicedata, stillpoints[count], sizeof accelnicedata);
      eeprom_write_block_safe(magnicedata, stillpoints[count]+3, sizeof magnicedata);
      eeprom_write_block_safe(&noise, stillpoints[count]+6, sizeof noise);

      /* make sure next point is nan to designate remaining points empty
         is set to nan to designate it as empty */
      if(count < NUM_STILL_POINTS - 1) {
         const float nan = NAN;
         eeprom_write_block_safe(&nan, stillpoints[count+1], sizeof nan);
      }
      return 1;
   }

   /* find closest mag entrees */
   float mine=1.0/0.0;
   for(i=0; i<NUM_STILL_POINTS; i++) {
      float e = dist2(mpoints[i], magnicedata) - noise;
      if(e < mine)
         mine = e;
   }

   /* overwrite the point with the closest neighbor if this
      point is farther from any point than the closest neighbors */
   if(mind < mine) {
      eeprom_write_block_safe(accelnicedata, stillpoints[mini], sizeof accelnicedata);
      eeprom_write_block_safe(magnicedata, stillpoints[mini]+3, sizeof magnicedata);
      eeprom_write_block_safe(&noise, stillpoints[mini]+6, sizeof noise);

      return 1;
   }
   return 0;
}

void spread_poll(void)
{
   if(gettime() < .1) /* hack to stop it from fitting too early on when usb is trying to start */
      return;

   /* if both sensors are still, suggest the current data for calibration */
   if(accelnoise < ACCEL_NT && magnoise < MAG_NT) {
      if(spread_autocal && spread_addpoint())
         /* if the point was taken, restart cal process */
         spreadfitstate = START_FITTING;

#if 0
      /* rebias magnetometer */
      float accelcaldata[3], magcaldata[3];
      spread_apply_accel_cal(accelcaldata, accelnicedata);
      spread_apply_mag_cal(magcaldata, magnicedata);

      float ang1 = acos(dot(accelcaldata, magcaldata)
                       / sqrt(magnitude2(accelcaldata) * magnitude2(magcaldata)));

      float n[3];
      cross(n, accelcaldata, magcaldata);
      normalize(n);

      float ang2 = eeprom_read_byte_safe(&hemisphere) ? 90 + magneticdip : 90 - magneticdip;
      ang2 *= M_PI/180;
      ang2 -= ang1;

      float q[4];
      angvec2quat(q, ang2, n);
      MultQuat1(biasquat, q);

      rotvecquat(magcaldata, biasquat);

      float m = magnitude2(magcaldata);

      float b[3];
      b[0] = magcaldata[0] * (m-1);
      b[1] = magcaldata[1] * (m-1);
      b[2] = magcaldata[2] * (m-1);

      normalize(magcaldata);
      float d = dot(b, magcaldata);
      float c[3];
      c[0] = d*magcaldata[0];
      c[1] = d*magcaldata[1];
      c[2] = d*magcaldata[2];

            b[0] -= c[0];
            b[1] -= c[1];
            b[2] -= c[2];
            //      magrebias[0] += b[0];
            //      magrebias[1] += b[1];
      //      magrebias[2] += b[2];
#endif


#if 0
            double x = magcaldata[0] / 100;
            if(x > 0) {
               if(x > magrebias[0])
                  magrebias[0] = x;
            } else {
               if(x < magrebias[0])
                  magrebias[0] = x;
            }
#endif
   }

#if 0
   /* show actual, min and max possible values */
   float magcaldata[3];
   spread_apply_mag_cal(magcaldata, magdata);

   float diff = magnitude(magcaldata) - 1;
   DEBUG("%f %f %f\n", magcaldata[0], magcaldata[0] + diff, magcaldata[0] - diff);
#endif


#if 0
   static float minp=-.05, maxp=.05;
   float magcaldata[3];
   spread_apply_mag_cal(magcaldata, magdata);
   if(magcaldata[0] > maxp) {
      maxp = magcaldata[0];
      minp = -.05;
   }

   if(magcaldata[0] < minp) {
      minp = magcaldata[0];
      maxp = .05;
   }

   if(magnoise < MAG_NT && fabs(magcaldata[0]) < .05) {
      if(minp < -.05)
         DEBUG("%f %f\n", minp, magcaldata[0]);
      if(maxp > .05)
         DEBUG("%f %f\n", maxp, magcaldata[0]);
      
      minp = -.05;
      maxp = .05;
   }
#endif

   switch(spreadfitstate) {
   case START_FITTING:
      spreadfitstate = ACCEL_FITTING;
      refresh_minpoint();
      FitAccel(1);
      break;
   case ACCEL_FITTING:
      if(FitAccel(0)) {
         spreadfitstate = MAG_FITTING;
         FitMag(1);
      }
      break;
   case MAG_FITTING:
      if(FitMag(0))
      {
         spreadfitstate = ALIGN_FITTING;
         FitAlign(1);
      }
      break;
   case ALIGN_FITTING:
      if(FitAlign(0))
         spreadfitstate = DONE_FITTING;
      break;
   }
}

#if 0

void spread_apply_mag_cal(float caldata[3], float data[3])
{
   /* apply scale, bias and alignment cal */
   ApplyBestFit(data, caldata, magbias, magscale, magalignment);
   
   /* rotate the mag into accel coordinates */
   float nquat[4];
   memcpy(nquat, alignmentquat, sizeof nquat);
   quatnormalize(nquat);
   rotvecquat(caldata, nquat);

#if 0
   /* rotate out short-term bias */
   rotvecquat(caldata, biasquat);

   caldata[0] -= magrebias[0];
   caldata[1] -= magrebias[1];
   caldata[2] -= magrebias[2];
#endif
}
#endif
