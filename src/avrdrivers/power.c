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

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <data/data.h>
#include <data/databasictypes.h>
#include <data/dataenums.h>
#include <data/databools.h>
#include <data/datadirectories.h>

#include <calibration/sensor.h>
#include <calibration/still.h>
#include <calibration/cal.h>

#ifdef __AVR_AT90USB1287__
#include <MyUSB/Drivers/USB/USB.h>
#undef bool
#endif

#include "pin.h"
#include "watchdog.h"
#include "itertimer.h"
#include "keypad.h"
#include "power.h"
#include "hotwire.h"

uint8_t usepower;

/* routines to manage power consumption */
DATA_ACCESSOR(name=power visiblevarname=usepower dirname=power)

void sca3000_enter_motion_detection(void);
void sca3000_exit_motion_detection(void);
int sca3000_filter_data(int32_t data[3], uint16_t count[3]);

void hmc105x_powerdown(void);
void hmc105x_powerup(void);

static const char none[] PROGMEM = "none";
static const char idle[] PROGMEM = "idle";
static const char power_save[] PROGMEM = "powersave";
static const char power_down[] PROGMEM = "powerdown";
const char *const sleeptypes[] PROGMEM = {none, idle, power_save, power_down, 0};

enum SleepModes {NONE, IDLE, POWER_SAVE, POWER_DOWN};

uint8_t stillsleeptype EEMEM = POWER_SAVE;
DATA_ACCESSOR(name=stillsleeptype mem=eeprom type=enumerant varname=stillsleeptype
              enumvalues=sleeptypes writable dir=power)
uint16_t stillsleeptimeout EEMEM = 60;
DATA_ACCESSOR(name=stillsleeptimeout mem=eeprom type=uint16_t
              varname=stillsleeptimeout writable dir=power)
float stillsleeptimeoutstart;

uint8_t buttonidlesleeptype EEMEM = POWER_DOWN;
DATA_ACCESSOR(name=buttonidlesleeptype mem=eeprom type=enumerant
              varname=buttonidlesleeptype enumvalues=sleeptypes writable
              dir=power)
uint16_t buttonidletimeout EEMEM = 600;
DATA_ACCESSOR(name=buttonidletimeout mem=eeprom type=uint16_t
              varname=buttonidletimeout writable dir=power)
float buttonidletimeoutstart;

uint8_t powerwakeonmotion EEMEM = 1;
DATA_ACCESSOR(name=wakeonmotion type=bool mem=eeprom 
              varname=powerwakeonmotion writable dir=power)

uint8_t lastsleeptype;
DATA_ACCESSOR(name=lastsleeptype type=enumerant enumvalues=sleeptypes dir=power)

DATA_OPERATOR(get, name=lastsleeptype,
              extern const char *const sleeptypes[];
              extern uint8_t lastsleeptype;
              data_enum_get(sleeptypes, lastsleeptype);
              lastsleeptype = 0; /* reset to zero */
              )

uint32_t lcdontime;

void power_init(void)
{
   usepower = 1;
   stillsleeptimeoutstart = buttonidletimeoutstart = gettime();
   lcdontime = getticks();
}

extern void ra8816_standby_mode(uint8_t);

extern volatile uint8_t button_pressed;

volatile uint8_t button_wakeup;

