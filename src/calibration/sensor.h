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

#ifdef MAKE_SENSOR_ACCESSORS

#include <data/dataenums.h>
#include <data/dataarrays.h>
DATA_ACCESSOR(name=sensors dirname=sensors)

static const char raw[] PROGMEM = "raw";
static const char filtered[] PROGMEM = "filtered";
static const char rawcalibrated[] PROGMEM = "rawcalibrated";
static const char filteredcalibrated[] PROGMEM = "filteredcalibrated";
const char *const outputtypes[] PROGMEM = {raw, filtered, rawcalibrated,
                                           filteredcalibrated, 0};

#define BUILD_SENSOR_ACCESSORS(NAME, DIMS, VISIBLENAME) \
    DATA_ACCESSOR(name=NAME dirname=NAME visiblevarname=VISIBLENAME \
                  dir=sensors)                                      \
    DATA_ACCESSOR(name=outputtype mem=eeprom type=enumerant \
                  varname=NAME##outputtype enumvalues=outputtypes writable \
                  dir=NAME) \
    DATA_ACCESSOR(name=failure type=bool mem=sram varname=NAME##failure \
                  dir=NAME) \
    DATA_ACCESSOR(name=output dir=NAME) \
    DATA_ACCESSOR(name=inputrate type=float mem=sram \
                  varname=NAME##inputrate dir=NAME) \
    DATA_ACCESSOR(name=outputrate type=float mem=eeprom \
                  varname=NAME##outputrate writable dir=NAME) \
    DATA_ACCESSOR(name=NAME##stilltolerance type=uint16_t mem=eeprom \
                  varname=NAME##stilltolerance writable dir=calibration)

DATA_OPERATOR(get, name=output dir,
              extern struct sensor_typ dir;
              sensor_show_output(&dir, 0);
    )

BUILD_SENSOR_ACCESSORS(accel, 3, sca3000_detected)
BUILD_SENSOR_ACCESSORS(mag, 3, ad7799_init_status)

#endif

enum sensor_outputtype {RAW, FILTERED, RAWCALIBRATED, FILTEREDCALIBRATED};

/* every pointer is to ram unless commented, many variables are pointers
   because they point to globals for data accessors */
struct sensor_typ {
   int dimensions;
   uint8_t *outputtype /* EEMEM */;
   uint8_t *failure;
   int32_t *accum;
   int32_t *raw;
   int32_t *filtered;
   uint8_t shift; /* fixed point for filtered data */

   uint8_t stillstate;
   int32_t *stillaccum;
   int32_t *stillaccummin, *stillaccummax;
   uint16_t stilltolerance;
   int16_t stillaccumcount;

   uint32_t lasttick;

   float *inputrate;
   uint32_t inputticks, lastinputtick;
   uint8_t inputcount;

   float *outputrate /* EEMEM */;
   uint32_t outputticks, lastoutputtick;

   int32_t persist[3];

   void (*apply_calibration)(float *data);

   const char *name; /* PROGMEM */
};

extern struct sensor_typ accel, mag;

extern volatile uint8_t outputlock;

void sensor_get_calibrated(struct sensor_typ *s, float *data);
void sensor_update(struct sensor_typ *s, int32_t *datain);

void sensor_init(void);
void sensor_poll(void);

void sensor_restore_calibration(void);

#define sensor_store_symmetric_matrix(P, StoredP, dim) \
   _sensor_store_symmetric_matrix((float*)P, StoredP, dim)
void _sensor_store_symmetric_matrix(float *P, float *StoredP, int dim);

#define sensor_restore_symmetric_matrix(P, StoredP, dim) \
   _sensor_restore_symmetric_matrix((float*)P, StoredP, dim)
void _sensor_restore_symmetric_matrix(float *P, float *StoredP, int dim);

int sensor_checknotlast(float lastx[3], float x[3], float tolerance);

extern uint8_t sensor_disable_calc;

extern uint16_t accelstilltolerance, magstilltolerance;
