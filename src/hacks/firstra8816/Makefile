PROGNAME = firstra8816
SRCS = firstra8816.c

CFLAGS = -Os
LDFLAGS = -lm

MCU = at90usb1287
CFLAGS += -DF_CPU=8000000UL -DBOOTLOADER_OFFSET=0x1e000
LDFLAGS += -Wl,-u,vfscanf -lscanf_flt -Wl,-u,vfprintf -lprintf_flt -lgfx.avr -lm

BAUDRATE ?= 38400

BOOTLOADER = avr109 # foodloader
DEVICE = /dev/ttyS0
INITIATE_BOOTLOADER = echo -e "\033" > /dev/ttyS0

INCPREFIX = ../../

include ../../make/app.mk
include ../../make/avr.mk
