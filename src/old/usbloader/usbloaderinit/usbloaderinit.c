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

/* This is an initialization program which can be used to tell applications
   to jump to the usbloader.  If already in the loader, it does no harm.

   For example, in the app:

byte_t usb_setup ( byte_t data[8] )
{
   switch(data[1]) {
   case USBASP_FUNC_CONNECT:
      gotobootloader = 1; // delayed bootloader jump flag
      return 2; // return 2 since we are in the app
   }

   return 0;
}

It is best to turn the usb off cleanly just before jumping to the bootloader.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <usb.h>

#include <avr/usbaspdefs.h>

usb_dev_handle *open_device(void)
{
   struct usb_bus *bus;
   struct usb_device *dev;

   usb_find_busses();
   usb_find_devices();

   for(bus = usb_get_busses(); bus; bus = bus->next)
      for(dev = bus->devices; dev; dev = dev->next)
         if(dev->descriptor.idVendor == 0x16c0)
            return usb_open(dev);

   return NULL;
}

int main(int argc, char *argv[])
{
   int retried = 0;
   usb_init();

 retry:;
   usb_dev_handle *handle = open_device();

   if(!handle) {
      printf("cannot find device\n");
      if(retried && retried++ < 5) {
         printf("RETRYING!!\n");
         usleep(1000000);
         goto retry;
      }
      return 1;
   }

   char buf[4];
   memset(buf, 0, sizeof(buf));
   int len = usb_control_msg(handle, USB_ENDPOINT_IN | USB_TYPE_VENDOR
                             | USB_RECIP_DEVICE,
                             USBASP_FUNC_CONNECT, 0, 0, buf, sizeof buf, 500);

   usb_close(handle);

   if(len < 0) {
      if(retried++ < 5) {
         printf("RETRYING!!\n");
         usleep(1000000);
         goto retry;
      }
      printf("failed to send bootloader command\n");
      return 1;
   }

   if(len == 2) {
      printf("usbloaderinit: sent jump to bootloader command\n");
      /* wait for it... */
      usleep(500000);
      while(!(handle = open_device()))
         usleep(100000);
      usb_close(handle);
   } else
      printf("usbloaderinit: was already in bootloader\n");

   return 0;
}
