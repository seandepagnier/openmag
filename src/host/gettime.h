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

#include <sys/time.h>

float ticks2seconds(float ticks)
{
   return ticks / 1000;
}

float seconds2ticks(float seconds)
{
   return seconds * 1000;
}

uint32_t getticks(void)
{
   struct timeval tv;
   gettimeofday(&tv, NULL);

   static long starttime;
   if(!starttime)
      starttime = tv.tv_sec;

   return ((tv.tv_sec - starttime)*1000000 + tv.tv_usec) / 1000;
}

float gettime(void)
{
   return ticks2seconds(getticks());
}