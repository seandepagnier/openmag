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

/* include this file to use EEMEM and PROGMEM attributes
   as well as the standard functions to access them.

   Emulation is provided if not running on an avr.
*/

#include <stdint.h>

float gettime(void);

#ifdef __AVR__
#define PGM_STR_FMT "%S"

#include <avr/pgmspace.h>
#include <avr/eeprom.h>

uint8_t eeprom_read_byte_safe(const uint8_t *addr);
uint8_t eeprom_read_byte_safe(const uint8_t *addr);
uint16_t eeprom_read_word_safe(const uint16_t *addr);
void  eeprom_read_block_safe(void *pointer_ram, const void *pointer_eeprom,
			      size_t n);
float eeprom_read_float_safe(float *var);

void eeprom_write_byte_safe(uint8_t *addr, uint8_t val);
void eeprom_write_word_safe(uint16_t *addr, uint16_t val);
void eeprom_write_block_safe(const void *pointer_ram, void *pointer_eeprom,
                             size_t n);
void eeprom_write_float_safe(float *var, float val);

#ifndef AVRDATA_SKIP_EEPROM_DEFINE

// undefine if already macros
#ifdef eeprom_read_byte
#undef eeprom_read_byte
#undef eeprom_read_word
#undef eeprom_write_byte
#undef eeprom_write_word
#endif

#define eeprom_read_byte eeprom_safe_error
#define eeprom_read_word eeprom_safe_error
#define eeprom_write_byte eeprom_safe_error
#define eeprom_write_word eeprom_safe_error

#endif

static void *pgm_read_ptr(const void *x) { return (void*)pgm_read_word(x); }

#define DATA_LOCK    uint8_t oldtimsk2 = TIMSK2; TIMSK2 &= ~_BV(TOIE2)
#define DATA_UNLOCK  TIMSK2 = oldtimsk2

#else
#define PSTR(x) x
#define PGM_STR_FMT "%s"

#include "nonavrdata.h"

#define eeprom_read_byte_safe eeprom_read_byte
#define eeprom_read_word_safe eeprom_read_word
#define eeprom_read_block_safe eeprom_read_block
#define eeprom_read_float_safe(var) (*var)

#define eeprom_write_byte_safe eeprom_write_byte
#define eeprom_write_word_safe eeprom_write_word
#define eeprom_write_block_safe eeprom_write_block
#define eeprom_write_float_safe(var, val) (*var = val)

#define DATA_LOCK calibration_server_lock();
#define DATA_UNLOCK calibration_server_unlock();

#endif
