// AT90USB/SUDAP.c
// Simple Usb Data Acquisition Program
// Sampling an analog voltage using the intern ADC of AT90USB chip and
// sending data to host (Linux PC) over USB in real time.
// Sampling interval and number of samples is selected with command line options.
// Additionally you can set output pins of digital port B.
// The main purpose of this tiny program is to demonstrate the functionality of my 
// USB firmware written for Atmels AT90USB controller.
// Licence: GPL-2
// Build: gcc -l usb SUDAP.c
// Usage: see function usage() below.
// S. Salewski, 24-MAR-2007 

#include <stdio.h>
#include <stdint.h>
#include <getopt.h>
#include <errno.h>
#include <usb.h>
#include "com_def.h"
#include "daq_dev.h"

#define TimeOut 2000 // ms

const char trstr[] = "Use 1ms, 500us, 200us, 100us, 50us or 20us for time resolution!";

static struct option long_options[] =
{
  {"timeres",	required_argument,	0, 't'},
  {"samples",	required_argument,	0, 's'},
  {"columns",	required_argument,	0, 'c'},
  {"portb",	required_argument,	0, 'p'},
  {"help",	no_argument,		0, 'h'},
  {0, 0, 0, 0}
};

static void
usage(void)
{
  puts("Usage: -t 100us -s 100 -c 1 -p 10001101");
  printf("-t or --timeres: ");
  puts(trstr);
  puts("-s or --samples: number of samples (1 up to 65535)");
  puts("-c or --columns: number of columns for output, default is 32");
  puts("-p or --portb:   output value for digital port b, use a 8-bit dual number");
}

static struct usb_device
*UsbFindDevice(void)
{
  struct usb_bus *bus;
  struct usb_device *dev;
  usb_init();
  usb_find_busses();
  usb_find_devices();
  for (bus = usb_busses; bus; bus = bus->next)
  {
    for (dev = bus->devices; dev; dev = dev->next)
    {
      if ((dev->descriptor.idVendor == MyUSB_VendorID) && (dev->descriptor.idProduct == MyUSB_ProductID))
        return dev;
    }
  }
  return NULL;
}

// We use EP2 for DAQ
static void
dodaq(struct usb_dev_handle *handle, uint16_t timeres, uint16_t samples, int columns)
{
  unsigned char buf[EP2_FIFO_Size];
  unsigned char *b;
  unsigned int m;
  unsigned int c = 0;
  if (usb_control_msg(handle, USB_VendorRequestCode, UC_ADC_Read, (int) timeres, (int) samples, NULL, 0, TimeOut) < 0)
    puts("USB_control_msg error!"); // should never occur
  while (samples)
  { 
    if (samples > (EP2_FIFO_Size/2))
      m = (EP2_FIFO_Size/2);
    else
      m = samples;
    if (usb_bulk_read(handle, 2, (char*) buf, m * 2, TimeOut) < (m * 2))
      puts("USB_bulk_read error!"); // should never occur
    b = buf;
    samples -= m;
    while (m--)
    {
      unsigned int k;
      k = (unsigned int) *b++;
      k += (unsigned int) *b++ * 256;
      c++;
      if (c == columns)
      {
        c = 0; 
        printf("%u\n", k);
      }
      else
        printf("%u ", k);
    }
  }
  if (c) putchar('\n');
}

