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
#include <stdlib.h>

#include <math.h>

#include <string.h>
#include <stdint.h>

#include <data/data.h>
#include <data/databasictypes.h>
#include <data/databools.h>
#include <data/datastrings.h>

#include <lpc21xx/LPC214x.h>
#include <lpc21xx/irq.h>
#include <lpc21xx/armVIC.h>
#include <lpc21xx/uart.h>
#include <lpc21xx/startup.h>

//#define DATADRIVER_USE_UART0
#define DATADRIVER_USE_USB
#include <lpc21xx/datadriver.h>

#include <lpc21xx/spi.h>

#define SCA_INITSS   IODIR0 |= 0x00000080
#define SCA_CLEARSS  IOSET0 =  0x00000080
#define SCA_SETSS    IOCLR0 =  0x00000080
#define SCA_BEFORE_SS
#define SCA_AFTER_SS
#define SCA_ENABLE_INT
#define SCA_DISABLE_INT
#include <sca3000.h>

#include <memusage.h>

void SoftwareInterrupt (void) __irq {}


uint8_t show_sca;
DATA_ACCESSOR(name=show_sca type=bool mem=sram
              varname=show_sca writable)

int main (void)
{
   Initialize();
   init_VIC();

   spi_init();

   datadriver_init();

   enableIRQ();

   DEBUG("init\n");

   extern char *heap;
   memusage_init(heap);

   sca3000_init();

   int i=0;
   for(;;) {
#if 1
      int32_t data[3];
      uint16_t count[3];
      DATA_OUTPUT("%d\n", i++);

      if(sca3000_filter_data(data, count)) {
         if(show_sca)
            DATA_OUTPUT("%d\t%d\t%d\n", data[0], data[1], data[2]);
      }
#endif
      datadriver_poll();
      for(volatile int j=0; j<500000; j++);
      //      for(volatile int j=0; j<100000; j++);
   }
   return 0;
}
