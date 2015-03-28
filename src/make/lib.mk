# Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

# library makefile
#
# each library should specify LIBNAME, SRCS, and optionally CFLAGS and LDFLAGS 
#

INCPREFIX ?= ../

include $(INCPREFIX)make/common.mk

ifndef SHAREDLIBNAME
FULLLIBNAME = lib$(LIBNAME)$(LIBSUFFIX)
$(FULLLIBNAME): $(OBJS)
	$(AR) -cr $@ $(OBJS)
	$(RANLIB) $@
endif

all install: $(FULLLIBNAME)

clean:
	rm -f $(SHAREDLIBNAME) $(FULLLIBNAME) $(OBJS) $(DEPS) $(GEN) $(GENDEPS)

ifdef MULTILIBDATAGEN
include $(INCPREFIX)make/datagen.mk
endif
