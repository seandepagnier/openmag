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
#include <data/datastrings.h>

#include <lpc21xx/irq.h>
#include <lpc21xx/armVIC.h>
#include <lpc21xx/uart.h>
#include <lpc21xx/startup.h>

#define DATADRIVER_USE_UART0
#define DATADRIVER_USE_USB
#include <lpc21xx/datadriver.h>

uint8_t x; DATA_ACCESSOR(name=x type=uint8_t mem=sram varname=x writable)

char strtest[20]; DATA_ACCESSOR(name=strtest type=string mem=sram varname=strtest
                                arraylen=20 writable)

#include <memusage.h>

int main (void)
{
   Initialize();
   init_VIC();

   datadriver_init();

   enableIRQ();

   DEBUG("init\n");

   extern char *heap;
   memusage_init(heap);

   int i = 0;
   while (1) 
   {
      datadriver_poll();
      for(volatile int j=0; j<500000; j++);
   }
   return 0;
}
