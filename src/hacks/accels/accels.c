#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 100000UL	      // Sets up the default speed for delay.h
#include <util/delay.h>

#include <avr/wdt.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <string.h>

#include "../include/uart.h"

static volatile uint16_t val;

ISR(ADC_vect)
{
   val = ADCW;
}

void adc_init(void) {
   DDRC=0x00;	/* Set ADC port as inputs. */
   PORTC=0x00;	/* DO NOT USE internal pull-up resistors.
   //                           They would mess up the measurement. */
   cli();
   ADCSRA = _BV(ADEN) | _BV(ADSC) | 0x7;
   //   ADCSRA |= 1<<6;
   //   ADCSRB = 0;
   sei();
}

int getadc(int off)
{
   ADMUX = off; /* | 0xc0;*/
   ADCSRA |= _BV(ADSC);   // Set the Start Conversion flag.
   while(ADCSRA & _BV(ADSC)); // Wait for conversion
   
   return ADCW;
}

#define SCALE_FACTOR_COUNT  (2*8 + 1)

static const float sf_angles[SCALE_FACTOR_COUNT] = {-90, -67.5, -45, -30, -22.5, -15, -10, -5,
                                                    0,   5,  10,  15, 22.5,  30,  45, 67.5, 90};

static float sf_force[SCALE_FACTOR_COUNT] = {-.9999999999991198, -.9238791517040105, -.7071063120935576, -.4999996169872557, -.3826831259154065, -.2588188315049383, -.17364803248493946, -.08715566931732228, 0, .08715566931732228, .17364803248493946, .2588188315049383, .3826831259154065, .4999996169872557, .7071063120935576, .9238791517040105, .9999999999991198};

const uint16_t XSF[SCALE_FACTOR_COUNT] =           {353,   366, 399, 432,   448, 467, 480, 496,
                                                  507, 520, 535, 548,  567, 583, 617,  648, 661};
const uint16_t YSF[SCALE_FACTOR_COUNT] =           {349,   359, 395, 427,   445, 464, 476, 491,
                                                  504, 526, 536, 546,  565, 580, 612,  646, 656};

#define SAMPLES 8
struct accel {
   uint16_t total, ring[SAMPLES], ringpos;
   uint16_t sf_counts[SCALE_FACTOR_COUNT];
};

static struct accel accelX, accelY;

static void sf_init(void)
{
   int i;
   for(i = 0; i<SCALE_FACTOR_COUNT; i++) {
      //      sf_force[i] = sin(sf_angles[i] / 180 * M_PI);

      memcpy(accelX.sf_counts, XSF, sizeof XSF);
      memcpy(accelY.sf_counts, YSF, sizeof YSF);
   }
}

static void accel_update(struct accel *a, int adcindex)
{
   a->total -= a->ring[a->ringpos];
   a->ring[a->ringpos] = getadc(adcindex);
   a->total += a->ring[a->ringpos];

   if(a->ringpos == SAMPLES - 1)
      a->ringpos = 0;
   else
      a->ringpos++;
}

static uint16_t accel_counts(struct accel *a)
{
   return a->total / SAMPLES;
}

/* use interpolation in the scale factor calibration table
   to calculate the acceleration */
static double accel_value(struct accel *a, uint16_t counts)

{
   double c = counts;
   /* less than first */
   if(a->sf_counts[0] > counts)
      return sf_force[0];

   /* linear interpolate */
   int i;
   for(i = 1; i<SCALE_FACTOR_COUNT; i++)
      if(a->sf_counts[i] > c) {
         double x = sf_force[i] - sf_force[i - 1];
         double y = a->sf_counts[i] - a->sf_counts[i - 1];
         return sf_force[i - 1] + x/y * (c - a->sf_counts[i - 1]);
      }

   /* greater than last */
   return sf_force[SCALE_FACTOR_COUNT - 1];
}

int main(void) {
   //   DIDR = 0; /* disable digital input buffer */
   
   DDRD = _BV(PD6);
   uart_init(12);

   adc_init();

   sf_init();

   _delay_ms(1000);
 
   for(;;) {
#if 1
      PORTD ^= _BV(PD6);
#endif

      accel_update(&accelX, 0);
      accel_update(&accelY, 1);

      uint16_t xc = accel_counts(&accelX);
      uint16_t yc = accel_counts(&accelY);

      double x = accel_value(&accelX, xc);
      double y = accel_value(&accelY, yc);

      double zm = 1 - x*x - y*y;
      if(zm < 0) /* this is a sensor or cal error, but still try */
         zm = 0;
      double z = sqrt(zm);

      /* tracking for z sign */
      static double lastzsin = 1;

      printf("xc: %d\t yc: %d\n", xc, yc);
      printf("x: %s%f\ty: %s%f\tz: %s%f\n",
             x<0 ? "" : " ", x,
             y<0 ? "" : " ", y,
             z<0 ? "" : " ", z);
      printf("inc: %f, %f\n\n", 180/M_PI*asin(x), 180/M_PI*asin(y));
   }
   return 0;
}
