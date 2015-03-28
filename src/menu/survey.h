/* Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
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

#define WAIT_LRUD(instructions, num)           \
    do { \
        static const char tmp[] PROGMEM = instructions; \
        float n = *(num);                                             \
        PT_WAIT_THREAD(&pt, survey_enter_lrud_thread(tmp, &n));       \
        if(isnan(n)) \
            goto restart; \
        *(num) = n;       \
    } while(0);

#define WAIT_ENTRY_SURVEYNAME(title, survey) \
   do { static const char tmp[] PROGMEM = title; \
        PT_WAIT_THREAD(&pt, survey_wait_entry_surveyname_P(tmp, survey));      \
      } while(0);

int survey_stats_thread(void);
int survey_stored_thread(void);
void survey_draw_incline_azimuth(float incline, float azimuth);
void survey_print_from_to(struct surveymeasurement *m);

int survey_enter_lrud_thread(const char *instructions, float *num);
int survey_wait_entry_surveyname_P(const char *title, struct surveyname *survey);

extern uint8_t rounding;
