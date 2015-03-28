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

#include <debug.h>
#include <stdint.h>
#include <LPC214x.h>

enum {CPHA = (1<<3), CPOL = (1<<4),
      MSTR = (1<<5), LSBR = (1<<6), SPIE = (1<<7)};
enum {ABRT = (1<<3), MODF = (1<<4), ROVR = (1<<5),
      WCOL = (1<<6), SPIF = (1<<7)};

void spi_init(void)
{
   S0SPCCR = 32; /* about 1.8 megabits */
   S0SPCR |= MSTR; /* set spi to master mode */

   PINSEL0 |= 0x00001500; /* enable SCK0 MISO0 MOSI0 */
}

uint8_t spi_transfer(uint8_t byte)
{
   S0SPDR = byte;
   while(!(S0SPSR & SPIF));
   return S0SPDR;
}
