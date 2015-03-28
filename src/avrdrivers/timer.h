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

void timer_init(void)
{
   cli();

   TCCR0A |= _BV(WGM10) | _BV(WGM11) | _BV(COM1A1) | _BV(COM1A0);
   TCCR0B |= _BV(CS10);

   TIMSK0 |= _BV(TOIE0);

   sei();
}

void timer_stop(void)
{
   TIMSK0 &= ~_BV(TOIE0);
}
