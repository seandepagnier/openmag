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

#include <string.h>

#include <avr/lcd/lcdprintf.h>

#include "temp.h"
#include "itertimer.h"

#define RUNLEN 12

static const char *text;
static int pos;
static float lasttime;

void marquee_start(const char *t)
{
   text = t;
   pos = 0;
   lasttime = gettime();
}

void marquee_poll(void)
{
   float time = gettime();

   /* set speed of marquee based on temperature
      At low temperatures, the lcd is not legible when updating too quickly */
   float period = 340/temperature - 1.2;
   if(period < .15)
      period = .15;

   if(time - lasttime < period)
      return;

   lasttime = time;
   char buf[RUNLEN];
   int len = strlen_P(text);

   if(len <= RUNLEN) {
      memcpy_P(buf, text, len);
      memset(buf, ' ', RUNLEN - len);
   } else {
      int cp = len - pos;
      if(cp > RUNLEN)
         cp = RUNLEN;
      memcpy_P(buf, text + pos, cp);
      memcpy_P(buf + cp, text, RUNLEN-cp);

      if(++pos == len)
         pos = 0;
   }

   lcd_setLine(0, 0, buf, RUNLEN);
}
