/* Copyright (C) 2007, 2008, 2009 Sean D'Epagnier <sean@depagnier.com>
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

#include <avrdata.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <math.h>

#include <gettime.h>

#include <linalg/vector.h>

#include <data/data.h>

#include "calc.h"
#include "sensor.h"
#include "cal.h"
#include "fit.h"
#include "filter.h"
#include "still.h"

uint8_t sensor_disable_calc;

/* the macro was too long for gcc, so split up */
#define MAKE_SENSOR_1(NAME, DIMS, STILLTOLERANCE)	  \
    uint8_t NAME##outputtype EEMEM = RAW; \
    uint8_t NAME##failure;                \
    int32_t NAME##accum[DIMS];            \
    int32_t NAME##raw[DIMS];              \
    int32_t NAME##filtered[DIMS];         \
    int32_t NAME##stillaccum[DIMS], NAME##stillaccummin[DIMS]; \
    int32_t NAME##stillaccummax[DIMS]; \
    float NAME##inputrate;                          \
    float NAME##outputrate EEMEM;                   \
    uint16_t NAME##stilltolerance EEMEM = STILLTOLERANCE;          \
    const char NAME##name[] PROGMEM = #NAME;

#define MAKE_SENSOR_2(NAME, DIMS, SHIFT)       \
         struct sensor_typ NAME = \
         {DIMS, &NAME##outputtype, &NAME##failure, NAME##accum, NAME##raw, \
          NAME##filtered, SHIFT,                                        \
          NOTSTILL, NAME##stillaccum, NAME##stillaccummin, NAME##stillaccummax, \
          0, 0,	\
          0, \
          &NAME##inputrate, 0, 0, 0, \
          &NAME##outputrate, 100000, 0, \
          {0, 0, 0}, \
          calibration_apply_##NAME, NAME##name};

/* accel */
MAKE_SENSOR_1(accel, 3, 60)
MAKE_SENSOR_2(accel, 3, 8)

/* mag */
MAKE_SENSOR_1(mag, 3, 90)
MAKE_SENSOR_2(mag, 3, 4)

void sensor_show_output(struct sensor_typ *s, const char *prefix)
{
   int count = s->dimensions, i;
   int32_t data[count];
   float dataf[count];

   /* display correct output type */
   int type = eeprom_read_byte_safe(s->outputtype);

   switch(type) {
   case RAW:
   case RAWCALIBRATED:
      memcpy(data, s->raw, sizeof data);
      break;
   case FILTERED:
   case FILTEREDCALIBRATED:
      memcpy(data, s->filtered, sizeof data);
      for(i = 0; i<count; i++)
        data[i] >>= s->shift;
      break;      
   }

   if(prefix) {
      char str[12];
      strcpy_P(str, prefix);
      DATA_OUTPUT_STR(str);
      DATA_OUTPUT_STR(": ");
   }

   if(type == RAW || type == FILTERED) {
      for(i = 0; i < count; i++) {
         char str[12];
         if(s == &accel) /* itoa is a lot faster than ltoa, so use it
                            in preference for the accel since it is always
                            in range */
            itoa((int)data[i], str, 10);
         else
            ltoa(data[i], str, 10);
         DATA_OUTPUT_STR(str);
         DATA_OUTPUT_CHAR(' ');
      }
   } else if(type == RAWCALIBRATED || type == FILTEREDCALIBRATED) {
      for(i = 0; i<count; i++)
         dataf[i] = data[i];
      s->apply_calibration(dataf);
      /* if the data is nan, which means we aren't calibrated, show N/A */
      if(isnan(dataf[0])) {
         DATA_OUTPUT("N/A\n");
         return;
      }
      for(i = 0; i<count; i++)
         DATA_OUTPUT("%.4f ", dataf[i]);
   } else
      DATA_OUTPUT_STR("Invalid output type\n");
   DATA_OUTPUT_CHAR('\n');
}

void sensor_get_calibrated(struct sensor_typ *s, float *data)
{
   DATA_LOCK;
   int32_t tmp[3];
   int i;
   for(i = 0; i<3; i++)
      tmp[i] = s->filtered[i];
   DATA_UNLOCK;
   for(i = 0; i<s->dimensions; i++)
     data[i] = tmp[i] / (float)(1 << s->shift);
   s->apply_calibration(data);
}

static void update_failure(struct sensor_typ *s)
{
   /* make sure we got data in the last 10000 ticks */
   DATA_LOCK;
   uint32_t lasttick = s->lastinputtick, ticks = getticks();
   DATA_UNLOCK;

   *s->failure = ticks - lasttick > 10000;
}

static void calculate_rates(struct sensor_typ *s)
{
   if(s->inputticks)
      *s->inputrate = .95**s->inputrate + .05/ticks2seconds(s->inputticks);

   float period = eeprom_read_float_safe(s->outputrate);
   if(period != 0)
      period = 1/period;
   uint32_t outputticks = seconds2ticks(period);
   /* if the rate changed, then reset the last output tick */
   if(outputticks != s->outputticks) {
      s->lastoutputtick = 0;
      s->outputticks = outputticks;
   }
}

void sensor_update(struct sensor_typ *s, int32_t *datain)
{
   int count = s->dimensions;
   uint8_t i;

   /* accumulate data for still points */
   still_accumulate(s, datain);

   /* accumulate data */
   for(i = 0; i<count; i++)
      s->accum[i] += datain[i];
   s->inputcount++;

   uint32_t tick = getticks();
   uint32_t diff = tick - s->lasttick;

   /* calculate input ticks */
   s->inputticks = tick - s->lastinputtick;
   s->lastinputtick = tick;

   /* only try to keep up with the output rate, or at least 8hz,
      otherwise bail out early to save cpu */
   uint32_t desiredticks = s->outputticks;
   const uint16_t maxticks = (F_CPU/1024) / 8;
   if(!desiredticks || desiredticks > maxticks)
      desiredticks = maxticks;

#ifdef __AVR__ /* disabled in emulation for high-speed data replay,
		  and when plotting. */
   if(!sensor_disable_calc && diff < desiredticks)
      return;
#endif

   if(diff > desiredticks*2) /* if we are clearly not keeping up */
      s->lasttick = tick;
   else
      s->lasttick += desiredticks;

   /* fill in raw data with accumulated data */
   for(i = 0; i<count; i++) {
      s->raw[i] = s->accum[i] / s->inputcount;
      s->accum[i] = 0;
   }

   s->inputcount = 0;

   /* filter the raw data */
   int shift = 7;
   for(i = 0; i<s->dimensions; i++) {
     int32_t val = (s->raw[i]<<s->shift) - s->filtered[i];
     if(val*s->persist[i] < 0) /* we crossed zero! */
       s->persist[i] = 0;
     else
       s->persist[i] += val;

     int mshift = 8 - abs(s->persist[i]) / 1000;
     if(mshift < shift)
       shift = mshift;
     if(shift < 0)
       shift = 0;
   }

   for(i = 0; i<s->dimensions; i++)
     s->filtered[i] += (((s->raw[i]) << s->shift) >> shift) - (s->filtered[i]>>shift);

   /* output data if needed */
   diff = tick - s->lastoutputtick;
   if(s->outputticks && diff > s->outputticks) {
      /* rate change or we are clearly not keeping up */
      if(!s->lastoutputtick || diff < s->outputticks*2)
         s->lastoutputtick = tick;
      else
         s->lastoutputtick += s->outputticks;

      sensor_show_output(s, s->name);
   }
}

void sensor_poll(void)
{
   update_failure(&accel);
   update_failure(&mag);

   calculate_rates(&accel);
   calculate_rates(&mag);

   /* to speed stuff up when plotting */
   if(!sensor_disable_calc) {
      calc_box_pitch_roll();
      calc_box_yaw_dip();

      still_poll();
      calibration_accelfast_poll();
      calibration_magfast_poll();
   }
}

void sensor_restore_calibration(void)
{
   calibration_restore_accelfast();
   calibration_restore_magfast();

   calibration_restore_accel();
   calibration_restore_mag();
   calibration_restore_magalign();
#ifndef __AVR_AT90USB1287__
   calibration_restore_magdip();
#endif
}

void _sensor_store_symmetric_matrix(float *P, float *StoredP, int dim)
{
   int i;
   for(i=0; i<dim; i++)
      eeprom_write_block_safe(P+i*dim, StoredP+i*(i+1)/2, (i+1)*sizeof *P);
}

void _sensor_restore_symmetric_matrix(float *P, float *StoredP, int dim)
{
   int i;
   for(i=0; i<dim; i++)
      eeprom_read_block_safe(P+i*dim, StoredP+i*(i+1)/2, (i+1)*sizeof *P);
}

int sensor_checknotlast(float lastx[3], float x[3], float tolerance)
{
   if(dist2(x, lastx) < tolerance)
      return 0;
   memcpy(lastx, x, 3*sizeof *lastx);
   return 1;
}
