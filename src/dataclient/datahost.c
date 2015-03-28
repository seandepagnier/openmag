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

#include <errno.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <stdarg.h>

#include <data/datahostopen.h>

#include <setjmp.h>

#include "datahost.h"

int fd;

#ifdef USE_LIBUSB
#include <usb.h>
usb_dev_handle *usb_handle;
int usb_bulkmode;
#endif

static uint8_t opened;

#include <data/data.h>
#include <data/datautil.h>

static const char prompt[] = DATA_PROMPT;
static char curdir[128] = "", lastdir[128] = "";

static char buffer[65536];
static int pos, ppos;
static int inreply, havereply;

void (*datahost_asyncfunc)(char) = 0;
void (*datahost_resultfunc)(const char*) = 0;

/* call this when received a character from the device */
static void gotchar(char c)
{
   /* ignore linefeed, added to streams to deal with hyperterminal and minicom */
   if(c == '\r')
       return;

   if(inreply) {
      if(c == prompt[ppos]) {
         if(++ppos == sizeof prompt - 1) {
            buffer[pos-ppos+1] = '\0';
            pos = 0;
            ppos = 0;
            inreply = 0;
            havereply = 1;
            return;
         }
      } else
         ppos = 0;
      buffer[pos++] = c;
      if(pos == sizeof buffer) {
         printf("out of buffer\n");
         exit(1);
      }
   } else {
      if(c == DATA_START_CHAR) {
          inreply = 1;
          if(havereply)
              printf("warning, unused reply (could be reset)\n");
      } else
         if(datahost_asyncfunc && opened)
            datahost_asyncfunc(c);
   }
}

/* wait until we get a reply from the device */
static char *sync_on_prompt(void)
{
   havereply = 0;
   while(!havereply)
      datahost_poll();

   return buffer;
}

static int write_buf(char *buf, int len)
{      
#ifdef USE_LIBUSB
   if(usb_handle) {
       if(usb_bulkmode) {
           if(usb_bulk_write(usb_handle, 1, buf, len, 5000) < 0)
               fprintf(stderr, "usb_bulk_write failed!\n");
       } else
           usb_control_msg(usb_handle, USB_ENDPOINT_OUT | USB_TYPE_VENDOR
                           | USB_RECIP_DEVICE, 0, 0, 0, buf, len, 5000);
   } else
#endif
   {
       /* make sure we write everything, we are non-blocking after all */
       int off = 0;
       while(len) {
           int ret = write(fd, buf + off, len);
           if(ret == -1)
              if(errno == EAGAIN)
                 continue;
              else
                 return -1;

           len -= ret;
           off += ret;
       }
   }
   return 0;
}

static int simplify_dir(char *namebuf)
{
    /* move through the string killing relative paths  */
    char *s;
    while((s = strstr(namebuf, ".."))) {
        if(s <= namebuf + 1 || s[-1] != '/')
            return -1;
            
        char *t = s - 2;
        while(t > namebuf && *t != '/')
            t--;

        if(*t == '/')
            t++;

        if(s[2] == '/')
            memmove(t, s+3, strlen(s));
        else
            memmove(t, s+2, strlen(s)+1);
    }
    return 0;
}

/* pass a printf style command to the device,
   return the reply */
