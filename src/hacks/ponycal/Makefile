PROGNAME = ponycal
SRCS = ponycal.c

CFLAGS = -g

LDFLAGS += -llinalg -lcalibration -lm

INCPREFIX = ../../

DATALIBS = $(INCPREFIX)calibration/calibration.da

include $(INCPREFIX)make/datagen.mk
include $(INCPREFIX)make/app.mk
include $(INCPREFIX)make/host.mk
