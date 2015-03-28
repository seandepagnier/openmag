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

void data_setbool(uint8_t *var, const char *data_param)
{
    if(data_param[0] == 't')
       *var = 1;
    else if(data_param[0] == 'f')
       *var = 0;
    else
        DATA_OUTPUT("set bool, invalid parameter: %s\n", data_param);
}

void data_getbool(uint8_t *var)
{
    if(*var)
        DATA_OUTPUT("true\n");
    else
        DATA_OUTPUT("false\n");
}
