#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "uartfs-internal.h"

static FILE filetable[10]; /* change this to suit your needs range (3 - 255) */
static const int filetablesize = (sizeof filetable) / (sizeof *filetable);
static int curfile;

static unsigned char uart_read(void)
{
   /* Wait for incomming data */
   while (!(UCSR0A & (1 << RXC0)));
   /* Return the data */
   return  UDR0;
}

static void uart_write(unsigned char data)
{
   /* Wait for empty transmit buffer */
   while (!(UCSR0A & (1 << UDRE0)));
   /* Start transmittion */
   UDR0 = data;
}

static void uartfs_selectfile(FILE *stream)
{
   int index = stream - filetable;
   if(curfile == index)
      return;
   
   uart_write(UARTFS_SELECT);
   uart_write(index);
   curfile = index;
}

/* Read and write functions */
static int uartfs_read(FILE *stream)
{
   uartfs_selectfile(stream);
   uart_write(UARTFS_READ);
   char c = uart_read();
   switch(c) {
   case UARTFS_EOF:
      return EOF;
   case UARTFS_ESCAPE:
      return uart_read();
   }
   return c;
}

static int uartfs_write(char data, FILE *stream)
{
   uartfs_selectfile(stream);
   if(data <= UARTFS_TELL)
      uart_write(UARTFS_TELL);
   uart_write(data);
   return 0;
}

static void uart_writestr(const char *str)
{
   while(*str) {
      uart_write(*str);
      str++;
   }
   uart_write(*str);
}

FILE *fopen(const char *filename, const char *mode)
{
   uart_write(UARTFS_OPEN);
   uart_writestr(filename);
   uart_writestr(mode);
   unsigned char ret = uart_read();
   if(ret == 255)
      return NULL;
   return filetable + ret;
}

int fclose(FILE *fp)
{
   uartfs_selectfile(fp);
   uart_write(UARTFS_CLOSE);
   return 0;
}

int fseek(FILE *stream, long offset, int whence)
{
   uartfs_selectfile(stream);
   uart_write(UARTFS_SEEK);

   switch(whence) {
   case SEEK_SET: uart_write(UARTFS_SEEK_SET); break;
   case SEEK_CUR: uart_write(UARTFS_SEEK_CUR); break;
   case SEEK_END: uart_write(UARTFS_SEEK_END); break;
   default: return -1;
   }

   union offset x;
   x.off = offset;
   int i;
   for(i = 0; i<4; i++)
      uart_write(x.data[i]);

   return 0;
}

long ftell(FILE *stream)
{
   uartfs_selectfile(stream);
   uart_write(UARTFS_TELL);

   union offset x;
   int i;
   for(i = 0; i<4; i++)
      x.data[i] = uart_read();

   return x.off;
}

void rewind(FILE *stream)
{
   fseek(stream, 0, SEEK_SET);
}

void uartfs_init(unsigned char baudrate)
{
   int i;
   for(i = 0; i<filetablesize; i++) {
      FILE x = FDEV_SETUP_STREAM(uartfs_write, uartfs_read, _FDEV_SETUP_RW);
      filetable[i] = x;
   }

   stdin = filetable + 0;
   stdout = filetable + 1;
   stderr = filetable + 2;

   /* Set the baud rate */
   UBRR0H = 0x00;
   UBRR0L = baudrate;

   /* Enable UART receiver and transmitter */
   UCSR0B = (1 << RXEN0) | (1 << TXEN0);
   /* set to 8 data bits, 1 stop bit */
   UCSR0C = (3 << UCSZ00);

   /* send reset, 6 times is the maximum count needed to get out of a state */
   for(i = 0; i<6; i++)
      uart_write(UARTFS_RESET);
}
