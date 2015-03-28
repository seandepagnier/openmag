#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL	      // Sets up the default speed for delay.h
#include <util/delay.h>
#include <avr/eeprom.h>

#include <stdio.h>
#include <stdlib.h>

#include <data/data.h>
#include <data/databasictypes.h>
#include <data/datadirectories.h>
#include <data/datastrings.h>
#include <data/datadriver.h>

uint8_t x; DATA_ACCESSOR(name=x type=uint8_t mem=sram varname=x writable)
uint16_t y EEMEM; DATA_ACCESSOR(name=y type=uint16_t mem=eeprom varname=y writable)

char hello[20] EEMEM; DATA_ACCESSOR(name=ident type=string mem=eeprom
                                    varname=hello writable arraylen=20)

int main(void)
{
    datadriver_init();
    sei();
    DEBUG("reset");

    for(;;) {
        datadriver_poll();
    }

    return(0);
}
