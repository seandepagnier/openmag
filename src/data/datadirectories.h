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

/* directory operators, and accessor to the root directory */

/* query directory name */
DATA_OPERATOR(_dir, dir,
    static char d[] PROGMEM = #dir;
    return d;
)

/* query name of directory */
DATA_OPERATOR(_dirname, dirname,
    static char d[] PROGMEM = #dirname;
    return d;
)

/* list directory contents */
DATA_OPERATOR(ls, dirname,
    static char d[] PROGMEM = #dirname;
    data_list_names(d);
)

/* type */
DATA_OPERATOR(type, dirname,
#ifndef DONT_USE_DATA_TYPE
              DATA_OUTPUT("directory\n");
#endif
              )
