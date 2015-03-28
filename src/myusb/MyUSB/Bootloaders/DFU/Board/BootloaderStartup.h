
#include <util/delay.h>

void custom_bootloader_startup(void);

void custom_bootloader_startup(void)
{
        uint8_t stay_in_bootloader = 0;

        /* see if the memory is cleared.. */
        if(pgm_read_word(0) == 0xffff)
           stay_in_bootloader = 1;

        /* check if it was from app */
        if (MCUSR & _BV(WDRF)) {
            if(eeprom_read_byte((unsigned char*)E2END-5) == 0x37) {
                MCUSR = 0;
                wdt_disable();

                eeprom_write_byte((unsigned char*)E2END-5, 0);
                stay_in_bootloader = 1;
            }
        }

        /* setup HWB as input with pullup */
        DDRE &= ~_BV(PE2);
        PORTE |= _BV(PE2);

        /* set keypad row 0 to internal pullup, and col 0 to output low. */
        DDRD |= _BV(PD4);
        PORTB |= _BV(PB4);

        _delay_us(10); /* let io pins settle */
        if(!(PINE & _BV(PE2))) /* check HWB */
            stay_in_bootloader = 1;

        if(!(PINB & _BV(PB4))) /* check keypad keys */
            stay_in_bootloader = 1;

        /* reset HWB */
        PORTE &= ~_BV(PE2);

        /* reset keypads */
        DDRD &= ~_BV(PD4);
        PORTB &= ~_BV(PB4);

        /* if we aren't supposed to stay, go to the app */
        if(!stay_in_bootloader)
            ((void (*)(void))0)();
}
