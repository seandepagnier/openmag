# Copyright (C) 2008, 2009 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

INCPREFIX ?= ../

BDFTOGFXFONT = $(INCPREFIX)gfx/bdftogfxfont/bdftogfxfont

defaultfont: all

$(BDFTOGFXFONT): ; $(error "Must build bdftogfxfont")

all install: $(FONTS)

clean: cleanfonts

cleanfonts:
	rm -f $(FONTS)

%.fonts.h: $($(@:.fonts.h=).BDFFONTS) $(BDFTOGFXFONT)
	cat $($(@:.fonts.h=).BDFFONTS) | $(BDFTOGFXFONT) $($(@:.fonts.h=).NUMFONTSBEFORENUMERIC) > $@
