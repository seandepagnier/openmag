# Copyright (C) 2008, 2009 Sean D'Epagnier <sean@depagnier.com>
#
# This Program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#

# avr specific

CC = $(CCACHE) avr-gcc
AR = avr-ar
OBJCOPY = avr-objcopy
RANLIB = avr-ranlib

MCU ?= at90usb1287

ifdef TARGET
MCU = $(TARGET)
endif

WARNS += -Wall -Wno-format -Wno-unused -Wno-uninitialized
OPTIMIZER := -Os -mcall-prologues -ffunction-sections -fdata-sections -fshort-enums
CFLAGS +=  -Wa,-adhlns=$(PROGNAME).lst -mmcu=$(MCU)
LDFLAGS += -Wl,-Map=$(PROGNAME).map,--cref -Wl,--gc-sections -Wl,--relax

BAUDRATE ?= 57600

PROGRAMMER ?= dapa   #-- for poor parallel port programmer
DEVICE ?= /dev/parport0
#PROGRAMMER ?= usbtiny

BOOTLOADER = avr109 # foodloader
BOOTLOADERDEVICE ?= /dev/ttyACM0
INITIATE_BOOTLOADER = if [ -e $(BOOTLOADERDEVICE) ] ; then echo -e '\033' > $(BOOTLOADERDEVICE); sleep 1.5; fi

AVRDUDEFLAGS += -y -s
AVRDUDE = avrdude $(AVRDUDEFLAGS)

FILES_BUILT = $(PROGNAME).hex $(PROGNAME).eeprom.hex $(PROGNAME).elf $(PROGNAME).lst $(PROGNAME).map

ifndef LIBNAME
all: $(PROGNAME).elf
endif

install:

$(PROGNAME).elf: $(DATA_SRC) $(OBJS)
	$(CC) -mmcu=$(MCU) $(OBJS) -o $(PROGNAME).elf $(LDFLAGS)

$(PROGNAME).hex: $(PROGNAME).elf
	$(OBJCOPY) -O ihex -R .eeprom $(PROGNAME).elf $(PROGNAME).hex

$(PROGNAME).eeprom.hex: $(PROGNAME).elf
	$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ihex $(PROGNAME).elf $(PROGNAME).eeprom.hex

# program using PROGRAMMER
prog: $(PROGNAME).hex
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -U flash:w:$(PROGNAME).hex

# isp over usb
ispprog:
	$(AVRDUDE) -u -p $(MCU) -P usb -c jtag2 -U f:w:$(PROGNAME).hex

# bootloader
upload: $(PROGNAME).hex
	$(INITIATE_BOOTLOADER)
	$(AVRDUDE) -P $(BOOTLOADERDEVICE) -u -p $(MCU) -c $(BOOTLOADER) -U f:w:$(PROGNAME).hex -b $(BAUDRATE)

download: $(PROGNAME).hex
	$(INITIATE_BOOTLOADER)
	$(AVRDUDE) -P $(BOOTLOADERDEVICE) -u -p $(MCU) -c $(BOOTLOADER) -U f:r:$(PROGNAME).hex -b $(BAUDRATE)

upload_eeprom: $(PROGNAME).eeprom.hex
	$(INITIATE_BOOTLOADER)
	$(AVRDUDE) -P $(BOOTLOADERDEVICE) -u -p $(MCU) -c $(BOOTLOADER) -U eeprom:w:$(PROGNAME).eeprom.hex -b $(BAUDRATE)

upload_prog_eeprom: $(PROGNAME).hex $(PROGNAME).eeprom.hex
	$(INITIATE_BOOTLOADER)
	$(AVRDUDE) -P $(BOOTLOADERDEVICE) -u -p $(MCU) -c $(BOOTLOADER) -U f:w:$(PROGNAME).hex -U eeprom:w:$(PROGNAME).eeprom.hex  -b $(BAUDRATE)

# verify an uploader
verify:
	$(AVRDUDE) -u -p $(MCU) -P $(BOOTLOADERDEVICE) -c $(BOOTLOADER) -U f:v:$(PROGNAME).hex -b $(BAUDRATE)

# display end of eeprom
eeprom_end:
	avr-nm $(PROGNAME).elf | grep eeprom_end

# display end of bss
bss_end: $(PROGNAME).elf
	avr-nm $(PROGNAME).elf | grep bss_end

# display data size
data_load_end: $(PROGNAME).elf
	avr-nm -td -S $(PROGNAME).elf | grep load_end

