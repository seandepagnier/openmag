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

#include <inttypes.h>
/* below is an implemetation for accessors for basic types,
   it is like a library to the core parser system, and is not required */

/* basic set and get routines ascii style */

#define MAKE_GET_EEPROM_OPERATOR(TYPE, PRINT) \
    void data_get_eeprom_##TYPE(TYPE *val) \
    { \
        TYPE tmp; \
        eeprom_read_block_safe(&tmp, val, sizeof tmp); \
        DATA_OUTPUT("%"PRINT, tmp); \
    }

#define MAKE_SET_EEPROM_OPERATOR(TYPE, SCAN) \
    void data_set_eeprom_##TYPE(TYPE *val, const char *data_param) \
    { \
        TYPE tmp; \
        DATA_INPUT("%"SCAN, &tmp); \
        eeprom_write_block_safe(&tmp, val, sizeof tmp); \
    }

#define MAKE_GET_OPERATOR(TYPE, PRINT) \
    void data_get_sram_##TYPE(TYPE *val) \
    { \
        DATA_OUTPUT("%"PRINT, *val); \
    } \
    MAKE_GET_EEPROM_OPERATOR(TYPE, PRINT) \
    void data_get_flash_##TYPE(TYPE *val) \
    { \
        TYPE tmp; \
        memcpy_P(&tmp, val, sizeof tmp); \
        DATA_OUTPUT("%"PRINT, tmp); \
    }

#define MAKE_SET_OPERATOR(TYPE, SCAN) \
    void data_set_sram_##TYPE(TYPE *val, const char *data_param) \
    { \
        DATA_INPUT("%"SCAN, val); \
    } \
    MAKE_SET_EEPROM_OPERATOR(TYPE, SCAN)

/* for uint8_t and int8_t types, we must read an int16_t because
   the format is not supported by scanf in avrlibc */
#define MAKE_SPECIAL_SET_OPERATOR(TYPE) \
    void data_set_sram_##TYPE(TYPE *val, const char *data_param) \
    { \
        int16_t tmp; \
        data_set_sram_int16_t(&tmp, data_param); \
        *val = tmp; \
    } \
    void data_set_eeprom_##TYPE(TYPE *val, const char *data_param) \
    { \
        TYPE tmp; \
        int16_t tmp2; \
        data_set_sram_int16_t(&tmp2, data_param); \
        tmp = tmp2; \
        eeprom_write_block_safe(&tmp, val, sizeof tmp); \
    }

MAKE_GET_OPERATOR(uint32_t, PRIu32)
MAKE_SET_OPERATOR(uint32_t, SCNu32)
MAKE_GET_OPERATOR(int32_t, PRId32)
MAKE_SET_OPERATOR(int32_t, SCNd32)

MAKE_GET_OPERATOR(uint16_t, PRIu16)
MAKE_SET_OPERATOR(uint16_t, SCNu16)
MAKE_GET_OPERATOR(int16_t, PRId16)
MAKE_SET_OPERATOR(int16_t, SCNd16)

MAKE_GET_OPERATOR(uint8_t, PRIu8)
#ifdef __AVR__
MAKE_SPECIAL_SET_OPERATOR(uint8_t)
#else
MAKE_SET_OPERATOR(uint8_t, SCNu8)
#endif
MAKE_GET_OPERATOR(int8_t, PRId8)
#ifdef __AVR__
MAKE_SPECIAL_SET_OPERATOR(int8_t)
#else
MAKE_SET_OPERATOR(int8_t, SCNd8)
#endif

MAKE_SET_OPERATOR(float, "g")
MAKE_GET_OPERATOR(float, "g")

MAKE_SET_OPERATOR(char, "c")
MAKE_GET_OPERATOR(char, "c")
