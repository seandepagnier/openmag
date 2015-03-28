# Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

MCU = at90usb1287

PROGNAME = pointer
CFLAGS = -std=gnu99 -g
LDFLAGS = -Wl,-u,vfscanf -lscanf_flt -Wl,-u,vfprintf -lprintf_flt -lMyUSB -lavrdrivers.$(MCU) -lcalibration.$(MCU) -llinalg.$(MCU) -lm

CFLAGS += -DF_CPU=8000000UL
CFLAGS += -DUSE_CDC -DUSE_MOUSE -DUSE_JOYSTICK -DUSE_UART

SRCS = pointer.c mouse.c joystick.c

include ../myusb/MyUSBCFLAGS

CFLAGS += -DBOOTLOADER_OFFSET=0x1e000
CFLAGS += -DUSE_DATA_MEM -DUSE_DATA_TYPE

DATALIBS = ../avrdrivers/avrdrivers.$(MCU).da ../calibration/calibration.$(MCU).da

include ../make/datagen.mk
include ../make/app.mk
include ../make/avr.mk
