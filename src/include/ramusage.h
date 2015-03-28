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
 * For more information on the GPL, please go to: * http://www.gnu.org/copyleft/gpl.html
 */

DATA_ACCESSOR(name=freeram dir=stats)
DATA_OPERATOR(get, name=freeram,
              DATA_OUTPUT("%d\n", getfreeram());
     )

extern int _end;
int ramusage_free(void *start);

/* avr does not have a heap */
#ifdef __AVR__
int getfreeram(void)
{
   return ramusage_free(&_end);
}
#else
int getfreeram(void)
{
   extern char *heap;
   return ramusage_free(heap);
}

DATA_ACCESSOR(name=heapusage)
DATA_OPERATOR(get, name=heapusage dir=stats,
              extern char *heap;
              extern int _end;
              DATA_OUTPUT("%d\n", heap - (char*)&_end);
     )
#endif

#define MAGIC 0x5A

/* calculate free space */
int ramusage_free(void *start)
{
   /* traverse from farthest stack can grow until it is no
      longer set to MAGIC */
   char *p;
   for(p = (char*)start; p <= (char*)&p; p++)
      if(*p != MAGIC)
         break;

   return p - (char*)start;
}

void ramusage_init(void *start)
{
   /* initialize from our current location on stack to the
      farthest the stack can grow */
   char *p;
   for(p = (char*)start; p < (char*)&p; p++)
      *p = MAGIC;
}
