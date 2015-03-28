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

#include <avrdata.h>

#define MAKE_PIN(NAME, NUM) {&DDR##NAME, &PIN##NAME, &PORT##NAME, _BV(P##NAME##NUM)}
struct pin {
   volatile uint8_t *ddr, *pin, *port, bv;
};

#define SET_PIN_INPUT_PULLUP(p)   *p.ddr &= ~p.bv, *p.port |=  p.bv
#define SET_PIN_INPUT_NOPULLUP(p) *p.ddr &= ~p.bv, *p.port &= ~p.bv
#define SET_PIN_OUTPUT_HIGH(p)    *p.ddr |=  p.bv, *p.port |=  p.bv
#define SET_PIN_OUTPUT_LOW(p)     *p.ddr |=  p.bv, *p.port &= ~p.bv
#define SET_PIN_OUTPUT_TOGGLE(p)  *p.ddr |=  p.bv, *p.port ^= p.bv

#define GET_PIN_INPUT(p) (!!(*p.pin & p.bv))
