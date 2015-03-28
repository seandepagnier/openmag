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

#ifdef NOSERVER

#include <stdarg.h>

void calibration_server_lock(void) {}
void calibration_server_unlock(void) {}

int datahost_server_printf(const char *__format, ...)
{
  va_list ap;
  va_start(ap, __format);
  int ret = vprintf(__format, ap);
  va_end(ap);
  return ret;
}

#else

#include <sys/time.h>

#include <data/data.h>
#include <data/databasictypes.h>
#include <data/databools.h>
#include <data/datadirectories.h>

#include <data/datahostserver.h>
#include <data/datahostopen.h>

#include <avrdata.h>

#include <calibration/cal.h>
#include <calibration/calibrationhostreadsensor.h>

#include <getopt.h>

static int calibrationserverfd = -1;

uint8_t usemenu;
uint8_t sca3000_detected=1, ad7799_init_status=1;

static void alarmsig(int sig)
{
   calibrationhost_read_sensors(calibrationserverfd);

   extern void plots_poll(void) __attribute__ ((weak));
   if(plots_poll) {
      static float t;
      if(gettime() - t > .05) {
         plots_poll();
	 t += .05;
      }
   }
}

static void calibrationserver_helpdevice(void)
{
   printf(
  "If the device is unspecified it is autodetected the same way dataclient\n"
  "detects the device.  This device should be configured to output raw\n"
  "sensor data.\n"
  "The device may be a file or socket which when read from contains data\n"
  "formatted:\n"
  "accel: X Y Z\n"
  "mag: X Y Z\n"
  "temp: X\n"
  "Where X Y Z are raw sensor counts\n"
  "For example, you may run the dataclient on the device with -p [PORT].\n"
  "Set it up to output raw data for accel and mag, then run this application\n"
  "with localhost:PORT to read the data.  You can then connect to this\n"
  "application with another dataclient.\n"
  );
   exit(0);
}

static void calibrationserver_help(void)
{
   printf(
  "Usage: calibrationhost [OPTION]... [DEVICE]\n"
  "calibrationhost runs autocalibration algorithms on a host \n"
  "computer making it easy to debug the algorithms as well as\n"
  "make it easier to use the algorithms with any hardware.\n\n"
  "Data for raw accelerometer and magnetometer must be specified via\n"
  "the device argument.  This program will compute calibration coefficients for\n"
  "both sensors.  The end result is accelerometer and magnetometer data\n"
  "aligned to the same coordinate system, normalized and orthogonalized.\n\n"
  "Options:\n"
  "-c, --cmd  command to pass to datainterface for this application\n"
  "  eg: to get accel output, pass: -c \"set sensors/accel/outputrate 10\"\n"
  "-h, --help  this message\n"
  "-p, --server-port [PORT] specify the port to listen for connections, set\n"
  "   to 0 to disable, default 7029\n"
  "-q, --quiet  suppress output data\n"
  "--help-device  Print information explaining the device.\n"
  "[DEVICE]  the device or file to connect to for reading data.\n"
  );
   exit(0);
}

static void calibration_server_setup(int argc, char *argv[])
{
   /* parse arguments */
   const struct option long_options[] = {
      {"cmd", 1, 0, 'c'},
      {"help", 0, 0, 'h'},
      {"server-port", 1, 0, 'p'},
      {"help-device", 0, 0, 1},
      {0, 0, 0, 0}};

   int port=7029;

   int c;
   for(;;) {
      switch(getopt_long(argc, argv, "c:hp:q", long_options, NULL)) {
      case 'c':
         data_process_cmd(optarg);
         break;
      case 'h':
         calibrationserver_help();
      case 'p':
         port = strtol(optarg, NULL, 10);
         break;
      case 1:
         calibrationserver_helpdevice();
      case -1:
         goto nomoreargs;
      }
   }

 nomoreargs:

   /* run the sever */
   if(port)
      datahost_server_start(port);

   /* initialize the sensors */
   calibrationhost_sensors_init();

   if(!argv[optind])
      fprintf(stderr, "No device specified, No sensor data\n");
   else {

      char *device = malloc(strlen(argv[optind]) + 1);
      strcpy(device, argv[optind]);

      /* open the device */
      if((calibrationserverfd = datahost_open(device, 0)) == -1) {
	 fprintf(stderr, "failed to open: %s\n", device);
	 exit(1);
      }
   }

   /* setup an alarm at 500hz to emulate the timer interrupt */
   struct itimerval value = {{0, 2000}, {0, 1}};
   signal(SIGALRM, alarmsig);
   setitimer(ITIMER_REAL, &value, 0);
}

void calibration_server_lock(void)
{
#if 0
   sigset_t sig;
   sigemptyset(&sig);
   sigaddset(&sig, SIGALRM);
   sigprocmask(SIG_BLOCK, &sig, 0);
#endif
}

void calibration_server_unlock(void)
{
#if 0
   sigset_t sig;
   sigemptyset(&sig);
   sigaddset(&sig, SIGALRM);
   sigprocmask(SIG_UNBLOCK, &sig, 0);
#endif
}

static void calibration_server_poll(void)
{
   datahost_server_poll();

   sensor_poll();

   extern void calibration_calc_queued_location(void) __attribute__ ((weak));
   if(calibration_calc_queued_location)
      calibration_calc_queued_location();
}

#endif
