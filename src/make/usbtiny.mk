# Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

# mk file for programs using usbtiny

USBTINY = $(INCPREFIX)old/usbtiny
USBTINY_CFLAGS = -I. # to include usbtiny.h
CFLAGS += -I$(USBTINY) -gstabs
OBJS += crc.o int.o usb.o
DEPS += crc.d int.d usb.d

TARGET_ARCH=-mmcu=$(MCU)

# redirect to all
default : all

crc.o:		$(USBTINY)/crc.S $(USBTINY)/def.h usbtiny.h
	$(COMPILE.c) $(USBTINY)/crc.S $(USBTINY_CFLAGS)
int.o:		$(USBTINY)/int.S $(USBTINY)/def.h usbtiny.h
	$(COMPILE.c) $(USBTINY)/int.S $(USBTINY_CFLAGS)
usb.o:		$(USBTINY)/usb.c $(USBTINY)/def.h $(USBTINY)/usb.h usbtiny.h
	$(COMPILE.c) $(USBTINY)/usb.c $(USBTINY_CFLAGS)
