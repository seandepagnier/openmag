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

/* This file contains routines for starting a server with sockets, and
   handling data requests using it.  Use datahost_server_start to start
   the server, and datahost_server_poll periodically to handle requests */

#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdarg.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#define DATAHOSTSERVERMAXFDS 64

static struct pollfd datahostserverfds[DATAHOSTSERVERMAXFDS];
static FILE *datahostserverfiles[DATAHOSTSERVERMAXFDS];
static int datahostservernfds;

static int datahostservercurfd;

static int datahostserverquiet;

int datahost_server_printf(const char *__format, ...)
{
  va_list ap;

  int ret;
  if(datahostservercurfd) {
     va_start(ap, __format);
     ret = vfprintf(datahostserverfiles[datahostservercurfd], __format, ap);
     va_end(ap);
  } else {
     int i;
     for(i = 1; i<datahostservernfds; i++) {
        va_start(ap, __format);
        /* disable SIGPIPE while printing since the client could have died */
        struct sigaction act,  oldact;
        act.sa_handler = SIG_IGN;

        sigaction(SIGPIPE, &act, &oldact);
        ret = vfprintf(datahostserverfiles[i], __format, ap);
        sigaction(SIGPIPE, &oldact, NULL);
        va_end(ap);
     }

     if(!datahostserverquiet) {
        va_start(ap, __format);
        ret = vfprintf(stderr, __format, ap);
        va_end(ap);
     }
  }
  return ret;
}

/* bind and listen for connections on the specified port */
void datahost_server_start(int port)
{
   int fd=socket(AF_INET, SOCK_STREAM, 0);

   /* SO_REUSEADDR lets us bind reuse the port if it is in TIME_WAIT */
   int i=1;
   if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i)) < 0)
      perror("SO_REUSEADDR failed");

   struct sockaddr_in datahostserverAddress;
   memset((char*) &datahostserverAddress, 0,sizeof(datahostserverAddress));
   datahostserverAddress.sin_family = AF_INET;
   datahostserverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
   datahostserverAddress.sin_port = htons(port);

   if (bind(fd, (struct sockaddr*)&datahostserverAddress,
            sizeof(datahostserverAddress)) != 0){
      fprintf(stderr, "bind failed for port: %d reason: ", port);
      perror("");
      return;
   }

   if (listen(fd, DATAHOSTSERVERMAXFDS-1) == -1) {
      perror("listen failed");
      return;
   }

   printf("Listening on port %d for connections from telnet or dataclient\n",
          port);

   datahostservernfds++;
   datahostserverfds[0].fd = fd;

   for(i = 0; i < DATAHOSTSERVERMAXFDS; i++)
      datahostserverfds[i].events = POLLIN;
}

/* open new connections connecting to our listening socket,
   and read data from any existing connections processing commands */
void datahost_server_poll(void)
{
   if(!poll(datahostserverfds, datahostservernfds, 0))
      return;

   /* new connection? */
   if(datahostserverfds[0].revents) {
      int fd;
      if ((fd=accept(datahostserverfds[0].fd, NULL, NULL)) != -1) {
         if(datahostservernfds == DATAHOSTSERVERMAXFDS)
            fprintf(stderr, "Data Server too many connections\n");
         else {
            datahostserverfds[datahostservernfds].fd = fd;
            fcntl(fd, F_SETFL, O_NONBLOCK);
            
            if(!(datahostserverfiles[datahostservernfds] = fdopen(fd, "r+")))
               perror("fdopen failed: ");
            else {
               setvbuf(datahostserverfiles[datahostservernfds],
                       NULL, _IONBF, 0);
#ifdef VERBOSE
               printf("datahostserver: new connection\n");
#endif
               datahostservernfds++;
            }
         }
      } else
         perror("accept error");
   }

   /* handle existing connections */
   int i;
   for(i = 1; i<datahostservernfds; i++)
      if(datahostserverfds[i].revents) {
         char buf[1024];
         while(fgets(buf, sizeof buf, datahostserverfiles[i])) {
            /* close connection for this type of input */
            if(buf[0] == -1) {
               fclose(datahostserverfiles[i]);
               goto close;
            }
            
            char *c = strchr(buf, '\n');
            if(c) {
               *c = '\0';
               c = strchr(buf, '\r');
               *c = '\0';
               /* handle request */
               datahostservercurfd = i;
               data_process_interactive_cmd(buf);
               datahostservercurfd = 0;
            }
         }
         
         if(feof(datahostserverfiles[i])) {
         close:
#ifdef VERBOSE
            printf("datahostserver: disconnect\n");
#endif
            /* adjust arrays to remove this connection */
            memmove(datahostserverfds + i, datahostserverfds + i + 1,
                    (datahostservernfds - i - 1)
                    * (sizeof *datahostserverfds));
            memmove(datahostserverfiles + i, datahostserverfiles + i + 1,
                    (datahostservernfds - i - 1)
                    * (sizeof *datahostserverfiles));
            datahostservernfds--;
            i--;
         }
      }
}
