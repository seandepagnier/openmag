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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __AVR_AT90USB1287__
#include <avrdrivers/descriptors.h>
#include <avrdrivers/descriptors_defs.h>
#undef bool
#endif

#include <avrdrivers/avrdrivers.h>

#include <avrdata.h>

/* data interface */
#include <data/data.h>
#include <data/databasictypes.h>
#include <data/datadirectories.h>
#include <data/databools.h>

#include <avrdrivers/datadriver.h>

#include <avrdrivers/keypad.h>
#include <avrdrivers/itertimer.h>
#include <avrdrivers/buzzer.h>

#define MAKE_SENSOR_ACCESSORS
#include <calibration/sensor.h>

#if defined(__AVR_ATmega2561__) /* only enabled on bigger flash version */
#define MAKE_CALC_ACCESSORS
#include <calibration/calc.h>
#endif

#include <menu/menu.h>

SET_SOFTWARE_NAME("dusi");

#include <avrdrivers/power.h>
#include <avrdrivers/battery.h>
#include <avrdrivers/backlight.h>

#include "laser.h"

#include <util/delay.h>

#define RA8816_RESETDDR DDRC
#define RA8816_RESETPORT PORTC
#define RA8816_RESETPIN PINC
#define RA8816_RESET PC4

#include <avrdrivers/ra8816.h>

uint8_t magbandwidth EEMEM = 1; /* medium */

void fastrate(void)
{
   keypad_poll();
}

#define USE_LINEFEED
#include <avrdrivers/uart.h>

int main(void)
{
   keypad_init();

   ra8816_init();
   settings_apply();

   avrdrivers_init();

   power_init();
   battery_init();

   sei();

   /* main loop, stuff that isn't time critical */
   for(;;) {
       itertimer_poll();
       sensor_poll();
       battery_poll(); 
       power_poll();

       /* magnetic location calculation */
#if defined(__AVR_ATmega2561__) /* only enabled on bigger flash version */
       extern void calibration_calc_queued_location(void);
       calibration_calc_queued_location();
       wdt_reset();
#endif

       /* draw the menu graphics */
       menu_poll();
       /* send the framebuffer to display */
       wdt_reset();
       ra8816_write_framebuffer();
       wdt_reset();
   }
}
