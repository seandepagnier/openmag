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

#include <gfx/gfx.h>
#include <gfx/font.h>
#include <gfx/lcd.h>

#include <avr-pt.h>

#include <debug.h>

#include <avrdata.h>

#include <calibration/sensor.h>

#include <avrdrivers/battery.h>

#include "menu.h"
#include "keys.h"

uint8_t menuscreendirty;

#include "readertext.h"
#include "internal.h"

void menu_preparearea(const struct gfx_rect *area)
{
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle(area, 1);
   gfx_setdrawtype(GFX_BLACK);
   gfx_setpos((area->left+area->right)/2, (area->top+area->bottom)/2);
}

void menu_draw_title_P(const struct gfx_rect *r, const char *str)
{
   struct gfx_rect rect;
   memcpy_P(&rect, r, sizeof rect);
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle(&rect, 1);
   gfx_setdrawtype(GFX_BLACK);
   gfx_center_in_rect(&rect);
   gfx_puts_P(str);
}

static struct gfx_rect selectionarea PROGMEM =
#ifdef __AVR_AT90USB1287__
{0, LCD_WIDTH-1, 16, LCD_HEIGHT-5};
#else
{0, LCD_WIDTH-1, 12, LCD_HEIGHT-1};
#endif

static void draw_mainmenu_title(void)
{
   /* clear screen */
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, LCD_WIDTH-1, 0, LCD_HEIGHT-1, 1);

   /* draw title */
   MENU_DRAW_TITLE(GFX_B12_FONT, 0, LCD_WIDTH-1, 0, LCD_HEIGHT/4-5, "Main Menu");
}

static int
mainmenu_page2_thread(void)
{
  static struct pt pt;
  PT_BEGIN(&pt);

restart:
  draw_mainmenu_title();

   /* draw selection */
   gfx_setfont(GFX_R08_FONT);

   DRAW_SELECTION_A(&selectionarea, "1 advanced survey\n2 anemometer\n"
		    "3 calculator\n4 sphere painter\nA about   D back");
   for(;;) {
      switch(key_lastup()) {
      case KEY_1: PT_WAIT_THREAD(&pt, lynnsurvey_thread()); goto restart;
      case KEY_2: PT_WAIT_THREAD(&pt, anemometer_thread()); goto restart;
      case KEY_3: PT_WAIT_THREAD(&pt, calculator_thread()); goto restart;
      case KEY_4: PT_WAIT_THREAD(&pt, spherepainter_thread()); goto restart;
      case KEY_A: PT_WAIT_THREAD(&pt, reader_thread(about)); goto restart;
      case KEY_0:
      case KEY_D: PT_EXIT(&pt);
      }

      MENU_YIELD(&pt);
   }

   PT_END(&pt);
}


/* main menu */
static int
mainmenu_thread(void)
{
   static struct pt pt;

   PT_BEGIN(&pt);
restart:

   draw_mainmenu_title();

   /* draw selection */
   gfx_setfont(GFX_R08_FONT);
   DRAW_SELECTION_A(&selectionarea, 
		    "1 compass\n2 survey\n3 pedometer\n4 gaussmeter\n"
		    "5 stats\n6 calibration\n7 settings\n"
#ifdef __AVR_AT90USB1287__
		    "A about";
#else
		    "8 plots\n9 vector view\n0 more\n"
#endif
      );
   
   for(;;) {
      switch(key_lastup()) {
      case KEY_1: PT_WAIT_THREAD(&pt, compass_thread()); goto restart;
      case KEY_2: PT_WAIT_THREAD(&pt, survey_thread()); goto restart;
      case KEY_3: PT_WAIT_THREAD(&pt, pedometer_thread()); goto restart;
      case KEY_4: PT_WAIT_THREAD(&pt, gaussmeter_thread()); goto restart;
      case KEY_5: PT_WAIT_THREAD(&pt, stats_thread()); goto restart;
      case KEY_6: PT_WAIT_THREAD(&pt, calibration_thread()); goto restart;
      case KEY_7: PT_WAIT_THREAD(&pt, settings_thread()); goto restart;
#ifdef __AVR_AT90USB1287__
      case KEY_A: PT_WAIT_THREAD(&pt, reader_thread(about)); goto restart;
#else
      case KEY_8: PT_WAIT_THREAD(&pt, plots_thread()); goto restart;
      case KEY_9: PT_WAIT_THREAD(&pt, vectorview_thread()); goto restart;
      case KEY_0: PT_WAIT_THREAD(&pt, mainmenu_page2_thread()); goto restart;
#endif
      }

      MENU_YIELD(&pt);
   }

   PT_END(&pt);
}