char *datahost_getcmd(const char *fmt, ...)
{
    static char buf[256];
    static char nil[] = "";

    char buffer[1024];
    va_list ap;
    va_start(ap, fmt);
    int len = vsprintf(buffer, fmt, ap);
    va_end(ap);

    /* apply curdir corrections */
    char *op, *name, *param;
    data_split_cmd(buffer, &op, &name, &param);

    /* convert name, and initial / means start at root,
       otherwise we start at curdir. */
    char namebuf[1024];
    if(!*name)
        strcpy(namebuf, "/");
    if(!strcmp(name, "-"))
        strcpy(namebuf, lastdir);
    else if(*name == '/')
       strcpy(namebuf, name + 1);
    else if(!strcmp(op, "cd") && !*name)
       *namebuf = 0;
    else if(*op) {
        strcpy(namebuf, curdir);
        strcat(namebuf, name);
    } else
        *namebuf = 0;

    if(simplify_dir(namebuf)) {
       snprintf(buf, sizeof buf, "Invalid directory\n");
       return buf;
    }

    /* client builtin operators */
    if(!strcmp(op, "pwd")) {
       if(*curdir) {
          snprintf(buf, sizeof buf, "%s\n", curdir);
          return buf;
       }
       return nil;
    }

    if(!strcmp(op, "cd")) {
        if(*namebuf && namebuf[strlen(namebuf)-1] != '/')
            strcat(namebuf, "/");

        char *vals = datahost_getcmd("ls /%s", namebuf);
        if(strstr(vals, "unknown") || strstr(vals, "unimplemented")) {
           snprintf(buf, sizeof buf, "No such directory: %s\n", namebuf);
           return buf;
        }
        else {
            strcpy(lastdir, curdir);
            strcpy(curdir, namebuf);

            /* update the prompt to include the directory */
            char newprompt[1024];
            if(*curdir)
               snprintf(newprompt, sizeof newprompt,
                        "%s %s", curdir, prompt);
            else
               strcpy(newprompt, prompt);
            rl_set_prompt(newprompt);
        }
        return nil;
    }

    char buffer2[1024];
    len = sprintf(buffer2, "%s %s %s\r\n", op, namebuf, param);

    if(len > 64) {
       snprintf(buf, sizeof buf, "request too long, ignoring\n");
       return buf;
    }

    if(write_buf(buffer2, len) == -1)
       return NULL;

    return sync_on_prompt();
}

/* for server requests */
void data_process_interactive_cmd(char *cmd)
{
   char *buf = datahost_getcmd("%s", cmd);
   datahost_server_printf("%c%s"DATA_PROMPT, DATA_START_CHAR, buf);
}

static char **match_completions(char *vals, char *match)
{
    char **results = malloc(1000 * sizeof *results);
    int numresults = 1;

    char *token = strtok(vals, " ");
    while(token) {
        if(strcmp(token, "\n"))
            if(!strncmp(match, token, strlen(match))) {
                results[numresults] = malloc(strlen(token) + 1);
                strcpy(results[numresults], token);
                numresults++;
            }
        token = strtok(NULL, " ");
    }

    if(numresults == 1)
        return NULL;

    if(numresults == 2) {
        results[0] = results[1];

        /* don't add a space after / */
        if(results[0][strlen(results[0])-1] == '/')
            rl_completion_suppress_append = 1;

        numresults--;
    } else {
        char *x = malloc(1000);
        int p = 0;
        for(;;) {
            int i;
            if(!(x[p] = results[1][p]))
                break;
            for(i = 2; i<numresults; i++) {
                if(x[p] != results[i][p])
                    goto done;
            }
            p++;
        }
    done:
        x[p] = 0;

        results[0] = x;
    }

    results[numresults] = 0;
    return results;
}

static char **datagen_completion_operator(char *op)
{
    char *vals = datahost_getcmd("allops /");
    strcat(vals, " pwd cd");
    return match_completions(vals, op);
}

static char **datagen_completion_accessor(char *name)
{
    char *r = strrchr(name, '/');
    if(r == name) {
       name = "/";
       r++;
    } else
    if(r) {
        *r = '\0';
        r++;
    } else {
        r = name;
        name = "";
    }

    char *vals = datahost_getcmd("ls %s", name);
    return match_completions(vals, r);
}

static char **datagen_completion_parameter(char *name, char *param)
{
    char *vals = datahost_getcmd("values %s", name);
    /* parameter completion is not guarenteed */
    if(strcmp(vals, "<unimplemented>\n"))
        return match_completions(vals, param);
    return NULL;
}

