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

/* this file emulates eeprom on a device and allows it be be
   filled with eeprom dumps to allow simulation of the algorithms
   on a host system */

#include <string.h>

char eeprom[8192];  /* should be big enough */
#define E2END (sizeof eeprom)

/* define our own eeprom accessors */
void eeprom_read_block(void *ram_pointer,
                       const void *eeprom_pointer, size_t n)
{
   memcpy(ram_pointer, eeprom + (long)eeprom_pointer, n);
}

void eeprom_write_block(const void *ram_pointer,
                        void *eeprom_pointer, size_t n)
{
   memcpy(eeprom + (long)eeprom_pointer, ram_pointer, n);
}

void readeedatafromfile(int fd)
{
   char buf[8192], *eeprom = 0;
   size_t len;
   /* read stdin and write to eeprom */
   while((len = read(fd, buf, sizeof buf)) > 0) {
      eeprom_write_block(buf, eeprom, len);
      eeprom += len;
   }
}
