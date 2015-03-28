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

/* store a set of stillpoints in ram,
   these are raw measurements from magnetometer and accelerometer sensors when
   the device was not moving.  These points are used to improve the estimation
   of the calibration state */

#include <math.h>

#ifdef __AVR__
#include <avr/wdt.h>
#include <avr/io.h>
#include <avrdrivers/magsetup.h>
#endif

#include <avrdata.h>

#include <data/data.h>
#include <data/databools.h>
#include <data/datadirectories.h>

#include <linalg/vector.h>
#include <linalg/matrix.h>

#include "cal.h"
#include "sensor.h"
#include "still.h"
#include "fit.h"

DATA_OPERATOR(apply, name=stillpoints,
	      extern const char accelname[];
	      extern const char magname[];
	      if(!strcmp_P(data_param, accelname))
		 still_calibrate_accel();
	      else
	      if(!strcmp_P(data_param, magname))
		 still_calibrate_mag();
	      else
		 DATA_OUTPUT("must specify either \"accel\" or \"mag\" to apply "
			     "stillpoints\n");
   )

DATA_OPERATOR(values, name=stillpoints,
	      DATA_OUTPUT("accel mag \n");
)

DATA_ACCESSOR(name=stillpoints dir=calibration)
DATA_OPERATOR(get, name=stillpoints,
	      still_show_stillpoints();
   )

DATA_OPERATOR(clear, name=stillpoints,
	      extern uint8_t numstillpoints;
	      numstillpoints = 0;
   )


DATA_ACCESSOR(name=debugging type=bool mem=eeprom
              varname=calibrationdebugging writable dir=calibration)
uint8_t calibrationdebugging EEMEM;

DATA_ACCESSOR(name=fastdebugging type=bool mem=eeprom
              varname=calibrationfastdebugging writable dir=calibration)
uint8_t calibrationfastdebugging EEMEM;

float stillpoints[NUMSTILLPOINTS][6];

uint8_t numstillpoints;
uint8_t jointstillpoints EEMEM = 1;

uint8_t still_update;

DATA_ACCESSOR(name=jointstillpointslocked type=bool mem=eeprom
              varname=jointstillpoints writable dir=calibration)

void still_show_stillpoints(void)
{
  int i;
  for(i=0; i<numstillpoints; i++)
    if(!isnan(stillpoints[i][0]) || !isnan(stillpoints[i][3])) {
      DATA_OUTPUT("%f %f %f %f %f %f\n",
		  stillpoints[i][0], stillpoints[i][1], stillpoints[i][2],
		  stillpoints[i][3], stillpoints[i][4], stillpoints[i][5]);
    }
}

int still_count_stillpoints(int sensor)
{
   int i, num = 0;
   for(i=0; i<numstillpoints; i++)
      if(!isnan(stillpoints[i][3*sensor]))
         num++;
   return num;
}

/* add a new point, replacing the closest one to it
   don't add the point if it's too close to other points */
