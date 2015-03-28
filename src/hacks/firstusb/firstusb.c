#include <stdio.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#define F_CPU 12000000UL
#include <util/delay.h>

#define BOOTLOADER_OFFSET 0xf800
#include <uart.h>

#include <usb.h>

#define PRINT

#ifdef PRINT
void printdata(byte_t *data, int len)
{
   int i;
   for(i = 0; i<len; i++)
      printf("%x ", data[i]);
}
#endif

byte_t usb_setup ( byte_t data[8] )
{
#ifdef PRINT
   printf("setup: ");
   printdata(data, 8);
   printf("\n");
#endif

   return 0xff;
}

byte_t usb_in ( byte_t* data, byte_t len )
{
#ifdef PRINT
   printf("usb in(%d)\n", len);
#endif
   return len;
}

void usb_out ( byte_t* data, byte_t len )
{
#ifdef PRINT
   printf("usb out(%d)\n", len);
   printdata(data, len);
   printf("\n");
#endif
}

int main(void) {
   uart_init(12);
   sei();

   _delay_ms(1);

#ifdef PRINT
   printf("init\n");
#endif

   usb_init();
   for(;;)
      usb_poll();
}
