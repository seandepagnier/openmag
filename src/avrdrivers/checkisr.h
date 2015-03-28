/* Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
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

/* install ISRS for every ISR so that if one occurs that shouldn't,
   it will tell us about it with a DEBUG output */

#define CHECK_ISR(NAME) ISR(NAME##_vect) { DEBUG("ISR " #NAME " occured"); }

#ifndef __AVR_ATmega2561__
CHECK_ISR(ADC)
#endif
CHECK_ISR(ANALOG_COMP)
CHECK_ISR(EE_READY)
CHECK_ISR(INT0)
CHECK_ISR(INT1)
CHECK_ISR(INT2)
CHECK_ISR(INT3)
CHECK_ISR(INT4)
CHECK_ISR(INT5)
//CHECK_ISR(INT6)
CHECK_ISR(INT7)
//CHECK_ISR(PCINT0)
CHECK_ISR(SPI_STC)
CHECK_ISR(SPM_READY)
CHECK_ISR(TIMER0_COMPA)
CHECK_ISR(TIMER0_COMPB)
//CHECK_ISR(TIMER0_OVF)
CHECK_ISR(TIMER1_CAPT)
CHECK_ISR(TIMER1_COMPA)
CHECK_ISR(TIMER1_COMPB)
CHECK_ISR(TIMER1_COMPC)
CHECK_ISR(TIMER1_OVF)
CHECK_ISR(TIMER2_COMPA)
CHECK_ISR(TIMER2_COMPB)
//CHECK_ISR(TIMER2_OVF)
CHECK_ISR(TIMER3_CAPT)
//CHECK_ISR(TIMER3_COMPA)
CHECK_ISR(TIMER3_COMPB)
CHECK_ISR(TIMER3_COMPC)
//CHECK_ISR(TIMER3_OVF)
CHECK_ISR(TWI)
CHECK_ISR(BADISR)
