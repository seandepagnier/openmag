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

static inline void SetSystemClockPrescaler(uint8_t PrescalerMask)
{
   uint8_t tmp = (1 << CLKPCE);
   __asm__ __volatile__ (
      "in __tmp_reg__,__SREG__" "\n\t"
      "cli" "\n\t"
      "sts %1, %0" "\n\t"
      "sts %1, %2" "\n\t"
      "out __SREG__, __tmp_reg__"
      : /* no outputs */
      : "d" (tmp),
        "M" (_SFR_MEM_ADDR(CLKPR)),
        "d" (PrescalerMask)
      : "r0");
}
