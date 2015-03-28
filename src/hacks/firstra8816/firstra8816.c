#include <stdio.h>
#include <string.h>

#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/io.h>
#include <avr/wdt.h>

#include <util/delay.h>

/* data interface */
#include <data/data.h>
#include <data/databasictypes.h>
#include <data/datadirectories.h>
#include <data/dataarrays.h>
#include <data/datastrings.h>
#include <data/databools.h>

#include <avrdrivers/avrdrivers.h>

#define RA8816_RESETDDR DDRC
#define RA8816_RESETPORT PORTC
#define RA8816_RESETPIN PINC
#define RA8816_RESET PC4

#include <avrdrivers/ra8816.h>

#include <gfx/gfx.h>
#include <gfx/font.h>

#include <math.h>

int main(void)
{
   sei();

   ra8816_init();

   int r = 0;
   for(;;) {
       memset(framebuffer, 0, sizeof framebuffer);

       gfx_setdrawtype(GFX_BLACK);

       gfx_setpos(60, 40);
       gfx_ellipse(1, 2, 20);

       gfx_setfont(GFX_B12_FONT);
       gfx_setpos(10, 20);

       gfx_setalignment(GFX_ALIGN_LEFT);
       static int x = 0;
       gfx_printf("hi there: %d", x);

       x++;

       ra8816_write_framebuffer();
       int i;
       for(i = 0; i<100; i++)
           _delay_ms(1);
   }
}
