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

#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>

#include <util/delay.h>

#include <avrdata.h>

/* data interface */
#include <data/data.h>
#include <data/databasictypes.h>
#include <data/datadirectories.h>
#include <data/datastrings.h>

DATA_ACCESSOR(name=softwareversion type=string mem=flash
              varname=softwareversion)

DATA_ACCESSOR(name=settings dirname=settings)
DATA_ACCESSOR(name=stats dirname=stats)

#include "spi.h"

#define HMC_INITSR (DDRE |= _BV(PE4) | _BV(PE5))
#define HMC_SET   PORTE = (PORTE & ~_BV(PE4)) | _BV(PE5)
#define HMC_RESET PORTE = (PORTE & ~_BV(PE5)) | _BV(PE4)

#define HMC_SSDDR  DDRC
#define HMC_SSPORT PORTC
#define HMC_SS     PC2

#include "hmc105x.h"

#define SCA_INITSS  DDRC |= _BV(PC3)
#define SCA_CLEARSS PORTC |= _BV(PC3)
#define SCA_SETSS   PORTC &= ~_BV(PC3)

#define SCA_INTDDR DDRE
#define SCA_INTPORT PORTE
#define SCA_INTBV DDE6
#define SCA_ENABLE_INT EIMSK |= _BV(INT6);
#define SCA_DISABLE_INT EIMSK &= ~_BV(INT6);
#define SCA_AVR_INT_vect INT6_vect

#define SCA_RESETDDR DDRE
#define SCA_RESETPORT PORTE
#define SCA_RESET PE7

#define SCA_BEFORE_SS do { hmc_disable_interrupt();       \
                           HMC_SSPORT |= _BV(HMC_SS); } while(0)
#define SCA_AFTER_SS  do { HMC_SSPORT &= ~_BV(HMC_SS); \
                           hmc_enable_interrupt(); } while(0)

#include "sca3000.h"

#include "checkisr.h"
#include "watchdog.h"
#include "itertimer.h"
#include "keypad.h"
#include "magsetup.h"
#include "systemclockprescaller.h"
#include "power.h"
#include "buzzer.h"

#include <ramusage.h>

#include <linalg/vector.h>

#include <calibration/cal.h>
#include <calibration/calc.h>
#include <calibration/temperature.h>
#include <calibration/sensor.h>
#include <calibration/still.h>

extern void datadriver_init(void);
extern void datadriver_poll(void);

/* use an interrupt to service things that must run faster
   than the main loop with nested interrupts */
ISR(TIMER2_OVF_vect)
{
   static volatile uint8_t nest;
   if(nest)
      return;
   nest = 1;

   sei();

   datadriver_poll();

   /* read from keys if the lcd has been on for 1000 ticks or more */
   void fastrate(void);
   if(lcdontime < getticks() - 1000)
      fastrate();

   /* read from sensors */
   int32_t data[3];
   if(sca3000_filter_data(data)) {
      sensor_update(&accel, data);

      extern void pedometer_poll(void) __attribute__ ((weak));
      if(pedometer_poll)
         pedometer_poll();

      extern void mouse_update_shake_wheel(int32_t data[3]) __attribute__ ((weak));
      if(mouse_update_shake_wheel) {
         int32_t a[3] = {data[0], data[1], data[2]};
         calibration_apply_accelfast(a);
         mouse_update_shake_wheel(a);
      }

      extern void joystick_update(int sensor, int32_t data[3]) __attribute__ ((weak));
      if(joystick_update) {
         int32_t a[3] = {data[0], data[1], data[2]};
         calibration_apply_accelfast(a);
         joystick_update(0, a);
      }
   }

   /* record data for plots if we have them */
   extern void plots_poll(void) __attribute__ ((weak));
   if(plots_poll)
      plots_poll();

   if(hmc105x_filter_data(data)) {
      sensor_update(&mag, data);

      extern void mouse_update(int32_t data[3]) __attribute__ ((weak));
      extern void joystick_update(int sensor, int32_t data[3]) __attribute__ ((weak));
      if(mouse_update || joystick_update) {
         int32_t m[3] = {data[0], data[1], data[2]};
         calibration_apply_magfast(m);

         if(mouse_update)
            mouse_update(m);

         if(joystick_update)
            joystick_update(1, m);
      }
   }

   /* divide down more to only about 15hz for temperature */
   static uint16_t div2;
   if(div2++>300) {
      int16_t temp = sca3000_read_temp();
      if(temp)
         temperature_update(temp);
      div2 = 0;
   }

   cli();
   nest = 0;
}

/* setup the system, and initialize all the drivers we use on all devices */
void avrdrivers_init(void)
{
   /* turn off watchdog and disable jtag */
   MCUCR = _BV(JTD);
   wdt_disable();

   SetSystemClockPrescaler(0);

   datadriver_init();

   sca3000_startup();
   watchdog_init();
   ramusage_init(&_end);
   itertimer_init();
   buzzer_init();
   magrange_init();

   spi_init();
   hmc105x_init();
   sca3000_init();

   /* start timer to read from sensors */
   TCCR2A = 0;
   TCCR2B = _BV(CS22); /* polling (F_CPU/256/64 = 488) hz */
   TIMSK2 |= _BV(TOIE2);
   
   sensor_restore_calibration();
}
