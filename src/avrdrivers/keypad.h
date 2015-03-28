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

enum KeyTypes {KEY_1 = 0x0001, KEY_2 = 0x0002, KEY_3 = 0x0004, KEY_4 = 0x0008,
               KEY_5 = 0x0010, KEY_6 = 0x0020, KEY_7 = 0x0040, KEY_8 = 0x0080,
               KEY_9 = 0x0100, KEY_0 = 0x0200, KEY_STAR = 0x0400, KEY_POUND = 0x0800,
               KEY_A = 0x1000, KEY_B = 0x2000, KEY_C = 0x4000, KEY_D = 0x8000};

#define KEYS_NUMERIC_TO(X) ((1<<X)-1)
#define KEYS_NUMERIC KEYS_NUMERIC_TO(10)
#define KEYS_SYMBOLS (KEY_STAR | KEY_POUND)
#define KEYS_ALPHA (KEY_A | KEY_B | KEY_C | KEY_D)
#define KEYS_ALPHANUMERIC (KEYS_ALPHA | KEYS_NUMERIC)
#define KEYS_ALL (KEYS_SYMBOLS | KEYS_ALPHANUMERIC)

void keypad_init(void);
void keypad_clear(void);
void keypad_poll(void);

void keypad_enable_wake(void);
void keypad_disable_wake(void);

void keypad_set(int key, int down);

extern uint16_t keys;

volatile uint8_t button_pressed;

enum Buttons {BUTTON_1 = 0x01, BUTTON_2 = 0x02,
	      BUTTON_3 = 0x04, BUTTON_4 = 0x08};

void buttons_poll(void);
void buttons_init(void);