static void addpoint(float a[3], float m[3])
{ 
   uint8_t replace = 0;

   float nm[3] = {m[0], m[1], m[2]}, na[3] = {a[0], a[1], a[2]};
   normalize(nm);
   normalize(na);

   float min = 1e10;
   int i, mini = -1;
   for(i=0; i<numstillpoints; i++) {
      float d[3], dist;
      /* we care more about mag, but if it isn't available, then use the accel
         to find the best place for the new point  */
      if(!isnan(a[0])) {
         memcpy(d, stillpoints[i]+3, sizeof d);
         normalize(d);
         vector_sub(d, na, d, 3);
         dist = magnitude2(d);
      } else
         dist = 0;

      if(!isnan(m[0])) {
         memcpy(d, stillpoints[i], sizeof d);
         normalize(d);
         vector_sub(d, nm, d, 3);
         dist += magnitude2(d);
      } else {
         /* do we not have mag and this one does? don't replace it,
            we want to keep as many in the table for mag as possible.  */
         if(!isnan(stillpoints[i][0]))
            continue;
      }

      if(dist < 4e-3) {
         /* we basically already have this point */
         if(!isnan(a[0]) && !isnan(m[0])) {
            mini = i; /* replace old one if we have accel and mag */
	    replace = 1;
            break;
         }
         return; /* otherwise don't bother */
      }

      if(numstillpoints == NUMSTILLPOINTS && dist < min) {
         min = dist;
         mini = i;
      }
   }

   /* didn't find a good place? */
   if(mini == -1) {
      /* table already full, bail */
      if(numstillpoints == NUMSTILLPOINTS)
         return;

      /* append it to the end */
      mini = numstillpoints++;
   }

   /* fill in data */
   for(i = 0; i<3; i++) {
      stillpoints[mini][i] = m[i];
      stillpoints[mini][i+3] = a[i];
   }

   if(!replace) {
      if(!isnan(a[0]))
	 still_update |= 1;
      if(!isnan(m[0]))
	 still_update |= 2;

      if(eeprom_read_byte_safe(&calibrationdebugging)) {
	 DATA_OUTPUT("stillpoint added:");
	 for(i=0; i<6; i++)
	    DATA_OUTPUT(" %f", stillpoints[mini][i]);
	 DATA_OUTPUT("\n");
      }
   }
}

/* take raw data for a given sensors, and accumulate it. */
void still_accumulate(struct sensor_typ *s, int32_t *datain)
{
   int count = s->dimensions;
   uint8_t i;

   int16_t countneeded;
   if(s == &accel) {
      countneeded = 256;
   } else {
#ifdef __AVR__
      /* don't do anything if we saturated */
      extern uint8_t hmc_saturated[3];
      if(hmc_saturated[0] || hmc_saturated[1] || hmc_saturated[2])
         return;

      extern uint8_t hmc_samples, hmc_rate;
      countneeded = 1 + 64 / (hmc_samples * hmc_rate);
#else
      countneeded = 8;
#endif
   }

   uint8_t movingtoomuch = 0;
   for(i = 0; i<count; i++) {
      if(s->stillaccummin[i] > datain[i]) {
         if(s->stillaccummax[i] - datain[i] > s->stilltolerance)
            movingtoomuch = 1;
         else
            s->stillaccummin[i] = datain[i];
      } else if(s->stillaccummax[i] < datain[i]) {
         if(datain[i] - s->stillaccummin[i] > s->stilltolerance)
            movingtoomuch = 1;
         else
            s->stillaccummax[i] = datain[i];
      }
   }
   
   if(movingtoomuch) {
      s->stillstate = NOTSTILL;
      s->stillaccumcount = 0;

      for(i = 0; i<count; i++) {
         s->stillaccummin[i] = s->stillaccummax[i] = datain[i];
         s->stillaccum[i] = 0;
      }
   } else {
      if(s->stillstate != STILL && s->stillaccumcount >= countneeded) {
         if(s->stillstate == NOTSTILL)
            s->stillaccumcount = 0;
         s->stillstate++;
      }

      if(s->stillstate != NOTSTILL)
         for(i = 0; i<count; i++)
            s->stillaccum[i] += datain[i];

      /* don't let it wrap, instead divide it down */
      if(++s->stillaccumcount > 32000) {
         for(i = 0; i<count; i++)
            s->stillaccum[i] >>= 1;
         s->stillaccumcount >>= 1;
      }
   }
}

#ifdef __AVR__
#include <avr/interrupt.h>
#endif

static int still_readpoint(struct sensor_typ *s, float *data, float *cov)
{
   if(s->stillstate != STILL)
      return 0;

   int i;
   int32_t a[s->dimensions], d[s->dimensions];
   int32_t count;

   DATA_LOCK;
   for(i = 0; i<s->dimensions; i++) {
      a[i] = s->stillaccum[i];
      d[i] = s->stillaccummax[i] - s->stillaccummin[i];
   }
   count = s->stillaccumcount;
   DATA_UNLOCK;

   int32_t total = 0;
   for(i = 0; i<s->dimensions; i++) {
      data[i] = (float)a[i] / count;
      total += d[i]*d[i];
   }
   *cov = total / count;

   return 1;
}

