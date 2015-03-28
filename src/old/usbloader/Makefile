PROGNAME = usbloader
SRCS = usbloader.c

INCPREFIX=../../

CFLAGS = -Os -g
LDFLAGS = -lm

MCU = atmega644

NRWW_SECTION_START = 0xe000
BOOT_SECTION_START = 0xf000

# the first time the bootloader is loaded, it must be held using the jumper

include $(INCPREFIX)make/bl.mk
include $(INCPREFIX)make/usbtiny.mk
include $(INCPREFIX)make/app.mk
include $(INCPREFIX)make/avr.mk

# hfuse: no OCD, no JTAG, enable SPI, WDT no always on, preserve EE,
#        set boot size to 2048 words, and reset in the bootloader,
# lfuse: no CKDIV8 or CKOUT, default SUT and crystal oscillator
# efuse: set brownout for 2.7v
# lock: don't let spm write to bootloader (in case app gets corrupt)

setfuses:
	avrdude -p $(MCU) -P $(DEVICE) -c $(PROGRAMMER) -U hfuse:w:0xd2:m -U lfuse:w:0xef:m -U efuse:w:0xfd:m -U lock:w:0x2f:m


setfusesold:
	avrdude -p $(MCU) -P $(DEVICE) -c $(PROGRAMMER) -U hfuse:w:0xd0:m -U lfuse:w:0xef:m -U efuse:w:0xfd:m -U lock:w:0x2f:m