static int
accelerometer_failure_thread(void)
{
   static struct pt pt;
   PT_BEGIN(&pt);

   static int sawfailed;
   if(sawfailed)
      PT_EXIT(&pt);

   PT_WAIT_THREAD(&pt, reader_thread(warning_accelerometer_failure));
   sawfailed = 1;

   PT_END(&pt);
}

static int
magnetometer_failure_thread(void)
{
   static struct pt pt;
   PT_BEGIN(&pt);

   static uint8_t sawfailed;
   if(sawfailed)
      PT_EXIT(&pt);
   sawfailed = 1;

   PT_WAIT_THREAD(&pt, reader_thread(warning_magnetometer_failure));

   PT_END(&pt);
}

static int
magnetometer_setreset_failure_thread(void)
{
   static struct pt pt;
   PT_BEGIN(&pt);

   static uint8_t sawfailed;
   if(sawfailed)
      PT_EXIT(&pt);
   sawfailed = 1;

   PT_WAIT_THREAD(&pt, reader_thread(warning_magnetometer_setreset_failure));

   PT_END(&pt);
}

static int
magnetometer_saturation_thread(void)
{
   extern uint8_t magrange_get(void);
   extern void magrange_set(uint8_t);

   static struct pt pt;

   PT_BEGIN(&pt);
restart:
   gfx_setdrawtype(GFX_WHITE);
   gfx_rectangle_abs(0, LCD_WIDTH-1, 0, LCD_HEIGHT-18, 1);

   gfx_setdrawtype(GFX_BLACK);
   gfx_setalignment(GFX_ALIGN_LEFT | GFX_ALIGN_TOP);

   gfx_setpos(0, -1);
   gfx_setfont(GFX_R08_FONT);
   GFX_PUTS("Magnetometer saturated!\n");
   if(magrange_get()) {
      gfx_setfont(GFX_R08_FONT);
      GFX_PUTS("You may double the range\n"
               "at loss of precision.  \n");

      gfx_setfont(GFX_4x6_FONT);
      GFX_PUTS("note: This value is adjustable\n"
               "      under settings.");
   } else {
      gfx_setfont(GFX_R08_FONT);
      GFX_PUTS("The range is already at the\n"
               "maximum.\n");
   }

   /* let the user select an item */
   gfx_setfont(GFX_R08_FONT);
   static const char items1[] PROGMEM = "1 switch to larger range\n2 continue";
   const char *items = items1;

   if(!magrange_get())
      items += 25;

   static const struct gfx_rect selectionarea PROGMEM =
      {0, LCD_WIDTH-1, LCD_HEIGHT-22, LCD_HEIGHT-1};
   draw_selection(&selectionarea, items);

   for(;;) {
      switch(key_lastup()) {
      case KEY_1:
         if(magrange_get())
            magrange_set(magrange_get() - 1);
      case KEY_2:
         PT_EXIT(&pt);
      }

      MENU_YIELD(&pt);
   }

   PT_END(&pt);
}

int menu_poll(void)
{
   static struct pt pt;
   PT_BEGIN(&pt);

   /* any sensors failed? */
   if(*accel.failure) {
      PT_WAIT_THREAD(&pt, accelerometer_failure_thread());
      menuscreendirty=1;
   }

   if(*mag.failure) {
      PT_WAIT_THREAD(&pt, magnetometer_failure_thread());
      menuscreendirty=1;
   }

   /* magnetometer strap test failed? */
   extern uint8_t hmc_setresetfailed[3];
   if(hmc_setresetfailed[0] ||
      hmc_setresetfailed[1] ||
      hmc_setresetfailed[2]) {
      PT_WAIT_THREAD(&pt, magnetometer_setreset_failure_thread());
      menuscreendirty=1;
   }

   /* test to see if we have magnetic saturation and need to alert the user */
   extern uint8_t magrange_saturated(void);
   if(magrange_saturated()) {
      PT_WAIT_THREAD(&pt, magnetometer_saturation_thread());
      menuscreendirty=1;
   }

   /* did we run out of power? */
   static uint8_t lastbatterystate;
   if(lastbatterystate && !batterystate) {
       PT_WAIT_THREAD(&pt, reader_thread(warning_power_failure));
       menuscreendirty=1;
   }
   lastbatterystate = batterystate;

   /* run the main menu */
   mainmenu_thread();

   PT_END(&pt)
}
