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

#if defined(__AVR_ATmega644__)
#define DDR_SPI DDRB
#define DD_MOSI DDB5
#define DD_MISO DDB6
#define DD_SCK  DDB7
#define DD_SS   DDB4
#elif defined(__AVR_AT90USB1287__) || defined(__AVR_ATmega2561__)
#define DDR_SPI DDRB
#define DD_MOSI DDB2
#define DD_MISO DDB3
#define DD_SCK  DDB1
#define DD_SS   DDB0
#else
#error "must add defines to spi.h for this cpu"
#endif

/* switch direction bit on MOSI to reduce power consumption,
   saves power by an additional 20 uA */
void spi_powersave(uint8_t save)
{
    if(save)
        DDR_SPI &= ~_BV(DD_MOSI);
    else
        DDR_SPI |= _BV(DD_MOSI);
}

void spi_init(void)
{
   /* Set MOSI, SCK, SS and RESET output */
   DDR_SPI |= (1<<DD_MOSI) | (1<<DD_SCK) | (1<<DD_SS);
   /* Set MISO input */
   DDR_SPI &= ~(1<<DD_MISO);

   /* Enable SPI, Master, set clock rate fck/8 */
   SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) | (1<<SPI2X);
}

void spi_setss(void)
{
   PORTB &= ~_BV(DD_SS);
}

void spi_clearss(void)
{
   PORTB |= _BV(DD_SS);
}

uint8_t __attribute__((always_inline)) spi_transfer(uint8_t data)
{
   /* Start transmission */
   SPDR = data;
   /* wait for transmission complete */
   while(!(SPSR & (1<<SPIF)));

   return SPDR;
}
