# Copyright (C) 2009 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

# main application makefile
#
# each app should specify PROGNAME, SRCS, and optionally CFLAGS and LDFLAGS 
#

CSCMSRCS = $(SCMSRCS:.scm=.c)

SRCS += $(CSCMSRCS)
GEN += $(CSCMSRCS)

LDFLAGS += -lchicken

%.c : %.scm
	chicken $<
