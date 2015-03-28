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
#include <stdint.h>
#include <string.h>

#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int datahost_open(const char *device, int verbose)
{
   int fd;
   if(!strcmp(device, "-")) {
      fd = 0;
      goto done;
   }

   if(verbose)
       fprintf(stderr, "trying %s as a file: ", device);
   fflush(stdout);
   if((fd = open(device, O_RDWR | O_NONBLOCK)) != -1) {
      struct termios tio;
      /* as a device, set up correctly */
      if(tcgetattr(fd, &tio) != -1) {
	 /* terminos settings for straight-through mode */  
	 tio.c_lflag &= ~(ICANON  | ISIG | IEXTEN | ECHO | ECHOE | ECHOK | ECHOCTL | ECHOKE);
	 tio.c_oflag &= ~(OPOST | ONLCR);
	 tio.c_iflag &= ~(ISTRIP | IGNCR | ICRNL | INLCR | IXOFF | IXON);
	 tio.c_iflag |= IGNBRK;

#ifdef __FreeBSD__ /* freebsd */
         tio.c_iflag &= ~(IXANY | IMAXBEL | BRKINT);
         tio.c_cflag |= CLOCAL | CRTSCTS;
#else
         tio.c_cflag &= ~(CLOCAL | CRTSCTS);
#endif

	 tio.c_cc[VMIN]  = 1;
	 tio.c_cc[VTIME] = 5;

	 if(tcsetattr(fd, TCSANOW, &tio) < 0) {
	    fprintf(stderr, "tcsetattr failed\n");
	    exit(1);
	 }
      }
      goto done;
   }

   if(verbose) {
       fprintf(stderr, "failed, trying as a host: ");
       fflush(stdout);
   }
   
   /* try to open as a socket */
   char buf[128];
   strncpy(buf, device, sizeof buf);
   char *c = strchr(buf, ':');
   if(!c)
      goto socketfailed;

   *c = '\0';

   int port = strtol(c+1, NULL, 10);
   
   struct hostent *h = gethostbyname(buf);
   if(h==NULL) {
      fprintf(stderr, "unknown host '%s'\n", buf);
      goto socketfailed;
   }
  
  struct sockaddr_in localAddr, servAddr;
  servAddr.sin_family = h->h_addrtype;
  memcpy((char *) &servAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
  servAddr.sin_port = htons(port);
  
  fd = socket(AF_INET, SOCK_STREAM, 0);
  if(fd < 0)
     goto socketfailed;
  
  localAddr.sin_family = AF_INET;
  localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  localAddr.sin_port = htons(0);
  
  if(bind(fd, (struct sockaddr *) &localAddr, sizeof(localAddr)) == -1)
     goto socketfailed;
  
  /* connect to server */
  if(connect(fd, (struct sockaddr *) &servAddr, sizeof(servAddr)) == -1)
     goto socketfailed;

 done:
  if(verbose)
      fprintf(stderr, "success\n");
  fcntl(fd, F_SETFL, O_NONBLOCK);

  return fd;

 socketfailed:
  if(verbose)
      fprintf(stderr, "failed\n");
  return -1;
}
