PROGNAME = dualusb
SRCS = dualusb.c

MCU = atmega644

CFLAGS += -DBOOT_SECTION_START=$(BOOT_SECTION_START)
BOOT_SECTION_START = 0xe000

include ../../make/usb.mk
include ../../make/app.mk
