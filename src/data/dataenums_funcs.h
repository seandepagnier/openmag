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

void data_enum_get(const char * const * vals, uint8_t offset)
{
   int pos = 0;
   const char *tab;
   while((tab = pgm_read_ptr(vals + pos))) {
      if(pos == offset) {
         DATA_OUTPUT(PGM_STR_FMT"\n", tab);
         return;
      }
      pos++;
   }
   DATA_OUTPUT("<invalid enum value: %d\n", offset);
}

void data_enum_set(const char * const * vals, uint8_t *var, const char *val,
                   const char *enumname)
{
    uint8_t pos = 0;
    const char *tab;

    while((tab = pgm_read_ptr(vals + pos))) {
        if(!strcmp_P(val, tab)) {
            *var = pos;
            return;
        }
        pos++;
    }

    DATA_OUTPUT("Invalid enumerant value for enum: "
                PGM_STR_FMT " \n", enumname);
}

void data_enum_list(const char * const * vals)
{
    uint8_t pos = 0;
    const char *tab;
    while((tab = pgm_read_ptr(vals + pos))) {
        DATA_OUTPUT(PGM_STR_FMT" ", tab);
        pos++;
    }

    DATA_OUTPUT("\n");
}
