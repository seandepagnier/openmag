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

/* very simple cooperative threads with setjmp/longjmp */

#include <setjmp.h>

typedef void (*thread)(void);

thread threads[NUM_THREADS];
jmp_buf thread_table[NUM_THREADS];
int thread_current;

#define thread_relinquish() \
do { \
    if(!setjmp(thread_table[thread_current])) { \
        if(++thread_current == NUM_THREADS) \
            thread_current = 0; \
        longjmp(thread_table[thread_current], 1); \
    } \
} while(0)

void threads_start(void)
{
    for(thread_current = 0; thread_current<NUM_THREADS; thread_current++)
        if(setjmp(thread_table[thread_current])) {
            threads[thread_current]();
            printf("thread must not exit!\n");
            for(;;);
        }

    thread_current = 0;
    longjmp(thread_table[0], 1);
}
