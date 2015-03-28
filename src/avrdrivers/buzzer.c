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

#include "pin.h"

#if defined(__AVR_AT90USB1287__)
static const struct pin buzzerpin = MAKE_PIN(C, 6);
#elif defined(__AVR_ATmega2561__)
static const struct pin buzzerpin = MAKE_PIN(E, 3);
#endif

static uint16_t buzzcounts;

ISR(TIMER3_COMPA_vect)
{
  /* toggle pin */
  SET_PIN_OUTPUT_TOGGLE(buzzerpin);

  OCR3A += 3; /* keep interrupting at 8000000/1024/3 = 2.604khz */

  if(--buzzcounts == 0)
    TIMSK3 &= ~_BV(OCIE3A);
}

void buzzer_init(void)
{
#if defined(__AVR_AT90USB1287__)
   /* detect if buzzer can be driven on older cpu */
   SET_PIN_INPUT_PULLUP(buzzerpin);
#endif
}

/* beep for duration ms */
void buzzer_buzz(int duration)
{
#if defined(__AVR_AT90USB1287__)
   /* if still an input, and held low, then no buzzer,
      trying to buzz could damage hardware */
   if(!GET_PIN_INPUT(buzzerpin))
      return;
#endif

   /* start timer interrupt */
   TIFR3 |= _BV(OCF3A); /* clear pending interrupt */
   TIMSK3 |= _BV(OCIE3A);
   OCR3A = TCNT3 + 1; /* start interrupting soon */

   buzzcounts = 2*duration;
}
