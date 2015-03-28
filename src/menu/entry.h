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

/* allows for setting numbers with keys 1 and 2 with acceleration on value */
#define DO_ENTRY_SCROLL(KEY, ONE, TWO, LABEL) \
      static int speed; \
      if(speed > 20) \
         speed = 20; \
      if(KEY && !keys) \
         speed = 3; \
      if(keys & KEY_1 || KEY == KEY_1) { \
          ONE \
          speed++; \
          LABEL \
      } \
      if(keys & KEY_2 || KEY == KEY_2) { \
          TWO \
          speed++; \
          LABEL; \
      } \
      speed = 0;

#define WAIT_ENTRY_NUMBER(TITLE, INSTRUCTIONS, value, accepted, min, max, \
			  decimalplaces, startpos, signs) \
    do { static const char tmptitle[] PROGMEM = TITLE; \
         static const char tmpinstructions[] PROGMEM = INSTRUCTIONS; \
         PT_WAIT_THREAD(&pt, entry_number_thread_P(tmptitle, tmpinstructions, \
                                                   value, accepted, \
                                                   min, max, decimalplaces, \
                                                   startpos, signs)); \
    } while(0)

int
entry_number_thread_P(const char *title, const char *instructions,
                      float *value, uint8_t *accepted, float min, float max,
                      int decimalplaces, int startpos, const char *signs);

#define WAIT_ENTRY_STRING(TITLE, INSTRUCTIONS, str, accepted, len) \
    do { static const char tmptitle[] PROGMEM = TITLE; \
         static const char tmpinstructions[] PROGMEM = INSTRUCTIONS; \
         PT_WAIT_THREAD(&pt, entry_string_thread_P(tmptitle, tmpinstructions, \
                                                   str, accepted, len)); \
    } while(0)

int
entry_string_thread_P(const char *title, const char *instructions,
                      char *str, uint8_t *accepted, int len);
