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


#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include <avr/wdt.h>

#include <debug.h>

#include <data/data.h>
#include <data/databasictypes.h>
#include <data/datadirectories.h>
#include <data/dataenums.h>
#include <data/databools.h>

#include <debug.h>

#include <avrdata.h>

#include <linalg/vector.h>
#include <linalg/rotate.h>
#include <linalg/quaternion.h>

#include <calibration/sensor.h>
#include <calibration/calc.h>
#include <calibration/cal.h>
#include <calibration/filter.h>

#include <avrdrivers/keypad.h>
#include <avrdrivers/descriptors.h>

extern uint8_t useusbjoystick;

DATA_ACCESSOR(name=joystick dirname=joystick)

DATA_ACCESSOR(name=rotation dir=joystick dirname=joystickrotation)

float joystickrotationangle EEMEM, joystickrotationx EEMEM;
float joystickrotationy EEMEM, joystickrotationz EEMEM;
DATA_ACCESSOR(name=angle mem=eeprom type=float varname=joystickrotationangle
              dir=joystickrotation writable)
DATA_ACCESSOR(name=x mem=eeprom type=float varname=joystickrotationx
              dir=joystickrotation writable)
DATA_ACCESSOR(name=y mem=eeprom type=float varname=joystickrotationy
              dir=joystickrotation writable)
DATA_ACCESSOR(name=z mem=eeprom type=float varname=joystickrotationz
              dir=joystickrotation writable)

enum JoystickInput {ROLL, PITCH, YAW, X, Y, Z};
static const char roll_s[] PROGMEM = "roll";
static const char pitch_s[] PROGMEM = "pitch";
static const char yaw_s[] PROGMEM = "yaw";
static const char x_s[] PROGMEM = "x";
static const char y_s[] PROGMEM = "y";
static const char z_s[] PROGMEM = "z";
const char *const joystickinputs[] PROGMEM = {roll_s, pitch_s, yaw_s,
                                              x_s, y_s, z_s, 0};

enum JoystickSensor {ACCEL, MAG, NONE};
static const char accel_s[] PROGMEM = "accel";
static const char mag_s[] PROGMEM = "mag";
static const char none_s[] PROGMEM = "none";
const char *const joysticksensors[] PROGMEM = {accel_s, mag_s, none_s, 0};

struct joystickaxis
{
   uint8_t *input, *sensor;
   int16_t *sensitivity;
   int16_t pos;
};

#define MAKE_AXIS(AXIS, DEFAULTSENSOR) \
   DATA_ACCESSOR(name=axis##AXIS dir=joystick dirname=axis##AXIS) \
   uint8_t axis##AXIS##input EEMEM = AXIS; \
   DATA_ACCESSOR(name=input type=enumerant mem=eeprom \
                 varname=axis##AXIS##input enumvalues=joystickinputs \
                 writable dir=axis##AXIS) \
   uint8_t axis##AXIS##sensor EEMEM = DEFAULTSENSOR; \
   DATA_ACCESSOR(name=sensor type=enumerant mem=eeprom \
                 varname=axis##AXIS##sensor enumvalues=joysticksensors \
                 writable dir=axis##AXIS) \
   int16_t axis##AXIS##sensitivity EEMEM = 256; \
   DATA_ACCESSOR(name=sensitivity type=int16_t mem=eeprom \
                 varname=axis##AXIS##sensitivity writable dir=axis##AXIS) \
   struct joystickaxis axis##AXIS = {&axis##AXIS##input, \
                                     &axis##AXIS##sensor, \
                                     &axis##AXIS##sensitivity};

#define NUM_AXES 6

MAKE_AXIS(0, 1)
MAKE_AXIS(1, 1)
MAKE_AXIS(2, 1)
MAKE_AXIS(3, 2)
MAKE_AXIS(4, 2)
MAKE_AXIS(5, 2)

struct joystickaxis *axes[] = {&axis0, &axis1, &axis2, &axis3, &axis4, &axis5};

#ifdef USE_JOYSTICK
void joystick_update(int sensor, int32_t data[3])
{
   if(!useusbjoystick)
      return;

   float nd[3] = {data[0], data[1], data[2]};

   float a, r[3];
   a = deg2rad(eeprom_read_float_safe(&joystickrotationangle));
   r[0] = eeprom_read_float_safe(&joystickrotationx);
   r[1] = eeprom_read_float_safe(&joystickrotationy);
   r[2] = eeprom_read_float_safe(&joystickrotationz);
   if(a) {
      float q[4];
      angvec2quat(q, a, r);
      rotvecquat(nd, q);
   }
   
   normalize(nd);

   static float lasto[2][3];
   memcpy(lasto[sensor], nd, sizeof nd);

   int i;
   for(i=0; i<NUM_AXES; i++)
      if(eeprom_read_byte_safe(axes[i]->sensor) == sensor) {
         int16_t val;
         uint8_t input = eeprom_read_byte_safe(axes[i]->input);
         int16_t sensitivity =
            eeprom_read_word_safe((uint16_t*)axes[i]->sensitivity);

         switch(input) {
         case ROLL: val = sensitivity*atan2(-nd[1], nd[2])/M_PI; break;
         case PITCH: val = sensitivity*asin(nd[0])/M_PI; break;
         case YAW: {
            float *other = lasto[!sensor];
            float q[4], z[3] = {0, 0, 1}, r[3] = {nd[0], nd[1], nd[2]};
            vec2vec2quat(q, other, z);
            rotvecquat(r, q);
            val = sensitivity*atan2(r[0], r[1])/M_PI;
         } break;
         case X: val = data[0]*sensitivity; break;
         case Y: val = data[1]*sensitivity; break;
         case Z: val = data[2]*sensitivity; break;
         }

         axes[i]->pos = filter_quadratic(axes[i]->pos, val, 64, 0);
       }
}

void joystick_transmit(void)
{
   /* don't do these calculations if there is no usb */
   if(!USB_VBUS_GetStatus())
      return;

   if(!useusbjoystick)
      return;

   /* Select the Joystick Report Endpoint */
   Endpoint_SelectEndpoint(JOYSTICK_EPNUM);

   /* Check if Joystick Endpoint Ready for Read/Write */
   if (!Endpoint_ReadWriteAllowed())
      return;

   /* Write Joystick Report Data */
   int i;
   for(i=0; i<NUM_AXES; i++)
      Endpoint_Write_Word(axes[i]->pos);

   Endpoint_Write_Byte(keys);

   /* Handshake the IN Endpoint - send the data to the host */
   Endpoint_ClearCurrentBank();
}
#endif
