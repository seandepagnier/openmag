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

#ifdef MAKE_CALC_ACCESSORS
#include <data/dataarrays.h>
#include <data/dataquaternions.h>

DATA_ACCESSOR(name=calc dirname=calc)

DATA_ACCESSOR(name=pitch mem=sram type=float varname=pitch dir=calc)
DATA_ACCESSOR(name=roll mem=sram type=float varname=roll dir=calc)
DATA_ACCESSOR(name=yaw mem=sram type=float varname=yaw dir=calc)
DATA_ACCESSOR(name=dip mem=sram type=float varname=dip dir=calc)

DATA_ACCESSOR(name=boxalignedaccel mem=sram type=float arraylen=3
              varname=boxalignedaccel dir=calc)
DATA_ACCESSOR(name=boxalignedmag mem=sram type=float arraylen=3
              varname=boxalignedmag dir=calc)

DATA_ACCESSOR(name=incline mem=sram type=float
	      varname=incline dir=calc)
DATA_ACCESSOR(name=azimuth mem=sram type=float
	      varname=azimuth dir=calc)
/* if incline or azimuth is requested, we have to compute it since it
   only auto-computes when in the survey thread of the menu */
DATA_OPERATOR(get, name=incline mem type varname,
	      calc_laser_incline_azimuth();
	      extern type varname;
              extern void data_get_##mem##_##type(type *);
              data_get_##mem##_##type(&varname);
              DATA_OUTPUT_CHAR('\n');
	      )

DATA_OPERATOR(get, name=azimuth mem type varname,
	      calc_laser_incline_azimuth();
	      extern type varname;
              extern void data_get_##mem##_##type(type *);
              data_get_##mem##_##type(&varname);
              DATA_OUTPUT_CHAR('\n');
	      )

DATA_ACCESSOR(name=inclination mem=eeprom type=float writable
              varname=inclination dir=calibration)
DATA_ACCESSOR(name=declination mem=eeprom type=float writable
              varname=declination dir=calibration)
DATA_ACCESSOR(name=fieldstrength mem=eeprom type=float writable
              varname=fieldstrength dir=calibration)

DATA_ACCESSOR(name=truenorth type=bool mem=eeprom
              varname=truenorth writable dir=settings)
#endif

void calc_boxalignment(int);
float calc_accellaseralignment(int);
float calc_maglaseralignment(int);

float calc_resolveheading(float heading);

void calc_box_pitch_roll(void);
void calc_box_yaw_dip(void);
void calc_laser_incline_azimuth(void);

extern float boxalignedaccel[3], boxalignedmag[3];
extern float pitch, roll, yaw, dip;
extern float inclination, declination, fieldstrength;
extern float temp;
extern float boxalignment[4], accellaseralignment[4], maglaseralignment[4];
extern float incline, azimuth;

extern uint8_t truenorth;
