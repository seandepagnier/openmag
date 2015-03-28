/* Copyright (C) 2007, 2008, 2009 Sean D'Epagnier <sean@depagnier.com>
 *
 * This Program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
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
 */

#include <avr/wdt.h>

enum DataDevices {DATA_UART = 1, DATA_USB = 2};
enum DataDevices data_device;

volatile uint8_t gotobootloader;

#if defined(USE_CDC) && (defined(USE_MOUSE) || defined(USE_JOYSTICK))
uint8_t usbcdcenabled EEMEM = 0;
DATA_ACCESSOR(name=usbcdcenabled type=bool mem=eeprom 
              varname=usbcdcenabled writable dir=settings)
uint8_t useusbcdc;
#endif

#ifdef USE_MOUSE
uint8_t usbmouseenabled EEMEM = 1;
DATA_ACCESSOR(name=usbmouseenabled type=bool mem=eeprom 
              varname=usbmouseenabled writable dir=settings)
uint8_t useusbmouse;
#endif

#ifdef USE_JOYSTICK
uint8_t usbjoystickenabled EEMEM = 0;
DATA_ACCESSOR(name=usbjoystickenabled type=bool mem=eeprom 
              varname=usbjoystickenabled writable dir=settings)
uint8_t useusbjoystick;
#endif

#if defined(USE_CDC) || defined(USE_MOUSE) || defined(USE_JOYSTICK)
#include "usb.h"


# if defined(USE_CDC)
#include "usbrb.h"
#include "usbrxline.h"
# endif

#else
#include <avr/interrupt.h>
#endif

#ifdef __AVR_AT90USB1287__
/* service the usb really fast */
ISR(TIMER0_OVF_vect)
{
   static uint8_t div;
   if(div++ < 4)
      return;
   div = 0;

   usbhw_poll();
}
#endif

#ifdef USE_UART

#define UART_TXSIZE 64
#include "uartrb.h"

#ifdef __AVR_ATmega2561__
MAKE_UART_RB(0)
#endif

MAKE_UART_RB(1)

#define UART_RXSIZE 64
#include "uartrxline.h"

#ifdef __AVR_ATmega2561__
MAKE_UART_RXLINE(0)

#endif
MAKE_UART_RXLINE(1)

#ifdef UART_ALLOW_SETBAUD  /* adds almost 2k to the binary */
uint16_t data_baud EEMEM = 12;

const uint16_t data_validbauds[] PROGMEM = {300, 600, 1200, 2400, 4800, 9600, 19200, 38400,
#if F_CPU == 12000000
                                            57600,
#endif
                                            0};

DATA_ACCESSOR(name=uartbaudrate mem=eeprom type=uint16_t baud dir=settings)

/* only allow exactly correct baud rates */
DATA_OPERATOR(set, baud,
              extern uint16_t data_baud;
              extern const uint16_t data_validbauds[];
              uint16_t input;
              data_set_sram_uint16_t(&input, data_param);
              int i = 0;
              uint16_t b;
              while((b = pgm_read_word(data_validbauds + i++))) {
                  if(input == b) {
                      uint16_t val = F_CPU / (16UL * input) - 1;
                      eeprom_write_word_safe(&data_baud, val);
                      DATA_OUTPUT("New baud will take effect after reset\n");
                      break;
                  }
              }
              if(!b)
                  DATA_OUTPUT("Invalid baud rate: %u\n", input);
    )

/* show actual baud rate */
DATA_OPERATOR(get, baud,
              extern uint16_t data_baud;
              uint16_t set = eeprom_read_word_safe(&data_baud);
              float b = F_CPU / (16 * (set + 1));
              extern void data_get_sram_float(float *);
              data_get_sram_float(&b);
              DATA_OUTPUT("\n");
    )

DATA_OPERATOR(values, baud,
              extern const uint16_t data_validbauds[];
              int i = 0;
              uint16_t b;
              while((b = pgm_read_word(data_validbauds + i++))) {
                  data_get_sram_uint16_t(&b);
                  DATA_OUTPUT(" ");
              }
              DATA_OUTPUT("\n");
    )
#endif // UART_ALLOW_SETBAUD

#endif // USE_UART

static int data_write(char c, FILE *str)
{
    /* to please stupid minicom and hyperterminal, add \r before \n */
   if(c == '\n')
      data_write('\r', str);

#ifdef USE_UART
   if(data_device & DATA_UART)
      uartrb1_write(c, str);
#endif

#ifdef __AVR_ATmega2561__ /* usb is uart 0 */
    if(data_device & DATA_USB)
       uartrb0_write(c, str);
#endif

#ifdef USE_CDC
    if(data_device & DATA_USB)
       usbrb_write(c);
#endif
    return 0;
}

