/* Copyright (C) 2009 Sean D'Epagnier <sean@depagnier.com>
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

struct atom {
   union {
      float val;
      int func;
      char op;
      int value;
   };
   enum {CONSTANT, FUNCTION, OPERATOR, VALUE} ops;
};

/* values supported for reading from sensors */
enum value {accelX, accelY, accelZ, accelRAWX, accelRAWY, accelRAWZ,
	    magX, magY, magZ, magRAWX, magRAWY, magRAWZ,
	    PITCH, ROLL, YAW, DIP, TEMP, TIME, ANEMOMETER};

float eval(struct atom queue[16], int queuepos);
int shunting_yard(char *buffer, struct atom opqueue[16], int *pos);
float factorial(float x);