int
main(int argc, char **argv)
{
  struct usb_device *dev;
  struct usb_dev_handle *handle;
  int port = -1;
  uint16_t samples = 0;
  uint16_t timeres = 0;
  int columns = 32;
  uint8_t status;
  if (argc == 1)
  {
    usage();
    exit(EXIT_FAILURE);
  }
  while (1)
  {
    char *tail;
    long int l;
    int option_index = 0;
    int c;
    c = getopt_long(argc, argv, "t:s:p:c:h", long_options, &option_index);
    if (c == -1) break;
    switch (c)
    {
      case 'h':
        usage();
        exit(EXIT_FAILURE);
      case 'p':
        errno = 0;
        l = strtol(optarg, &tail, 2);
        if ((errno == 0) && (*tail == '\0') && (l >= 0) && (l < 256))
          port = (int) l;
        else
        {
          puts("Use a 8-bit binary number for setting port pins!");
          exit(EXIT_FAILURE);
        }
        break;
      case 'c':
        errno = 0;
        l = strtol(optarg, &tail, 0);
        if ((errno == 0) && (*tail == '\0') && (l > 0) && (l <= 256))
          columns = (int) l;
        else
        {
          puts("Use a positive integer <= 256 for number of columns!");
          exit(EXIT_FAILURE);
        }
        break;
      case 't':
        if ((optarg[0]=='1') && (optarg[1]=='m') && (optarg[2]=='s') && (optarg[3]==0))
          timeres = ADC1ms;
        else if ((optarg[0]=='5') && (optarg[1]=='0') && (optarg[2]=='0') && (optarg[3]=='u') && (optarg[4]=='s') && (optarg[5]==0))
          timeres = ADC500us;
        else if ((optarg[0]=='2') && (optarg[1]=='0') && (optarg[2]=='0') && (optarg[3]=='u') && (optarg[4]=='s') && (optarg[5]==0))
          timeres = ADC200us;
        else if ((optarg[0]=='1') && (optarg[1]=='0') && (optarg[2]=='0') && (optarg[3]=='u') && (optarg[4]=='s') && (optarg[5]==0))
          timeres = ADC100us;
        else if ((optarg[0]=='5') && (optarg[1]=='0') && (optarg[2]=='u') && (optarg[3]=='s') && (optarg[4]==0))
          timeres = ADC50us;
        else if ((optarg[0]=='2') && (optarg[1]=='0') && (optarg[2]=='u') && (optarg[3]=='s') && (optarg[4]==0))
          timeres = ADC20us;
        else
        {
          puts(trstr);
          exit(EXIT_FAILURE);
        }
        break;
      case 's':
        errno = 0;
        l = strtol(optarg, &tail, 0);
        if ((errno == 0) && (*tail == '\0') && (l > 0) && (l <= MaxSamples))
          samples = (uint16_t) l;
        else
        {
          puts("Use 0 < samples < 65536!");
          exit(EXIT_FAILURE);
        }
        break;
      case '?':
        exit(EXIT_FAILURE);
        break;
      default:
        abort();
    }
  }
  if (optind < argc)
  {
    printf("Unsupported arguments: ");
    while (optind < argc)
      printf("%s ", argv[optind++]);
    putchar('\n');
    exit(EXIT_FAILURE);
  }
  if ((!samples) && (timeres))
  {
    puts("Required option: -s or --samples");
    exit(EXIT_FAILURE);
  }
  if ((samples) && (!timeres))
  {
    puts("Required option: -t or --timeres");
    exit(EXIT_FAILURE);
  }
  dev = UsbFindDevice();
  if (dev == NULL)
  {
    puts("USB device not found!");
    exit(EXIT_FAILURE);
  }
  handle = usb_open(dev);
  if (handle == NULL)
  {
    puts("USB device handle not found!");
    exit(EXIT_FAILURE);
  }
  if (usb_claim_interface(handle, 0) < 0)
  {
    puts("Can not claim interface!");
    usb_close(handle); 
    exit(EXIT_FAILURE);
  }
  if (port != -1)
  {
    status = (uint8_t) port;
    if (usb_bulk_write(handle, 3, (char*) &status, 1, TimeOut) < 1)
      puts("Can not set digital port B!");
  }
  if (samples && timeres)
  {
    dodaq(handle, timeres, samples, columns);
    usb_bulk_read(handle, 1, (char*) &status, 1, TimeOut);
    switch (status)
    {
      case (UsbDevStatusOK):
        break;
      case (UsbDevStatusDAQ_BufferOverflow):
        puts("DAQ Buffer Overflow!");
        break;
      case (UsbDevStatusDAQ_TimerOverflow):
        puts("DAQ Timer Overflow!");
        break;
      case (UsbDevStatusDAQ_ConversionNotFinished):
        puts("DAQ Conversion was not finished when reading result!");
        break;
      default:
        puts("Unknown error has occurred!");
    }
  }
  usb_release_interface(handle, 0);
  usb_close(handle);
  exit(EXIT_SUCCESS);
}
