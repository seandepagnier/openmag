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

/* the routines in avrlibc for reading and writing to eeprom are not
   interrupt safe

   I disable interrupt 2 (if it was enabled)
*/

#include <avr/interrupt.h>

#define AVRDATA_SKIP_EEPROM_DEFINE
#include <avrdata.h>

uint8_t
eeprom_read_byte_safe(const uint8_t *addr)
{
   uint8_t val;
   DATA_LOCK;
   val = eeprom_read_byte(addr);
   DATA_UNLOCK;
   return val;
}

uint16_t
eeprom_read_word_safe(const uint16_t *addr)
{
   DATA_LOCK;
   uint16_t val = eeprom_read_word(addr);
   DATA_UNLOCK;
   return val;
}

void
eeprom_read_block_safe(void *pointer_ram, const void *pointer_eeprom,
                       size_t n)
{
   int i;
   for(i = 0; i<n; i++)
      ((uint8_t*)pointer_ram)[i] =
	 eeprom_read_byte_safe((uint8_t*)pointer_eeprom + i);
}

float
eeprom_read_float_safe(float *var)
{
   float val;
   eeprom_read_block_safe(&val, var, sizeof val);
   return val;
}

void
eeprom_write_byte_safe(uint8_t *addr, uint8_t val)
{
   DATA_LOCK;
   eeprom_write_byte(addr, val);
   DATA_UNLOCK;
}

void
eeprom_write_word_safe(uint16_t *addr, uint16_t val)
{
   DATA_LOCK;
   eeprom_write_word(addr, val);
   DATA_UNLOCK;
}

void
eeprom_write_block_safe(const void *pointer_ram, void *pointer_eeprom,
                        size_t n)
{
   int i;
   for(i = 0; i<n; i++)
      eeprom_write_byte_safe((uint8_t*)pointer_eeprom + i,
                             ((uint8_t*)pointer_ram)[i]);
}

void
eeprom_write_float_safe(float *var, float val)
{
   eeprom_write_block_safe(&val, var, sizeof val);
}
