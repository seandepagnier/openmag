# Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

%.pcb : %
	gsch2pcb $<
	@touch $<.pcb

%_schematic : %
	mkdir -p $<_schematic
	FILES=`grep schematics $< | sed 's/schematics //g'`; cp $$FILES $<_schematic; \
	for i in $$FILES; do gschem -p -o `echo $$i | sed s/sch/png/g` -s image.scm $$i; mv `echo $$i | sed s/sch/png/g` $<_schematic; done;
	tar cjf $@.tar.bz2 $<_schematic
	cd $<_schematic; zip $@.zip *; mv $@.zip ..
	tar cjvf $@.tar.bz2 $@

%_layout : %.pcb
	mkdir -p $@
	cp $< $@
	cd $@; pcb -x gerber $<; zip $@.zip *; mv $@.zip ..
	tar cjvf $@.tar.bz2 $@

%_layout.png : %.pcb
	pcb -x png --outfile $@ $<

DEVICES = dusi pointer

PCBS = $(DEVICES:=.pcb)
PNGS = $(DEVICES:=_layout.png)
SCHEMATICS = $(DEVICES:=_schematic)
LAYOUTS = $(DEVICES:=_layout)

all: $(PCBS) $(PNGS) $(SCHEMATICS) $(LAYOUTS)

install: all

clean:
	rm -fr .refdes_renum_uptodate *.bak* *.net *.net.pcb PCB*backup PCB*save *.pcb- *.log *.xy $(PNGS) $(SCHEMATICS) $(LAYOUTS)

SCHS = $(shell ls *.sch)

$(PCBS) : .refdes_renum_uptodate components/* footprints/*

.refdes_renum_uptodate: $(SCHS)
	refdes_renum $(SCHS)
	touch $@
