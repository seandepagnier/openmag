/* Copyright (C) 2007, 2008, 2009 Sean D'Epagnier <sean@depagnier.com>
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

/* file for storing and accessing saved measurements

   Things are complicated because writing to eeprom is so slow,
   so using linked lists in eeprom allows much less writing than
   shifting the whole memory every time
*/

#include <debug.h>

#include <math.h>

#include <string.h>

#ifndef M_PI
#define M_PI (2*asin(1))
#endif

#include <stdint.h>

#include "surveymeasurement.h"

/* __eeprom_end is the end of EEMEM variables,
   measurement data starts here E2END is the end of eeprom memory.
   Leave the last 4 for flash count */
#ifdef __AVR__

#include <avrdata.h>

extern uint8_t __eeprom_end;
#define EEPROM_FREE_START ((uint16_t)&__eeprom_end)
#define EEPROM_FREE_END (E2END - 5)

#define eeprom_read_offset(off) \
        eeprom_read_word_safe((uint16_t*)(off))
#define eeprom_read_byte_offset(off) \
        eeprom_read_byte_safe((uint8_t*)(off))
#define eeprom_read_block_offset(ram, off, len) \
        eeprom_read_block_safe(ram, (uint16_t*)(off), len)
#define eeprom_write_offset(off, val) \
        eeprom_write_word_safe((uint16_t*)(off), val)
#define eeprom_write_block_safe_offset(ram, off, len) \
        eeprom_write_block_safe(ram, (uint16_t*)(off), len)

/* head pointers to measurements and free list */
static uint16_t head EEMEM;
static uint16_t freehead EEMEM;

#define HEAD (uint16_t)(&head)
#define FREEHEAD (uint16_t)(&freehead)
#else
#include "host/eeprom.h"

#define EEPROM_FREE_START 1023
#define EEPROM_FREE_END ((sizeof eeprom) - 5)

#define eeprom_read_offset(off) \
        eeprom_read_word((uint16_t*)((uint8_t*)0 + off))
#define eeprom_read_byte_offset(off) \
        eeprom_read_byte(((uint8_t*)0 + off))
#define eeprom_read_block_offset(ram, off, len) \
        eeprom_read_block_safe(ram, (uint16_t*)((uint8_t*)0 + off), len)
#define eeprom_write_offset(off, val) \
        eeprom_write_word_safe((uint16_t*)((uint8_t*)0 + off), val)
#define eeprom_write_block_safe_offset(ram, off, len) \
        eeprom_write_block_safe(ram, (uint16_t*)((uint8_t*)0 + off), len)

#define eeprom_read_byte_safe eeprom_read_byte
#define eeprom_read_word_safe eeprom_read_word
#define eeprom_read_block_safe eeprom_read_block

#define eeprom_write_byte_safe eeprom_write_byte
#define eeprom_write_word_safe eeprom_write_word
#define eeprom_write_block_safe eeprom_write_block

#define HEAD 2
#define FREEHEAD 4
#endif

#define TO_FIXED_INCLINE(x) ((int16_t)(x*20))
#define TO_FLOAT_INCLINE(x) ((float)x/20)
#define TO_FIXED_AZIMUTH(x)   ((uint16_t)(x*10))
#define TO_FLOAT_AZIMUTH(x)   ((float)x/10)

/* convert incline and azimuth into 12 bit fixed point integers
   so both fit in 3 bytes */
static void compress_incline_azimuth(uint8_t data[3], float incline, float azimuth)
{
   uint32_t p = TO_FIXED_INCLINE(incline);
   uint32_t y = TO_FIXED_AZIMUTH(azimuth);

   uint32_t val = p<<12 | y;
   data[0] = (val & 0x0000ff) >> 0;
   data[1] = (val & 0x00ff00) >> 8;
   data[2] = (val & 0xff0000) >> 16;
}

static void decompress_incline_azimuth(uint8_t data[3], float *incline, float *azimuth)
{
   uint32_t val = data[0] | (uint32_t)data[1]<<8 | (uint32_t)data[2]<<16;
   int16_t p = (int16_t)((val>>12)<<4)>>4;
   uint16_t y = val&0xfff;

   if(incline)
      *incline = TO_FLOAT_INCLINE(p);
   if(azimuth)
      *azimuth = TO_FLOAT_AZIMUTH(y);
}

