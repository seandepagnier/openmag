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

/* routines for basic types, see databasictype_funcs.h
   this is like a library to the core parser system, and is not required */

/* operators get and set */
DATA_OPERATOR(set, mem type varname writable,
              extern type varname;
              data_set_##mem##_##type(&varname, data_param);
    )

DATA_OPERATOR(get, mem type varname,
              extern type varname;
              extern void data_get_##mem##_##type(type *);
              data_get_##mem##_##type(&varname);
              DATA_OUTPUT_CHAR('\n');
    )

/* clearable values */
DATA_OPERATOR(clear, type mem varname clearvalue,
              type tmp = clearvalue;
              extern type varname;
              put_bytes_##mem(&tmp, &varname, sizeof tmp);
    )
