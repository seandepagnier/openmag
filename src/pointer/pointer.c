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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "mouse.h"
#include "joystick.h"

#include <avrdrivers/descriptors.h>
#include <avrdrivers/descriptors_defs.h>

#include <avrdrivers/avrdrivers.h>

#include <avrdata.h>

/* data interface */
#include <data/data.h>
#include <data/databasictypes.h>
#include <data/datadirectories.h>
#include <data/databools.h>

#define UART_ALLOW_SETBAUD
#include <avrdrivers/datadriver.h>

#include <avrdrivers/keypad.h>
#include <avrdrivers/itertimer.h>

#include <calibration/cal.h>
#define MAKE_SENSOR_ACCESSORS
#include <calibration/sensor.h>
#define MAKE_CALC_ACCESSORS
#include <calibration/calc.h>

SET_SOFTWARE_NAME("pointerdevice");

uint8_t magbandwidth EEMEM = 2; /* fast */

uint8_t button1outputsshot EEMEM;
DATA_ACCESSOR(name=button1outputsshot type=bool mem=eeprom 
              varname=button1outputsshot writable dir=settings)

void fastrate(void)
{
   buttons_poll();
   mouse_pollbuttons();

   if(eeprom_read_byte_safe(&button1outputsshot)) {
      static uint8_t button1reset;
      if(keys & BUTTON_1) {
         if(!button1reset) {
            button1reset = 1;
            DATA_LOCK;
            DATA_OUTPUT("pitch: %f roll: %f yaw: %f dip: %f\n",
                  pitch, roll, yaw, dip);
            calc_laser_incline_azimuth();
            DATA_OUTPUT("incline: %f azimuth: %f\n", incline, azimuth);
            DATA_UNLOCK;
         }
      } else
         button1reset = 0;
   }
}

int main(void)
{
   buttons_init();
   avrdrivers_init();

   sei();

   /* main loop, stuff that isn't time critical */
   for(;;) {
      itertimer_poll();
      sensor_poll();

      wdt_reset();
   }
}
