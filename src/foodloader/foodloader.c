/* vim:fdm=marker ts=4 et ai
 * {{{
 *
 * (c) by Alexander Neumann <alexander@bumpern.de>
 *     Lars Noschinski <lars@public.noschinski.de>
 *
 *     Idea and implementation for char startup mode by
 *     Scott Torborg - storborg@mit.edu - August 2006
 *
 *     Bug fixes and modifications to support 256kb parts
 *     Sean D'Epagnier - geckosenator@gmail.com - April 2009
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * For more information on the GPL, please go to:
 * http://www.gnu.org/copyleft/gpl.html
 }}} */

#include <avr/io.h>
#include <avr/boot.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "config.h"
#include "uart.h"

uint32_t flash_address;             /* start flash (byte address, converted) write at this address */
uint16_t eeprom_address;            /* start eerprom (byte address) write at this address */


/* defines */
#define HIGH(x) ( (uint8_t) (x >> 8) )
#define LOW(x)  ( (uint8_t) x )

#define noinline __attribute__((noinline))

/** output one character */
static noinline void uart_putc(uint8_t data)
/*{{{*/ {

    /* loop until data has been transmitted */
    while (!(_UCSRA_UART0 & _BV(_UDRE_UART0)));

    /* put data in buffer */
    _UDR_UART0 = data;

} /* }}} */

/** output a string */
static inline void uart_puts(uint8_t buffer[])
/*{{{*/ {

    /* send everything until end of string */
    while (*buffer != 0) {
        uart_putc(*buffer);
        buffer++;
    }

} /* }}} */

/** block until one character has been read */
static noinline uint8_t uart_getc(void)
/*{{{*/ {

    /* wait if a byte has been received */
   while (!(_UCSRA_UART0 & _BV(_RXC_UART0)));

    /* return received byte */
    return _UDR_UART0;

} /* }}} */

/** init the hardware uart */
static inline void init_uart(void)
/*{{{*/ {

    /* set baud rate */
    _UBRRH_UART0 = (uint8_t)(UART_UBRR >> 8);  /* high byte */
    _UBRRL_UART0 = (uint8_t)UART_UBRR;         /* low byte */

    /* set mode */
//    _UCSRC_UART0 = UART_UCSRC;

    /* enable transmitter, receiver */
    _UCSRB_UART0 = _BV(_TXEN_UART0) | _BV(_RXEN_UART0);

} /* }}} */

/** move interrupt vectors to application section and jump to main program */
static inline void start_application(void)
/* {{{ */ {
        /* move interrupt vectors to application section and jump to main program */
        _IVREG = _BV(IVCE);
        _IVREG = 0;

	((void (*)(void))0)();
} /* }}} */

/* I have a funny way of startup, I always start in the bootloader,
   but jump to the app immediately in any of 3 cases
   1. memory is cleared (address 0 is 0xffff)
   2. watchdog reset, and eeprom 5 bytes from end is set to 0x37
                           (this is how app jumps to bootloader)
   3. if keypad key 1 is held down
*/
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

        /* set keypad row 0 to internal pullup, and col 0 to output low. */
        DDRD |= _BV(PD4);
        PORTB |= _BV(PB4);

        _delay_us(10); /* let io pins settle */
        if(!(PINB & _BV(PB4))) /* check keypad keys */
            stay_in_bootloader = 1;

        /* reset keypads */
        DDRD &= ~_BV(PD4);
        PORTB &= ~_BV(PB4);

        /* if we aren't supposed to stay, go to the app */
        if(!stay_in_bootloader)
	    start_application();
}

