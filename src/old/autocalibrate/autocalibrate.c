#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL	      // Sets up the default speed for delay.h
#include <util/delay.h>

#include <avr/wdt.h>

#include <string.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

/* my headers */
#include <debug.h>

#include <uart.h>
#include <adc.h>

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

#include <lcd.h>

#define DDR_SPI DDRB
#define DD_MISO DDB4
#define DD_MOSI DDB3
#define DD_SCK  DDB5
#define DD_SS   DDB2

#include <spi.h>

#define MAGDRDYPORT PINB
#define MAGDRDYDDR DDRB
#define MAGDRDY (_BV(PB1))
#define MAGDRDYINT (_BV(PCINT1))

#define MAGRESETPORT PORTB
#define MAGRESETDDR DDRB
#define MAGRESET (_BV(PB0))

#include <mag.h>

#include <linalg.h>

#include <avr/eeprom.h>

#include <fit.h>

#include <button.h>
#include <timer.h>
#include <memcheck.h>

#include <quaternion.h>

static volatile uint8_t button1hit;
ISR(BUTTON1)
{
   button1hit = 1;
}

static volatile int32_t acceldata[3];

/* are we sitting still and able to use this reading
   for dynamic calibration? */
#define STILL_TOLERANCE 280  /* lower values makes it harder to count as still,
                                be careful to make sure it's larger than noise
                                or a still shot may never be taken (should this
                                value be dynamic?)  */
int stillpos(void)
{
   static int32_t ring[4][3];
   static int8_t ringpos;
   const int ringsize = (sizeof ring) / (sizeof *ring);

   uint8_t i, j, ret = 1;
   for(i = 0; i < ringsize; i++)
      for(j = 0; j<3; j++)
         if(abs(ring[i][j] - acceldata[j]) > STILL_TOLERANCE) {
//            DEBUG("off by %d\n", abs(ring[i][j] - val[j]));
            ret = 0;
            goto done;
         }
 done:

   /* update the ring with new input */
   for(i = 0; i<3; i++)
      ring[ringpos][i] = acceldata[i];
   ringpos++;
   if(ringpos == ringsize)
      ringpos = 0;

   return ret;
}

void readaccels(void)
{
   uint8_t i, j;

   for(i = 0; i<3; i++)
      acceldata[i] = 0;
   for(j = 0; j<192; j++) /* space out each channel over time equally */
      for(i = 0; i<3; i++)
         acceldata[i] += getadc(i);
}

#if 1
float quaternion_heading(float aval[3], float mval[3])
{
   float tilt[4]; /* tilt is a quaternion rotation */

   //   DEBUG("g: %f %f %f\n", vec[0], vec[1], vec[2]);

   /* now vec is gravity swap due to mounted orientation */
   float vec1[3] = {aval[0], aval[1], aval[2]};

   //   DEBUG("accel: %f %f %f\n", vec1[0], vec1[1], vec1[2]);
   //   DEBUG("mag: %f %f %f\n", mval[0], mval[1], mval[2]);

   /* rotate out tilt */
   float vec2[3] = {0, vec1[1], vec1[2]};
   normalize(vec2);
   vec2vec2quat(tilt, vec1, vec2);
   rotvecquat(mval, tilt);

   /* now rotate to g */
   vec1[0] = 0, vec1[1] = 0, vec1[2] = 1;
   vec2vec2quat(tilt, vec2, vec1);
   rotvecquat(mval, tilt);

   //   DEBUG("magr: %f %f %f\n", mval[0], mval[1], mval[2]);
      
   /* now mval[2] is the downward magnetic component, but it's sign is useful */
   float rot = atan2(mval[1], mval[0]);

   if(mval[2] < 0)
      rot *= -1;
      
   if(rot < 0)
      rot += 2 * M_PI;

   return 180*rot/M_PI; /* return as degrees */
}
#endif

void off_axis_align_accel(float aval[3])
{
   float laser[3] = {1, 0, .00};
   float true[3] = {1, 0, 0};
   float tilt[4];
   normalize(laser);
   vec2vec2quat(tilt, true, laser);
   //   DEBUG("quat: %f %f %f %f\n", tilt[0], tilt[1], tilt[2], tilt[3]);
   rotvecquat(aval, tilt);
}

void display_angles(float ang, float rot)
{
   char sign = '+';
   if(ang < 0) {
      sign = '-';
      ang = -ang;
   }

   int iang = ang, dang = 100*(ang - (float)iang);
   int irot = rot, drot = 100*(rot - (float)irot);

   lcd_printf(1, "%c%02d.%02d  %03d.%02d    ", sign, iang, dang, irot, drot);
   //   DEBUG("lcd output: %c%02d.%02d  %03d.%02d\n", sign, iang, dang, irot, drot);
}

