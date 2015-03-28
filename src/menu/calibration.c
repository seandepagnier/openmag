/* Copyright (C) 2008, 2009 Sean D'Epagnier <sean@depagnier.com>
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include <avrdata.h>

#include <avrdrivers/buzzer.h>

#include <data/data.h>
#include <data/dataquaternions.h>

#include <gfx/gfx.h>
#include <gfx/font.h>
#include <gfx/lcd.h>
#include <gfx/gfx3d.h>

#include <linalg/vector.h>
#include <linalg/quaternion.h>
#include <linalg/rotate.h>
#include <linalg/matrix.h>

#include <calibration/sensor.h>
#include <calibration/fit.h>
#include <calibration/cal.h>
#include <calibration/calc.h>
#include <calibration/still.h>
#include <calibration/show.h>
#include <calibration/temperature.h>

#include <avr-pt.h>

#include "menu.h"
#include "keys.h"
#include "internal.h"
#include "readertext.h"
#include "entry.h"

static void
extrap(float gh[], float ghr[], int len, float start, float year)
{
   float factor = year - start;
   int i;
   for(i = 0; i<len; i++)
      gh[i] += factor*ghr[i];
}

/* inputs:
   lat - latitude degrees
   lon - longitude degrees
   gh - gh data
   nmax - degree
   field - output set to field strength in nT
*/
static int
shval3(float lat, float lon, float gh[], int nmax, float field[3])
{
   const float earths_radius = 6371.2;
   const float dtr = 0.01745329; /* degrees to radians */
   /* a2 and b2 are squares of axes of reference spheroid for transforming
      coordinates */
   const float a2 = 40680631.59, b2 = 40408299.98;
#define elev 0 /* set elevation to 0 */
   float slat;
   float clat;
   float ratio;
   float aa, bb, cc, dd;
   float sd, cd, r, rr, fm, fn;
   float sl[14];
   float cl[14];
   float p[119];
   float q[119];
   int ii,j,k,l,m,n;
   int npq;
   int ios;

   ios = 0;
   r = elev;
   aa = lat * dtr;
   slat = sin( aa );
   clat = cos( aa );
   aa = lon * dtr;
   sl[1] = sin( aa );
   cl[1] = cos( aa );

   memset(field, 0, sizeof field);

   sd = 0.0;
   cd = 1.0;
   l = 1;
   n = 0;
   m = 1;
   npq = (nmax * (nmax + 3)) / 2;

   aa = a2 * clat * clat;
   bb = b2 * slat * slat;
   cc = aa + bb;
   dd = sqrt( cc );
   r = sqrt( 
#if elev != 0 /* to reduce code size */
       elev * (elev + 2.0 * dd) + 
#endif
   (a2 * aa + b2 * bb) / cc );
   cd = (elev + dd) / r;
   sd = (a2 - b2) / dd * slat * clat / r;
   aa = slat;
   slat = slat * cd - clat * sd;
   clat = clat * cd + aa * sd;

   ratio = earths_radius / r;
   aa = sqrt( 3.0 );
   p[1] = 2.0 * slat;
   p[2] = 2.0 * clat;
   p[3] = 4.5 * slat * slat - 1.5;
   p[4] = 3.0 * aa * clat * slat;
   q[1] = -clat;
   q[2] = slat;
   q[3] = -3.0 * clat * slat;
   q[4] = aa * (slat * slat - clat * clat);

   for ( k = 1; k <= npq; ++k)
   {
      if (n < m)
      {
	 m = 0;
	 n = n + 1;
	 rr = pow(ratio, n + 2);
	 fn = n;
      }
      fm = m;
      if (k >= 5)
      {
	 if (m == n)
	 {
	    aa = sqrt(1.0 - 0.5/fm);
	    j = k - n - 1;
	    p[k] = (1.0 + 1.0/fm) * aa * clat * p[j];
	    q[k] = aa * (clat * q[j] + slat/fm * p[j]);
	    sl[m] = sl[m-1] * cl[1] + cl[m-1] * sl[1];
	    cl[m] = cl[m-1] * cl[1] - sl[m-1] * sl[1];
	 }
	 else
	 {
	    aa = sqrt( fn*fn - fm*fm );
	    bb = sqrt( (fn - 1.0)*(fn-1.0) - (fm * fm) )/aa;
	    cc = (2.0 * fn - 1.0)/aa;
	    ii = k - n;
	    j = k - 2 * n + 1;
	    p[k] = (fn + 1.0) * (cc * slat/fn * p[ii] - bb/(fn - 1.0) * p[j]);
	    q[k] = cc * (slat * q[ii] - clat/fn * p[ii]) - bb * q[j];
	 }
      }

      aa = rr * gh[l];

      if (m == 0)
      {
         field[0] += aa * q[k];
         field[2] -= aa * p[k];
	 l = l + 1;
      }
      else
      {
         bb = rr * gh[l+1];
         cc = aa * cl[m] + bb * sl[m];
         dd = aa * sl[m] - bb * cl[m];
         field[0] += cc * q[k];
         field[2] -= cc * p[k];
         if (clat > 0)
            field[1] += dd *
               fm * p[k]/((fn + 1.0) * clat);
         else
            field[1] += dd * q[k] * slat;
         l = l + 2;
      }

      m = m + 1;
   }

   aa = field[0];
   field[0] = field[0] * cd + field[2] * sd;
   field[2] = field[2] * cd - aa * sd;
   return(ios);
}