volatile uint8_t stillcalibrateaccel, stillcalibratemag;
void still_poll(void)
{
   /* re-read still tolerances from eeprom */
   accel.stilltolerance = eeprom_read_word_safe(&accelstilltolerance);

   mag.stilltolerance =
#ifdef __AVR__
      magsetup_fixstilltolerance
#endif
      (eeprom_read_word_safe(&magstilltolerance));

   /* see if we have stillpoints for accel, mag, or both, and run calibration
      algorithms */
   float as[3], ar, ms[3], mr;
   int havea = 0, havem;
   if(!still_readpoint(&accel, as, &ar))
      as[0] = as[1] = as[2] = NAN;

   if(!still_readpoint(&mag, ms, &mr))
      ms[0] = ms[1] = ms[2] = NAN;

   static float lasta[3], lastm[3];
   if((!isnan(as[0]) && sensor_checknotlast(lasta, as, 1000))
      || (!isnan(ms[0]) && sensor_checknotlast(lastm, ms, 4e7))) {
      int joint = eeprom_read_byte_safe(&jointstillpoints);
      /* if one of the sensors isn't there, don't require it */
      extern uint8_t sca3000_detected, ad7799_init_status;
      if(!sca3000_detected || !ad7799_init_status)
	 joint = 0;
      if(!joint || (!isnan(as[0]) && !isnan(ms[0])))
	 addpoint(as, ms);
   }

#ifdef __AVR__
   TIMSK2 &= ~_BV(TOIE2); /* don't let sensor interrupt nest here
			     or we will run out of stack and crash */
#endif

   if(stillcalibrateaccel)
      if(calibration_accel_stillcalibrate())
	 stillcalibrateaccel = 0;

   switch(stillcalibratemag) {
   case 1:
      if(calibration_mag_stillcalibrate())
	 stillcalibratemag = 2;
      break;
   case 2:
      if(calibration_magalign_stillcalibrate())
#ifdef __AVR_AT90USB1287__
	 stillcalibratemag = 0;
#else
	 stillcalibratemag = 3;
      break;
   case 3:
      if(calibration_magdip_stillcalibrate())
	 stillcalibratemag = 0;
      break;
#endif
   }

#ifdef __AVR__
   TIMSK2 |= _BV(TOIE2);
#endif
}

int still_cal_done(float olddev, float dev)
{
   if(!dev)
      return 1;
   if(!olddev)
      return 0;
   return dev > olddev - .0001;
}

void still_calibrate_accel(void)
{
   calibration_clear_accel();
   stillcalibrateaccel = 1;
}

void still_calibrate_mag(void)
{
   calibration_clear_mag();
   calibration_clear_magalign();
#ifndef __AVR_AT90USB1287__
   calibration_clear_magdip();
#endif
   stillcalibratemag = 1;
}

/* n is the number of states
   take a bunch of points, and compute a update using all of them
 */
float still_runpoints(int (*makerow)(float *H, float *Z,
                                     const float *X, int i),
                      float *X, int m, int n)
{
   float newX[m];
   memcpy(newX, X, sizeof newX);

   if(ApplyLeastSquaresCallback(newX, makerow, m, n)) {
     DEBUG("failed to run least squares\n");
     return 0;
   }

#ifdef __AVR__
   /* this routine can take a really long time especially if the interrupts
      are using a lot of cpu (ie: sensor data out at a fast rate via usb)
      reset the watchdog here so we don't get a wrongful watchdog reset */
   wdt_reset();
#endif

   memcpy(X, newX, sizeof newX);      
   float dev = CalcDeviationCallback(X, makerow, n);
   if(isfinite(dev) && dev < .5)
      return dev;
   return 0;
}
