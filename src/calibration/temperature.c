/* Copyright (C) 2007, 2008 Sean D'Epagnier <sean@depagnier.com>
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

/* driver for temperature */

#include <stdint.h>

#include <avrdata.h>

#include <data/data.h>
#include <data/databasictypes.h>
#include <data/datadirectories.h>
#include <data/dataenums.h>
#include <data/databools.h>

uint8_t temptype EEMEM;

static const char tempf[] PROGMEM = "F";
static const char tempc[] PROGMEM = "C";
static const char raw[] PROGMEM = "raw";
const char *const temptypes[] PROGMEM = {tempf, tempc, raw, 0};
DATA_ACCESSOR(name=temp dirname=temp visiblevarname=sca3000_detected dir=sensors)
DATA_ACCESSOR(name=outputtype mem=eeprom type=enumerant varname=temptype
              enumvalues=temptypes writable dir=temp)

DATA_ACCESSOR(name=output dir=temp)
DATA_OPERATOR(get, name=output dir=temp,
              temp_output();
    )

DATA_OPERATOR(set, name=output dir=temp,
              float temp;
              DATA_INPUT("%f", &temp);
              temp_set(temp);
    )

uint8_t autooutputtemp EEMEM;
DATA_ACCESSOR(name=autooutput type=bool mem=eeprom varname=autooutputtemp
              writable dir=temp)

float temperature = 256, temperatureoffset EEMEM;

/* take raw temp counts and convert to degrees celcius */
static float sca3000_temp_celcius(float temp)
{
    return 23 + (temp - 256) / 1.8;
}

/* take temperature in celcius and convert to raw counts */
static float sca3000_rawtemp_celcius(float temp)
{
   return 1.8*(temp - 23) + 256;
}

/* take raw temp counts and convert to degrees farenheight */
static float sca3000_temp_farenheight(float temp)
{
    return 73.4 + (temp - 256);
}

/* take temperature in celcius and convert to raw counts */
static float sca3000_rawtemp_farenheight(float temp)
{
   return temp - 73.4 + 256;
}

/* return the temperature in the current units, if c is not null,
   fill it with 'C', 'F', or 'R' */
float temp_get(char *c)
{
    float temp = temperature - eeprom_read_float_safe(&temperatureoffset);
    switch(eeprom_read_byte_safe(&temptype)) {
    case 0: /* farenheight */
       if(c)
          *c = 'F';
       return sca3000_temp_farenheight(temp);
    case 1: /* celcius */
       if(c)
          *c = 'C';
       return sca3000_temp_celcius(temp);
    case 2:
       if(c)
          *c = 'R';
       return temp;
    }
    return 0.0/0.0;
}

void temp_format(char *buf, int len)
{
    static const char fmt[] PROGMEM = "%.1f%c";

    char c;
    float temp = temp_get(&c);
    snprintf_P(buf, len, fmt, temp, c);
}

void temp_output(void)
{
   char buf[8];
   temp_format(buf, sizeof buf);
   DATA_OUTPUT("%s\n", buf);
}

/* set the temperature, essentially update
   the temperature offset to make the temperature correct
 */
void temp_set(float temp)
{
    float sca3000_rawtemp_farenheight(float);
    float sca3000_rawtemp_celcius(float);

    float rawtemp;
    switch(eeprom_read_byte_safe(&temptype)) {
    case 0: /* farenheight */
       rawtemp = sca3000_rawtemp_farenheight(temp);
       break;
    case 1: /* celcius */
       rawtemp = sca3000_rawtemp_celcius(temp);
       break;
    case 2: /* raw */
        rawtemp = temp;
        break;
    default:
       DATA_OUTPUT("Cannot set temperature, no temperature type selected\n");
       return;
    }

    eeprom_write_float_safe(&temperatureoffset,
                            temperature - rawtemp);
}

void temperature_update(int16_t data)
{
   float lp = temperature - data;
   lp = lp*lp / 100;
   if(lp > 1)
      lp = 1;
   temperature = (1-lp)*temperature + lp*data;

   if(eeprom_read_byte_safe(&autooutputtemp)) {
       DATA_OUTPUT("temp: ");
       temp_output();
   }
}