static int
calc_from_lat_lon_year(float lat, float lon, float year)
{
   const float epoch = 2005;
   static const float ghf[] PROGMEM = {
      0, -29556.8, -1671.7, 5079.8, -2340.6, 3046.9, -2594.7, 1657.0, -516.7,
      1335.4, -2305.1, -199.9, 1246.7, 269.3, 674.0, -524.2, 919.8, 798.1,
      281.5, 211.3, -226.0, -379.4, 145.8, 100.0, -304.7, -227.4, 354.7, 42.4,
      208.7, 179.8, -136.5, -123.0, -168.3, -19.5, -14.1, 103.6, 73.2, 69.7,
      -20.3, 76.7, 54.7, -151.2, 63.6, -14.9, -63.4, 14.6, -0.1, -86.3, 50.4};
   float gh[(sizeof ghf) / (sizeof *ghf)];
   memcpy_P(gh, ghf, sizeof ghf);

   static const float ghrf[] PROGMEM = {
      0, 8, 10.6, -20.9, -15.1, -7.8, 23.2, -.8, -14.6, .4, -2.6, 5, -1.2, -7,
      -6.5, -.6, -2.5, 2.8, 2.2, -7, 1.6, 6.2, 5.8, -3.8, .1, -2.8, 0.7, 0.0,
      -3.2, 1.7, -1.1, 2.1, 0.1, 4.8, -0.8, -1.1, -0.7, 0.4, -0.6, -0.3, -1.9,
      2.3, -0.4, -2.1, -0.5, -0.6, -0.3, 1.4, 0.7};
   float ghr[(sizeof ghrf) / (sizeof *ghrf)];
   memcpy_P(ghr, ghrf, sizeof ghrf);

   const int n = (sizeof gh) / (sizeof *gh);
   extrap(gh, ghr, n, epoch, year);

   float field[3] = {0};
   shval3(lat, lon, gh, 6, field); 

   /* set inclination and declination */
   float h = sqrt(field[0]*field[0] + field[1]*field[1]);
   eeprom_write_float_safe(&inclination, rad2deg(atan2(field[2], h)));
   eeprom_write_float_safe(&declination, rad2deg(atan2(field[1], field[0])));

   /* should also divide by relative magnitude */
   eeprom_write_float_safe(&fieldstrength, magnitude(field)*1e-5);

   /* return 1 if inclination isn't close to dip */
   if(fabs(eeprom_read_float_safe(&inclination) - dip) > 5)
      return 1;

   return 0;
}

static volatile uint8_t queue_ret;
static float queue_lat EEMEM = 40, queue_lon EEMEM = -105;
static float queue_year EEMEM = 2009.5;
int queue_calc_from_lat_lon_year(void)
{
   static struct pt pt;
   PT_BEGIN(&pt);
   queue_ret = 2;

   while(queue_ret == 2)
      PT_YIELD(&pt);

   PT_END(&pt);
}

void
calibration_calc_queued_location(void)
{
   if(queue_ret == 2)
      queue_ret = calc_from_lat_lon_year(eeprom_read_float_safe(&queue_lat),
					 eeprom_read_float_safe(&queue_lon),
					 eeprom_read_float_safe(&queue_year));
}

