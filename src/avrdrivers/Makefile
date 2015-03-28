# Copyright (C) 2009 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

LIBNAME=avrdrivers

CFLAGS = -std=gnu99 -fgnu89-inline -g

CFLAGS += -DF_CPU=8000000UL -DNDEBUG

include ../myusb/MyUSBCFLAGS

SRCS = avrdrivers.c magsetup.c keypad.c eeprom.c itertimer.c watchdog.c power.c battery.c buzzer.c hotwire.c

GEN += version.h

MULTILIBDATAGEN=1
HAVE_HOST=0

all install: version.h

version.h: version.h.in
	sed s,%%DATE%%,`date +%D`,g < version.h.in > version.h

include ../make/multilib.mk
