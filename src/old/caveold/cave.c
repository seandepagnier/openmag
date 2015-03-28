#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL	      // Sets up the default speed for delay.h
#include <util/delay.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <string.h>

#include "../include/uart.h"

/* LCD I/O definitions */

#define LCDDBPORT PORTD
#define LCDDBPIN PIND
#define LCDDBDDR DDRD
#define LCDDBMASK 0xf0
#define LCDDBSHIFT 4            /* how many bits left to shift
                                ** a DB nibble to get it to line up */
#define LCDRPORT PORTB
#define LCDRDDR DDRB
#define LCDR (_BV(PB7))

#define LCDRSPORT PORTB
#define LCDRSDDR DDRB
#define LCDRS (_BV(PB6))

#define LCDEPORT PORTD
#define LCDEDDR DDRD
#define LCDE (_BV(PD3))

#include "../include/lcd.h"


#define DDR_SPI DDRB
#define DD_MOSI DDB3
#define DD_MISO DDB4
#define DD_SCK  DDB5
#define DD_SS   DDB2

#include "../include/spi.h"

#define MAGDRDYPORT PINB
#define MAGDRDYDDR DDRB
#define MAGDRDY (_BV(PB1))
#define MAGDRDYINT (_BV(PCINT1))

#define MAGRESETPORT PORTB
#define MAGRESETDDR DDRB
#define MAGRESET (_BV(PB0))

#include "../include/mag.h"

#include "adc.h"

#include "linalg.h"
#include "quaternion.h"

static volatile uint16_t val;

ISR(ADC_vect)
{
   val = ADCW;
}

uint16_t getavgadc(int off, int count)
{
   uint16_t total = 0;
   int i = 0;
   for(i = 0; i<count; i++)
      total += getadc(off);
   return total / count;
}

#define SCALE_FACTOR_COUNT  (2*8 + 1)

static const float sf_angles[SCALE_FACTOR_COUNT] = {-90, -67.5, -45, -30, -22.5, -15, -10, -5,
                                                    0,   5,  10,  15, 22.5,  30,  45, 67.5, 90};

const uint16_t XSF[SCALE_FACTOR_COUNT] =           {365,   381, 434, 486,   515, 544, 565, 590,
                                                  610, 633, 657, 679,  709, 737, 793,  848, 872};
const uint16_t YSF[SCALE_FACTOR_COUNT] =           {362,   382, 436, 489,   518, 549, 569, 593,
                                                  610, 634, 659, 680,  709, 737, 790,  843, 863};
const uint16_t ZSF[SCALE_FACTOR_COUNT] =           {340,   330, 384, 437,   465, 489, 510, 534,
                                                  556, 576, 603, 621,  650, 678, 731,  787, 807};
static float sf_force[SCALE_FACTOR_COUNT];

/* eventually load cal data from eeprom here */
static void sf_init(void)
{
   int i;
   for(i = 0; i<SCALE_FACTOR_COUNT; i++)
      sf_force[i] = sin(sf_angles[i] / 180 * M_PI);
}

/* use interpolation in the scale factor calibration table
   to calculate the acceleration */
static float sf_apply(const uint16_t SF[SCALE_FACTOR_COUNT], uint16_t counts)

{
   float c = counts;
   /* less than first */
   if(SF[0] > counts)
      return sf_force[0];

   /* linear interpolate */
   int i;
   for(i = 1; i<SCALE_FACTOR_COUNT; i++)
      if(SF[i] > c) {
         float x = sf_force[i] - sf_force[i - 1];
         float y = SF[i] - SF[i - 1];
         return sf_force[i - 1] + x/y * (c - SF[i - 1]);
      }

   /* greater than last */
   return sf_force[SCALE_FACTOR_COUNT - 1];
}

/* X: */
/* max:  3660 790 -120 */
/* min: -2350 63  -200 */

/* Y: */
/* max: 890 3680 20 */
/* min: 250 -3050 380 */

/* Z: */
/* max: 920 480 2030 */
/* min: -600 -100 -2450 */

const float magscale[3] = {1.0/3005, 1.0/3365, 1.0/2240}, magoffset[3] = {555, 515, -100};

static void magcal_apply(float out[3], int16_t in[3])
{
   int i;
   for(i = 0; i<3; i++)
      out[i] = magscale[i]*((float)in[i] - magoffset[i]);
}