static void
getposfastcalnormalized(int usemag, float np[3])
{
   int32_t p[3] = {np[0], np[1], np[2]};
   if(usemag)
      calibration_apply_magfast(p);
   else
      calibration_apply_accelfast(p);
   np[0] = p[0], np[1] = p[1], np[2] = p[2];
   normalize(np);
}

static int
calibration_sphereplot_thread(int usemag)
{
   static struct pt pt;
   PT_BEGIN(&pt);

   gfx3d_reset_rotation();

   gfx3d_reset_translation();
   gfx3d_translate(0, 0, 3); /* start zoomed out */

   gfx3d_screenpos(LCD_WIDTH/2, 40);

restart:

   /* draw title */
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 12, "Calibration Sphere Plot");

   for(;;) {
      gfx_setdrawtype(GFX_WHITE);
      gfx_rectangle_abs(0, LCD_WIDTH-1, 12, LCD_HEIGHT-1, 1);
      gfx_setdrawtype(GFX_BLACK);
      
      gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
      gfx_setfont(GFX_4x6_FONT);
      gfx_setpos(0, 50);
      GFX_PUTS("A about\nD done");

      static struct gfx_rect paintingarea = {0, LCD_WIDTH-1, 12, LCD_HEIGHT-1};
      gfx_setcliprect(&paintingarea);

      /* draw current vector */
      float origin[3] = {0, 0, 0}, curpos[3];
      int i;
      for(i=0; i<3; i++)
	 curpos[i] = (usemag?mag:accel).raw[i];

      getposfastcalnormalized(usemag, curpos);
      gfx3d_line(origin, curpos);

      /* draw the stillpoints */
      for(i=0; i<numstillpoints; i++) {
	 int off = usemag ? 0 : 3;
	 if(!isnan(stillpoints[i][off])) {
	    float np[3] = {stillpoints[i][off+0],
			   stillpoints[i][off+1],
			   stillpoints[i][off+2]};
	    getposfastcalnormalized(usemag, np);
	    gfx3d_point(np);
	 }
      }

      gfx_setcliprect(NULL);

      gfx_setpos(110, 52);
      gfx3d_draw_axes();

      gfx3d_rotate(3, 0, 1, 0);
      gfx3d_rotate(.8*fabs(sin(gettime())), 1, 0, 0);
      gfx3d_rotate(.3*fabs(sin(gettime()/10)), 0, 0, 1);

      switch(key_lastup()) {
      case KEY_A:
	 PT_WAIT_THREAD(&pt, reader_thread(about_calibration_sphereplot));
	 goto restart;
      case KEY_D: PT_EXIT(&pt);
      }

     MENU_YIELD(&pt);
   }

  PT_END(&pt);
}

static int
calibration_info_thread(int accel)
{
   static struct pt pt;

   PT_BEGIN(&pt);
   static uint8_t page;
   static int8_t off;

   off = 0;
   page = 0;
 restart:
   if(accel)
      MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 11,
		      "Accel Calibration Info");
   else
      MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 11,
		      "Mag Calibration Info");


   gfx_setfont(GFX_B08_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 12, LCD_HEIGHT-1, "");
   gfx_setpos(LCD_WIDTH/2, 9);
   gfx_setalignment(GFX_ALIGN_CENTER | GFX_ALIGN_TOP);
   if(accel) {
      switch(page) {
      case 0: GFX_PUTS("accelfast"); break;
      case 1: GFX_PUTS("accel"); break;
      }
   } else {
      switch(page) {
      case 0: GFX_PUTS("magfast"); break;
      case 1: GFX_PUTS("mag"); break;
      case 2: GFX_PUTS("magalign"); break;
      case 3: GFX_PUTS("magdip"); break;
      }
   }

   gfx_setpos(-off, 21);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);

   if(accel) {
      gfx_setfont(GFX_4x6_FONT);
      switch(page) {
      case 0:
	 calibration_show_sphere(gfx_printf_P, PSTR(""),
				 accelfastX, accelfastX[3]);
	 break;
      case 1:
	 calibration_show_rotatedellipsoid3rdorder(gfx_printf_P, PSTR(""),
						   accelX);
	 break;
      }
   } else {
      gfx_setfont(GFX_5x8_FONT);
      switch(page) {
      case 0:
	 calibration_show_sphere(gfx_printf_P, PSTR(""),
				 magfastX, magfastX[3]);
	 break;
      case 1:
	 calibration_show_rotatedellipsoid(gfx_printf_P, PSTR(""), magX);
	 break;
      case 2:
	 calibration_show_align(gfx_printf_P, PSTR(""), magalignX);
	 break;
#ifndef __AVR_AT90USB1287__
      case 3:
	 gfx_setfont(GFX_4x6_FONT);
	 calibration_show_transformation(gfx_printf_P, PSTR(""), magdipX);
	 break;
#endif
      }
   }

   gfx_setpos(0, 56 + accel);
   GFX_PUTS("1 page  2 <-  3 ->  D done");

   for(;;) {
      MENU_YIELD(&pt);

      if(keys & KEY_2) {
	 if((off-=5) < 0)
	    off = 0;
	 goto restart;
      }

      if(keys & KEY_3) {
	 if((off+=5) > 100)
	    off = 100;
	 goto restart;
      }

      switch(key_lastup()) {
      case KEY_1:
	 if(++page == (accel ? 2 : 4))
	    page = 0;
	 off = 0;
	 goto restart;
      case KEY_D:
         PT_EXIT(&pt);
      }
   }

   PT_END(&pt); 
}

