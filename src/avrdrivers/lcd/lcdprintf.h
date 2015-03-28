/* lcd driver Copyright (C) 2007 Sean D'Epagnier <sean@depagnier.com>
 *
 * This Driver is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
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
#include <avr/pgmspace.h>

#define LCD_NUMCHAR 20

void lcd_setLine(uint8_t line, uint8_t off, char *data, uint8_t count);

/* make it a macro so we can avoid having the fmt in sram */
#define lcd_printf(line, off, fmt, ...) \
  do { \
     static const char tmp[] PROGMEM = fmt; \
     char buf[LCD_NUMCHAR]; \
     snprintf_P(buf, sizeof buf, tmp, ## __VA_ARGS__); \
     lcd_setLine(line, off, buf, strlen(buf)); \
  } while(0)
