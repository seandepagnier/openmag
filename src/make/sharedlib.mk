# Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

# shared library makefile
#
# each host app should specify PROGNAME, SRCS, and optionally CFLAGS and LDFLAGS 
#

include $(INCPREFIX)make/config.mk

CURCC := $(CC)
CC = $(CCACHE) $(CURCC)

OBJSUFFIX ?= .So
ifdef STATICLIBS
LIBSUFFIX ?= .a
else
LIBSUFFIX ?= .so
endif
SHAREDLIBNAME = lib$(LIBNAME)$(LIBSUFFIX)

include $(INCPREFIX)make/lib.mk

BINDIR ?= $(INCPREFIX)bin
CFLAGS += $(VCFLAGS) -g
ifndef STATICLIBS
CFLAGS += -fpic
endif

all: $(SHAREDLIBNAME)

$(SHAREDLIBNAME): $(OBJS)
ifdef STATICLIBS
	ar -cr $(SHAREDLIBNAME) $(OBJS)
else
	$(CC) $(OBJS) -o $(SHAREDLIBNAME) -shared $(LDFLAGS)
endif

ifdef STATICLIBS
install: $(SHAREDLIBNAME)
else
install: $(BINDIR) $(BINDIR)$(SHAREDLIBNAME)

$(BINDIR):
	mkdir $(BINDIR)

$(BINDIR)$(SHAREDLIBNAME): $(SHAREDLIBNAME) $(BINDIR)
	install $(SHAREDLIBNAME) $(BINDIR)/$(SHAREDLIBNAME)
endif


# include dependencies
sinclude $(DEPS)
