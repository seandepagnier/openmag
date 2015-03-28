# Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

# common makefile, supports multiple targets with TARGET set

include $(INCPREFIX)make/config.mk

ifdef TARGET
DOTTARGET = .$(TARGET)
endif

OBJSUFFIX ?= $(DOTTARGET).o
DEPSUFFIX ?= $(DOTTARGET).d
LIBSUFFIX ?= $(DOTTARGET).a

ifdef USE_CCACHE
CCACHE = ccache
endif

OBJS += $(ASRCS:.S=$(OBJSUFFIX)) $(SRCS:.c=$(OBJSUFFIX)) $(CPPSRCS:.cpp=$(OBJSUFFIX))
DEPS += $(ASRCS:.S=$(DEPSUFFIX)) $(SRCS:.c=$(DEPSUFFIX)) $(CPPSRCS:.cpp=$(DEPSUFFIX))

OPTIMIZER ?= -gdwarf-2
CFLAGS += $(WARNS) $(VCFLAGS) $(OPTIMIZER) -I$(INCPREFIX) -I$(INCPREFIX)include -D_GNU_SOURCE

# for freebsd
CFLAGS += -I/usr/local/include
LDFLAGS += -L/usr/local/lib

%$(OBJSUFFIX) : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:$(OBJSUFFIX)=$(DEPSUFFIX))
	@sed -i"" s/$(@:$(OBJSUFFIX)=.o)/$@/1 $(@:$(OBJSUFFIX)=$(DEPSUFFIX))

%$(OBJSUFFIX) : %.c
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:$(OBJSUFFIX)=$(DEPSUFFIX))
	@sed -i"" s/$(@:$(OBJSUFFIX)=.o)/$@/1 $(@:$(OBJSUFFIX)=$(DEPSUFFIX))

%$(OBJSUFFIX) : %.S
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:$(OBJSUFFIX)=$(DEPSUFFIX))
	@sed -io"" s/$(@:$(OBJSUFFIX)=.o)/$@/1 $(@:$(OBJSUFFIX)=$(DEPSUFFIX))
