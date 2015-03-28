/* Copyright (C) 2007 Sean D'Epagnier <sean@depagnier.com>
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

/* DEBUG should act as fprintf to stderr if enabled */

/* for the avr target, we don't want the format string taking up precious sram,
   so must use fprintf_P (regular fprintf would get undefined results) */

#include <stdio.h>

#ifndef NDEBUG
# ifdef __AVR__
# include <avr/pgmspace.h>
#   define DEBUG(s, ...) \
    do { static const char tmp[] PROGMEM = s; \
         fprintf_P(stderr, tmp, ## __VA_ARGS__); } while(0)
# else
#  define DEBUG(...) fprintf(stderr, __VA_ARGS__)
# endif
# define ASSERT(X) do { if (!(X)) DEBUG("ASSERT on %s:%d failed!\n", \
                        __FILE__, __LINE__); } while(0)
#else
# define DEBUG(...)
# define ASSERT(X)
#endif
