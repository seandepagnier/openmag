#include <avr/io.h>
#include "uartfs.h"

int main (void)
{
   /* (clock rate) / (16 * baud rate) - 1 */
   uartfs_init (207); /* 300 baud at 1mhz cpu */

   FILE *file = fopen("demo.c", "r");
   fseek(file, 0, SEEK_END);
   long pos = ftell(file);
   printf("length: %ld\n", pos);
   rewind(file);

   int line = 1;
   for(;;) {
      char buf[256];

      if(fgets(buf, sizeof buf, file))
         printf("%d: %s", line++, buf);
      else {
         printf("Done!\n");
         for(;;);
      }

#if 0
      /* flash leds */
      DDRD = _BV(PD3);
      PORTD ^= _BV(PD3);
#endif
   }
}
