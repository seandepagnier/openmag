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

#include <data/data.h>
#include <data/databasictypes.h>
#include <data/dataenums.h>
#include <data/datadirectories.h>

#ifdef __AVR_AT90USB1287__
#include <MyUSB/Drivers/USB/USB.h>
#undef bool
#endif

#include "pin.h"
#include "itertimer.h"
#include "battery.h"
#include "power.h"

static void adc_start(uint8_t mux)
{
   ADMUX = mux;
   ADCSRA |= _BV(ADSC);   // Set the Start Conversion flag.
}

/* routines to monitor battery and system voltages as well as
   autocalibrate detection of battery charge state */
DATA_ACCESSOR(name=battery dirname=battery dir=power)

float batteryvoltage;
DATA_ACCESSOR(name=voltage type=float mem=sram varname=batteryvoltage
              dir=battery);

float batterychargedvoltage EEMEM = 4.1;
DATA_ACCESSOR(name=chargedvoltage type=float mem=eeprom
              varname=batterychargedvoltage dir=battery);

float batterydischargedvoltage EEMEM = 2.8;
DATA_ACCESSOR(name=dischargedvoltage type=float mem=eeprom
              varname=batterydischargedvoltage dir=battery);

enum BatteryStates {DISCHARGED, DISCHARGING, CHARGED, CHARGING};
static const char discharged[] PROGMEM = "discharged";
static const char discharging[] PROGMEM = "discharging";
static const char charged[] PROGMEM = "charged";
static const char charging[] PROGMEM = "charging";
const char *const batterystates[] PROGMEM = {discharged, discharging,
                                             charged, charging, 0};

uint8_t batterystate;
DATA_ACCESSOR(name=state mem=sram type=enumerant varname=batterystate
              enumvalues=batterystates dir=battery);

#define BATTERY_VOLTAGE_MUX 0xC0

static const struct pin regerr = MAKE_PIN(D, 0);
static const struct pin battchg = MAKE_PIN(D, 1);

uint8_t battery_not_freerunning;

void battery_init(void)
{
    SET_PIN_INPUT_PULLUP(regerr);
    SET_PIN_INPUT_PULLUP(battchg);

    DIDR0 |= _BV(PF0); /* disable digital io on first adc */
//    ADCSRA = _BV(ADEN) | _BV(ADPS2); /* enable adc divide by 16 */

    ADCSRA = _BV(ADEN) |  _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); /* enable adc divide by 128 */

    /* start first conversion */
    adc_start(BATTERY_VOLTAGE_MUX);
}

void battery_measurement(float voltage)
{
   /* don't lowpass at startup */
   if(gettime() < 5)
      batteryvoltage = voltage;
   else
      batteryvoltage = batteryvoltage*.9 + voltage*.1;

    /* fix charged voltage */
    if(eeprom_read_float_safe(&batterychargedvoltage)+.005 < batteryvoltage)
       eeprom_write_float_safe(&batterychargedvoltage, batteryvoltage);

#ifdef __AVR_AT90USB1287__
    if(USB_VBUS_GetStatus()) {
#endif
       /* see if the battery is charged */
       if(GET_PIN_INPUT(battchg)) {
          /* if we just switched to charged, or if the charged voltage is lower than
             the current voltage, then update it */
          if(batterystate != CHARGED)
             eeprom_write_float_safe(&batterychargedvoltage, batteryvoltage);
          batterystate = CHARGED;
#ifdef __AVR_AT90USB1287__
       } else
          batterystate = CHARGING;
#endif
    } else
       /* read regulator error flag */
       if(!GET_PIN_INPUT(regerr)) {
          if(batterystate != DISCHARGED)
             eeprom_write_float_safe(&batterydischargedvoltage, batteryvoltage);
          batterystate = DISCHARGED;
       } else
          batterystate = DISCHARGING;
}

void battery_poll(void)
{
#ifdef __AVR_ATmega2561__
   if(battery_not_freerunning)
      return; /* anemometer is handling adc */
#endif

    if(ADCSRA & _BV(ADSC))
       return; /* bail if no conversion ready */

    adc_start(BATTERY_VOLTAGE_MUX);

    battery_measurement(2*ADCW*2.54/1023.0);
}

/* return float from 0 to 1 indicating battery charge */
float battery_charge(void)
{
   float charged = eeprom_read_float_safe(&batterychargedvoltage);
   float discharged = eeprom_read_float_safe(&batterydischargedvoltage);

   return (batteryvoltage - discharged)/(charged - discharged);
}

/* return 0 - 4 to indicate battery charge */
int battery_status(void)
{
   int state = battery_charge() * 4 + 1;
   if(state > 4)
      state = 4;
   return state;
}
