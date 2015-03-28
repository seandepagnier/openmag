/* Copyright (C) 2009 Sean D'Epagnier <sean@depagnier.com>
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

#include <gfx/gfx.h>
#include <gfx/gfx3d.h>
#include <gfx/font.h>
#include <gfx/lcd.h>

#include <linalg/vector.h>
#include <linalg/rotate.h>

#include <calibration/compute.h>

#include <avr-pt.h>

#include <avrdata.h>

#include "menu.h"
#include "keys.h"
#include "internal.h"
#include "surveymeasurement.h"
#include "readertext.h"
#include "entry.h"

static float ang[3], movo[3];

static float translationspeed EEMEM = 1;
static float minpos[2], maxpos[2];

static uint8_t useflag EEMEM = 0;
static const char FS[] PROGMEM = "FS";
static const char BS[] PROGMEM = "BS";
static const char FSBS[] PROGMEM = "FS/BS";
static const char *usestrs[] PROGMEM = {FS, BS, FSBS};

static uint8_t lrudflag EEMEM = 1;
static const char none[] PROGMEM = "none";
static const char spokes[] PROGMEM = "spokes";
static const char *lrudstrs[] PROGMEM = {none, spokes};

static uint8_t shownames EEMEM = 1;

static void setminmax(float pos[3])
{
   int i;
   for(i=0; i<2; i++) {
      if(maxpos[i] < pos[i])
	 maxpos[i] = pos[i];
      else if(minpos[i] > pos[i])
	 minpos[i] = pos[i];
   }
}

static void plot_draw(void)
{
   /* use a queue to match measurements */
   struct {
      struct surveymeasurement m;
      int grounded;
      float posfrom[3], posto[3];
   } queue[10];
   int queuepos = 0, queuelen = 0;

   uint8_t ruseflag = eeprom_read_byte_safe(&useflag);
   uint8_t rlrudflag = eeprom_read_byte_safe(&lrudflag);
   uint8_t rshownames = eeprom_read_byte_safe(&shownames);

   float best_incline(float incline, float bincline)
   {
      switch(ruseflag) {
      case 1: return -bincline;
      case 2: return (incline-bincline)/2;
      }
      return incline;
   }

   float best_azimuth(float azimuth, float bazimuth)
   {
      switch(ruseflag) {
      case 1: return compute_resolveheading(bazimuth+180);
      case 2: return compute_resolveheading
	    ((azimuth + compute_resolveheading(bazimuth + 180)) / 2);
      }
      return azimuth;
   }

   void show_name(struct surveyname n)
   {
      if(rshownames)
	 GFX_PRINTF("%c%c%c%03d", n.name[0], n.name[1], n.name[2], n.num);
   }

   void show_spokes(struct surveymeasurement *m, float pos[3])
   {
      if(!rlrudflag || !(m->header & LRUD))
	 return;

      float azimuth = best_azimuth(m->azimuth, m->bazimuth);
      float lrud[4][3] =
	 {{m->lrud[0]*cos(deg2rad(azimuth)),
	   m->lrud[0]*sin(deg2rad(azimuth)),
	   0},
	  {-m->lrud[1]*cos(deg2rad(azimuth)),
	   -m->lrud[1]*sin(deg2rad(azimuth)),
	   0},
	  {0, 0, -m->lrud[2]},
	  {0, 0, m->lrud[3]}};

      int i;
      for(i=0; i<4; i++) {
	 vector_add(lrud[i], lrud[i], pos, 3);
	 gfx3d_line(pos, lrud[i]);
      }
   }

   void get_vector(float x[3], struct surveymeasurement *m)
   {
      vector_from_polar(x, m->distance,
			best_incline(m->incline, m->bincline),
			best_azimuth(m->azimuth, m->bazimuth));
      float t = x[0];
      x[0] = x[1];
      x[1] = t;
   }

   void groundpos(int p)
   {
      if(queue[p].grounded)
	 return;

      int i;
      for(i=0; i < queuelen; i++) {
	 int dir = 0;
	 if(!surveymeasurement_namecmp(&queue[i].m.to, &queue[p].m.from))
	    dir = 1;
	 if(!surveymeasurement_namecmp(&queue[i].m.from, &queue[p].m.to))
	    dir = -1;
	 if(dir) {
	    groundpos(i);
	    if(queue[i].grounded) {
	       float x[3];
	       get_vector(x, &queue[p].m);
	       if(dir > 0) {
		  memcpy(queue[p].posfrom, queue[i].posto,
			 sizeof queue[i].posto);
		  vector_add(queue[p].posto, queue[p].posfrom, x, 3);

		  gfx3d_setpos(queue[p].posto);
		  setminmax(queue[p].posto);

		  show_name(queue[p].m.to);
	       } else {
		  memcpy(queue[p].posto, queue[i].posfrom,
			 sizeof queue[i].posfrom);
		  vector_sub(queue[p].posfrom, queue[p].posto, x, 3);

		  gfx3d_setpos(queue[p].posfrom);
		  setminmax(queue[p].posfrom);

		  show_name(queue[p].m.from);
	       }

	       queue[p].grounded = 1;
	       gfx3d_line(queue[p].posfrom, queue[p].posto);

	       show_spokes(&queue[p].m, queue[p].posfrom);
	    }
	 }
      }
   }

   void groundqueue(void)
   {
      int i;
      for(i=0; i<queuelen; i++)
	 groundpos(i);
   }

   void forceground(int p)
   {
      queue[p].grounded = 1;

      queue[p].posfrom[0] = maxpos[0];
      queue[p].posfrom[1] = 0;
      queue[p].posfrom[2] = 0;

      float x[3];
      get_vector(x, &queue[p].m);
      vector_add(queue[p].posto, queue[p].posfrom, x, 3);
            
      gfx3d_setpos(queue[p].posfrom);
      show_name(queue[p].m.from);
      show_spokes(&queue[p].m, queue[p].posfrom);
      setminmax(queue[p].posfrom);

      gfx3d_setpos(queue[p].posto);
      show_name(queue[p].m.to);
      gfx3d_line(queue[p].posfrom, queue[p].posto);
      setminmax(queue[p].posto);
      
      groundqueue();
   }

   int i;
   for(i=0; i<2; i++)
      minpos[i] = maxpos[i] = 0;

   int pos = 0;
   struct surveymeasurement m;
   while((pos = surveymeasurement_read(pos, &m))) {
      if(!(m.header & DISTANCE))
	 continue;
      if(ruseflag && !(m.header & BACKSIGHT))
	 continue;

      if(queuepos < queuelen && !queue[queuepos].grounded) {
	 forceground(queuepos);
	 groundqueue();
      }
      
      queue[queuepos].grounded = 0;
      queue[queuepos].m = m;
      if(queuelen == 0)
	 forceground(0);

      if(++queuepos == (sizeof queue) / (sizeof *queue))
	 queuepos = 0;
      else
	 if(queuepos > queuelen)
	    queuelen = queuepos;
   }
   groundqueue();
}

