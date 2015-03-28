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

#include <stdio.h>
#include <stdint.h>

#include <avrdata.h>

#include <gfx/lcd.h>

#include <avrdrivers/hotwire.h>

#include "emulation.h"

char framebuffer[LCD_WIDTH*LCD_SPAN];

float temperature;

uint8_t hmc_setresetfailed[3];

uint8_t batterystate;

uint8_t magrange = 3;

uint8_t usemenu = 1;

void magrange_set(uint8_t range)
{
   magrange = range;
}

uint8_t magrange_get(void)
{
   return magrange;
}

void laser_on(void)
{
}

void laser_off(void)
{
}

uint8_t magrange_saturated(void)
{
   return 0;
}

float battery_charge(void)
{
    return 1;
}

int32_t hotwire_value(void)
{
  return 0;
}

enum hotwirestate hotwirestate;

float batteryvoltage;

uint8_t hotwiremode, hotwirefilter;
void hotwire_poll(void) {}

void hotwire_enable(void) {}
void hotwire_disable(void) {}

uint8_t backcolor[3];
void backlight_set(uint8_t white, uint8_t green)
{
   backcolor[0] = white * 32;
   backcolor[1] = white * 32 + green * 64;
   backcolor[2] = white * 32;
}

void mouse_pollbuttons(void)
{
}

uint16_t keys, lastkeyup;
void key_set(int key, int down)
{
   if(down) {
      if(!(keys & key))
         keys |= key;
   } else {
      if(keys & key) {
         lastkeyup = key;
         keys &= ~key;
      }
   }
}

uint16_t keypad_lastup(void)
{
   uint16_t k = lastkeyup;
   lastkeyup = 0;
   return k;
}

void buzzer_buzz(int duration) {}

uint8_t stillsleeptype;
uint8_t buttonidlesleeptype;

uint16_t stillsleeptimeout;
uint16_t buttonidletimeout;

const char *const sleeptypes[] = {"none", "sleeptest1", 0};

uint8_t powerwakeonmotion;