static uint8_t flash, buzz;

static void showflash(void)
{
   if(flash) {
      gfx_setdrawtype(GFX_INVERT);      
      gfx_rectangle_abs(0, 62, 22, 31, 1);
      gfx_setdrawtype(GFX_BLACK);
   }
}

int
accelcal_thread(void)
{
   static struct pt pt;

   if(still_update & 1) {
      still_update &= ~1;
      flash = !flash;
      buzz = 1;
   }

   PT_BEGIN(&pt);
   buzz = 0;

 restart:
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 11, "Accel Calibration");

   /* draw selection */
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
   gfx_setfont(GFX_R08_FONT);
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, LCD_WIDTH-1, 40, 49, 1);
   gfx_setpos(0, 40);
   gfx_setdrawtype(GFX_BLACK);
   GFX_PUTS("1 calibrate  2 clear");

#ifndef __AVR_AT90USB1287__
   GFX_PUTS("  3 info");
#endif

   DRAW_SELECTION(0, LCD_WIDTH-1, 50, LCD_HEIGHT-1,
#ifndef __AVR_AT90USB1287__
		  "4 plot  "
#endif
		  "A about   D done\n");

   for(;;) {
      /* this makes button idle sleeps not sleep anymore */
      button_pressed = 1;

      /* draw info */
      gfx_setdrawtype(GFX_WHITE);
      gfx_rectangle_abs(0, LCD_WIDTH-1, 12, 40, 1);
      gfx_setdrawtype(GFX_BLACK);
      gfx_setpos(0, 11);
      gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);

      float a[3];
      sensor_get_calibrated(&accel, a);
      GFX_PRINTF("Force %.4f\n", magnitude(a));

      GFX_PRINTF("Still points %d\n", still_count_stillpoints(1));
      showflash();

      GFX_PRINTF("%.5f%%", 1e2*acceldev);

      if(stillcalibrateaccel) {
      showcalibrating:
	 gfx_setpos(70, 21);
	 GFX_PUTS("Calibrating");
      }

      switch(key_lastup()) {
      case KEY_1:
	 still_calibrate_accel();
	 goto showcalibrating;
      case KEY_2:
	 calibration_clear_accel();
	 goto restart;
#ifndef __AVR_AT90USB1287__
      case KEY_3:
         PT_WAIT_THREAD(&pt, calibration_info_thread(1));
	 goto restart;
      case KEY_4:
         PT_WAIT_THREAD(&pt, calibration_sphereplot_thread(0));
	 goto restart;
#endif
      case KEY_A:
         PT_WAIT_THREAD(&pt, reader_thread(about_calibration_accelerometer));
         goto restart;
      case KEY_D:
         PT_EXIT(&pt);
      }
      MENU_YIELD(&pt);
   }

   PT_END(&pt);
}

int
magcal_thread(void)
{
   static struct pt pt;

   if(still_update & 2) {
      still_update &= ~2;
      flash = !flash;
      buzz = 1;
   }

   PT_BEGIN(&pt);
   buzz = 0;

 restart:
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 11, "Mag Calibration");

   /* draw selection */
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
   gfx_setfont(GFX_R08_FONT);
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, LCD_WIDTH-1, 40, 49, 1);
   gfx_setpos(0, 40);
   gfx_setdrawtype(GFX_BLACK);
   GFX_PUTS("1 calibrate  2 clear");

