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

#if defined(DATADRIVER_USE_UART0) || defined(DATADRIVER_USE_UART1)
#include "uart.h"
#endif

#if defined(DATADRIVER_USE_USB)
void usb_init(void);
void usb_write(int c);
int usb_read(void);
#endif

/* generic getline */
char *getline_gen(int (*byte)(void), char *buf, volatile int *len,
                  const int size, const char *name)
{
   int c;
   while((c = byte()) != EOF) {
      if(*len == size) {
         DEBUG("buffer overflow on %s\n", name);
         *len = 0;
         return NULL;
      }
      if(c == '\n' || c == '\r') {
         buf[*len] = '\0';
         *len = 0;
         return buf;
      }
      buf[(*len)++] = c;
   }
   return NULL;
}

#define INPUT_BUFFER_LEN 128

#define MAKE_BUFFER(name) \
   char name_buf_in[INPUT_BUFFER_LEN]; \
   volatile int name_buf_in##len; \
   char *name##_getline(void) { getline_gen(name##_read, name_buf_in, \
                                            &name_buf_in##len, \
                                            sizeof name_buf_in, #name); }

#ifdef DATADRIVER_USE_UART0
MAKE_BUFFER(uart0)
#endif
#ifdef DATADRIVER_USE_UART1
MAKE_BUFFER(uart1)
#endif
#ifdef DATADRIVER_USE_USB
MAKE_BUFFER(usb)
#endif

enum DataDevices {DATA_UART0 = 1, DATA_UART1 = 2, DATA_USB = 4, DATA_ALL = 7};
enum DataDevices data_device;

char *get_input_line(void)
{
#ifdef DATADRIVER_USE_UART0
   if(data_device == DATA_UART0)
      return uart0_getline();
#endif
#ifdef DATADRIVER_USE_UART1
   if(data_device == DATA_UART1)
      return uart1_getline();
#endif
#ifdef DATADRIVER_USE_USB
   if(data_device == DATA_USB)
      return usb_getline();
#endif
}

void stdout_write(char c)
{
#ifdef DATADRIVER_USE_UART0
   if(data_device & DATA_UART0)
      uart0_write(c);
#endif
#ifdef DATADRIVER_USE_UART1
   if(data_device & DATA_UART1)
      uart1_write(c);
#endif
#ifdef DATADRIVER_USE_USB
   if(data_device & DATA_USB)
      usb_write(c);
#endif
}

void stderr_write(char c)
{
   if(c == DATA_START_CHAR)
      return;

#ifdef DATADRIVER_USE_UART0
   uart0_write(c);
#elif !defined(NDEBUG)
#error "Debugging output turned on, and no debugging stream set!"
#endif
}

void datadriver_init(void)
{
#ifdef DATADRIVER_USE_UART0
   uart0_init(115200);
#endif
#ifdef DATADRIVER_USE_UART1
   uart1_init(115200);
#endif
#ifdef DATADRIVER_USE_USB
   usb_init();
#endif

   /* turn off buffering, important for correct operation */
   setvbuf(stdout, 0, _IONBF, 0);
   setvbuf(stderr, 0, _IONBF, 0);

   data_device = DATA_ALL;
   DATA_OUTPUT_CHAR(DATA_START_CHAR);
}

void datadriver_processinput(void)
{
   char *data = get_input_line();

   if(!data)
      return;

   data_process_interactive_cmd(data);
}

void datadriver_poll(void)
{
#ifdef DATADRIVER_USE_UART0
   data_device = DATA_UART0;
   datadriver_processinput();
#endif

#ifdef DATADRIVER_USE_UART1
   data_device = DATA_UART1;
   datadriver_processinput();
#endif

#ifdef DATADRIVER_USE_USB
   data_device = DATA_USB;
   datadriver_processinput();
#endif
   data_device = DATA_ALL;
}