int main(void)
/* {{{ */ {

    /* determine if we should stay in the bootloader or jump to app */
    custom_bootloader_startup();

    // set to 8mhz
//    OSCCAL = 0x80;

    uint8_t memory_type;

    /* BUF_T is defined in config.h, according the pagesize */
    BUF_T buffer_size;

    init_uart();

    /* send boot message */
#if SEND_BOOT_MESSAGE
        uart_putc('b');
#endif

    /* main loop */
    while (1)
    {
        uint8_t command;

        /* block until a command has been received */
        command = uart_getc();

        switch (command)
        {
            case 'P':   /* enter programming mode, respond with CR */
            case 'L':   /* leave programming mode, respond with CR */
#if EXIT_BOOTLOADER == 0
            case 'E':   /* exit bootloader, ignored */
#endif
                        uart_putc('\r');
                        break;

            case 'a':   /* report if we support address autoincrementing: yes, of course */
                        uart_putc('Y');
                        break;

            case 'A':   /* set write address start (in words), read high and low byte and respond with CR */
                        /* {{{ */

                        /* eeprom address is a byte address */
	                eeprom_address = uart_getc() << 8;
			eeprom_address |= uart_getc();

                        /* flash address is a byte address too, but we get a
                         * word address so convert it */
                        flash_address = eeprom_address << 1;

                        /* acknowledge */
                        uart_putc('\r');
                        break;

                        /* }}} */

#if __AVR_3_BYTE_PC__
            case 'H':   /* set ext write address start (in words), read high and low byte and respond with CR */
                        /* {{{ */
	                flash_address = (uint32_t)uart_getc() << 16;
			flash_address |= uart_getc() << 8;
			flash_address |= uart_getc();

			eeprom_address = 0;

                        /* flash address is a byte address too, but we get a
                         * word address so convert it */
                        flash_address <<= 1;

                        /* acknowledge */
                        uart_putc('\r');
                        break;
                        /* }}} */
#endif
            case 'e':   /* do a chip-erase, respond with CR afterwards */
                        /* {{{ */

                        /* iterate over all pages in flash, and try to erase every single
                         * one of them (the bootloader section should be protected by lock-bits (!) */

                        for (flash_address = 0; flash_address < BOOT_SECTION_START;
                             flash_address += SPM_PAGESIZE) {
                            boot_page_erase_safe(flash_address);
                        }

                        uart_putc('\r');
                        break;

                        /* }}} */

            case 'T':   /* select device type: received device type and respond with CR */
                        /* ignore this command, only the device this bootloader
                         * is installed on can be programmed :) */

                        /* discard byte and acknowledge */
                        uart_getc();
                        uart_putc('\r');
                        break;

            case 's':   /* read signature bytes: respond with the three signature bytes for this MCU */
                        uart_putc(_SIG_BYTE_3);
                        uart_putc(_SIG_BYTE_2);
                        uart_putc(_SIG_BYTE_1);
                        break;

            case 't':   /* return supported device codes (only one in this case), and terminate with a nullbyte */
                        uart_putc(_AVR910_DEVCODE);
                        uart_putc(0);
                        break;

            case 'S':   /* give software identifier, send exactly 7 chars */
                        uart_puts((uint8_t *)"FDL v");

            case 'V':   /* return software version (2 byte) */
                        uart_putc(VERSION_BYTE_1);
                        uart_putc(VERSION_BYTE_2);
                        break;

            case 'p':   /* send programmer type, in this case 'S' for serial */
                        uart_putc('S');
                        break;

#if EXIT_BOOTLOADER == 1
            case 'E':   /* exit bootloader */
#endif
            case 0:
            case 'X':   /* start application */
                        uart_putc('\r');
                        start_application();

                        break;

            case 'b':   /* check block support: return yes and 2 bytes block size we support */
                        uart_putc('Y');
                        uart_putc(HIGH(BLOCKSIZE));
                        uart_putc(LOW(BLOCKSIZE));
                        break;

            case 'B':   /* start block flash or eeprom load (fill mcu internal page buffer) */
                        /* {{{ */

                        /* first, read buffer size (in bytes) */
                        buffer_size = uart_getc() << 8;
                        buffer_size |= uart_getc();

                        /* check if our buffer can hold all this data */
                        if (buffer_size > BLOCKSIZE) {
                            uart_putc('?');
                            break;
                        }

                        /* then, read flash ('F') or eeprom ('E') memory type */
                        memory_type = uart_getc();

                        /* memory type is flash */
                        if (memory_type == 'F')
                        /* {{{ */ {

                            BUF_T i;
                            uint16_t temp_word_buffer;

                            if (flash_address > BOOT_SECTION_START) {
                                uart_putc(0);
                            }

                            uint32_t temp_address = flash_address;
                            boot_spm_busy_wait();

                            /* read data, wordwise, low byte first */
                            for (i = 0; i < buffer_size/2; i++) {

                                /* get data word */
                                temp_word_buffer = uart_getc() | (uart_getc() << 8);

                                /* write data to temporary buffer */
                                boot_page_fill(temp_address, temp_word_buffer);

                                /* increment by two, since temp_address is a byte
                                 * address, but we are writing words! */
                                temp_address += 2;
                            }

                            /* after filling the temp buffer, write the page and wait till we're done */
                            boot_page_write_safe(flash_address);
                            boot_spm_busy_wait();

                            /* re-enable application flash section, so we can read it again */
                            boot_rww_enable();

                            /* store next page's address, since we do auto-address-incrementing */
                            flash_address = temp_address;

                            uart_putc('\r');

                        } /* }}} */
                        else if (memory_type == 'E')
                        /* {{{ */ {

                            //uart_putc('E');
                            uint8_t temp_data;
                            BUF_T i;

                            for (i = 0; i < buffer_size; i++) {
                                temp_data = uart_getc();
                                eeprom_write_byte( (uint8_t *)eeprom_address, temp_data);

                                eeprom_address++;
                            }

                            uart_putc('\r');

                        } /* }}} */
                        else {
                            uart_putc('?');
                        }

                        break;

                        /* }}} */
            case 'g':   /* start block flash or eeprom read */
                        /* {{{ */

                        /* first, read byte counter */
                        buffer_size = (uart_getc() << 8) | uart_getc();

                        /* then, read memory type */
                        memory_type = uart_getc();

                        /* memory type is flash */
                        if (memory_type == 'F')
                        /* {{{ */ {

                            /* read buffer_size words */
                            for (BUF_T i = 0; i < buffer_size; i += 2) {
                                uint32_t temp_word_buffer;

                                /* read word */
                                temp_word_buffer =
#ifdef __AVR_3_BYTE_PC__
				   pgm_read_word_far(flash_address);
#else
				   pgm_read_word(flash_address);
#endif

                                /* send data */
                                uart_putc(LOW(temp_word_buffer));
                                uart_putc(HIGH(temp_word_buffer));

                                /* increment address by 2, since it's a byte address */
                                flash_address += 2;
                            }

                        } /* }}} */
                        /* if memory type is eeprom */
                        else if (memory_type == 'E')
                        /* {{{ */ {

                            for (uint8_t i = 0; i < buffer_size; i += 1) {
                                uint8_t temp_buffer;

                                /* read and send byte */
                                temp_buffer = eeprom_read_byte((uint8_t *)eeprom_address);
                                uart_putc(temp_buffer);

                                eeprom_address++;
                            }
                        } /* }}} */
                        else {
                            uart_putc('?');
                        }

                        break;
                        /* }}} */

            /* NOT IMPLEMENTED: */
            /* {{{ */
            /* 'c': write program memory, low byte -- NOT IMPLEMENTED */
            /* 'C': write program memory, high byte -- NOT IMPLEMENTED */
            /* 'm': issue page write -- NOT IMPLEMENTED */
            /* 'r': read lock bits -- NOT IMPLEMENTED */
            /* 'R': read program memory -- NOT IMPLEMENTED */
            /* 'd': read data (== eeprom) memory -- NOT IMPLEMENT */
            /* 'D': write data (== eeprom) memory -- NOT IMPLEMENTED */
            /* 'l': write lock bits -- NOT IMPLEMENTED */
            /* 'F': read fuse bits -- NOT IMPLEMENTED */
            /* 'N': read high fuse bits -- NOT IMPLEMENTED */
            /* 'Q': read extended fuse bits -- NOT IMPLEMENTED */ /* }}} */

            default:    /* default: respond with '?' */
                        uart_putc('?');
                        break;
        }

    }
} /* }}} */