/* convert to a 8bit unsigned float (3 exponent 5 mantissa),
   biased by 1 mantissa and doubled, so the range is 0-127, but resolution
   is .0625 near 0, and 2 near 127 */
static uint8_t compress_float8(float c)
{
   float d = c/2 + 1;
   int e = log(d) / log(2);
   if(e > 7) e = 7;
   if(e < 0) e = 0;
   double trunc(double);
   int m = trunc((d / pow(2, e) - 1) * (1 << 5));
   return (e << 5) | m;
}

/* convert for 8bit float to normal float */
static float decompress_float8(uint8_t c)
{
   int e = c >> 5;
   int m = c & 0x1f;
   return 2 * (pow(2, e) * (1 + (float)m / (1<<5)) - 1);
}

/* for distance, use unsigned fixed point 10.6 format */
static void compress_distance(uint8_t *data, float distance)
{
   *(uint16_t*)data = (uint16_t)(distance * 64);
}

static float decompress_distance(uint8_t *data)
{
   return (float)*(uint16_t*)data / 64;
}

static int
simplecmp(int x, int y)
{
   if(x > y)
      return 1;
   if(x < y)
      return -1;
   return 0;
}

int
surveymeasurement_namecmp(struct surveyname *x, struct surveyname *y)
{
   return (simplecmp(x->name[0], y->name[0]) << 3)
        + (simplecmp(x->name[1], y->name[1]) << 2)
        + (simplecmp(x->name[2], y->name[2]) << 1)
        + (simplecmp(x->num, y->num) << 0);
}

/* given a header, figure out how much memory the measurement uses
   possible sizes:  5, 7, 8, 9, 10, 11, 12, 14, 15, 17, 18, 19, 20, 21, 22, 24 */
static uint8_t measurement_headersize(uint8_t header)
{
    uint8_t len = 5; /* header is 2, incline and azimuth is 3 */
    if(header & NEWNAME)
        len += 10;
    if(header & BACKSIGHT)
        len += 3;
    if(header & DISTANCE)
        len += 2;
    if(header & LRUD)
        len += 4;
    return len;
}

static void measurement_free(uint16_t data, uint16_t size)
{
    ASSERT(size >= 4);
    /* first go through the free list, and try to merge with
       an existing entree */
    uint16_t pos, last = FREEHEAD;
    uint16_t s;
    while((pos = eeprom_read_offset(last))) {
        s = eeprom_read_offset(pos);

        /* are we right in front of this block? */
        if(data+size == pos) {
            size += s;
            eeprom_write_offset(last, eeprom_read_offset(pos+2));
            break;
        }

        /* are we right after? */
        if(data == pos + s) {
            size += s;
            data = pos;
            eeprom_write_offset(last, eeprom_read_offset(pos+2));
            break;
        }

        last = pos + 2;
    }

    /* find where to insert into free list, keep order smallest first */
    last = FREEHEAD;
    while((pos = eeprom_read_offset(last))) {
        if(size < eeprom_read_offset(pos))
            break;
        last = pos + 2;
    }

    /* insert into free list */
    uint16_t next = eeprom_read_offset(last);
    eeprom_write_offset(last, data);
    eeprom_write_offset(data, size);
    eeprom_write_offset(data+2, next);
}

/* allocate size bytes in eeprom for a measurement */
static uint16_t measurement_allocate(uint16_t size)
{
    ASSERT(size >= 4);
    /* go through the free list and see if we can find a block
       the right size, either an exact match, or at least 4 bytes
       larger than needed so we can put the remaining data back on
       free list */
    uint16_t pos, last = FREEHEAD;
    while((pos = eeprom_read_offset(last))) {
        uint16_t s = eeprom_read_offset(pos);

        if(s == size || s >= size + 4) {
            /* remove it from the list */
            eeprom_write_offset(last, eeprom_read_offset(pos+2));

            /* put remaining data back in the free list */
            if(s -= size)
                measurement_free(pos, s);

            return pos + s;
        }

        last = pos + 2;
    }
    return 0;
}

/* total up all the bytes in the free list */
static uint16_t measurement_freespace(void)
{
    uint16_t total = 0;
    uint16_t pos = FREEHEAD;
    while((pos = eeprom_read_offset(pos))) {
        total += eeprom_read_offset(pos);
        pos += 2;
    }
    return total;
}

