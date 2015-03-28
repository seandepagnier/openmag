# Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

INCPREFIX ?= ../
include $(INCPREFIX)make/common.mk

ifdef SUBDIRTARGET
.PHONY: $(SUBDIRS)

default: $(SUBDIRS)

$(SUBDIRS):
	unset SUBDIRTARGET; $(MAKE) -C $@ ${SUBDIRTARGET}

else
.PHONY: all-subdirs install-subdirs clean-subdirs

all: all-subdirs

install: install-subdirs

clean: clean-subdirs

all-subdirs:
	SUBDIRTARGET=all $(MAKE)

install-subdirs:
	SUBDIRTARGET=install $(MAKE)

clean-subdirs:
	SUBDIRTARGET=clean $(MAKE)

export SUBDIRS

endif
