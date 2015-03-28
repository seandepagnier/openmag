# Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

# To rebuild docs type make, to clean generated docs type make clean
# generate pdf, dvi, html (multiple pages and all-in-one), info and text
# you might need to install a package called tetex.

DOCS = UserGuide.texi DUSIDatasheet.texi PointerDeviceDatasheet.texi Calibration.texi ProgrammingGuide.texi

OBJS += $(DOCS:.texi=.pdf)
#OBJS += $(DOCS:.texi=.dvi)
OBJS += $(DOCS:.texi=.html)
OBJS += $(DOCS:.texi=.info)
OBJS += $(DOCS:.texi=.txt)

DIRS = $(DOCS:.texi=)
LOGS = $(DOCS:.texi=.log)

DEPS != *.texih

all: $(OBJS) $(DIRS) index.html

install: all

$(OBJS): $(DEPS)

index.html: index.html.in genindex.sh $(DOCS)
	./genindex.sh $(DIRS)

%.pdf: %.texi
	texi2pdf -q -b -c $<

%.dvi: %.texi
	texi2dvi -q -b -c $<

%.html: %.texi
	makeinfo --html --no-split $<

%: %.texi
	rm -fr $@
	makeinfo --html $@.texi

%.info: %.texi
	makeinfo $< -o $@

%.txt: %.texi
	makeinfo --plaintext $< -o $@

clean:
	rm -rf $(OBJS) $(DIRS) $(LOGS) index.html