# display all variables and functions by size in bytes
var_size: $(PROGNAME).elf
	avr-nm -td -S $(PROGNAME).elf | awk '{print $$2 " " $$4}' | sort  | grep -vi ^[a-z]

# display _etext
_etext: $(PROGNAME).elf
	avr-nm $(PROGNAME).elf | grep _etext

# dump eeprom
dumpeeprom:
	$(INITIATE_BOOTLOADER)
	$(AVRDUDE) -u -p $(MCU) -P $(BOOTLOADERDEVICE) -c $(BOOTLOADER) -U eeprom:r:eeprom:r -b $(BAUDRATE) 

# clear the eeprom
blankeeprom:
	$(INITIATE_BOOTLOADER)
	$(AVRDUDE) -u -p $(MCU) -P $(BOOTLOADERDEVICE) -c $(BOOTLOADER) -U eeprom:w:blank:r -b $(BAUDRATE) 
# run avarice
avarice:
	avarice --jtag usb -2 :4242 -f $(PROGNAME).elf

# read fuses
readfuse:
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -U lfuse:r:lfuse:h
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -U hfuse:r:hfuse:h
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -U efuse:r:efuse:h
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -U lock:r:lock:h
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -U cal:r:cal:h
	cat lfuse
	cat hfuse
	cat efuse
	cat lock
	cat cal
	rm -f lfuse hfuse efuse lock cal

# load using dfu-programmer
dfu_prog: $(PROGNAME).hex
	$(INITIATE_BOOTLOADER)
	dfu-programmer at90usb1287 erase
	dfu-programmer at90usb1287 flash $(PROGNAME).hex

dfu_eeprom: $(PROGNAME).eeprom.hex
	dfu-programmer at90usb1287 flash-eeprom $(PROGNAME).eeprom.hex

dfu_reset:
	dfu-programmer at90usb1287 reset

.PHONY: dfu_prog dfu_eeprom dfu_reset

dfu_prog_eeprom: dfu_prog dfu_eeprom

dfu: dfu_prog_eeprom dfu_reset

dfu_prog_only: dfu_prog dfu_reset

$(INCPREFIX)myusb/MyUSB/Bootloaders/CDC/BootloaderCDC.hex:
	$(MAKE) -C $(INCPREFIX)myusb/MyUSB/Bootloaders/CDC

$(INCPREFIX)myusb/MyUSB/Bootloaders/DFU/BootloaderDFU.hex:
	$(MAKE) -C $(INCPREFIX)myusb/MyUSB/Bootloaders/DFU

load_cdc_bootloader: $(INCPREFIX)myusb/MyUSB/Bootloaders/CDC/BootloaderCDC.hex
	$(AVRDUDE) -p at90usb1287 -c dapa -U flash:w:"$<"
	$(MAKE) setfuse

load_dfu_bootloader: $(INCPREFIX)myusb/MyUSB/Bootloaders/DFU/BootloaderDFU.hex
	$(AVRDUDE) -p at90usb1287 -c dapa -U flash:w:"$<"
	$(MAKE) setfuse

$(PROGNAME).tar.bz2: $(PROGNAME).hex $(PROGNAME).eeprom.hex
	mkdir $(PROGNAME); cp $(PROGNAME).hex $(PROGNAME).eeprom.hex $(PROGNAME); tar cjf $@ $(PROGNAME); rm -r $(PROGNAME)

$(PROGNAME).zip: $(PROGNAME).hex $(PROGNAME).eeprom.hex
	zip $@ $(PROGNAME).hex $(PROGNAME).eeprom.hex

dist: $(PROGNAME).tar.bz2 $(PROGNAME).zip

# hfuse no OCD, no JTAG, keep SPI prog, and wd not always on preserve eeprom,
#       bootsz 2048 words, bootrst on = 0xd0
# lfuse no div8, no ckout, SUT 10, clock 1111, = 0xef  (faster startup use 0xcf)
# efuse, leave default, keep hardware boot and brown out at 2.6v = 0xf3
# lock, leave default, or set to 0x3f

# for at90usb1287
setfuse:
	$(AVRDUDE) -p at90usb1287 -c $(PROGRAMMER) -U hfuse:w:0xd0:m -U lfuse:w:0xee:m -U efuse:w:0xfb:m -U lock:w:0x2f:m

# for atmega2561
#setfuse:
#	$(AVRDUDE) -p atmega2561 -c $(PROGRAMMER) -U hfuse:w:0xd0:m -U lfuse:w:0xe2:m

# include dependencies
sinclude $(DEPS)
