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

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <usb.h>

usb_dev_handle *handle;

/* set output io, 1 is high, 0 low */
void setio(int val)
{
    usb_control_msg(handle, USB_ENDPOINT_IN | USB_TYPE_VENDOR
                    | USB_RECIP_DEVICE, 1, val, 0, 0, 0, 5000);
}

int main(void)
{
   struct usb_bus *bus;
   struct usb_device *dev;

   usb_init();
   usb_find_busses();
   usb_find_devices();

   for(bus = usb_get_busses(); bus; bus = bus->next) {
      for(dev = bus->devices; dev; dev = dev->next) {
         if(dev->descriptor.idVendor == 0x16c0) {
            goto found;
         }
      }
   }
   printf("cannot find device\n");
   return 1;

 found:;
   //   printf("found device\n");
   handle = usb_open(dev);

   setvbuf(stdout, NULL, _IONBF, 0);

   int x = 0;
   for(;;) {
       setio(x);
       x = !x;
      unsigned char buf[8];

      int len = usb_control_msg(handle, USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
                                0, 0, 0, buf, 7, 5000);

      if(len == -1) {
          printf("Error communicating with device\n");
          break;
      }

      if(len == 7) {
         int data = buf[3] << 24 | buf[2] << 16 | buf[1] << 8 | buf[0];
         int datacount = buf[5]<<8 | buf[4];

         /*      printf("num: 0x%hhx 0x%hhx 0x%hhx 0x%hhx 0x%hhx 0x%hhx 0x%hhx\n",
                 len, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);*/
         
         double val = (double)data/datacount - 3;
         //         printf("%d %d %f %f\n", data, datacount, val, val*5/5120.0);
         
         //printf("%.5f %d %d %d\n", val*5/5120.0, buf[6], datacount, data/datacount/10);
                  printf("%.4f\n", val*4.7/5120.0);
      }
      usleep(50000);
   }

   usb_close(handle);

   return 0;
}
