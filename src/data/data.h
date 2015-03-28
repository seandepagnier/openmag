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

#include <stdio.h>
#include <string.h>

#include <stdint.h>

#ifndef __cplusplus
#include "debug.h"
#endif

typedef const char *(*data_accessor)(int, const char*);
#define DATA_UNIMPLEMENTED ((const char*)-1)
#define DATA_START_CHAR 0 /* null character is separator */
#define DATA_RESET "reset"
#define DATA_PROMPT "$-> "

/* redefine to something it is easier to generate code for */
#define get_byte_sram(val) *val
#define get_byte_eeprom eeprom_read_byte_safe
#define get_byte_flash pgm_read_byte
#define get_bytes_sram memcpy
#define get_bytes_eeprom eeprom_read_block_safe
#define get_bytes_flash memcpy_P

#define put_byte_sram(var, val) (*var = val)
#define put_byte_eeprom eeprom_write_byte_safe
#define put_bytes_sram(src, dst, size) memcpy(dst, src, size)
#define put_bytes_eeprom eeprom_write_block_safe

void data_process_cmd(char *cmd);
void data_process_interactive_cmd(char *cmd);

extern const char data_extraparammsg[];

#ifdef NO_DATA

#define DATA_OUTPUT_CHAR(c)
#define DATA_OUTPUT(s, ...)
#define DATA_INPUT(s, ...)

#else

#ifdef COMPILE_DATAGEN
#include <avrdata.h>
#endif

#ifdef __AVR__
/* on avr put format strings only in flash */
#define DATA_OUTPUT_CHAR(c) putchar(c)
#define DATA_OUTPUT(s, ...) \
    do { static const char data_out_tmp[] PROGMEM = s; \
         printf_P(data_out_tmp, ## __VA_ARGS__); } while(0)
#define DATA_OUTPUT_FUNC printf_P
#define DATA_OUTPUT_STR(s) fputs(s, stdout)
#define DATA_OUTPUT_P printf_P

#define DATA_INPUT(s, ...) \
    do { static const char data_in_tmp[] PROGMEM = s; \
    if(data_param) \
        sscanf_P(data_param, data_in_tmp, ## __VA_ARGS__); \
    else \
        printf_P(data_extraparammsg); \
    } while(0)
#else

/* special wrappers for on host to send to the right socket */
extern int datahost_server_printf (const char *__format, ...);

#define DATA_OUTPUT_CHAR(c) datahost_server_printf("%c", c)
#define DATA_OUTPUT datahost_server_printf
#define DATA_OUTPUT_FUNC datahost_server_printf
#define DATA_OUTPUT_STR(s) datahost_server_printf("%s", s)
#define DATA_OUTPUT_P datahost_server_printf
#define DATA_INPUT(...) \
    do { \
       if(data_param) \
          sscanf(data_param, ## __VA_ARGS__); \
       else \
          datahost_server_printf(data_extraparammsg); \
    } while(0)

#endif
#endif

#ifdef DATAGEN
#define DATA_ACCESSOR(ATTRIBUTES) \data_accessor ATTRIBUTES \data_end
#define DATA_OPERATOR(OP, ATTRIBUTES, CMDS) \
           \data_operator OP \attributes ATTRIBUTES \cmds CMDS \data_end
#else

#define DATA_ACCESSOR(X)
#define DATA_OPERATOR(X, Y, Z)
#endif

/* very common operators */

/* to query the name (for internal use) */
DATA_OPERATOR(_name, name,
    static char n[] PROGMEM = #name;
    return n;
)

/* if attribute visiblevarname is set, it is a uint8_t specifying
   whether to show or hide this accessor */
DATA_OPERATOR(_name, name visiblevarname,
    static char n[] PROGMEM = #name;
    extern uint8_t visiblevarname;
    if(visiblevarname)
        return n;
    return DATA_UNIMPLEMENTED;
)

/* query the type */
DATA_OPERATOR(type, type,
#ifndef DONT_USE_DATA_TYPE
    DATA_OUTPUT(#type"\n");
#endif
)

/* to query the memory */
DATA_OPERATOR(mem, mem,
#ifndef DONT_USE_DATA_MEM
    DATA_OUTPUT(#mem"\n");
#endif
)

/* to query the description */
DATA_OPERATOR(description, description,
#ifndef DONT_USE_DATA_DESCRIPTION
    DATA_OUTPUT(description"\n");
#endif
)

/* to query operations */
DATA_OPERATOR(ops, ,
    data_show_operations(implemented_ops);
)