#ifndef __AVR_AT90USB1287__
   GFX_PUTS(" 3 info");
#endif

   DRAW_SELECTION(0, LCD_WIDTH-1, 50, LCD_HEIGHT-1,
#ifndef __AVR_AT90USB1287__
		  "4 plot  "
#endif
		  "A about   D done\n");

   for(;;) {
      /* this makes button idle sleeps not sleep anymore */
      button_pressed = 1;

      /* draw info */
      gfx_setdrawtype(GFX_WHITE);
      gfx_rectangle_abs(0, LCD_WIDTH-1, 12, 40, 1);
      gfx_line(64, 11, LCD_WIDTH-1, 11); /* erase top pixel of degree symbol */
      gfx_setdrawtype(GFX_BLACK);
      gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);

      gfx_setpos(0, 11);
      GFX_PRINTF("Mag %.4f", calibration_get_mag_magnitude());

      float diperr = calibration_get_mag_diperror();
      GFX_PUTS(" Dip Er");
      NICE_NUM("%.3f", diperr);
      gfx_circle(1); /* draw degree symbol */

      gfx_setpos(0, 21);
      GFX_PRINTF("Still points %d\n", still_count_stillpoints(0));
      showflash();

      GFX_PRINTF("%.4f%% %.4f%%", 100*magdev, 100*magaligndev);
#ifndef __AVR_AT90USB1287__
      GFX_PRINTF(" %.4f%%", 100*magdipdev);
#endif

      if(stillcalibratemag) {
      showcalibrating:
	 gfx_setpos(70, 21);
	 GFX_PUTS("Calibrating");
      }

      switch(key_lastup()) {
      case KEY_1:
         still_calibrate_mag();
	 goto showcalibrating;
      case KEY_2:
	 calibration_clear_mag();
	 calibration_clear_magalign();
#ifdef __AVR_AT90USB1287__
	 goto restart;
#else
	 calibration_clear_magdip();
	 goto restart;
      case KEY_3:
         PT_WAIT_THREAD(&pt, calibration_info_thread(0));
	 goto restart;
      case KEY_4:
         PT_WAIT_THREAD(&pt, calibration_sphereplot_thread(1));
	 goto restart;
#endif
      case KEY_A:
         PT_WAIT_THREAD(&pt, reader_thread(about_calibration_magnetometer));
         goto restart;
      case KEY_D:
         PT_EXIT(&pt);
      }

      MENU_YIELD(&pt);
   }

   PT_END(&pt); 
}

static void
show_laser_align_errors(float *alignment, float error)
{
   float q = eeprom_read_float_safe(alignment);
   GFX_PRINTF("r: %.2f", rad2deg(quatangle(&q)));
   gfx_circle(1);
   GFX_PRINTF("  err: %.3f", rad2deg(2*asin(error)));
   gfx_circle(1);
}

int
laseralign_thread(void)
{
   static struct pt pt;

   PT_BEGIN(&pt);
 restart:
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 11, "Laser Alignment");

   /* draw instructions */
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, LCD_WIDTH-1, 12, LCD_HEIGHT-1, 1);
   gfx_setdrawtype(GFX_BLACK);
   gfx_setfont(GFX_5x8_FONT);
   gfx_setpos(0, 12);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
   GFX_PUTS("Take the same shot with\nrotations around the laser");

   gfx_setalignment(GFX_ALIGN_CENTER | GFX_ALIGN_TOP);
   gfx_setpos(66, 50);
   GFX_PUTS("1 shoot      2 reset\nA about      D done");

   for(;;) {
      static float accelerror, magerror;

      gfx_setdrawtype(GFX_WHITE);
      gfx_rectangle_abs(0, LCD_WIDTH-1, 31, 50, 1);
      gfx_setdrawtype(GFX_BLACK);

      gfx_setfont(GFX_5x8_FONT);
      gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);

      gfx_setpos(0, 32);
      GFX_PUTS("accel ");
      show_laser_align_errors(accellaseralignment, accelerror);

      gfx_setpos(0, 40);
      GFX_PUTS("mag   ");
      show_laser_align_errors(maglaseralignment, magerror);

      static float laserofftime;
      if(keys & KEY_1) {
         laser_on();
         laserofftime = 0;
      }
     
      if(laserofftime && laserofftime + 1 < gettime()) {
         accelerror = calc_accellaseralignment(2);
         magerror = calc_maglaseralignment(2);
         laser_off();
         laserofftime = 0;
	 buzzer_buzz(200);
      }

      switch(key_lastup()) {
      case KEY_1:
         laserofftime = gettime();
         break;
      case KEY_2:
      {
        calc_accellaseralignment(0);
        calc_maglaseralignment(0);
      } break;
      case KEY_A:
         PT_WAIT_THREAD(&pt, reader_thread(about_calibration_laseralignment));
         goto restart;
      case KEY_D:
         laser_off(); /* make sure laser is off */
         PT_EXIT(&pt);
      }
      MENU_YIELD(&pt);
   }

   PT_END(&pt);

   return 0;
}

