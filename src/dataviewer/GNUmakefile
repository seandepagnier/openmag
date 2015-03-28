# jumpstart make if Makefile isn't built yet

ifeq ($(shell ls Makefile), Makefile)
include Makefile
else
all install clean:
	qmake
	$(MAKE) $@
endif