int main(void) {
   uart_init(12);
   printf("init\n");

   lcd_init();

   spi_init();

   mag_init();

   adc_init();

   sf_init();

   lcd_setLine(0, 0, "booting         ", 16);

   _delay_ms(3000);

   char buf[16];

   for(;;) {
      /* could get this through interrupt to speed things up */
      int xadc, yadc, zadc;
      xadc = getavgadc(2, 16);
      yadc = getavgadc(1, 16);
      zadc = getavgadc(0, 16);

      static float x, y, z;
      const float a = .6;
      x = a*x + (1-a)*sf_apply(XSF, xadc);
      y = a*y + (1-a)*sf_apply(YSF, yadc);
      z = a*z + (1-a)*sf_apply(ZSF, zadc);

#if 0 /* display forces */
      snprintf(buf, sizeof buf, "%.2f %.2f %.2f  ", (double)x, (double)y, (double)z);
      lcd_setLine(0, 0, buf, strlen(buf));
#endif
      
#if 0 /* display the x angle with gravity */
      float val;

      if(fabs(x) < .7)
         val = asin(x);
      else {
         /* calculate angle using y and z instead at high angles */
         float den = 1 - y*y - z*z;
         if(den < 0)
            den = 0; /* oops, did they drop it? or bad cal :-P */
         float ox = sqrt(den);
         if(x < 0) /* keep same sign */
            ox = -ox;
         val = asin(ox);
      }

      sprintf(buf, "%s%.2f %d %.1f  ", val < 0 ? "" : "+", (double)180/M_PI * val,
              xadc, sqrt(x*x+y*y+z*z));

      lcd_setLine(0, 0, buf, strlen(buf));
#endif
   printf("init20\n");

#if 1 /* read mags */
      int i;
      int mval[3];
      for(i = 0; i<3; i++) {
         mag_reset();
         spi_setss();
         _delay_ms(100);
         mag_command(i);
         mag_waitdata();
         mval[i] = mag_read();
         // spi_clearss();
      }
   printf("init21\n");

      float fmval[3];
      magcal_apply(fmval, mval);

#if 1
      snprintf(buf, sizeof buf, "%d/%d/%d    ", mval[0], mval[1], mval[2]);
      lcd_setLine(0, 0, buf, strlen(buf));
#endif
#if 0
      snprintf(buf, sizeof buf, "%.2f/%.2f/%.2f    ", (double)fmval[0], (double)fmval[1],
               (double)fmval[2]); 
      lcd_setLine(0, 0, buf, strlen(buf));
#endif
#if 0
      snprintf(buf, sizeof buf, "%.2f/%.2f/%.2f    ",
               (double)atan2(fmval[0], fmval[1]),
               (double)atan2(fmval[1], fmval[2]),
               (double)atan2(fmval[2], fmval[0]));
      lcd_setLine(1, 0, buf, strlen(buf));
#endif
#if 0 /* euler heading (doesn't work with both pitch and roll right now) */
 {
      float pitch = asin(x);
      float pitchcos = cos(pitch), pitchsin = sin(pitch);

      float roll = asin(y);
      float rollcos = cos(roll), rollsin = sin(roll);

      float xh = fmval[1]*pitchcos - fmval[0]*rollsin*pitchsin + fmval[2]*rollcos*pitchsin;
      float yh = fmval[0]*rollcos - fmval[2]*rollsin;


      float rot = atan2(yh, xh);

      snprintf(buf, sizeof buf, "Heading: %.2f     ", 180 / M_PI * rot);
      lcd_setLine(0, 0, buf, strlen(buf));
 }
#endif
#if 1  /* quaternion heading */
      float g[3] = {0, 0, 1};
      float vec[3] = {y, -x, z}; /* swap x and y due to mounted orientation */
      float tilt[4]; /* tilt is a quaternion rotation */
      vec2vec2quat(tilt, vec, g);
      
      /* apply this rotation to mag */
      rotvecquat(fmval, tilt);
      
      /* now fmval[2] is the downward magnetic component, but it's sign is useful */
      float rot = atan2(fmval[0], fmval[1]);

      if(fmval[2] < 0)
         rot *= -1;
      
      if(rot < 0)
         rot += 2 * M_PI;

      rot = 180*rot/M_PI; /* convert to degrees */

#if 0
      static float oldrot;
      if(oldrot - rot > 180)
         rot += 360;
      if(rot - oldrot > 180)
         oldrot += 360;

      oldrot = oldrot*.5 + rot*.5;
      if(oldrot > 360)
         oldrot -= 360;
      rot = oldrot;
#endif

      snprintf(buf, sizeof buf, "%.2f %.2f    ", rot, 180/M_PI*asin(fmval[2]));
      lcd_setLine(1, 0, buf, strlen(buf));
#endif
#endif
   }
   return 0;
}