int
boxalign_thread(void)
{
   static struct pt pt;
   static uint8_t page;

   PT_BEGIN(&pt);

 restart:
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 11, "Box Alignment");

   /* draw instructions */
   const struct gfx_rect instrarea = {0, LCD_WIDTH-1, 12, 29};
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle(&instrarea, 1);
   gfx_setdrawtype(GFX_BLACK);
   gfx_setfont(GFX_5x8_FONT);
   gfx_setpos(0, 14);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
   switch(page) {
   case 0: GFX_PUTS("Align level and facing\nmagnetic north"); break;
   case 1: GFX_PUTS("First align level"); break;
   case 2: GFX_PUTS("Now align incline of 90\nor pointed straight up"); break;
   }

   /* draw selection */
   gfx_setfont(GFX_R08_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 42, LCD_HEIGHT-1,
		  "1 align 2 alternate 3 reset\nA about   D done\n");

   for(;;) {
      gfx_setdrawtype(GFX_WHITE);
      gfx_rectangle_abs(0, LCD_WIDTH-1, 30, 41, 1);
      gfx_setdrawtype(GFX_BLACK);
      gfx_setfont(GFX_R08_FONT);
      gfx_setpos(0, 30);

      GFX_PUTS("p ");
      NICE_NUM("%04.2f  ", pitch);

      GFX_PUTS("r ");
      NICE_NUM("%04.2f  ", roll);

      GFX_PUTS("y ");
      GFX_PRINTF("%06.2f", yaw);

      switch(key_lastup()) {
      case KEY_1:
	 calc_boxalignment(page+1);
	 switch(page) {
	 case 1: page = 2; break;
	 case 2: page = 1; break;
	 }
	 goto restart;
      case KEY_2:
	 page = !page;
	 goto restart;
      case KEY_3:
	 {
	    calc_boxalignment(0);
	    if(page == 2)
	       page = 1;
	 } break;
      case KEY_A:
         PT_WAIT_THREAD(&pt, reader_thread(about_calibration_boxalignment));
         goto restart;
      case KEY_D:
         PT_EXIT(&pt);
      }
      MENU_YIELD(&pt);
   }
   PT_END(&pt);
}