void surveymeasurements_clear(void)
{
   eeprom_write_offset(HEAD, 0);
   eeprom_write_offset(FREEHEAD, EEPROM_FREE_START);
   eeprom_write_offset(EEPROM_FREE_START, EEPROM_FREE_END - EEPROM_FREE_START);
   eeprom_write_offset(EEPROM_FREE_START+2, 0);
}

/* read measurement from pos in eeprom, return position in
   eeprom of the next measurement or 0 if there is no more */
uint16_t surveymeasurement_read(uint16_t pos, struct surveymeasurement *m)
{
    /* if head and freehead are set to 0, then initialize the free list */
    if(!eeprom_read_offset(HEAD) && !eeprom_read_offset(FREEHEAD))
        surveymeasurements_clear();

   if(pos == 0) {
      struct surveyname d = {SURVEYMEASUREMENT_DEFAULTNAME, 1};
      memcpy(&m->to, &d, sizeof d);
      pos = HEAD;
   }

    /* read pointer and header */
    uint16_t val = eeprom_read_offset(pos);

    /* if null pointer, end of list, no data to read */
    if(!val)
        return 0;

    /* header is top 4 bits */
    m->header = val>>12;

    /* pointer is lower 12 bits */
    pos = val&0xfff;

    if(m->header & NEWNAME) {
       eeprom_read_block_offset(&m->from.name, pos, 3);
       pos += 3;
       eeprom_read_block_offset(&m->from.num, pos, 2);
       pos += 2;
       eeprom_read_block_offset(&m->to.name, pos, 3);
       pos += 3;
       eeprom_read_block_offset(&m->to.num, pos, 2);
       pos += 2;
    } else {
       m->from = m->to;
       m->to.num++;
    }
    
   /* read incline and azimuth */
   uint8_t data[3];
   eeprom_read_block_offset(data, pos, 3);
   pos += 3;
   decompress_incline_azimuth(data, &m->incline, &m->azimuth);

   if(m->header & BACKSIGHT) {
      eeprom_read_block_offset(data, pos, 3);
      pos += 3;
      decompress_incline_azimuth(data, &m->bincline, &m->bazimuth);
   }

   /* read distance */
   if(m->header & DISTANCE) {
      eeprom_read_block_offset(data, pos, 2);
      m->distance = decompress_distance(data);
      pos += 2;
   }

   /* read lrud */
   int i;
   if(m->header & LRUD) {
      for(i = 0; i<4; i++)
         m->lrud[i] = decompress_float8(eeprom_read_byte_offset(pos+i));
      pos += 4;
   }

   return pos;
}

void surveymeasurement_delete(struct surveyname *from, struct surveyname *to)
{
    uint16_t pos = 0, nextpos;
    char na[3];
    uint16_t n;
    struct surveymeasurement m;

    while((nextpos = surveymeasurement_read(pos, &m))) {
       if(!surveymeasurement_namecmp(&m.from, from) && !surveymeasurement_namecmp(&m.to, to)) {
          /* this is the first measurement, need to bump pos to head */
          if(pos == 0)
             pos = HEAD;

          /* write in the nextpos to pos to un-link this node */
          uint16_t data = eeprom_read_offset(pos);
          eeprom_write_offset(pos, eeprom_read_offset(nextpos));
          measurement_free(data & 0xFFF, measurement_headersize(m.header));

          /* see if the next measurement after this one does
             not have a newname set, it will need one now */
          struct surveymeasurement m2;
          if(surveymeasurement_read(pos, &m2) && !(m2.header & NEWNAME)) {
              uint8_t adata[26]; /* new raw data */

              /* read in previous data */
              uint16_t adatalen = measurement_headersize(m2.header);
              data = eeprom_read_offset(pos) & 0xfff;
              eeprom_read_block_offset(adata + 10, data, adatalen);

              /* augment this node with NEWNAME data */
              m2.header |= NEWNAME;
              memcpy(adata, &m.to.name, 3);
              memcpy(adata+3, &m.to.num, 2);
              m.to.num++;
              memcpy(adata+5, &m.to.name, 3);
              memcpy(adata+8, &m.to.num, 2);

              /* free old data */
              measurement_free(data & 0xFFF, adatalen);

              /* allocate new data */
              adatalen += 10; /* add 10 for newname */
              uint16_t a = measurement_allocate(adatalen);

              /* store new data */
              eeprom_write_block_safe_offset(adata, a, adatalen);

              /* update pointer */
              eeprom_write_offset(pos, a | (m2.header << 12));
          }
          return; /* done */
      }
      pos = nextpos;
   }
}

