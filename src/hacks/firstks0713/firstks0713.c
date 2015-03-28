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

#define KS0713_RESETDDR DDRC
#define KS0713_RESETPORT PORTC
#define KS0713_RESETPIN PINC
#define KS0713_RESET PC4
#define CRYSTALFONTZLCD

#if 1
#define KS0713_USE_XMEM
#else
#define KS0713_WRDDR DDRE
#define KS0713_WRPORT PORTE
#define KS0713_WR PE0
#define KS0713_RDDDR DDRE
#define KS0713_RDPORT PORTE
#define KS0713_RD PE1
#define KS0713_RSDDR DDRC
#define KS0713_RSPORT PORTC
#define KS0713_RS PC0
#define KS0713_DATADDR DDRA
#define KS0713_DATAPORT PORTA
#define KS0713_DATAPIN PINA
#endif
#include <avr/ks0713.h>

#define DATADRIVER_USE_UART
#include <avr/datadriver.h>

const char softwareversion[20] PROGMEM = "firstks0713";
DATA_ACCESSOR(name=softwareversion type=string mem=flash arraylen=20 varname=softwareversion)

#include <gfx/gfx.h>
#include <gfx/font.h>

#include <math.h>

int main(void)
{
   datadriver_init();

   sei();

   ks0713_init();

   int r = 0;
   for(;;) {
       memset(framebuffer, 0, sizeof framebuffer);

       gfx_setdrawtype(GFX_BLACK);
//       gfx_line(20, 10, 20, 10);

       gfx_setpos(60, 40);
       gfx_ellipse(1, 2, 20);

       gfx_setfont(GFX_B12_FONT);
       gfx_setpos(10, 20);

       gfx_setalignment(GFX_ALIGN_LEFT);
       static int x = 0;
       gfx_printf("hi there: %d", x);

       x++;

#if 0
       static int b;
       ks0713_setbattery(b++);

       if(b > 4)
           b = 0;
#endif

       ks0713_write_framebuffer();

       int i;
       for(i = 0; i<100; i++)
           _delay_ms(1);

       datadriver_poll();
   }
}