static void setang(float x, float y, float z)
{
   ang[0] = x;
   ang[1] = y;
   ang[2] = z;
}

static void settra(float x, float y, float z)
{
   movo[0] += x*eeprom_read_float_safe(&translationspeed);
   movo[1] += y*eeprom_read_float_safe(&translationspeed);
   movo[2] += z*eeprom_read_float_safe(&translationspeed);
}

static void autoscale(void)
{
   gfx3d_reset_translation();
   gfx3d_translate(-(minpos[0] + maxpos[0]) / 2,
		   -(minpos[1] + maxpos[1]) / 2,
		   MAX(maxpos[0] - minpos[0],
		       2*(maxpos[1] - minpos[1])));
}

static int
setup_thread(void)
{
 static struct pt pt;
  PT_BEGIN(&pt);
restart:

   /* draw title */
   MENU_DRAW_TITLE(GFX_B08_FONT, 0, LCD_WIDTH-1, 0, 11, "Survey Plot Setup");

   gfx_setfont(GFX_R08_FONT);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);
   DRAW_SELECTION(0, LCD_WIDTH-1, 12, LCD_HEIGHT-1, "1 autoscale\n"
		  "2 translation speed\n3 use\n4 lrud\n"
		  "5 show names          D done");

   gfx_setpos(95, 22);
   GFX_PRINTF("%.2f\n", eeprom_read_float_safe(&translationspeed));

   gfx_setpos(70, 32);
   static const char t[] PROGMEM = "true", f[] PROGMEM = "false";
   GFX_PRINTF(PGM_STR_FMT"\n"PGM_STR_FMT"\n"PGM_STR_FMT,
	      pgm_read_ptr(usestrs+eeprom_read_byte_safe(&useflag)),
	      pgm_read_ptr(lrudstrs+eeprom_read_byte_safe(&lrudflag)),
	      eeprom_read_byte_safe(&shownames) ? t : f);

   for(;;) {
      switch(key_lastup()) {
      case KEY_1:
	 autoscale();
	 PT_EXIT(&pt);
      case KEY_2:
      {
	 float val = eeprom_read_float_safe(&translationspeed);
	 uint8_t accepted;
	 WAIT_ENTRY_NUMBER("Plot Translation Speed",
			   "Enter the speed",
			   &val, &accepted, .01, 10, 2, 0, NULL);
	 if(accepted)
	    eeprom_write_float_safe(&translationspeed, val);
      } goto restart;
      case KEY_3:
      {
	 int i = eeprom_read_byte_safe(&useflag);
	 if(++i == 3)
	    i = 0;
	 eeprom_write_byte_safe(&useflag, i);
      } goto restart;
      case KEY_4:
      {
	 int i = eeprom_read_byte_safe(&lrudflag);
	 if(++i == 2)
	    i = 0;
	 eeprom_write_byte_safe(&lrudflag, i);
      } goto restart;
      case KEY_5:
	 eeprom_write_byte_safe(&shownames, !eeprom_read_byte_safe(&shownames));
      goto restart;
      case KEY_D: PT_EXIT(&pt);
      }

     MENU_YIELD(&pt);
   }

  PT_END(&pt);
}