static char **datagen_completion(const char *text, int start, int end)
{
    char *buf = rl_copy_text(0, end);
    char **ret;

    char *name = nextspace(buf);

    if(!*name)     /* if it's an operator */
        ret = datagen_completion_operator(buf);
    else {
        while(*name == ' ')
            name++;
        char *param = nextspace(name);
        if(!*param) /* if it's an accessor */
            ret = datagen_completion_accessor(name);
        else { /* it's a parameter */
            while(*param == ' ')
                param++;

            ret = datagen_completion_parameter(name, param);
        }
    }

    free(buf);
    rl_attempted_completion_over = 1;
    return ret;
}

void readline_done(char *line)
{
    if(!line) {
        puts("");
        exit(0);
    }

    if(strlen(line))
       add_history(line);

    datahost_resultfunc(datahost_getcmd("%s", line));
    free(line);
}

#ifdef USE_LIBUSB
static int open_usb(void)
{
   struct usb_bus *bus;
   struct usb_device *dev;

   usb_init();
   usb_find_busses();
   usb_find_devices();

   for(bus = usb_get_busses(); bus; bus = bus->next)
      for(dev = bus->devices; dev; dev = dev->next) {
         if(dev->descriptor.idVendor == 0x16c0) {
             if(dev->descriptor.idProduct == 0x05dc) {
                 usb_handle = usb_open(dev);
                 return 0;
             }

             if(dev->descriptor.idProduct == 0x06dd) {
                 usb_handle = usb_open(dev);
                 if (usb_claim_interface(usb_handle, 0) < 0)
                 {
                     fprintf(stderr, "Can not claim interface!\n");
                     exit(1);
                 }
                 usb_bulkmode = 1;
                 return 0;
             }
         }
      }

   return -1;
}
#endif

static sigjmp_buf timeout;

void
jump_timeout(int sig)
{
   siglongjmp(timeout, 1);
}

char *
datahost_opendevice(char **device, int verbose)
{
   while(*device) {
#ifdef USE_LIBUSB
      if(!strcmp(*device, "usb")) {
	 if(open_usb()) {
	    fd = 0;
	    break;
	 }
      }
#endif

      if((fd = datahost_open(*device++, verbose)) == -1)
	 continue;

      /* timeout in 1 second */
      signal(SIGALRM, jump_timeout);
      alarm(1);
      if(sigsetjmp(timeout, 1)) {
         if(verbose)
             fprintf(stderr, "timeout\n");
      } else {
	 /* write an empty command clear its buffer */
	 if(datahost_getcmd("")) {
	    signal(SIGALRM, SIG_IGN);
	    opened = 1;
	    break;
	 }

         if(verbose)
            perror("writing to device");
      }

      signal(SIGALRM, SIG_IGN);
      close(fd);
      fd = -1;
   }

   if(fd == -1) {
      fprintf(stderr, "opening device failed\n");
      exit(1);
   }

   return device[-1];
}

void datahost_readline_start(void)
{
    /* tell readline how to do completion,
       and set it up in non-blocking mode */
    rl_attempted_completion_function =
        (rl_completion_func_t*)datagen_completion;
    rl_basic_word_break_characters = " /";
    rl_callback_handler_install(prompt, readline_done);
}

void datahost_poll(void)
{
#ifdef USE_LIBUSB
   if(usb_handle)
   {
      char buf[192];
      int len;
      if(usb_bulkmode) {
          len = usb_bulk_read(usb_handle, 2, buf, sizeof buf, 100);

          if(len < 0) {
              rl_callback_handler_remove();
              fprintf(stderr, "usb_bulk_read failed!\n");
              exit(1);
          }
      } else {
      retry:
          len = usb_control_msg(usb_handle, USB_ENDPOINT_IN
                                | USB_TYPE_VENDOR
                                | USB_RECIP_DEVICE, 0, 0, 0, buf,
                                sizeof buf, 5000);

          if(len < 0) {
              usleep(400000);
              while(open_usb())
                  usleep(100000);
              goto retry;
          }
      }

      int i;
      for(i = 0; i<len; i++)
         gotchar(buf[i]);
   } else
#endif
   {
      char c;
      while(read(fd, &c, 1) == 1)
          gotchar(c);
   }
}
