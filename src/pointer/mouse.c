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
 * * For more information on the GPL, please go to:
 * http://www.gnu.org/copyleft/gpl.html
 */

#ifdef USE_MOUSE

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include <avr/wdt.h>

#include <debug.h>

#include <data/data.h>
#include <data/databasictypes.h>
#include <data/datadirectories.h>
#include <data/dataenums.h>
#include <data/databools.h>

#include <debug.h>

#include <avrdata.h>

#include <linalg/vector.h>
#include <linalg/rotate.h>
#include <linalg/quaternion.h>

#include <calibration/sensor.h>
#include <calibration/calc.h>
#include <calibration/cal.h>
#include <calibration/filter.h>

#include <avrdrivers/keypad.h>
#include <avrdrivers/descriptors.h>

extern uint8_t useusbmouse;

DATA_ACCESSOR(name=mouse dirname=mouse)

uint8_t mouseshakewheelenabled EEMEM;
DATA_ACCESSOR(name=enableshakewheel type=bool mem=eeprom 
              varname=mouseshakewheelenabled writable dir=mouse)

static const char X[] PROGMEM = "X";
static const char Y[] PROGMEM = "Y";
static const char Z[] PROGMEM = "Z";
const char *const orientations[] PROGMEM = {X, Y, Z, 0};

uint8_t mousehorizontalorientation EEMEM;
DATA_ACCESSOR(name=horizontalorientation type=enumerant mem=eeprom 
              varname=mousehorizontalorientation enumvalues=orientations
              writable dir=mouse)

uint8_t mouseverticalorientation EEMEM;
DATA_ACCESSOR(name=verticalorientation type=enumerant mem=eeprom 
              varname=mouseverticalorientation enumvalues=orientations
              writable dir=mouse)

int16_t mousehorizontalsensitivity EEMEM = 500;
DATA_ACCESSOR(name=horizontalsensitivity type=int16_t mem=eeprom 
              varname=mousehorizontalsensitivity writable dir=mouse)

int16_t mouseverticalsensitivity EEMEM = 600;
DATA_ACCESSOR(name=verticalsensitivity type=int16_t mem=eeprom 
              varname=mouseverticalsensitivity writable dir=mouse)

int16_t mousescrollsensitivity EEMEM = 50;
DATA_ACCESSOR(name=scrollsensitivity type=int16_t mem=eeprom 
              varname=mousescrollsensitivity writable dir=mouse)

struct pos {
   int32_t x, y;
} x, y;

struct age
{
   struct pos pos, last;
   uint8_t running;
};

static struct age point;

static volatile uint8_t buttons;
static volatile int8_t shakewheel;

enum MouseMode {POINT, SCROLL};
static enum MouseMode mousemode;

enum {BUTTON_LEFT, BUTTON_RIGHT, BUTTON_MIDDLE};

static void setbutton(int button, int down)
{
   if(down)
      buttons |= 1<<button;
   else
      buttons &= ~(1<<button);
}

static uint8_t doubleclicktrigger;
static struct pos trigger;
void mouse_pollbuttons(void)
{
   static uint16_t middletrigger;
   uint8_t inmiddletriggerregion =
      abs(trigger.x - point.pos.x) + abs(trigger.y - point.pos.y) < 1000;

   setbutton(BUTTON_LEFT, keys & BUTTON_1);

   if(middletrigger && inmiddletriggerregion && keys & BUTTON_1) {
      doubleclicktrigger = 2;
      middletrigger = 0;
   }

   if(keys & BUTTON_3) {
      if(inmiddletriggerregion)
         middletrigger = 500;
      if(mousemode != SCROLL) {
         point.running = 0;
         mousemode = SCROLL;
      }
   } else
      if(mousemode != POINT) {
         point.running = 0;
         mousemode = POINT;
      }
    
   if(middletrigger) {
      int down = keys & BUTTON_2;
      setbutton(BUTTON_MIDDLE, down);
      if(!down)
         middletrigger--;
      setbutton(BUTTON_RIGHT, 0);
   } else {
      setbutton(BUTTON_MIDDLE, 0);
      setbutton(BUTTON_RIGHT, keys & BUTTON_2);
   }
}

static uint8_t pickshakewheelval(uint8_t val)
{
    if(val < 6)
        return 1;
    if(val < 12)
        return 4;
    return val;
}

