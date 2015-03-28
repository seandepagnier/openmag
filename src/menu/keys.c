/* Copyright (C) 2008, 2008 Sean D'Epagnier <sean@depagnier.com>
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

#include <stdint.h>
#include <debug.h>
#include "keys.h"

uint8_t key_todigit(int key)
{
   ASSERT(key);

   if(key == 512)
      return 0;

   if(key & 1)
      return 1;

   return key_todigit(key >> 1) + 1;
}

extern uint16_t lastkeyup;
uint16_t key_lastup(void)
{
   uint16_t k = lastkeyup;
   lastkeyup = 0;
   return k;
}