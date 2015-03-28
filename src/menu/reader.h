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

#define SIZE_MASK 7
#define ALIGNMENT_MASK 24

enum {
   /* pick 1 */
   SIZE0, SIZE1, SIZE2, SIZE3, SIZE4, SIZE5,
   /* pick 1 */
   LEFT=0, RIGHT=8, CENTER=16, JUSTIFY=24,
   /* control or not? if 0, this is just a regular character,
      and the above codes don't apply */
   CONTROL=128
};

#define DEFAULT_ATTRIBS (SIZE3)
