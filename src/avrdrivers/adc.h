/* adc driver Copyright (C) 2007 Sean D'Epagnier <sean@depagnier.com>
 *
 * This Driver is free software; you can redistribute it and/or
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

void adc_init(void) {
   DDRC=0x00;	/* Set ADC port as inputs. */
   PORTC=0x00;	/* DO NOT USE internal pull-up resistors.
                   They would mess up the measurement. */
   ADCSRA = _BV(ADEN) | _BV(ADSC) | 0x7;
}

int getadc(int off)
{
   ADMUX = off; /* | 0xc0;*/

   ADCSRA |= _BV(ADSC);   // Set the Start Conversion flag.
   while(ADCSRA & _BV(ADSC)); // Wait for conversion
   
   return ADCW;
}
