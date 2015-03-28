PROGNAME = lpc2148test.c
CFLAGS = -std=gnu99 -fgnu89-inline
LDFLAGS = -lm -llpc214xusb

SRCS = lpc2148test.c

CFLAGS += -DUSE_DATA_MEM -DUSE_DATA_TYPE

include ../make/datagen.mk
include ../make/app.mk
include ../make/arm.mk
