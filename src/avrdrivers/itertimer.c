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

#include <avr/interrupt.h>
#include <avr/io.h>

#include <data/data.h>
#include <data/datadirectories.h>

static volatile uint32_t runtime;
ISR(TIMER3_OVF_vect)
{
   runtime++;
}

float ticks2seconds(float ticks)
{
   return 1024.0 * (ticks / F_CPU);
}

float seconds2ticks(float seconds)
{
   return (seconds / 1024.0) * F_CPU;
}

DATA_ACCESSOR(name=runtime dir=stats)
DATA_OPERATOR(get, name=runtime,
              extern float gettime(void);
              DATA_OUTPUT("%.2fs\n", gettime());
     )

uint32_t getticks(void)
{
   return (runtime << 16) + TCNT3;
}

uint32_t lastticks;
float avgiter;

DATA_ACCESSOR(name=mainloopfreq dir=stats)
DATA_OPERATOR(get, name=mainloopfreq,
              extern float avgiter;
              extern float ticks2seconds(float);
              DATA_OUTPUT("%.2fhz\n", 1/ticks2seconds(avgiter));
   )

/* return runtime in seconds */
float gettime(void)
{
   return ticks2seconds(getticks());
}

void itertimer_init(void)
{
   TCCR3A = 0;
   TCCR3B = _BV(CS32) | _BV(CS30);

   TCNT3 = 0; /* reset to 0 */
   /* enable timer interrupt */
   TIMSK3 |= _BV(TOIE3);
}

void itertimer_poll(void)
{
   uint32_t ticks = getticks();
   float iter = ticks - lastticks;
   lastticks = ticks;
   avgiter = .05*iter + .95*avgiter; /* lowpass it */
}
