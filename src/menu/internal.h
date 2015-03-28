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

#include <gfx/gfx.h>

#define MIN(a, b) ((a < b) ? (a) : (b))
#define MAX(a, b) ((a > b) ? (a) : (b))

void nice_num_P(const char *str, float val);

#define NICE_NUM(str, val) \
   do { \
       static const char tmp[] PROGMEM = str; \
       nice_num_P(tmp, val); \
   } while(0)

void menu_preparearea(const struct gfx_rect *area);

void menu_draw_title_P(const struct gfx_rect *r, const char *str);

#define MENU_DRAW_TITLE(FONT, L, R, U, D, STR) \
   do { static const char tmp[] PROGMEM = STR; \
        static const struct gfx_rect area PROGMEM = {L, R, U, D}; \
        gfx_setfont(FONT); \
        menu_draw_title_P(&area, tmp); } while(0)

int reader_thread(const char *text);

void draw_selection(const struct gfx_rect *a, const char *t);

#define DRAW_SELECTION(a, b, c, d, t)				\
   do {								\
      static const struct gfx_rect atmp PROGMEM = {a, b, c, d};	\
      static const char ttmp[] PROGMEM = t;			\
      draw_selection(&atmp, ttmp);				\
   } while(0)

#define DRAW_SELECTION_A(a, t)	    \
   do {					    \
      static const char ttmp[] PROGMEM = t; \
      draw_selection(a, ttmp);		    \
   } while(0)

int compass_thread(void);
int survey_thread(void);
int lynnsurvey_thread(void);
int pedometer_thread(void);
int gaussmeter_thread(void);
int plots_thread(void);
int hotwire_thread(void);
int stats_thread(void);
int calibration_thread(void);
int settings_thread(void);
int plots_thread(void);
int graph_entry_custom_thread(void);
int vectorview_thread(void);
int anemometer_thread(void);
int calculator_thread(void);
int spherepainter_thread(void);
int surveyplot_thread(void);

#define MENU_YIELD(pt) \
    do { \
        if(menuscreendirty) { \
            menuscreendirty=0; \
            goto restart; \
        } \
        PT_YIELD(pt); \
    } while(0)

extern uint8_t menuscreendirty;

char *calculator_linebuffer(void);
void plots_setup(float s, float o, const char *l);
float calibrated_anemometer(void);
