#include <stdio.h>
#include <stdlib.h>

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "uartfs-internal.h"

#define DEFAULT_DEVICE "/dev/ttyS0"
#define DEFAULT_BAUD_RATE "300"

static int fd;
static FILE *port;
static struct termios oldtio, newtio;

static void CloseComm(void)
{
  tcsetattr(fd,TCSANOW,&oldtio);
  fclose(port);
}

static void OpenComm(const char *device, speed_t baudrate) {
  if((fd = open(device, O_RDWR | O_NOCTTY)) < 0) {
      fprintf(stderr, "Failure to open %s\n", device);
      exit(0);
  }

  tcgetattr(fd,&oldtio); // save current port settings
  // set new port settings for canonical input processing
  // I'm sorry, this level of hacking is pretty bad
  newtio.c_iflag = 0x1;
  newtio.c_oflag = 0x0;
  newtio.c_cflag = 0x1cb1;
  newtio.c_lflag = 0x0;
  newtio.c_line = 0x0;
  newtio.c_cc[0] = 0x0;
  newtio.c_cc[1] = 0x0;
  newtio.c_cc[2] = 0x0;
  newtio.c_cc[3] = 0x0;
  newtio.c_cc[4] = 0x0;
  newtio.c_cc[5] = 0x5;
  newtio.c_cc[6] = 0x1;     
  newtio.c_ispeed = 0x1001;
  newtio.c_ospeed = 0x1001;

  cfsetispeed(&newtio, baudrate);
  cfsetospeed(&newtio, baudrate);

  tcflush(fd, TCIFLUSH);
  tcsetattr(fd, TCSANOW, &newtio);

  atexit(CloseComm);

  port = fdopen(fd, "w+");
}

speed_t ConvertBaud(const char *baudratestr)
{
   switch(atoi(baudratestr)) {
   case 300: return B300;
   case 600: return B600;
   case 1200: return B1200;
   case 1800: return B1800;
   case 2400: return B2400;
   case 4800: return B4800;
   case 9600: return B9600;
   case 19200: return B19200;
   case 38400: return B38400;
   case 57600: return B57600;
   default:
      printf("Invalid baud rate: %s\n", baudratestr);
      exit(0);
   }
}

static int readstr(char *str, int len)
{
   int pos = 0;
   do {
      str[pos] = getc(port);
      pos++;
      if(pos == len)
         return -1;
   } while(str[pos - 1]);
   str[pos] = 0;
   return 0;
}

static FILE *filetable[255];
static const int filetablesize = (sizeof filetable) / (sizeof *filetable);
static int curfile;

static void doreset(void)
{
   int i;
   for(i = 3; i<filetablesize; i++)
      if(filetable[i]) {
         fclose(filetable[i]);
         filetable[i] = NULL;
      }
}

static void doselect(void)
{
   curfile = getc(port);
}

static void doopen(void)
{
   /* get the filename */
   char filename[1024];
   if(readstr(filename, sizeof filename))
      goto fail;

   /* get the mode */
   char mode[10];
   if(readstr(mode, sizeof mode))
      goto fail;

   /* find a position in the table */
   unsigned char index;
   for(index = 0; index < filetablesize; index++) {
      if(!filetable[index]) {
         filetable[index] = fopen(filename, mode);
         if(!filetable[index])
            goto fail;
         putc(index, port);
         return;
      }
   }

 fail:
   index = 255;
   putc(index, port);
}

static void doclose(void)
{
   if(filetable[curfile])
      fclose(filetable[curfile]);
   else
      fprintf(stderr, "[host] close of invalid file\n");
}

static void doread(void)
{
   if(filetable[curfile]) {
      char data;
      if(fread(&data, 1, 1, filetable[curfile])) {
         if(data <= UARTFS_ESCAPE)
            putc(UARTFS_ESCAPE, port);
         putc(data, port);
      } else
         putc(UARTFS_EOF, port);
   } else
      fprintf(stderr, "[host] read from invalid file\n");
}

static void dowrite(void)
{
   char data = getc(port);

   if(filetable[curfile])
      putc(data, filetable[curfile]);
   else
      fprintf(stderr, "[host] write to invalid file\n");
}

static void doseek(void)
{
   int whence;
   switch(getc(port)) {
   case UARTFS_SEEK_SET: whence = SEEK_SET; break;
   case UARTFS_SEEK_CUR: whence = SEEK_CUR; break;
   case UARTFS_SEEK_END: whence = SEEK_END; break;
   default: return;
   }

   union offset x;
   int i;
   for(i = 0; i<4; i++)
      x.data[i] = getc(port);

   if(filetable[curfile])
      fseek(filetable[curfile], x.off, whence);
   else
      fprintf(stderr, "[host] seek in invalid file\n");
}

static void dotell(void)
{
   union offset x;
   x.off = -1;

   if(filetable[curfile])
      x.off = ftell(filetable[curfile]);
   else
      fprintf(stderr, "[host] tell of invalid file\n");

   int i;
   for(i = 0; i<4; i++)
      putc(x.data[i], port);
}

int main(int argc, char **argv) {
   const char *device = DEFAULT_DEVICE;
   const char *baudratestr = DEFAULT_BAUD_RATE;

   int i;
   for(i = 1; i<argc; i++) {
      if(!strcmp(argv[i], "-d"))
         if(++i < argc)
            device = argv[i];
         else
            printf("-d requires an argument\n");
      else
      if(!strcmp(argv[i], "-b"))
         if(++i < argc)
            baudratestr = argv[i];
         else
            printf("-b requires an argument\n");
      else {
         printf("invalid argument, valid arguments:\n"
                "-d [DEVICE]    (default /dev/ttyS0)\n"
                "-b [BAUD_RATE] (default 300)\n");
         exit(0);
      }
   }

   speed_t baudrate = ConvertBaud(baudratestr);

   OpenComm(device, baudrate);
   
   filetable[0] = stdin;
   filetable[1] = stdout;
   filetable[2] = stderr;

   int lastread = 0;
   for(;;) {
      char c = getc(port);

      switch(c) {
      case UARTFS_RESET: doreset(); break;
      case UARTFS_SELECT: doselect(); break;
      case UARTFS_OPEN: doopen();  break;
      case UARTFS_CLOSE: doclose(); break;
      case UARTFS_READ: doread(); break;
      default: ungetc(c, port); /* assume write */
      case UARTFS_WRITE: dowrite(); break;
      case UARTFS_SEEK: doseek(); break;
      case UARTFS_TELL: dotell(); break;
      }
   }
}
