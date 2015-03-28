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

/* routines for enumerated types, right now enums are limited to 256 unique
   identifiers, and the variables to hold them must be uint8_t.

   To make a new enum type, simply make a null-terminated array in PROGMEM
   of strings in PROGMEM to specify all the possible values, eg:

   static const char val1[] PROGMEM = "test1";
   static const char val2[] PROGMEM = "test2";
   const char *const enumtest_vals[] PROGMEM = {val1, val2, 0};

   then specify accessors, eg:

   uint8_t enumtest;
   DATA_ACCESSOR(name=enumtest type=enum mem=sram varname=enumtest
                                 enumvalues=enumtest_vals writable)
*/

DATA_OPERATOR(get, type=enumerant mem varname enumvalues,
    extern const char *const enumvalues[];
    extern uint8_t varname;
    data_enum_get(enumvalues, get_byte_##mem(&varname));
)

DATA_OPERATOR(set, type=enumerant mem varname enumvalues writable,
    extern const char *const enumvalues[];
    extern uint8_t varname;
    static const char enumname[] PROGMEM = #enumvalues;
    uint8_t data_tmp = get_byte_##mem(&varname);
    data_enum_set(enumvalues, &data_tmp, data_param, enumname);
    put_byte_##mem(&varname, data_tmp);
)

DATA_OPERATOR(values, type=enumerant enumvalues,
    extern const char *const enumvalues[];
    data_enum_list(enumvalues);
)
