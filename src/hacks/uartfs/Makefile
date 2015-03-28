PROGNAME = demo
CFLAGS = -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wall -Wstrict-prototypes  -std=gnu99
LDFLAGS = -lm
CPU = atmega164
AVRDUDE = avrdude
AVRDUDE_FLAGS = -p $(CPU) -P /dev/parport0 -c dapa -E reset

#CFLAGS += -fprofile-arcs -ftest-coverage
#LDFLAGS += -lgcov

all:
	avr-gcc -c -mmcu=$(CPU)p -I. $(CFLAGS) demo.c -o demo.o
	avr-gcc -c -mmcu=$(CPU)p -I. $(CFLAGS) uartfs.c -o uartfs.o
	avr-gcc -mmcu=$(CPU)p -I. -g $(LDFLAGS) uartfs.o demo.o -o $(PROGNAME).elf
	avr-objcopy -O ihex -R .eeprom $(PROGNAME).elf $(PROGNAME).hex

clean:
	rm -f $(PROGNAME).hex $(PROGNAME).elf demo.o uartfs.o uartfs-host

prog:
	avrdude $(AVRDUDE_FLAGS) -U flash:w:$(PROGNAME).hex

host: uartfs-host

uartfs-host: uartfs-host.c
	gcc -o uartfs-host uartfs-host.c -g
