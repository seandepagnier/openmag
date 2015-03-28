# Copyright (C) 2009 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

LIBNAME=gfx
SRCS=line.c circle.c ellipse.c font.c compass.c rectangle.c state.c putpixel.c clip.c gfx3d.c

at90usb1287.BDFFONTS += 4x6.bdf 5x8.bdf
at90usb1287.BDFFONTS += helvR08.bdf helvR12.bdf timB18.bdf UTRG__24.bdf 
at90usb1287.NUMFONTSBEFORENUMERIC = 4
FONTS += at90usb1287.fonts.h

default.BDFFONTS += 4x6.bdf 5x8.bdf
default.BDFFONTS += helvR08.bdf helvR12.bdf timR18.bdf UTRG__24.bdf
default.BDFFONTS += helvB08.bdf helvB12.bdf timB18.bdf UTB___24.bdf
default.NUMFONTSBEFORENUMERIC = 10 # none
FONTS += default.fonts.h

include ../make/font.mk
include ../make/multilib.mk

all install: first

first:
	$(MAKE) -C bdftogfxfont