void mouse_update(int32_t m[3])
{
   /* don't do these calculations if there is no usb */
   if(!USB_VBUS_GetStatus())
      return;

   if(!useusbmouse)
      return;

   float nm[3] = {m[0], m[1], m[2]};
   normalize(nm);

   float r, p;
   switch(eeprom_read_byte_safe(&mousehorizontalorientation)) {
      case 0:
         r = atan2(-nm[1], nm[2]);
	 break;
      case 1:
         r = atan2(nm[0], nm[2]);
	 break;
      case 2:
         r = atan2(-nm[0], nm[1]);
	 break;
   }

   switch(eeprom_read_byte_safe(&mouseverticalorientation)) {
      case 0:
	 p = asin(nm[0]);
	 break;
      case 1:
         p = asin(nm[1]);
	 break;
      case 2:
         p = asin(nm[2]);
	 break;
   }

   /* calculate rolls */
   static int rolls;
   static float lastr;
   if(lastr > M_PI*.9 && r < -M_PI*.9)
      rolls++;
   if(lastr < -M_PI*.9 && r > M_PI*.9)
      rolls--;
   lastr = r;
   r += 2*M_PI*(float)rolls;

   int16_t horizontalsensitivity =
      (int16_t)eeprom_read_word_safe((uint16_t*)&mousehorizontalsensitivity);

   int16_t verticalsensitivity =
      mousemode == POINT ?
      (int16_t)eeprom_read_word_safe((uint16_t*)&mouseverticalsensitivity):
      (int16_t)eeprom_read_word_safe((uint16_t*)&mousescrollsensitivity);

   struct pos pos = {r*horizontalsensitivity*(1<<4), p*verticalsensitivity*(1<<4)};

   /* if the fast cal changes, then reset pos so the pointer doesn't jump */
   static float lastfastmag;
   extern float magfastX[4];
   if(lastfastmag != magfastX[3]) {
      lastfastmag = magfastX[3];
      point.running = 0;
   }

   /* if running, filter and update, otherwise
      reset everything to the current position */
   if(point.running) {
      point.pos.x = filter_quadratic(point.pos.x, pos.x, 256, 0);
      point.pos.y = filter_quadratic(point.pos.y, pos.y, 256, 0);
   } else {
      trigger = point.last = point.pos = pos;
      point.running = 1;
   }
}

void mouse_update_shake_wheel(int32_t a[3])
{
   /* don't do these calculations if there is no usb */
   if(!USB_VBUS_GetStatus())
      return;

   if(!useusbmouse)
      return;

   if(!eeprom_read_byte_safe(&mouseshakewheelenabled))
      return;

   uint32_t mag = a[0]*a[0]+a[1]*a[1]+a[2]*a[2];

   static uint8_t shakewheelstate;
   static uint16_t timeout;
   static uint8_t shakewheelval;
   enum {IDLE, LOW, LOWREC, HIGH, WAIT};

   switch(shakewheelstate) {
   case IDLE:
       if(mag < 3e5) {
           shakewheelstate = LOW;
           shakewheelval = 1;
           timeout = 20;
       } else
       if(mag > 5e6) {
           shakewheelstate = HIGH;
           shakewheelval = 1;
           timeout = 90;
       }       
       break;
   case LOW:
       if(mag > 2e6) {
           shakewheelstate = LOWREC;
           timeout = 30;
       } else
           if(mag < 3e5)
               timeout = 20;
       break;
   case LOWREC:
       if(timeout == 1) {
           shakewheel = -pickshakewheelval(shakewheelval);
           shakewheelstate = WAIT;
           timeout = 60;
       } else {
           int32_t val = (mag-17e5)/1e6;
           if(shakewheelval < val)
               shakewheelval = val;
       }
       break;
   case HIGH:
       if(mag > 3e6) {
           int32_t val = (mag-17e5)/1e6;
           if(shakewheelval < val)
               shakewheelval = val;
       } else
       if(mag < 5e5) {
           shakewheel = pickshakewheelval(shakewheelval);
           shakewheelstate = WAIT;
           timeout = 60;
       }
       break;
   }

   if(timeout)
       if(!--timeout)
           shakewheelstate = IDLE;
}

int32_t getmovement(int32_t pos, int32_t *last, int32_t maxmove)
{
   int32_t x = pos - *last;

   x >>= 4;

   if(x > maxmove)
      x = maxmove;
   if(x < -maxmove)
      x = -maxmove;

   int32_t ret = x;

   x <<= 4;
   *last += x;

   return ret;
}

void mouse_transmit(void)
{
   if(!useusbmouse)
      return;

   /* Select the Mouse Report Endpoint */
   Endpoint_SelectEndpoint(MOUSE_EPNUM);

   /* Check if Mouse Endpoint Ready for Read/Write */
   if (!Endpoint_ReadWriteAllowed())
      return;

   /* Write Mouse Report Data */
   uint8_t mask = 0;
   
   switch(doubleclicktrigger) {
   case 2:
      doubleclicktrigger = 1;
      break;
   case 1:
      doubleclicktrigger = 0;
      buttons &= ~BUTTON_1;
      break;
   }
   
   mask |= buttons;
   Endpoint_Write_Byte(mask);
   
   struct pos p = {0, 0};
   
   if(mousemode == POINT) {
      p.x = getmovement(point.pos.x, &point.last.x, 8);
      p.y = getmovement(point.pos.y, &point.last.y, 6);
   }
   
   Endpoint_Write_Byte(p.x);
   Endpoint_Write_Byte(p.y);
   
   int8_t wheel = 0;
   
   wheel += shakewheel;
   shakewheel = 0;
   
   if(mousemode == SCROLL)
      wheel -= getmovement(point.pos.y, &point.last.y, 16);
   
   Endpoint_Write_Byte(wheel);
   
   /* Handshake the IN Endpoint - send the data to the host */
   Endpoint_ClearCurrentBank();
}

#endif
