LIBNAME=MyUSB

VPATH = MyUSB/MyUSB/Drivers/USB/LowLevel MyUSB/MyUSB/Drivers/USB/HighLevel MyUSB/MyUSB/Drivers/USB/Class

SRCS = LowLevel.c Endpoint.c DevChapter9.c USBTask.c USBInterrupt.c Events.c StdDescriptors.c ConfigDescriptor.c HIDParser.c

CFLAGS = -std=gnu99 -DF_CPU=8000000

include ../myusb/MyUSBCFLAGS

MCU ?= at90usb1287

include ../make/lib.mk
include ../make/avr.mk

cleanmyusb:
	make -C MyUSB clean

clean: cleanmyusb
