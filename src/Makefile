# Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

include make/config.mk

SUBDIRS = dataparser linalg gfx menu calibration

ifdef HAVE_HOST
SUBDIRS += dataclient menuhost calibrationhost
 ifdef HAVE_QT
SUBDIRS += dataviewer
 endif
endif


ifdef HAVE_AVR
SUBDIRS += avrdrivers myusb pointer dusi
endif

INCPREFIX =

include make/subdir.mk

calibration: linalg

menu: gfx

avrdrivers: dataparser myusb

pointer: calibration avrdrivers

survey: calibration menu avrdrivers

calibrationhost: calibration

menuhost: calibration menu

ifdef HAVE_AVR
menu: avrdrivers
endif
