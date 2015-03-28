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

#define RB_DECLARE(name, type, index_type, size) \
static type name[size]; \
static index_type name##head; \
static index_type name##tail; \
static const int name##len = size;

#define RB_EMPTY(rb) (rb##tail == rb##head)
#define RB_FULL(rb) (rb##tail == ((rb##head+1 == rb##len) ? 0 : rb##head+1))
#define RB_GET(rb, c) do { c = rb[rb##tail++]; if(rb##tail == rb##len) rb##tail=0; } while(0)
#define RB_PUT(rb, c) do { rb[rb##head++] = c; if(rb##head == rb##len) rb##head=0; } while(0)
#define RB_FREE(rb) (rb##tail > rb##head ? rb##tail - rb##head : rb##len - rb##head + rb##tail)
#define RB_SET_EMPTY(rb) (rb##tail = rb##head = 0)