#define CLAMP(X, MIN, MAX) do { if(X > MAX) X = MAX; if(X < MIN) X = MIN; } while(0)
void show_calculations(void)
{
   /* apply calculations */
   float aconv[3], mconv[3];
   
   ApplyBestFit(acceldata, aconv, ACCEL);
   ApplyBestFit(magdata, mconv, MAG);
//   DEBUG("mag: %ld %ld %ld\n", magdata[0], magdata[1], magdata[2]);

   /* clamp */
   int i;
   for(i = 0; i<3; i++) {
      CLAMP(aconv[i], -1, 1);
      CLAMP(mconv[i], -1, 1);
   }

#if 0
   float alen = magnitude(aconv);
   float mlen = magnitude(mconv);
   DEBUG("alen: %f mlen: %f\n", alen, mlen);
#endif

#if 1
   /* easy way to clean up */
   normalize(aconv);
   normalize(mconv);
#endif

   /* flip stuff */
   float t = aconv[0];
   aconv[0] = -aconv[2];
   aconv[2] = t;

   float anga = 180*asin(-aconv[0])/M_PI;
   //   DEBUG("anga: %f %f\n", anga, aconv[0]);
   /* off axis alignment */
   off_axis_align_accel(aconv);

   /* inclination */
   float ang = 180*asin(-aconv[0])/M_PI;

#if 1
   /* quaternion heading */
   float rot = quaternion_heading(aconv, mconv);
#else
   float ang1 = asin(aconv[2]), ang2 = asin(aconv[1]);
   float x = mconv[0]*cos(ang2) - mconv[2]*sin(ang2);
   float y = mconv[1]*cos(ang1) + mconv[0]*sin(ang2)*sin(ang1) + mconv[2]*cos(ang2)*sin(ang1);

   float rot = 180/M_PI*atan2(y, x);

#if 1
   DEBUG("ang1: %f\n", ang1*180/M_PI);
   DEBUG("ang2: %f\n", ang2*100/M_PI);
   DEBUG("accel: %f %f %f\n", aconv[0], aconv[1], aconv[2]);
   DEBUG("mag: %f %f %f\n", mconv[0], mconv[1], mconv[2]);
   DEBUG("x y rot: %f %f %f\n", x, y, rot);
#endif
#endif

   if(rot < 0)
      rot += 360;

   display_angles(ang, rot);

   //   DEBUG("aval: %d %d %d\n", aval[0], aval[1], aval[2]);
   //   DEBUG("val: %f %f %f\n", aconv[0], aconv[1], aconv[2]);
   //   DEBUG("mval: %d %d %d\n", magdata[0], mval[1], mval[2]);
}

int main(void) {
   uart_init(12);
   sei();

   lcd_init();

   lcd_setLine(0, 0, "booting         ", 16);

   //memcheck_init(); /* check ram usage in timer interrupt */

   DEBUG("\ninit REBOOT\n\n");

   adc_init();

   spi_init();
   mag_init();

   button1_init();
   //   InitPoints(ACCEL);
   _delay_ms(1000);

   uint8_t canrecal = 0;
   for(;;) {
#if 1
      printf("%ld %ld %ld\n", magdata[0], magdata[1], magdata[2]);
      _delay_ms(1000);
      continue;
#endif

      DEBUG("going\n"); /* we know it isn't stuck */
      /* read accels */
      readaccels();
      /* mag is interrupt driven */

      show_calculations();

      static int locksa, locksm;
      static const char tab[] = "1234567890";
      static uint8_t tabind;
      static char accellock, maglock;

#if 1
      if(button1hit) {
         accellock = 'n', maglock = 'n';
         locksa = locksm = 0;
         InitPoints(ACCEL);
         InitPoints(MAG);
         button1hit = 0;
         canrecal = 1;
      }
#endif

#if 1 /* recal? */
      if(canrecal && stillpos()) {
      // if(button1hit) {
         button1hit = 0;

         tabind++;
         if(tabind == sizeof tab)
            tabind = 0;
         if(AddPoint(acceldata, ACCEL)) {
            locksa++;
            if(CalcBestFit(ACCEL))
               accellock = 'l';
            else
               accellock = 'c';
         } else
            accellock = 'd';

         if(AddPoint(magdata, MAG)) {
            locksm++;
            if(CalcBestFit(MAG))
               maglock = 'l';
            else
               maglock = 'c';
         } else
            maglock = 'd';
      }
#endif
      lcd_printf(0, "a:%c %d m:%c %d %c", accellock, locksa, maglock, locksm, tab[tabind]);
   }
}
