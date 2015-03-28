#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <usb.h>

/* simplest possible example to read and write to low-speed usb
   in serial data form
*/

int main(int argc, char **argv)
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
   printf("found device\n");
   usb_dev_handle *handle = usb_open(dev);

   fcntl(0, F_SETFL, O_NONBLOCK);

   for(;;) {
      char buf[250];

      int len = usb_control_msg(handle, USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
                                0, 0, 0, buf, sizeof buf, 5000);

      if(len > 0)
         write(0, buf, len);

      len = read(0, buf, sizeof buf);
      if(len == 0)
         break;
      if(len > 0)
         usb_control_msg(handle, USB_ENDPOINT_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
                         0, 0, 0, buf, len, 5000);
      else
         usleep(10000);
   }

   usb_close(handle);

   return 0;
}
