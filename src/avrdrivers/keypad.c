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

#include <util/delay.h>

#include <avr/interrupt.h>

#include <avrdrivers/pin.h>
#include <avrdrivers/testwiring.h>

#include <data/data.h>

#include "keypad.h"
#include "power.h"

#define NUMROWS 4
#define NUMCOLS 4
#define NUM (NUMROWS + NUMCOLS)

static struct pin keypad[NUM] = {
   MAKE_PIN(B, 4), MAKE_PIN(B, 5), MAKE_PIN(B, 6), MAKE_PIN(B, 7),
   MAKE_PIN(D, 4), MAKE_PIN(D, 5), MAKE_PIN(D, 6), MAKE_PIN(D, 7)
};

volatile uint8_t button_pressed;

uint16_t keys, lastkeyup;
void keypad_set(int key, int down)
{
   if(down) {
      if(!(keys & key)) {
         button_pressed = 1;
         keys |= key;
      }
   } else {
      if(keys & key) {
         lastkeyup = key;
         keys &= ~key;
      }
   }
}

/* handle io pins for keypad, and call
   key_set for each key to update the state */
static void keypad_setlowinput(void)
{
   int i;
   for(i = 0; i < NUMROWS; i++)
      SET_PIN_INPUT_PULLUP(keypad[i]);
   
   for(i = 4; i < NUM; i++)
      SET_PIN_OUTPUT_LOW(keypad[i]);
}

static void keypad_sethighinput(void)
{
   int i;
   for(i = 4; i < NUM; i++)
      SET_PIN_INPUT_PULLUP(keypad[i]);

   for(i = 0; i < NUMROWS; i++)
      SET_PIN_OUTPUT_LOW(keypad[i]);
}

static uint8_t readpins(struct pin pins[], int count)
{
   uint8_t ret=0, i;
   for(i = 0; i<count; i++)
      ret |= *pins[i].pin&pins[i].bv ? 0 : 1<<i;
   return ret;
}

void keypad_enable_wake(void)
{
    /* last row is PCINT */
    PCMSK0 |= _BV(PCINT4) | _BV(PCINT5) | _BV(PCINT6) | _BV(PCINT7);
}

void keypad_disable_wake(void)
{
    /* undo stuff from above */
    PCMSK0 &= ~(_BV(PCINT4) | _BV(PCINT5) | _BV(PCINT6) | _BV(PCINT7));
}

static void apply_keypad_keys(uint8_t high, uint8_t on,
                              int key0, int key1, int key2, int key3)
{
   keypad_set(key0, on?high&1:0);
   keypad_set(key1, on?high&2:0);
   keypad_set(key2, on?high&4:0);
   keypad_set(key3, on?high&8:0);
}

void keypad_poll(void)
{
   uint8_t low = readpins(keypad, 4);
   keypad_sethighinput();
   _delay_us(1); /* delay to let io settle, this may not be needed */
   uint8_t high = readpins(keypad + 4, 4);
   keypad_setlowinput();

   apply_keypad_keys(high, low&1, KEY_1, KEY_2, KEY_3, KEY_A);
   apply_keypad_keys(high, low&2, KEY_4, KEY_5, KEY_6, KEY_B);
   apply_keypad_keys(high, low&4, KEY_7, KEY_8, KEY_9, KEY_C);
   apply_keypad_keys(high, low&8, KEY_STAR, KEY_0, KEY_POUND, KEY_D);
}

void keypad_init(void)
{
   keypad_setlowinput();
   DEBUG("about to get to keypad poll...\n");
   keypad_poll();
}

void keypad_clear(void)
{
   keys = lastkeyup = button_pressed = 0;
}

void buttons_poll(void)
{
   keypad_set(BUTTON_1, !GET_PIN_INPUT(keypad[0]));
   keypad_set(BUTTON_2, !GET_PIN_INPUT(keypad[1]));
   keypad_set(BUTTON_3, !GET_PIN_INPUT(keypad[2]));
   keypad_set(BUTTON_4, !GET_PIN_INPUT(keypad[3]));
}

void buttons_init(void)
{
   keypad_setlowinput();
   buttons_poll();
}