int surveyplot_thread(void)
{
   static struct pt pt;
   PT_BEGIN(&pt);

   static uint8_t rotating;
   gfx3d_reset_rotation();

   static uint8_t reautoscale;
   reautoscale = 1;

restart:
   /* clear out whole screen */
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, LCD_WIDTH-1, 0, LCD_HEIGHT-1, 1);

   /* draw selection */
   gfx_setfont(GFX_4x6_FONT);
   DRAW_SELECTION(0, 90, 58, LCD_HEIGHT-1,
		  "A about\nB setup\nC    \nD menu");

   gfx_setpos(68, 58);
   if(rotating)
      GFX_PUTS("rotate");
   else
      GFX_PUTS("translate");

   gfx_setalignment(GFX_ALIGN_CENTER | GFX_ALIGN_TOP);

   gfx3d_screenpos(LCD_WIDTH/2, LCD_HEIGHT/2);

   static uint8_t ango;
   for(;;) {
      static struct gfx_rect paintingarea = {0, LCD_WIDTH-1, 0, 57};

      gfx_setdrawtype(GFX_WHITE);
      gfx_rectangle(&paintingarea, 1);
      gfx_rectangle_abs(104, LCD_WIDTH-1, 49, LCD_HEIGHT-1, 1);

      gfx_setdrawtype(GFX_BLACK);
      gfx_setcliprect(&paintingarea);

      plot_draw();
      gfx_setcliprect(NULL);

      if(reautoscale) {
	 autoscale();
	 reautoscale = 0;
      }

      gfx_setpos(116, 53);
      gfx3d_draw_axes();

      if(rotating) {
	 if(keys & KEY_1) setang(1, 1, 0);
	 if(keys & KEY_2) setang(1, 0, 0);
	 if(keys & KEY_3) setang(1, -1, 0);
	 if(keys & KEY_4) setang(0, 1, 0);
	 if(keys & KEY_6) setang(0, -1, 0);
	 if(keys & KEY_7) setang(-1, 1, 0);
	 if(keys & KEY_8) setang(-1, 0, 0);
	 if(keys & KEY_9) setang(-1, -1, 0);
      } else {
	 if(keys & KEY_1) settra(1, -1, 0);
	 if(keys & KEY_2) settra(0, -1, 0);
	 if(keys & KEY_3) settra(-1, -1, 0);
	 if(keys & KEY_4) settra(1, 0, 0);
	 if(keys & KEY_6) settra(-1, 0, 0);
	 if(keys & KEY_7) settra(1, 1, 0);
	 if(keys & KEY_8) settra(0, 1, 0);
	 if(keys & KEY_9) settra(-1, 1, 0);
      }

      if(keys & KEY_POUND) setang(0, 0, 1);
      if(keys & KEY_STAR)  setang(0, 0, -1);
      if(keys & KEY_0) settra(0, 0, 1);
      if(keys & KEY_5) settra(0, 0, -1);

      if(!keys) {
	 setang(0, 0, 0);
	 ango = 0;
	 movo[0] = movo[1] = movo[2] = 0;
      }

      gfx3d_rotate(ango++, ang[0], ang[1], ang[2]);
      gfx3d_translate_after(movo[0], movo[1], movo[2]);
      gfx3d_bound_translation(-10000, -10000, 0, 10000, 10000, 100);

      switch(key_lastup()) {
      case KEY_A:
	 PT_WAIT_THREAD(&pt, reader_thread(about_surveyplot));
	 goto restart;
      case KEY_B:
         PT_WAIT_THREAD(&pt, setup_thread());
	 goto restart;
      case KEY_C:
	 rotating = !rotating;
	 goto restart;
      case KEY_D: PT_EXIT(&pt);
      }
      MENU_YIELD(&pt);
   }
   
   PT_END(&pt);
}