static void datadriver_setalloutput(void)
{
    data_device = 0;
#ifdef USE_UART
    data_device |= DATA_UART;
#endif

#ifdef __AVR_ATmega2561__ /* usb is uart 0 */
    data_device |= DATA_USB;
#endif

#if defined USE_CDC
# if defined(USE_MOUSE) || defined(USE_JOYSTICK)
    if(useusbcdc)
# endif
       data_device |= DATA_USB;
#endif
}

FILE data_str = FDEV_SETUP_STREAM(data_write, NULL, _FDEV_SETUP_WRITE);

#ifdef __AVR_ATmega2561__
FILE debug_str = FDEV_SETUP_STREAM(uartrb1_write, NULL, _FDEV_SETUP_WRITE);
#else
FILE debug_str = FDEV_SETUP_STREAM(data_write, NULL, _FDEV_SETUP_WRITE);
#endif

void datadriver_init(void)
{
    extern uint16_t keys;

    /* enter bootloader
       normally the bootloader checks for this, but
       if the default bootloader is used with default fuses, it won't */
    if(keys & 1)
       ((void (*)(void))BOOTLOADER_OFFSET)();

    /* allow the user to override the data mode */
    if(keys & 2) {
       if(keys & 4) {
#ifdef USE_JOYSTICK
          useusbjoystick = 1;
#endif
       } else {
#if defined(USE_CDC) && (defined(USE_MOUSE) || defined(USE_JOYSTICK))
        useusbcdc = 1;
#endif
       }
    } else {
       if(keys & 4) {
#ifdef USE_MOUSE
        useusbmouse = keys&4;
#endif
       } else {
#if defined(USE_CDC) && (defined(USE_MOUSE) || defined(USE_JOYSTICK))
          useusbcdc = eeprom_read_byte_safe(&usbcdcenabled);
#endif
#ifdef USE_MOUSE
          useusbmouse = eeprom_read_byte_safe(&usbmouseenabled);
#endif
#ifdef USE_JOYSTICK
          useusbjoystick = eeprom_read_byte_safe(&usbjoystickenabled);
#endif
       }
    }

#ifdef USE_UART
    int baud;
#ifdef UART_ALLOW_SETBAUD
    baud = eeprom_read_word_safe(&data_baud);
    if(baud == 0xff || baud == 0x00) /* in case something bad happens */
#endif
        baud = 12;

    /* Set the baud rate */
    UBRR1H = baud >> 8;
    UBRR1L = baud;

    /* Enable UART receiver and transmitter */
    UCSR1B = (1 << RXEN1) | (1 << TXEN1);

    /* enable pullup on RX to avoid spurious data when no serial is connected */
    PORTD |= _BV(PD2);

    uart1_rxline_init();
#endif

#ifdef __AVR_ATmega2561__
    /* Set the baud rate fixed to 38400 */
    UBRR0H = 0;
    UBRR0L = 12;

    /* Enable UART receiver and transmitter */
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    /* enable pullup on RX to avoid spurious data when no serial is connected */
    PORTD |= _BV(PD2);

    uart0_rxline_init();
#endif

    stdout = &data_str;
    stderr = &debug_str;
    datadriver_setalloutput();

#if defined(USE_CDC) || defined(USE_MOUSE)
    USB_Init(USB_OPT_REG_DISABLED);

    TCCR0A = 0;
    TCCR0B = _BV(CS01); /* polling (F_CPU/256/8 = 3904) hz */
    TIMSK0 |= _BV(TOIE0);
#endif

    DATA_OUTPUT_CHAR(DATA_START_CHAR);
    DATA_OUTPUT(DATA_RESET DATA_PROMPT);
}

void datadriver_poll(void)
{
    char *data;
#ifdef USE_UART
    if((data = uart1_getline())) {
       data_device = DATA_UART;
       data_process_interactive_cmd(data);
       uart1_ackline();
    }
#endif

#ifdef __AVR_ATmega2561__
    if((data = uart0_getline())) {
       data_device = DATA_USB;
       data_process_interactive_cmd(data);
       uart0_ackline();
    }
#endif

#ifdef USE_CDC
    if((data = usb_getline())) {
       data_device = DATA_USB;
       data_process_interactive_cmd(data);
       usb_ackline();
    }
#endif

    if(gotobootloader) {
#if defined(USE_CDC) || defined(USE_MOUSE)
	USB_ShutDown();
#endif
        cli();
        wdt_enable(WDTO_15MS);
        /* bootloader checks this byte to see if it is 0x37
           to stay in bootloader */
        eeprom_write_byte_safe((unsigned char*)E2END-5, 0x37);
        for(;;); /* watchdog reset */
    }

    datadriver_setalloutput();
}
