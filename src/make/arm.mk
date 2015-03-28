# Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

# avr specific

CC = $(CCACHE) arm-elf-gcc
AR = arm-elf-ar
OBJCOPY = arm-elf-objcopy
RANLIB = arm-elf-ranlib

#CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -mcpu=arm7tdmi-s
#LDFLAGS += -Wl,--gc-sections

BAUDRATE ?= 38400

FILES_BUILT = $(PROGNAME).srec $(PROGNAME).elf $(PROGNAME).lst $(PROGNAME).map

ifndef LIBNAME
all: $(PROGNAME).elf

ARMDIR = $(INCPREFIX)lpc21xx

# add crt.o, and make sure it comes first!
OBJS := crt.o $(OBJS)
DEPS := crt.d $(DEPS)

crt.o : $(INCPREFIX)lpc21xx/crt.S
	$(CC) -DROM_RUN -x assembler-with-cpp -c $< -o $@ $(CFLAGS) -MD

CFLAGS += -Wa,-adhlns=$(PROGNAME).lst
LDFLAGS += -nostartfiles -Wl,-Map=$(PROGNAME).map,--cref -T $(ARMDIR)/lpc2148-rom.ld
# liblpc21xx uses functions in libc that use functions in liblpc21xx
LDFLAGS += -L$(ARMDIR) -llpc21xx -lc -llpc21xx -L$(INCPREFIX)lpc214xusb
endif

install:

upload: $(PROGNAME).srec
	lpc_prog -d /dev/ttyS0 -w -r $< -b $(BAUDRATE)

$(PROGNAME).srec: $(PROGNAME).elf
	$(OBJCOPY) -O srec $< $@

$(PROGNAME).elf: $(DATA_SRC) $(OBJS)
	$(CC) $(OBJS) -o $(PROGNAME).elf $(LDFLAGS)
