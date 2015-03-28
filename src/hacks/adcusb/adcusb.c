/* Copyright (C) 2007 Sean D'Epagnier <sean@depagnier.com>
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

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>

#define F_CPU 12000000UL
#include <util/delay.h>

#include <usb.h>

#include <stdlib.h>

/* lfuse 0xef atmega 644p */

int32_t adcdata;
uint16_t adcdatacount;
uint8_t adcdatamode;

enum { MODE_1X, MODE_10X };

int16_t getadc(void)
{
   /* AVCC with external cap at AREF ADC0 and ADC1 */
   if(adcdatamode == MODE_1X)
      ADMUX = _BV(REFS0) | _BV(ADLAR) | 0x10; 
   else
      ADMUX = _BV(REFS0) | _BV(ADLAR) | 0x09; 

   ADCSRA |= _BV(ADSC);   // Set the Start Conversion flag.
   while(ADCSRA & _BV(ADSC)); // Wait for conversion
   return (int16_t)ADCW >> 6; // sign extended 10-bit result
}

byte_t usb_setup ( byte_t data[8] )
{
    switch(data[1]) {
    case 0:
        if(adcdatacount) {
            data[0] = adcdata;
            data[1] = adcdata>>8;
            data[2] = adcdata>>16;
            data[3] = adcdata>>24;
            data[4] = adcdatacount;
            data[5] = adcdatacount>>8;
            data[6] = adcdatamode;

            adcdata = 0;
            adcdatacount = 0;
            return 7;
        }
        break;
    case 1:
        PORTC = data[2];
        break;
   }

   return 0;
}

int main(void) {
   /* turn on pullup on D- so we can be detected on usb */
   DDRD |= _BV(PD6);
   PORTD |= _BV(PD6);

   DDRC = 0xff; /* PORTC is general io output */

   usb_init();

   DIDR0 = 0xff; /* disable IO PORTA */

   ADCSRA = _BV(ADEN) | 0x3; /* enable adc */

   set_sleep_mode(SLEEP_MODE_STANDBY); /* go into standby when data is not requested */

   for(;;) {
      usb_poll();

      if(adcdatacount < 65535) {
         int16_t val = getadc();

         if(adcdatamode == MODE_1X) {
            val *= -10; /* make 1x reading like 10x reading, and invert it */
            if(abs(val) < 40)
               adcdatamode = MODE_10X;
         } else
            if(abs(val) > 400) {
               adcdatamode = MODE_1X;
               continue; /* throw away data since we might have saturated */
            }
            
         adcdata += val;
         adcdatacount++;
      }
      else
         sleep_mode();
   }
   return 0;
}
