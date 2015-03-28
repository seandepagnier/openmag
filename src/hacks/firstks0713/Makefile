PROGNAME = firstks0713
SRCS = firstks0713.c

CFLAGS = -Os
LDFLAGS = -lm

MCU = at90usb1287
CFLAGS += -DF_CPU=8000000UL -DBOOTLOADER_OFFSET=0x1e000
LDFLAGS += -Wl,-u,vfscanf -lscanf_flt -Wl,-u,vfprintf -lprintf_flt -lgfx.avr -lm

CFLAGS += -DUSE_DATA_MEM -DUSE_DATA_TYPE

BAUDRATE ?= 38400

BOOTLOADER = avr109 # foodloader
INITIATE_BOOTLOADER = echo -e "\033" > /dev/ttyS0

INCPREFIX = ../../

include ../../make/datagen.mk
include ../../make/app.mk
include ../../make/avr.mk
