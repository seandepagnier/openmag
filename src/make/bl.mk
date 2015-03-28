# Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

LDFLAGS += -Wl,--section-start=.text=$(BOOT_SECTION_START)
CFLAGS += -DNRWW_SECTION_START=$(NRWW_SECTION_START)
CFLAGS += -DBOOT_SECTION_START=$(BOOT_SECTION_START)
