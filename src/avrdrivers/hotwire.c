/* Copyright (C) 2009 Sean D'Epagnier <sean@depagnier.com>
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


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <avr/wdt.h>

#include <data/data.h>

#include "hotwire.h"
#include "pin.h"
#include "itertimer.h"
#include "power.h"
#include "battery.h"

static const struct pin vdden = MAKE_PIN(C, 5), hotwireen = MAKE_PIN(C, 6);
static const struct pin hotwireb = MAKE_PIN(F, 3);

static volatile int32_t hotwireval, hotwirecount;

enum hotwirestate hotwirestate;

extern uint8_t battery_not_freerunning;

uint8_t hotwiremode EEMEM = 1;

static uint8_t hotwiremode_ram;

#define GAIN_1X _BV(MUX0) | _BV(MUX1) | _BV(MUX3) | _BV(MUX4)
#define GAIN_10X _BV(MUX0) | _BV(MUX2) | _BV(MUX3)
#define REF3_0 _BV(REFS0) | _BV(ADLAR)
#define REF2_56 _BV(REFS1) | _BV(REFS0) | _BV(ADLAR)
#define REF1_1 _BV(REFS1) | _BV(ADLAR)

static void start_adc(void)
{
   switch(hotwiremode_ram) {
   default:
   case 0: ADMUX = GAIN_1X | REF2_56; break;
   case 1: ADMUX = GAIN_1X | REF1_1; break;
   case 2: ADMUX = GAIN_10X | REF3_0; break;
   case 3: ADMUX = GAIN_10X | REF2_56; break;
   case 4: ADMUX = GAIN_10X | REF1_1; break;
   }
   ADCSRA |= _BV(ADSC);   // Set the Start Conversion flag.
}

void hotwire_disable(void)
{
   SET_PIN_OUTPUT_LOW(vdden);
   SET_PIN_OUTPUT_HIGH(hotwireen);
   hotwirestate = DISABLED;

   battery_not_freerunning = 0; /* turn off battery */

   ADCSRA &= ~_BV(ADIE);
   battery_init();
}

static void hotwire_wait(void)
{
   hotwire_disable();

   DIDR0 &= ~_BV(PF3); /* enable digital io on hot wire pins */
  
   SET_PIN_INPUT_PULLUP(hotwireb);
   hotwirestate = WAITING;
}

void hotwire_enable(void)
{
   battery_not_freerunning = 1; /* turn off battery */

   SET_PIN_INPUT_NOPULLUP(hotwireb);
   DIDR0 |= _BV(PF2) | _BV(PF3); /* disable digital io on hot wire pins */

   SET_PIN_OUTPUT_HIGH(vdden);
   SET_PIN_OUTPUT_LOW(hotwireen);

   hotwirestate = ENABLED;

   start_adc();
   ADCSRA |= _BV(ADIE);

   hotwireval = 0;
   hotwirecount = 0;
}

void hotwire_poll(void)
{
   /* if out of power */
   if(!batterystate) {
      hotwire_disable();
      hotwirestate = POWERFAIL;
   }

   switch(hotwirestate) {
   case DISABLED:
   case POWERFAIL:
      hotwire_wait();
      break;
   case WAITING:
      /* if waiting, if the probe is inserted, enable */
      if(!GET_PIN_INPUT(hotwireb))
	 hotwire_enable();
      break;
   case ENABLED:
      /* if saturated, wait */
      if(hotwireval / hotwirecount > 31000) {
	 hotwire_wait();
	 hotwirestate = SATURATED;
      }	 
      break;
   case SATURATED:
      if(GET_PIN_INPUT(hotwireb))
	 hotwire_wait();
      else
	 hotwire_enable();
      break;
   }

   /* re-read eeprom settings (can read eeprom in fast interrupt)*/
   hotwiremode_ram = eeprom_read_byte_safe(&hotwiremode);
}

#ifndef __AVR_AT90USB1287__
ISR(ADC_vect)
{
   hotwireval += (int16_t)ADCW;
   hotwirecount++;
   start_adc();
}
#endif

int32_t hotwire_value(void)
{
   static int32_t last;
   if(hotwirecount) {
      uint8_t lastadcsra = ADCSRA;
      ADCSRA &= ~_BV(ADIE);
      int32_t val = hotwireval, count = hotwirecount;
      hotwireval = 0;
      hotwirecount = 0;
      ADCSRA |= lastadcsra;
      last = val / count;
   }
   return last;
}
