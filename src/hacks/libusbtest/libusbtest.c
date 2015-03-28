#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <usb.h>

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
   printf("found device\n");
   usb_dev_handle *handle = usb_open(dev);

   //   for(;;)
      {
      char buf[1024];
      //      int ret = read(0, buf, 8);

      int len = usb_control_msg(handle, USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
                                1, 0x1537, 0, buf, 1024, 5000);
      printf("num: %d\n", len);
   }

#if 0
   printf("data(%d): ", len);
   int i;
   for(i=0; i<len; i++)
      printf("0x%hhx ", x[i]);
   printf("\n");
#endif

   usb_close(handle);

   return 0;
}
