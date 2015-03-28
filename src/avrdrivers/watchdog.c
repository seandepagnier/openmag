/* Copyright (C) 2007, 2008 Sean D'Epagnier <sean@depagnier.com>
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

#include <avr/wdt.h>
#include <avr/interrupt.h>

#include <data/data.h>
#include <data/databasictypes.h>
#include <data/datadirectories.h>

#include <avrdata.h>

/* for watchdog, increment a queryable counter to keep
   track of these unexpected resets */
uint16_t watchdog_resets EEMEM;
DATA_ACCESSOR(name=watchdog_resets mem=eeprom type=uint16_t
              varname=watchdog_resets clearvalue=0 dir=stats)

volatile uint8_t watchdog_wakeme;
ISR(WDT_vect)
{
   if(watchdog_wakeme)
      watchdog_wakeme = 2;
   else {
      eeprom_write_word_safe(&watchdog_resets,
                             eeprom_read_word_safe(&watchdog_resets) + 1);
      extern char *_end;
   }
}

#define _wdt_write_interrupt_reset(value)   \
    __asm__ __volatile__ (  \
        "in __tmp_reg__,__SREG__" "\n\t"    \
        "cli" "\n\t"    \
        "wdr" "\n\t"    \
        "sts %0,%1" "\n\t"  \
        "out __SREG__,__tmp_reg__" "\n\t"   \
        "sts %0,%2" \
        : /* no outputs */  \
        : "M" (_SFR_MEM_ADDR(_WD_CONTROL_REG)), \
        "r" (_BV(_WD_CHANGE_BIT) | _BV(WDE)), \
        "r" ((uint8_t) ((value & 0x08 ? _WD_PS3_MASK : 0x00) | \
            _BV(WDE) | _BV(WDIE) | (value & 0x07)) ) \
        : "r0"  \
    )

#define wdt_enable_interrupt_reset(timeout) _wdt_write_interrupt_reset(timeout)

void watchdog_init(void)
{
   /* set up watchdog with 2s timeout and interrupt with reset */
   wdt_enable_interrupt_reset(WDTO_2S);
   watchdog_wakeme = 0;
}

void watchdog_set_wake(void)
{
   /* set up watchdog to sleep for 8 seconds then interrupt  */
   watchdog_wakeme = 1;
   wdt_enable_interrupt_reset(WDTO_8S);
}

uint8_t watchdog_woke(void)
{
   if(watchdog_wakeme == 2) {
       watchdog_wakeme = 1;
       return 1;
   }
   return 0;
}
