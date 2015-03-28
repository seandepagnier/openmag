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

DATA_OPERATOR(get, arraylen mem type varname,
              extern type varname[arraylen];
              extern void data_get_##mem##_##type(type *);
              data_getarray(varname, sizeof *varname, arraylen,
                            data_get_##mem##_##type);
    )

DATA_OPERATOR(type, type arraylen,
#ifdef USE_DATA_TYPE
              DATA_OUTPUT(#type"["#arraylen"]\n");
#endif
    )

/* 2d arrays */
DATA_OPERATOR(get, type mem arraylen count varname,
              extern type varname[count][arraylen];
              extern void data_get_##mem##_##type(type *);
              data_getarray_2D(varname, sizeof **varname, arraylen, count,
                               data_get_##mem##_##type);              
    )

DATA_OPERATOR(type, type arraylen count,
#ifdef USE_DATA_TYPE
              DATA_OUTPUT(#type"["#count"]["#arraylen"]\n");
#endif
    )

void data_getarray(void *data, int width, int len, void getdata(void*));
void data_getarray_2D(void *data, int width, int len, int count,
                      void getdata(void*));
