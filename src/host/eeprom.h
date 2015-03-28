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

/* this file emulates eeprom memory on an avr
   don't use with nonavrdata.h */

#include <string.h>
#include <unistd.h>
#include <stdint.h>

char eeprom[4096];  /* should be big enough */

/* define our own eeprom accessors */
static uint8_t eeprom_read_byte(const uint8_t *eeprom_pointer)
{
   return eeprom[(long)eeprom_pointer];
}

static uint16_t eeprom_read_word(const uint16_t *eeprom_pointer)
{
   return *(uint16_t*)(eeprom+(long)eeprom_pointer);
}

static void eeprom_read_block(void *ram_pointer,
                              const void *eeprom_pointer, size_t n)
{
   memcpy(ram_pointer, eeprom + (long)eeprom_pointer, n);
}

static void eeprom_write_byte(void *eeprom_pointer, uint8_t byte)
{
   eeprom[(long)eeprom_pointer] = byte;
}

static void eeprom_write_word(void *eeprom_pointer, uint16_t word)
{
   *(uint16_t*)(eeprom+(long)eeprom_pointer) = word;
}

static void eeprom_write_block(const void *ram_pointer,
                        void *eeprom_pointer, size_t n)
{
   memcpy(eeprom + (long)eeprom_pointer, ram_pointer, n);
}


void eeprom_load(void)
{
   FILE *file = fopen("eepromdata", "r");
   if(!file)
      return;
   fread(eeprom, sizeof eeprom, 1, file);
   fclose(file);
}

void eeprom_save(void)
{
   FILE *file = fopen("eepromdata", "w");
   if(!file)
      return;
   fwrite(eeprom, sizeof eeprom, 1, file);
   fclose(file);
}
