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

/* define standard avr memory access functions to access
   ram for any memory since we aren't on an avr */

#include <stdint.h>

#define EEMEM
#define PROGMEM
#define printf_P printf
#define vsnprintf_P vsnprintf
#define snprintf_P snprintf
#define strcpy_P strcpy
#define strncpy_P strncpy
#define fprintf_P fprintf
#define sscanf_P sscanf
#define memcpy_P memcpy
#define strcmp_P strcmp
#define strncmp_P strncmp
#define strchr_P strchr
#define strlen_P strlen
#define puts_P   puts
#define fputs_P  fputs

#define eeprom_read_word(x) (*x)
#define eeprom_read_byte(x) (*x)
#define eeprom_read_block memcpy

#define eeprom_write_word(x, val) (*x = val)
#define eeprom_write_byte(x, val) (*x = val)
#define eeprom_write_block(src, dst, len) memcpy(dst, src, len)

static uint16_t pgm_read_word(const void *x) { return *((uint16_t*)x); }
static void *pgm_read_ptr(const void *x) { return *((void**)x); }
static uint8_t pgm_read_byte(const void *x) { return *((uint8_t*)x); }

#include <stdio.h>
static inline void ltoa(long val, char *string, int base)
{
   if(base != 10) {
      fprintf(stderr, "itoa hack can only use base 10!\n");
      return;
   }
   sprintf(string, "%ld", val);
}

#define itoa(val, str, base) ltoa(val, str, base)
