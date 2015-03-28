# Copyright (C) 2009 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

MCU = at90usb1287
CFLAGS += -DUSE_CDC -DNDEBUG

include ../common.mk

LDFLAGS += -lMyUSB

include $(INCPREFIX)myusb/MyUSBCFLAGS

CFLAGS += -DF_CPU=8000000UL -DBOOTLOADER_OFFSET=0x1e000

include $(INCPREFIX)make/datagen.mk
include $(INCPREFIX)make/app.mk
include $(INCPREFIX)make/avr.mk
