# Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

# host makefile
#
# each host app should specify PROGNAME, SRCS, and optionally CFLAGS and LDFLAGS 
#

CURCC := $(CC)
CC = $(CCACHE) $(CURCC)

PLATFORM := $(shell uname)
ifeq ($(findstring WIN,$(PLATFORM)), WIN)
PROGNAME := $(PROGNAME).exe
endif

FILES_BUILT = $(PROGNAME)

BINDIR ?= $(INCPREFIX)bin

all: $(PROGNAME)

$(PROGNAME): $(OBJS) $(GEN)
	$(CC) $(OBJS) -o $(PROGNAME) $(LDFLAGS)

install: $(BINDIR) $(BINDIR)$(PROGNAME)

$(BINDIR):
	mkdir $(BINDIR)

$(BINDIR)$(PROGNAME): $(PROGNAME) $(BINDIR)
	install $(PROGNAME) $(BINDIR)/$(PROGNAME)

# include dependencies
sinclude $(DEPS)
