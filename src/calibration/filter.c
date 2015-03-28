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

#include <stdlib.h>
#include <stdint.h>

int32_t sqrt_int32_t(int32_t x)
{
   int32_t f = x, g = 0;
   while(f) {
      int32_t h = f + g;
      if(h*h <= x)
         g += f;
      f >>= 1;
   }
   return g;
} 

/* fixed point low-pass filter, uses quadratic equations to calculate
   the filter constant

   filter uses 32bit fixed point math with 12bits of fraction,
   this function has smooth derivatives,
   heavy filtering when barely changing and no filtering when changing fast

   cur is the current value,
   val is the latest value,
   thr is the movement threshold to calculate lowpass,
   d is optional persistant data to allow accumulation of error
   return the new filtered value

   A basic lowpass filter of the form:
   new = (1-lp)*cur * lp*val
   
   lp is the lowpass constant.

   The lowpass constant is computed based on the movement df:
   df = new - old;

   if df < thr, then
      lp = (df/thr)^2
   if df < thr*2, then
      lp = -(df/thr - 1)^2+1
   otherwise
      lp = 1
*/

int32_t filter_quadratic(int32_t cur, int32_t val, int32_t thr, int32_t *d)
{
   int32_t diff = val - cur, df, lpf;

   if(d) {
      /* accumulate difference if we don't cross raw data */
      if((diff < 0 && *d < 0) || (diff > 0 && *d > 0))
         *d += diff;
      else
         *d = diff;
      df = *d;
   } else
      df = diff;

   df = abs(df);

   if(df < thr)
      lpf = ((df*df)<<11)/(thr*thr);
   else
   if(df < thr*2) {
      int32_t t = (df<<11)/thr-(1<<12);
      lpf = -((t*t)>>11)+(1<<12);
   } else
      lpf = 1<<12;

   return (cur*((1<<12)-lpf) + lpf*val)>>12;
}