/* determine if we should sleep, and if so, go to sleep */
void power_poll(void)
{
   float time = gettime();

   uint16_t still_timeout = eeprom_read_word_safe(&stillsleeptimeout);
   uint16_t button_idle_timeout = eeprom_read_word_safe(&buttonidletimeout);

   /* if usb is plugged in, never sleep.  Idle mode works fine,
      but deeper sleeps do not wake from usb interrupts,
      there is no shortage of power when plugged into usb anyway */
#ifdef __AVR_AT90USB1287__
   if(USB_VBUS_GetStatus()) {
      stillsleeptimeoutstart = buttonidletimeoutstart = time;
      return;
   }
#endif

   /* if we aren't pretty still, reset still sleep timeout */
   if(accel.stillstate != STILL)
      stillsleeptimeoutstart = time;

   /* if a button was pressed recently */
   if(button_pressed) {
      buttonidletimeoutstart = time;
      button_pressed = 0;
   }

   /* determine what sleep mode if any is appropriate */
   uint8_t sleeptype = NONE;

   uint8_t stillsleep = 0;
   uint8_t wakeonmotion = 0;

   if(time - stillsleeptimeoutstart > still_timeout)
       /* for still, must not have buttons pressed within the timeout either */
       if(time - buttonidletimeoutstart > still_timeout)
         if((sleeptype = eeprom_read_byte_safe(&stillsleeptype))) {
            wakeonmotion = eeprom_read_byte_safe(&powerwakeonmotion);
            stillsleep = 1;
         }

   uint8_t ram_buttonidlesleeptype = eeprom_read_byte_safe(&buttonidlesleeptype);
   if(ram_buttonidlesleeptype > sleeptype
      && time - buttonidletimeoutstart > button_idle_timeout)
       sleeptype = ram_buttonidlesleeptype;

   /* not appropriate to sleep right now */
   if(sleeptype == NONE)
      return;

   /* reconfigure sensors in the new state */
   cli();
   if(wakeonmotion)
      sca3000_enter_motion_detection();

   hmc105x_powerdown();
   sei();

   /* let keypad wake us up */
   keypad_enable_wake();

   /* make sure laser is off */
   extern void laser_off(void);
   laser_off();

#ifndef __AVR_AT90USB1287__
   /* make sure hotwire is off */
   hotwire_disable();
#endif

   /* turn off watchdog */
   MCUSR = 0;
   wdt_disable();

   /* disable adc */
   ADCSRA &= ~_BV(ADEN);
   power_adc_disable();

   /* turn off timer to read from sensors */
   TIMSK2 &= ~_BV(TOIE2);

   /* disable periphial devices,
      if spi is disabled motion detection wakeup won't work */
   power_twi_disable();

   power_timer0_disable();
   power_timer1_disable();
   power_timer2_disable();
   power_timer3_disable();

 sleep:

   /* store last sleep type for debugging */
   lastsleeptype = sleeptype;

   /* we need to be able to wake up from watchdog to switch to a 
      deeper sleep mode, or disable wakeonmotion */
   if(sleeptype < ram_buttonidlesleeptype
      || (sleeptype == ram_buttonidlesleeptype && wakeonmotion))
      watchdog_set_wake();

   switch(sleeptype) {
   case IDLE:
      set_sleep_mode(SLEEP_MODE_IDLE);
      break;
   case POWER_DOWN:
#ifdef OLD_BOARD
       ks0713_standby_mode(1);
#else
       ra8816_standby_mode(1);
#endif
       extern void backlight_set(uint8_t, uint8_t);
       backlight_set(0, 0);
   case POWER_SAVE:
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);
      break;
   }

   /* reduce power if not waking on motion */
   extern void sca3000_powerdown(void);
   uint8_t sca_powereddown;
   if(!wakeonmotion && sleeptype > IDLE) {
       sca3000_powerdown();
       sca_powereddown = 1;
   } else
       sca_powereddown = 0;

   extern void spi_powersave(uint8_t save);
   spi_powersave(1);
   sleep_mode();
   spi_powersave(0);

   /* if we woke from a watchdog, might need to go to a deeper sleep */
   if(watchdog_woke()) {
      buttonidletimeoutstart -= 8; /* watchdog takes 8 seconds */
      /* test if we need to switch to deeper sleep mode */
      if(time - buttonidletimeoutstart > button_idle_timeout) {
         sleeptype = ram_buttonidlesleeptype;
         stillsleep = 0;
         /* can't wake on motion from this sleep mode */
         if(wakeonmotion)
            sca3000_exit_motion_detection();
         /* disable watchdog */
         wdt_disable();
      }
      goto sleep;
   }

   /* powerup device */
   void sca3000_init(void);
   if(sca_powereddown)
       sca3000_init();

   power_timer0_enable();
   power_timer1_enable();
   power_timer2_enable();
   power_timer3_enable();

   power_adc_enable();
   ADCSRA |= _BV(ADEN);

   /* turn off keypad wake */
   keypad_disable_wake();

   /* enable sensors */
   sca3000_exit_motion_detection();

   cli();
   hmc105x_powerup();
   sei();

   watchdog_init();

   /* reset timeouts */
   time = gettime();

   /* button idle timeout is not affected by motion wakeup,
      set still timeout to go off again in .5 seconds */
   keypad_poll();
   if(keys)
      buttonidletimeoutstart = time;
   keypad_clear();

   stillsleeptimeoutstart = time;

   /* flush sca3000 buffer after a delay (initial data is bad) */
   volatile int i; for(i = 0; i < 30000; i++);
   int32_t data[3];
   uint16_t count[3];
   sca3000_filter_data(data, count);

   if(sleeptype == POWER_DOWN) {
      lcdontime = time;
      ra8816_standby_mode(0);
   }

   /* turn on timer to read from sensors */
   TIMSK2 |= _BV(TOIE2);

   void settings_apply(void);
   settings_apply();
}
