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

#include <avrdata.h>

#include <data/data.h>
#include <data/databasictypes.h>
#include <data/datadirectories.h>
#include <data/dataarrays.h>
#include <data/dataenums.h>
#include <data/databools.h>

DATA_ACCESSOR(name=mag dir=settings dirname=magsettings)

enum { AD7799_470_HZ = 1, AD7799_242_HZ, AD7799_123_HZ, AD7799_62_HZ,
       AD7799_50_HZ, AD7799_39_HZ, AD7799_33_2_HZ, AD7799_19_6_HZ,
       AD7799_16_7_1_HZ, AD7799_16_7_2_HZ, AD7799_12_5_HZ, AD7799_10_HZ,
       AD7799_8_33_HZ, AD7799_6_25_HZ, AD7799_4_17_HZ };

static const char _6_4[] PROGMEM = "6.4";
static const char _3_2[] PROGMEM = "3.2";
static const char _1_6[] PROGMEM = "1.6";
static const char _0_8[] PROGMEM = "0.8";
const char *const magranges[] PROGMEM = {_6_4, _3_2, _1_6, _0_8, 0};

uint8_t magrange EEMEM = 2; /* default to 1.6 gauss */
DATA_ACCESSOR(name=range mem=eeprom type=enumerant varname=magrange
              enumvalues=magranges writable dir=magsettings);

DATA_OPERATOR(set, name=range type mem varname enumvalues writable,
              extern const char *const enumvalues[];
              extern uint8_t varname;
              static const char enumname[] PROGMEM = #enumvalues;
              uint8_t data_tmp = get_byte_##mem(&varname);
              data_enum_set(enumvalues, &data_tmp, data_param, enumname);
              put_byte_##mem(&varname, data_tmp);
              hmc105x_setrange(eeprom_read_byte_safe(&varname));
              )

DATA_ACCESSOR(name=saturated mem=sram dir=mag)
DATA_OPERATOR(get, name=saturated,
              extern uint8_t hmc_saturated[3];
              static const char yes[] PROGMEM = "yes";
              static const char no[] PROGMEM = "no";
              DATA_OUTPUT("x: %S  y: %S  z: %S\n",
                          hmc_saturated[0] ? yes : no,
                          hmc_saturated[1] ? yes : no,
                          hmc_saturated[2] ? yes : no);
    )

DATA_ACCESSOR(name=setreset mem=sram dir=mag)
DATA_OPERATOR(get, name=setreset,
              extern uint8_t hmc_setresetfailed[3];
              static const char passed[] PROGMEM = "passed";
              static const char failed[] PROGMEM = "failed";
              DATA_OUTPUT("x: %S  y: %S  z: %S\n",
                          hmc_setresetfailed[0] ? failed : passed,
                          hmc_setresetfailed[1] ? failed : passed,
                          hmc_setresetfailed[2] ? failed : passed);
    )


static const char slow[] PROGMEM = "slow";
static const char medium[] PROGMEM = "medium";
static const char fast[] PROGMEM = "fast";
const char *const magbandwidths[] PROGMEM = {slow, medium, fast, 0};


extern uint8_t magbandwidth;
DATA_ACCESSOR(name=bandwidth mem=eeprom type=enumerant varname=magbandwidth
              enumvalues=magbandwidths writable dir=magsettings);

DATA_OPERATOR(set, name=bandwidth type mem varname enumvalues writable,
              extern const char *const enumvalues[];
              extern uint8_t varname;
              static const char enumname[] PROGMEM = #enumvalues;
              uint8_t data_tmp = get_byte_##mem(&varname);
              data_enum_set(enumvalues, &data_tmp, data_param, enumname);
              put_byte_##mem(&varname, data_tmp);
	      magsetup_setbandwidth();
              )

#include <calibration/sensor.h>

/* for the magnetometer, the still tolerance needs to change depending
   on the range and bandwidth settings */
uint16_t magsetup_fixstilltolerance(uint16_t tolerance)
{
   extern uint8_t hmc_gain;
   uint8_t mul = 1, shift = 7-hmc_gain;
   switch(eeprom_read_byte_safe(&magbandwidth)) {
   case 0:
      mul = 1;
   case 1:
      mul = 2;
   case 2:
      mul = 6;
   }

   uint32_t val = ((uint32_t)tolerance*mul)<<shift;

   if(val > 65536)
      val = 65536;
   return val;
}

void magsetup_setbandwidth(void)
{
   uint8_t samples, rate;
   switch(eeprom_read_byte_safe(&magbandwidth)) {
   case 0:
      samples = 8;
      rate = AD7799_123_HZ;
      break;
   case 1:
      samples = 2;
      rate = AD7799_123_HZ;
      break;
   case 2:
      samples = 1;
      rate = AD7799_470_HZ;
      break;
   default:
      DEBUG("No valid mag bandwidth set\n");
      return;
   }

   void hmc105x_setbandwidth(uint8_t samples, uint8_t rate);
   hmc105x_setbandwidth(samples, rate);
}

extern void hmc105x_setrange(uint8_t range);
void magrange_init(void)
{
   hmc105x_setrange(eeprom_read_byte_safe(&magrange));
   magsetup_setbandwidth();
}

static uint8_t lastsat;
void magrange_set(uint8_t range)
{
   /* if setting the range, clear any saturation status */
   lastsat = 0;

   eeprom_write_byte_safe(&magrange, range);
   hmc105x_setrange(range);
}

uint8_t magrange_get(void)
{
   return eeprom_read_byte_safe(&magrange);
}

/* return 1 only if transition from not saturated to saturated */
uint8_t magrange_saturated(void)
{
   extern uint8_t hmc_saturated[3];
   uint8_t ret, sat = hmc_saturated[0] || hmc_saturated[1] || hmc_saturated[2];
   ret = !lastsat && sat;
   lastsat = sat;
   return ret;
}
