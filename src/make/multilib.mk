# Copyright (C) 2008, 2009 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

# include this to build a library for multiple platforms

export LIBNAME
export SRCS
export CFLAGS
export WARNS

export MULTILIBDATAGEN
export MULTILIBSWITCHTABLEGOTO

INCPREFIX ?= ../
include $(INCPREFIX)make/config.mk

export INCPREFIX

all clean install:
ifeq ($(HAVE_HOST), 1)
	$(MAKE) -f $(INCPREFIX)make/sharedlib.mk $@
endif
#	$(MAKE) TARGET=arm -f $(INCPREFIX)make/lib.mk -f $(INCPREFIX)make/arm.mk $@
ifdef HAVE_AVR
	$(MAKE) TARGET=at90usb1287 -f $(INCPREFIX)make/lib.mk -f $(INCPREFIX)make/avr.mk $@
	$(MAKE) TARGET=atmega2561 -f $(INCPREFIX)make/lib.mk -f $(INCPREFIX)make/avr.mk $@
endif
