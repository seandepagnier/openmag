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
#include <sys/stat.h>

/* overload these for writing */
void stdout_write(int c);
void stderr_write(int c);

/* should overload this if reading is desired */
int stdin_read()
{
   return EOF;
}

/* simple implementation for stdout and stderr writing */
_ssize_t _read(int file, void *ptr, size_t len)
{
   if(file == 0) {
      char *p = ptr;
      int l;
      for(l = 0; l<len; l++) {
         int c = stdin_read();
         if(c == EOF)
            break;
         p[l] = c;
      }
      return l;
   }
   return 0;
}

static void writechar(int file, char c)
{
   switch(file) {
   case 1:
      stdout_write(c);
      break;
   case 2:
      stderr_write(c);
      break;
   }
}

_ssize_t _write (int file, const void *ptr, size_t len)
{
   int i;
   char *p;
   
   p = (char*) ptr;
   
   for (i = 0; i < len; i++) {
      if (*p == '\n' )
         writechar(file, '\r');
      writechar(file, *p++);
   }
   
   return len;
}

int _close(int file)
{
   return 0;
}

_off_t _lseek(int file, _off_t ptr, int dir)
{
   return (_off_t)0;	/*  Always indicate we are at file beginning.	*/
}

int _fstat(int file, struct stat *st)
{
	/*  Always set as character device.				*/
	st->st_mode = S_IFCHR;	
        /* assigned to strong type with implicit 	*/
        /* signed/unsigned conversion.  Required by 	*/
        /* newlib.					*/
	return 0;
}

int isatty(int file)
{
   return 1;
}

extern char  _end[];
char *heap =  _end;
void * _sbrk(ptrdiff_t nbytes)
{
   char  *base = heap;
   heap += nbytes;
   return base;
}
