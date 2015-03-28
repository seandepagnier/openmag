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

#include <calibration/sensor.h>

static int getdata(const char *buffer, const char *name, float *data, int n)
{
   if(strncmp(buffer, name, strlen(name)))
      return 0;
   if(buffer[strlen(name)] != ':')
      return 0;

   const char *p = buffer + strlen(name) + 1;
   int count = 0;

   while(count < n) {
      char *endptr;
      data[count++] = strtod(p, &endptr);
      if(p == endptr)
         return 0;
      p = endptr;
   }
   return 1;
}

void calibrationhost_read_sensors(int fd)
{
   static char buf[1024];
   static int offset;

   if(fd == -1)
      return;

   int i;
   for(i = 0; i<1000; i++) {
       switch(read(fd, buf + offset, 1)) {
       case 0:
           printf("input EOF\n");
           exit(0);
       case -1:
           return;
       }

       if(buf[offset++] != '\n')
           continue;

       buf[offset] = '\0';

       float data[6];
       int32_t idata[3];
       if(getdata(buf, "accel", data, 3)) {
          idata[0] = data[0], idata[1] = data[1], idata[2] = data[2];
          sensor_update(&accel, idata);
       }
   
       if(getdata(buf, "mag", data, 3)) {
          idata[0] = data[0], idata[1] = data[1], idata[2] = data[2];
          sensor_update(&mag, idata);
       }

       if(getdata(buf, "temp", data, 1)) {
          idata[0] = data[0];
          temperature_update(idata[0]);
       }

       offset = 0;

       sensor_poll();
   }
}

void calibrationhost_sensors_init(void)
{
   calibration_clear_accelfast();
   calibration_clear_accel();

   calibration_clear_magfast();
   calibration_clear_mag();
   calibration_clear_magalign();
   calibration_clear_magdip();
}