static int
magneticlocation_thread(void)
{
   static struct pt pt;

   PT_BEGIN(&pt);
 restart:
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 11, "Magnetic Location");

   /* draw selection */
   gfx_setfont(GFX_R08_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 12, LCD_HEIGHT-1,
		  "1 inclination\n2 declination\n3 field strength\n"
#ifndef __AVR_AT90USB1287__ /* disabled on smaller flash version */
		  "4 enter location\n"
#endif
		  "A about    D menu");

   /* draw in declination and magnitude */
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);

   gfx_setpos(68, 12);
   NICE_NUM("%05.2f", eeprom_read_float_safe(&inclination));
   int x, y;
   gfx_getpos(&x, &y);
   gfx_setpos(x+1, y+1);
   gfx_circle(1);

   gfx_setpos(68, 22);
   NICE_NUM("%05.2f", eeprom_read_float_safe(&declination));
   gfx_getpos(&x, &y);
   gfx_setpos(x+1, y+1);
   gfx_circle(1);

   gfx_setpos(74, 32);
   extern float magX[8];
   GFX_PRINTF("%05.3f gauss", eeprom_read_float_safe(&fieldstrength));

   float val;
   uint8_t accepted;
   for(;;) {
      switch(key_lastup()) {
      case KEY_1:
          val = eeprom_read_float_safe(&inclination);
          WAIT_ENTRY_NUMBER("Magnetic Inclination",
                            "Enter the Inclination\nangle in degrees",
                            &val, &accepted, -90, 90, 2, 1, NULL);
	  if(accepted)
	     eeprom_write_float_safe(&inclination, val);
	  goto restart;
      case KEY_2:
          val = eeprom_read_float_safe(&declination);
          WAIT_ENTRY_NUMBER("Magnetic Declination",
                            "Enter the Declination\nangle in degrees",
                            &val, &accepted, -179.99, 180, 2, 2, NULL);
	  if(accepted)
	     eeprom_write_float_safe(&declination, val);
	  goto restart;
      case KEY_3:
	 val = eeprom_read_float_safe(&fieldstrength);
	 WAIT_ENTRY_NUMBER("Magnetic Magnitude", "Enter the field\n"
			   "strength in gauss",
			   &val, &accepted, 0, 9.999, 5, 0, NULL);
	 if(accepted)
	    eeprom_write_float_safe(&fieldstrength, val);
	 goto restart;
#if defined(__AVR_ATmega2561__) /* only enabled on bigger flash */
      case KEY_4:
	 {
	    static const char NS[2] = "NS";
	    val = eeprom_read_float_safe(&queue_lat);
	    WAIT_ENTRY_NUMBER("Latitude", "Enter latitude", &val, &accepted,
			      -90, 90, 2, 0, NS);
	    if(!accepted)
	       goto restart;
	    eeprom_write_float_safe(&queue_lat, val);
	 
	    static const char EW[2] = "EW";
	    val = eeprom_read_float_safe(&queue_lon);
	    WAIT_ENTRY_NUMBER("Longitude", "Enter longitude", &val, &accepted,
			      -179.99, 180, 2, 0, EW);
	    if(!accepted)
	       goto restart;
	    eeprom_write_float_safe(&queue_lon, val);
	    val = eeprom_read_float_safe(&queue_year);

	    WAIT_ENTRY_NUMBER("Year", "Enter Year", &val, &accepted, 2008,
			      2020, 1, 2, NULL);
	    if(!accepted)
	       goto restart;
	    eeprom_write_float_safe(&queue_year, val);

	    PT_WAIT_THREAD(&pt, queue_calc_from_lat_lon_year());

	    if(queue_ret)
	       /* warn user that calculated inclination does not match measured */
	       PT_WAIT_THREAD(&pt, reader_thread
			      (warning_calibration_inconsistant_inclination));
	    goto restart;
	 }
#endif
      case KEY_A:
         PT_WAIT_THREAD(&pt, reader_thread(about_calibration_magneticlocation));
         goto restart;
      case KEY_D: /* menu */
         PT_EXIT(&pt);
      }         

      MENU_YIELD(&pt);
   }

   PT_END(&pt);
}

