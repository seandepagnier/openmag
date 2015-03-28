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

#define MAKE_AUTOCAL_EXTERNS(NAME) \
    void calibration_clear_##NAME(void); \
    void calibration_restore_##NAME(void); \
    float calibration_level_##NAME(void); \
    void calibration_store_##NAME(void);  \
    extern float NAME##X[];         \
    extern float NAME##dev;	    \

MAKE_AUTOCAL_EXTERNS(accelfast)
MAKE_AUTOCAL_EXTERNS(accel)
void calibration_apply_accel(float *data);
void calibration_apply_accelfast(int32_t data[3]);

MAKE_AUTOCAL_EXTERNS(magfast)
MAKE_AUTOCAL_EXTERNS(mag)
MAKE_AUTOCAL_EXTERNS(magalign)
void calibration_apply_mag(float data[3]);
void calibration_apply_magfast(int32_t data[3]);
void calibration_mag_scale(float data[3]);

#ifndef __AVR_AT90USB1287__
MAKE_AUTOCAL_EXTERNS(magdip)
void calibration_apply_magdip(float data[3]);
#endif

#define MAKE_AUTOCAL_STATE(NAME, DIM, VISIBLENAME)      \
    DATA_ACCESSOR(name=NAME dir=calibration cal=NAME \
		  visiblevarname=VISIBLENAME)	     \
    float NAME##X[DIM] = {0};         \
    float NAME##StoredX[DIM] EEMEM; \

#define MAKE_AUTOCAL_STILL(NAME) \
    float NAME##dev;             \
    float NAME##Storeddev EEMEM = 0; \
    void calibration_store_##NAME(void) \
    { \
       eeprom_write_block_safe(NAME##X, NAME##StoredX, sizeof NAME##X); \
       eeprom_write_float_safe(&NAME##Storeddev, NAME##dev);			\
    } \
    void calibration_restore_##NAME(void) \
    { \
        eeprom_read_block_safe(NAME##X, NAME##StoredX, sizeof NAME##X); \
	NAME##dev = eeprom_read_float_safe(&NAME##Storeddev);	\
    }

#define MAKE_AUTOCAL_COVARIANCE(NAME, DIM) \
    float NAME##P[DIM][DIM]; \
    float NAME##StoredP[DIM*(DIM+1)/2] EEMEM;   \
    void calibration_store_##NAME(void) \
    { \
       eeprom_write_block_safe(NAME##X, NAME##StoredX, sizeof NAME##X);      \
       sensor_store_symmetric_matrix(NAME##P, NAME##StoredP, DIM);           \
    } \
    void calibration_restore_##NAME(void) \
    { \
        eeprom_read_block_safe(NAME##X, NAME##StoredX, sizeof NAME##X); \
        if(NAME##X[0] == 0) \
            calibration_clear_##NAME(); \
        else \
           sensor_restore_symmetric_matrix(NAME##P, NAME##StoredP, DIM);  \
    }

DATA_OPERATOR(clear, cal,
              calibration_clear_##cal();
    )

DATA_OPERATOR(get, cal,
              calibration_show_##cal(printf_P);
    )

void calibration_accelfast_poll(void);
int calibration_accel_stillcalibrate(void);

float calibration_get_mag_magnitude(void);
float calibration_get_mag_diperror(void);

int calibration_mag_stillcalibrate(void);
int calibration_magalign_stillcalibrate(void);
int calibration_magdip_stillcalibrate(void);
void calibration_mag_poll(const float a[3], const float m[3], float r, int havea);
void calibration_magalign_poll(const float a[3], const float m[3]);
void calibration_magfast_poll(void);

int calibration_mag_detectdistortions(void);

extern uint8_t usemagdip;