/* return 0 if out of space, otherwise return 1, and
   either insert the measurement, or replace a measurement with identical
   name and number */
int surveymeasurement_store(struct surveymeasurement *m)
{
   uint16_t pos = 0, nextpos;

   /* make sure we don't already have a measurement
      with this name and number, unfortunately if we don't have enough memory
      to store the measurement (attributes added) it will get deleted, it
      would be better if it were left without the new attributes added */
   surveymeasurement_delete(&m->from, &m->to);

   /* figure out where to insert, try to insert after the last
      measurement with a to equal to this one's from */
   uint8_t hadname = 0;
   struct surveyname lastto = {SURVEYMEASUREMENT_DEFAULTNAME, 1};
   struct surveymeasurement n;
   while((nextpos = surveymeasurement_read(pos, &n))) {
      if(surveymeasurement_namecmp(&m->to, &n.from) <= 0)
         break;
      if(surveymeasurement_namecmp(&m->from, &n.to) >= 0)
         hadname = 1;
      else if(hadname)
	 if(surveymeasurement_namecmp(&m->from, &n.from)) /* append at end */
	    break; 

      lastto = n.to;
      pos = nextpos;
   }

   /* this is the first measurement, need to bump pos to head */
   if(pos == 0)
      pos = HEAD;

   uint8_t data[26] = {0};
   int i;
   int datalen=0;

   /* store new name if needed */
   if(surveymeasurement_namecmp(&m->from, &lastto)
      || (lastto.num++, surveymeasurement_namecmp(&m->to, &lastto))) {
       m->header |= NEWNAME;
       memcpy(data + datalen, &m->from.name, 3);
       datalen += 3;
       memcpy(data + datalen, &m->from.num, 2);
       datalen += 2;
       memcpy(data + datalen, &m->to.name, 3);
       datalen += 3;
       memcpy(data + datalen, &m->to.num, 2);
       datalen += 2;
   } else
       m->header &= ~NEWNAME;

   /* store incline and azimuth */
   compress_incline_azimuth(data+datalen, m->incline, m->azimuth);
   datalen += 3;

   if(m->header & BACKSIGHT) {
      compress_incline_azimuth(data+datalen, m->bincline, m->bazimuth);
      datalen += 3;
   }

   if(m->header & DISTANCE) {
       compress_distance(data + datalen, m->distance);
       datalen+=2;
   }

   if(m->header & LRUD) {
       for(i = 0; i<4; i++) {
           data[datalen] = compress_float8(m->lrud[i]);
           datalen++;
       }
   }

   /* store next pointer and its header */
   *(uint16_t*)(data + datalen) = eeprom_read_offset(pos);
   datalen+=2;

   uint16_t d = measurement_allocate(datalen);
   if(!d)
       return 0; /* not enough space to store this measurement */

   /* update previous next pointer and header */
   eeprom_write_offset(pos, d | (m->header << 12));

   /* store the data */
   eeprom_write_block_safe_offset(data, d, datalen);

   /* if the next entry is right after this one, then
      delete and re-add it so it doesn't need to store the newname,
      this decreases memory by 10 bytes each time! */
   if(nextpos && surveymeasurement_read(d + datalen - 2, &n))
       if(!surveymeasurement_namecmp(&m->to, &n.from)) {
           surveymeasurement_delete(&n.from, &n.to);
           surveymeasurement_store(&n);
       }

   return 1;
}

void surveymeasurement_stats(float *totaldist, int *freespace)
{
   uint16_t pos = 0;

   *totaldist = 0;

   struct surveymeasurement m;
   while((pos = surveymeasurement_read(pos, &m)))
      if(m.header & DISTANCE)
         *totaldist += m.distance;

   *freespace = measurement_freespace();
}

int surveymeasurement_find(struct surveymeasurement *m)
{
   uint16_t pos = 0;
   struct surveymeasurement n;
   while((pos = surveymeasurement_read(pos, &n)))
      if(!surveymeasurement_namecmp(&n.from, &m->from)
      && !surveymeasurement_namecmp(&n.to, &m->to)) {
         *m = n;
         return 1;
      }
   return 0;
}