static int
calibration_config_thread(void)
{
   static struct pt pt;

   if(still_update) {
      still_update = 0;
      flash = !flash;
      buzz = 1;
   }

   PT_BEGIN(&pt);
   buzz = 0;
restart:
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 11,  "Calibration Config");

   gfx_setfont(GFX_5x8_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 12, LCD_HEIGHT-1, "");

   gfx_setpos(0, 12);
   static const char t[] PROGMEM = "true", f[] PROGMEM = "false";

   GFX_PRINTF("1 clear stillpoints\n"
	      "2 joint stillpoints "PGM_STR_FMT"\n"
	      "3 accel still tol %u\n"
	      "4 mag   still tol %u\n"
	      "5 temperature units %c\n"
#ifndef __AVR_AT90USB1287__
	      "6 use mag dip "PGM_STR_FMT"\n"
#else
	      "\n"
#endif
	      "A about  D done\n",
	      eeprom_read_byte_safe(&jointstillpoints) ? t : f,
	      eeprom_read_word_safe(&accelstilltolerance),
	      eeprom_read_word_safe(&magstilltolerance),
	      eeprom_read_byte_safe(&temptype) ? 'C' : 'F'
#ifndef __AVR_AT90USB1287__
	      ,eeprom_read_byte_safe(&usemagdip) ? t : f
#endif
      );

   for(;;) {
      gfx_setdrawtype(GFX_WHITE);
      gfx_rectangle_abs(110, LCD_WIDTH-1, 12, 20, 1);
      gfx_setdrawtype(GFX_BLACK);
      gfx_setpos(110, 12);
      GFX_PRINTF("%d", numstillpoints);

      switch(key_lastup()) {
      case KEY_1:
	 numstillpoints = 0;
	 goto restart;
      case KEY_2:
	 eeprom_write_byte_safe(&jointstillpoints,
				!eeprom_read_byte_safe(&jointstillpoints));
	 goto restart;
      case KEY_3:
      {
	 float val = eeprom_read_word_safe(&accelstilltolerance);
	 uint8_t accepted;
	 WAIT_ENTRY_NUMBER("Accel Still Tolerance",
                            "Enter the new still tolerance",
                            &val, &accepted, 0, 999, 0, 0, NULL);
	 if(accepted)
	    eeprom_write_word_safe(&accelstilltolerance, val);
      } goto restart;
      case KEY_4:
      {
	 float val = eeprom_read_word_safe(&magstilltolerance);
	 uint8_t accepted;
	 WAIT_ENTRY_NUMBER("Mag Still Tolerance",
                            "Enter the new still tolerance",
                            &val, &accepted, 0, 999, 0, 0, NULL);
	 if(accepted)
	    eeprom_write_word_safe(&magstilltolerance, val);
      } goto restart;
      case KEY_5:
	 eeprom_write_byte_safe(&temptype, !eeprom_read_byte_safe(&temptype));
	 goto restart;
#ifndef __AVR_AT90USB1287__
      case KEY_6:
	 eeprom_write_byte_safe(&usemagdip, !eeprom_read_byte_safe(&usemagdip));
	 goto restart;
#endif
      case KEY_A:
         PT_WAIT_THREAD(&pt, reader_thread(about_calibration_config));
         goto restart;
      case KEY_D:
         PT_EXIT(&pt);
      }
      MENU_YIELD(&pt);
   }

   PT_END(&pt); 
}

int
calibration_thread(void)
{
   static struct pt pt;

   if(buzz) {
      buzzer_buzz(150);
      buzz = 0;
   }

   PT_BEGIN(&pt);
 restart:

   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 11, "Calibration");

   /* draw selection */
   gfx_setfont(GFX_5x8_FONT);
   DRAW_SELECTION(0, LCD_WIDTH-1, 12, LCD_HEIGHT-1,
		  "1 set magnetic location\n2 accelerometer\n3 magnetometer\n"
		  "4 laser alignment\n5 box alignment\n6 set temperature\n"
		  "7 config   A about  D menu");

   /* draw in temp */
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);

   gfx_setpos(90, 48);
   char temps[8];
   extern void temp_format(char *buf, int len, char usedeg);
   temp_format(temps, sizeof temps, 9);
   gfx_puts(temps);
   
   for(;;) {
      switch(key_lastup()) {
      case KEY_1:
         PT_WAIT_THREAD(&pt, magneticlocation_thread());
         goto restart;
      case KEY_2:
         PT_WAIT_THREAD(&pt, accelcal_thread());
         goto restart;
      case KEY_3:
         PT_WAIT_THREAD(&pt, magcal_thread());
         goto restart;
      case KEY_4:
         PT_WAIT_THREAD(&pt, laseralign_thread());
         goto restart;
      case KEY_5:
	 PT_WAIT_THREAD(&pt, boxalign_thread());
         goto restart;
      case KEY_6:
      {
	 float temp_get(char *c);
	 float newtemp = temp_get(NULL);
	 uint8_t accepted;
	 WAIT_ENTRY_NUMBER("Temperature Calibration",
			   "Enter the correct temperature",
			   &newtemp, &accepted, -100, 200, 1, 1, NULL);
	 if(accepted) {
	    void temp_set(float temp);
	    temp_set(newtemp);
	 }
      }  goto restart;
      case KEY_7:
         PT_WAIT_THREAD(&pt, calibration_config_thread());
	 goto restart;
      case KEY_A:
         PT_WAIT_THREAD(&pt, reader_thread(about_calibration));
         goto restart;
      case KEY_D: /* menu */
         PT_EXIT(&pt);
      }         

      MENU_YIELD(&pt);
   }

   PT_END(&pt);
}
