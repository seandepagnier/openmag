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

/* operators for booleans, they are set to true or false, which is either
   0 or 1.  The variable type must be uint8_t

   To make a bool type, specify and accessor eg:

   uint8_t enbG;
   DATA_ACCESSOR(name=autoupdate mem=sram type=bool varname=enbG writable)
*/

DATA_OPERATOR(set, mem type=bool varname writable,
              extern uint8_t varname;
              uint8_t data_tmp = get_byte_##mem(&varname);
              data_setbool(&data_tmp, data_param);
              put_byte_##mem(&varname, data_tmp);
    )

DATA_OPERATOR(get, mem type=bool varname,
              extern uint8_t varname;
              uint8_t data_tmp = get_byte_##mem(&varname);
              data_getbool(&data_tmp);
    )

DATA_OPERATOR(values, type=bool,
              DATA_OUTPUT("true false \n");
    )
