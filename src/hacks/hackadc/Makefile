PROGNAME = hackadc
LDFLAGS = -lm -Wl,-u,vfprintf -lprintf_flt

all:
	avr-gcc -c -mmcu=atmega168 -I. -g -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wall -Wa,-adhlns=$(PROGNAME).lst  -std=gnu99 $(PROGNAME).c -o $(PROGNAME).o
	avr-gcc -mmcu=atmega168 -I. -g -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wall -Wa,-adhlns=$(PROGNAME).o  -std=gnu99 $(PROGNAME).o --output $(PROGNAME).elf -Wl,-Map=$(PROGNAME).map,--cref $(LDFLAGS)
	avr-objcopy -O ihex -R .eeprom $(PROGNAME).elf $(PROGNAME).hex

clean:
	rm $(PROGNAME).hex $(PROGNAME).elf  $(PROGNAME).lst $(PROGNAME).map  $(PROGNAME).o

prog:
	avrdude -V -p atmega168 -P /dev/parport0 -c dapa -E reset -U flash:w:$(PROGNAME).hex

upload:
	avrdude -u -V -p atmega168 -P /dev/ttyS0 -c avr109 -U f:w:$(PROGNAME).hex -b 4800 
