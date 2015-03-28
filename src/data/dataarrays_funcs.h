/* Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
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

void data_getarray(void *data, int width, int len, void getdata(void*))
{
   int i;
   char (*wdata)[width] = data;
   for(i=0; i < len; i++) {
      if(i)
         DATA_OUTPUT_CHAR(' ');
      getdata(wdata[i]);
   }
   DATA_OUTPUT_CHAR('\n');
}

void data_getarray_2D(void *data, int width, int len, int count,
                      void getdata(void*))
{
   int i, j;
   char (*wdata)[count][width] = data;
   for(i=0; i < count; i++) {
      for(j=0; j < len; j++) {
         if(j)
            DATA_OUTPUT_CHAR(' ');
         getdata(wdata[i][j]);
      }
      DATA_OUTPUT_CHAR('\n');
   }
}
