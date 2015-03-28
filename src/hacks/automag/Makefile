PROGNAME = automag
CFLAGS = -std=gnu99 -fgnu89-inline -DNDEBUG
LDFLAGS = -lm -llpc214xusb

SRCS = automag.c

CFLAGS += -DUSE_DATA_MEM -DUSE_DATA_TYPE

include ../make/datagen.mk
include ../make/app.mk
include ../make/arm.mk
