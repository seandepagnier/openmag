# Copyright (C) 2009 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

PROGNAME = dusi
CFLAGS += -std=gnu99 -fgnu89-inline -g
LDFLAGS += -Wl,-u,vfscanf -lscanf_flt -Wl,-u,vfprintf -lprintf_flt -lcalibration.$(MCU) -llinalg.$(MCU) -lmenu.$(MCU) -lgfx.$(MCU) -lavrdrivers.$(MCU) -lm

VPATH = ..
SRCS = dusi.c

DATALIBS = $(INCPREFIX)avrdrivers/avrdrivers.$(MCU).da $(INCPREFIX)calibration/calibration.$(MCU).da $(INCPREFIX)menu/menu.$(MCU).da

INCPREFIX = ../../
