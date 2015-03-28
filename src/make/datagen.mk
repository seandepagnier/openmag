# Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

# Makefile to handle the dataparser

INCPREFIX ?= ../

DATAPROCESSDIR = $(INCPREFIX)data
DATAPROCESS = dataprocess

DATAPARSER = $(INCPREFIX)dataparser/dataparser
DATAPARSER_CFLAGS = -E -DDATAGEN

DATASUFFIX = $(DOTTARGET).do
DATALIBSUFFIX = $(DOTTARGET).da

DATA_SRC = datagen.c
DATA_HDR = datagen.h

DATAFILES := $(SRCS:.c=$(DATASUFFIX))
DATADEPS := $(DATAFILES:$(DATASUFFIX)=$(DATASUFFIX).d)

ifdef LIBNAME
DATALIBNAME = $(LIBNAME)$(DATALIBSUFFIX)
else
DATAFILES := $(DATAPROCESS)$(DATASUFFIX) $(DATAFILES)
DATADEPS := $(DATAPROCESS)$(DATASUFFIX).d $(DATADEPS)
SRCS += $(DATA_SRC)
OBJS += $(DATAPROCESS).o
DEPS += $(DATAPROCESS).d
endif

GEN += $(DATA_HDR) $(DATA_SRC) $(DATAFILES) $(DATALIBNAME)
DEPS += $(DATADEPS)

all install : $(DATALIBNAME)

$(DATALIBNAME): $(DATAFILES)
	cat $(DATAFILES) > $@

datagen.o: datagen.c
	$(CC) -c datagen.c $(CFLAGS) -Wno-implicit-function-declaration -fno-builtin-sqrt -MD

# because dataprocess.c needs to include generated code, it cannot be built as
# a library, instead it is built as an extra object in the same directory with
# the regular program sources

$(DATAPROCESS)$(DATASUFFIX): $(DATAPROCESSDIR)/$(DATAPROCESS).c
	$(CC) $(DATAPROCESSDIR)/$(DATAPROCESS).c $(DATAPARSER_CFLAGS) $(CFLAGS) -MD -MF $@.d -I. | $(DATAPARSER) > $@
	@sed -i"" s/$(shell echo $@ | sed s/\\$(DATASUFFIX)/\\.o/1)/$@/1 $@.d

dataprocess.o: $(DATAPROCESSDIR)/$(DATAPROCESS).c $(DATA_HDR)
	$(COMPILE.c) $(DATAPROCESSDIR)/$(DATAPROCESS).c -MD -I.

$(DATAPARSER): ; $(error "Must build dataparser")

$(DATA_SRC) $(DATA_HDR) : $(DATAPARSER) $(DATAFILES) $(DATALIBS)
	$(DATAPARSER) $(DATAFILES) $(DATALIBS)

%$(DATASUFFIX) : %.c
	$(CC) $< $(DATAPARSER_CFLAGS) $(CFLAGS) -MD -MF $@.d | $(DATAPARSER) > $@
	@sed -i"" s/$(@:$(DATASUFFIX)=.o)/$@/1 $@.d
