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
 * For more information on the GPL, please go to:
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>
#include <fcntl.h>
#include <poll.h>
#include <getopt.h>

#include <data/data.h>
#include <data/datahostserver.h>

#include "datahost.h"

uint8_t raw;

static struct cmd_t
{
   char *cmd;
   struct cmd_t *next;
} *cmds, *cmdstail;

static void asyncfunc(char c)
{
   datahost_server_printf("%c", c);
}

static void resultfunc(const char *result)
{
   if(!result)
      exit(1);

   fputs(result, stdout);
}

static void showprompt(void)
{
    const char prompt[] = DATA_PROMPT;
    fputs(prompt, stdout);
    fflush(stdout);
}

int main(int argc, char *argv[])
{
    const char usage[] =
        "Usage: dataclient [OPTIONS]... [DEVICE]\n"
        "Options may include: \n"
        "-h, --help  print this message\n"
        "-c, --cmd [CMD] execute CMD display any result and exit\n"
        "           this option may be specified multiple times\n"
        "-p, --port [PORT] run a server on this port listening for\n"
        "           remote connections from dataclients\n"
        "-q, --quiet  do not output streaming data\n"
        "-r, --raw do not use readline\n"
        "-v, --verbose verbose output\n";

    int verbose = 0;

    /* handle command line arguments */
    const struct option long_options[] = {
        {"help", 0, 0, 'h'},
        {"device", 1, 0, 'd'},
        {"cmd", 1, 0, 'c'},
        {"port", 1, 0, 'p'},
        {"quiet", 1, 0, 'q'},
        {"raw", 0, 0, 'r'},
        {"verbose", 0, 0, 'v'},
        {0, 0, 0, 0}};

    char *cmd = NULL;
    int port = 0;
    for(;;)
        switch(getopt_long(argc, argv, "hc:p:qrv", long_options, NULL)) {
        case -1:
            goto nomoreargs;
        case 'h':
            fputs(usage, stdout);
            exit(0);
        case 'c':
           {
              struct cmd_t *c = malloc(sizeof c);
              c->cmd = malloc(strlen(optarg) + 1);
              strcpy(c->cmd, optarg);
              if(cmds)
                 cmdstail->next = c;
              else
                 cmds = c;
              cmdstail = c;
           } break;
        case 'p':
           port = strtol(optarg, NULL, 10);
           break;
        case 'q':
           datahostserverquiet = 1;
           break;
        case 'r':
            raw = 1;
            break;
        case 'v':
            verbose = 1;
            break;
        }

 nomoreargs:
    ;

    struct {
       const char *fmt;
       int count;
    } defaultdevices[] = {
#ifdef __linux
       "/dev/ttyACM%d", 4,
       "/dev/ttyUSB%d", 4,
#endif
#ifdef __FreeBSD__
       "/dev/ttyU%d", 4,
#endif
#ifdef __CYGWIN__
       "/dev/com%d", 17,
#else
       "/dev/ttyS%d", 4,
#endif
       "localhost:7029", 1,
};

    int defaultdevicecount = (sizeof defaultdevices) / (sizeof *defaultdevices);
    char *devices[256];
    int devicecount = 0, defaultdevicepos = 0;

    int autodetect = 0;
    if(optind < argc) {
       while(optind < argc)
	  asprintf(&devices[devicecount++], "%s", argv[optind++]);
    } else {
       fprintf(stderr, "No device specified, performing autodetection\n");
       autodetect = 1;
       while(defaultdevicepos < defaultdevicecount) {
	  int i;
	  if(strstr(defaultdevices[defaultdevicepos].fmt, "%d"))
	     for(i = 0; i<defaultdevices[defaultdevicepos].count; i++) {
		asprintf(&devices[devicecount],
			 defaultdevices[defaultdevicepos].fmt, i);
		devicecount++;
	     }
	  else {
	     asprintf(&devices[devicecount], defaultdevices[defaultdevicepos].fmt);
	     devicecount++;
	  }
	  defaultdevicepos++;
       }
    }
    devices[devicecount] = 0;

    if(!cmds) {
       /* setup callbacks */
       datahost_asyncfunc = asyncfunc;
       datahost_resultfunc = resultfunc;
    }

    char *dev = datahost_opendevice(devices, verbose);
    if(autodetect)
       fprintf(stderr, "Found device '%s'\n", dev);

    /* batch mode, execute command and exit */
    struct cmd_t *c;
    for(c = cmds; c; c = c->next) {
       char *reply = datahost_getcmd("%s", c->cmd);
       fputs(reply, stdout);
    }

    if(cmds)
       exit(0);

    if(port)
       datahost_server_start(port);

    if(raw)
        showprompt();
    else
        datahost_readline_start();

    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | ~O_NONBLOCK);

    struct pollfd fd = {0, POLLIN};

    for(;;) {
        if(raw) {
            static char buf[256];
            static int pos;

            while(read(0, buf+pos, 1) == 1) {
                if(buf[pos]=='\n') {
                    buf[pos] = '\0';
                    pos = 0;
                    fputs(datahost_getcmd("%s", buf), stdout);
                    showprompt();
                } else
                    pos++;
            }
        }
        else
           if(poll(&fd, 1, 0) == 1)
              rl_callback_read_char();

       datahost_poll();
       datahost_server_poll();
       usleep(1000);
    }
}
