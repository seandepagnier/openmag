PROGNAME = menuhost
SRCS = menuhost.c emulation.c

LDFLAGS = -llinalg -lgfx -lmenu -lcalibration -lglut -lm -lncurses
CFLAGS = -g

# for freebsd
LDFLAGS += -L/usr/X11R6/lib
CFLAGS += -I/usr/X11R6/include

DATALIBS = ../calibration/calibration.da ../menu/menu.da

include ../make/datagen.mk
include ../make/app.mk
include ../make/host.mk
