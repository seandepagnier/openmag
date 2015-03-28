# Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

# main application makefile
#
# each app should specify PROGNAME, SRCS, and optionally CFLAGS and LDFLAGS 
#

all:

INCPREFIX ?= ../
include $(INCPREFIX)make/common.mk

WARNS ?= -Wno-format -Wno-unused
LDFLAGS := $(LDFLAGS) -L$(INCPREFIX)calibration -L$(INCPREFIX)menu -L$(INCPREFIX)linalg -L$(INCPREFIX)avrdrivers -L$(INCPREFIX)gfx -L$(INCPREFIX)myusb

clean:
	rm -f $(FILES_BUILT) $(OBJS) $(DEPS) $(GEN)
