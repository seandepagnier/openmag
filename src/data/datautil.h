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

static char *nextspace(char *str)
{
    char *p = str;
    char quoted = 0;
    do {
        if(*p == '"')
            quoted = !quoted;
        p++;
        if(!*p)
            break;
    } while(*p != ' ' || quoted);
    return p;
}

void data_split_cmd(char *cmd, char **op, char **name, char **param)
{
    *op = cmd;
    while(**op == ' ')
        (*op)++;

    /* pull out name */
    *name = nextspace(*op);
    if(**name) {
        **name = '\0';
        do
            (*name)++;
        while(**name == ' ');
    }

    if(**name) {
        if(*(*param = nextspace(*name))) {
            **param = '\0';
            do
                (*param)++;
            while(**param == ' ');

            char *end = *param + strlen(*param);
            do
               end--;
            while(*end == ' ');
            end[1] = '\0';
        }
            
    } else {
        /* just use the root */
        *param = *name;
    }
}

