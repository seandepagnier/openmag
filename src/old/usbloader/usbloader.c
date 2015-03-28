/* usbloader 1.1 Copyright (C) 2007 Sean D'Epagnier <sean@depagnier.com>
 *
 *     Using usbtiny Copyright (C) 2006 Dick Streefland
 *     Implements usbasp protocol Copyright (C) 2006 Thomas Fischl
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
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include <avr/eeprom.h>
#include <avr/boot.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#define F_CPU 12000000UL
#include <util/delay.h>

#include <avr/wdt.h>

#include <usb.h>

#include "usbaspdefs.h"

#include "usbtiny.h"
#include "config.h"

enum {PROG_STATE_IDLE, PROG_STATE_WRITEFLASH, PROG_STATE_READFLASH,
      PROG_STATE_READEEPROM, PROG_STATE_WRITEEEPROM};

#define PROG_BLOCKFLAG_FIRST    1
#define PROG_BLOCKFLAG_LAST     2

static uint8_t prog_state = PROG_STATE_IDLE;

static uint16_t prog_address;
static unsigned int prog_nbytes = 0;
static unsigned int prog_pagesize;
static uint8_t prog_blockflags;
static uint8_t prog_pagecounter;

static volatile uint8_t disconnected = 1;

#define READ_SIGNATURE   (_BV(SPMEN) | _BV(SIGRD))
byte_t read_signature( uint16_t sig )
{
   uint8_t result;
    __asm__ __volatile__
    (
        "movw r30, %3\n\t"
        "sts %1, %2\n\t"
        "lpm %0, Z\n\t"
        : "=r" (result)
        : "i" (_SFR_MEM_ADDR(__SPM_REG)),
          "r" ((uint8_t)READ_SIGNATURE),
          "r" (sig)
        : "r0", "r30", "r31"
    );
    return result;
}

byte_t usb_setup ( byte_t data[8] )
{
   switch(data[1]) {
   case USBASP_FUNC_CONNECT:
      return 0;
   case USBASP_FUNC_DISCONNECT:
      disconnected = 1;
      return 0;
   case USBASP_FUNC_TRANSMIT:
      switch(data[2]) { /* see page 300 in atmega644.pdf */
      case 0x38: /* calibration byte read like signature */
         data[3] = read_signature(1);
         break;
      case 0x30:
         data[3] = read_signature(data[4]<<1);
         break;
      case 0x50:
         if(data[3] == 0x00)
            data[3] = boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS);
         else if(data[3] == 0x08)
            data[3] = boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS);
         else
            return 0;
         break;
      case 0x58:
         if(data[3] == 0x00)
            data[3] = boot_lock_fuse_bits_get(GET_LOCK_BITS);
         else if(data[3] == 0x08)
            data[3] = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
         else
            return 0;
         break;
      case 0xa0:
         data[3] = eeprom_read_byte((uint8_t*)(data[3]<<8 | data[4]));
         break;
      case 0xac:
         switch(data[3]) {
         case 0xe0: /* write lock bits */
         case 0xa0: /* write fuse low bits */
         case 0xa8: /* write fuse high bits */
         case 0xa4: /* write fuse extended bits */
            /* no support for these */
            break;
         case 0x80: /* chip erase */
            {
               uint16_t addr;
               for (addr = 0; addr < NRWW_SECTION_START; addr += SPM_PAGESIZE)
                  boot_page_erase_safe(addr);
            } break;
         default:
            /* invalid op */
            break;
         }
         break;
      case 0xc0:
         eeprom_write_byte_safe((uint8_t*)(data[3]<<8 | data[4]), data[5]);
         break;
      default:
         /* invalid op */
         break;
      }
      return 4;
   case USBASP_FUNC_READFLASH:
      prog_nbytes = (data[7] << 8) | data[6];
      if(prog_state != PROG_STATE_READFLASH) {
         prog_address = 0;
         prog_state = PROG_STATE_READFLASH;
      }
      return 0xff;
   case USBASP_FUNC_ENABLEPROG:
      data[0] = 0;
      return 1;
   case USBASP_FUNC_WRITEFLASH:
      prog_pagesize = data[4];
      prog_blockflags = data[5] & 0x0F;
      prog_pagesize += (((unsigned int)data[5] & 0xF0)<<4); //TP: Mega128 fix
      if (prog_blockflags & PROG_BLOCKFLAG_FIRST) {
         prog_pagecounter = prog_pagesize;
         prog_address = 0;
      }
      prog_nbytes = (data[7] << 8) | data[6];
      prog_state = PROG_STATE_WRITEFLASH;
      boot_spm_busy_wait();
      return 0;
   case USBASP_FUNC_READEEPROM:
      prog_nbytes = (data[7] << 8) | data[6];
      if(prog_state != PROG_STATE_READEEPROM) {
         prog_address = 0;
         prog_state = PROG_STATE_READEEPROM;
      }
      return 0xff;
   case USBASP_FUNC_WRITEEEPROM:
      prog_pagesize = 0;
      prog_blockflags = 0;
      prog_nbytes = (data[7] << 8) | data[6];
      if(prog_state != PROG_STATE_WRITEEEPROM) {
         prog_address = 0;
         prog_state = PROG_STATE_WRITEEEPROM;
      }
      return 0;
   default:
      /* invalid op */
      return 0;
   }
}

byte_t usb_in ( byte_t* data, byte_t len )
{
   /* fill packet */
   if (prog_state == PROG_STATE_READFLASH)
      memcpy_P(data, (void*)prog_address, len);
   else
   if (prog_state == PROG_STATE_READEEPROM)
      eeprom_read_block(data, (void*)prog_address, len);
   else
      return 0;

   prog_address += len;

   return len;
}

/* data buffered in ram that goes in NRWW section.. Can't write it until we are
   done talking usb since writing will block interrupts (and therefore usb)
   This allows for up to 3k into the nrww sectionk on atmega644 */
static uint16_t rambuf[(RAMEND - 0x500)/2];
static uint16_t nrwwbuflast;

void writepage(void)
{
   int j;
   for(j = 0; j<prog_pagesize-prog_pagecounter; j+=2)
      boot_page_fill_safe(j, rambuf[j>>1]);
               
   /* last block and page flush pending, so flush it now */
   boot_page_write_safe(prog_address);
   boot_spm_busy_wait();
}

void usb_out ( byte_t* data, byte_t len )
{
   if (prog_state == PROG_STATE_WRITEFLASH) {
      uint8_t i;
      for (i = 0; i < len; i+=2) {
         uint16_t worddata = *(uint16_t*)(data+i);
         if (prog_address >= NRWW_SECTION_START) {
            /* can't write this data yet since interrupts have to be on */
            uint16_t ind = (prog_address-NRWW_SECTION_START)>>1;
            if(ind >= sizeof rambuf)
               for(;;); /* die */
            rambuf[ind] = worddata;
            nrwwbuflast = prog_address;
         } else
         if (prog_pagesize == 0) {
            /* not paged, currently not supported */
         } else {
            /* paged */
            rambuf[(prog_address&0xff)>>1] = worddata;

            prog_pagecounter -= 2;
            if (prog_pagecounter == 0) {
               writepage();
               prog_pagecounter = prog_pagesize;

#ifdef BOOTLOADER_USE_LCD
               static int lcdd;
               if(++lcdd == 3) {
                  lcdd = 0;

                  char str[20];
                  size_t strlen(const char*);
                  char * itoa(int, char *, int);
                  itoa(prog_address>>8, str, 10);
                  lcd_setLine(1, 0, str, strlen(str));
               }
#endif
            }
         
            prog_nbytes -= 2;
         
            if (prog_nbytes == 0) {
               prog_state = PROG_STATE_IDLE;
               /* write the last part of the final page */
               if ((prog_blockflags & PROG_BLOCKFLAG_LAST) &&
                   (prog_pagecounter != prog_pagesize)) {
                  writepage();
               }
               boot_rww_enable_safe(); /* done writing, enable flash section for reading */
            }
         }
         prog_address += 2;
      }
   } else
   if(prog_state == PROG_STATE_WRITEEEPROM) {
      eeprom_write_block_safe(data, (void*)prog_address, len);
      prog_address += len;
   }
}

ISR(__vector_default)
{
   /* unrecognized interrupt, reset with watchdog */
   wdt_enable(WDTO_15MS);
   for(;;);
}

int main(void)
{
#ifdef BOOTLOADER_JUMPER
   /* configure pin as input and enable pullup */
   BOOTLOADER_DDR &= ~BOOTLOADER_MASK;
   BOOTLOADER_PORT |= BOOTLOADER_MASK;
#endif

   /* bootloader activation methods */
   /* 1) activation via watchdog reset with 5th to
      last byte of eeprom set to 0x37 */
   if (MCUSR & _BV(WDRF)) {
      if(eeprom_read_byte((unsigned char*)E2END-5) == 0x37) {
         MCUSR = 0;
         wdt_disable();

         eeprom_write_byte_safe((unsigned char*)E2END-5, 0);
         disconnected = 0;
      }
   }

#ifdef BOOTLOADER_JUMPER
   /* 2) activation via jumper */
   _delay_ms(1); /* wait to give it time to pull up */
   if (!(BOOTLOADER_PIN & BOOTLOADER_MASK))
      disconnected = 0;
#endif

   if(!disconnected) {
#ifdef BOOTLOADER_USE_LCD
      lcd_setup();
      lcd_setLine(0, 0, "usb ldr v1.1", 12);
#endif

      MCUCR = (1 << IVCE);  /* enable change of interrupt vectors */
      MCUCR = (1 << IVSEL); /* move interrupts to boot flash section */

      usb_init();
      sei();
      
      int timeout = 10000;
      while(timeout) {
         if(disconnected)
            timeout--;
         usb_poll();
      }

      /* turn off usb */
      usb_off();
    
      /* disable interrupts */
      cli();

      MCUCR = (1 << IVCE);  /* enable change of interrupt vectors */
      MCUCR = (0 << IVSEL); /* move interrupts to application section */

      /* now finish flashing any data that is in the nrww section */
      for(prog_address=NRWW_SECTION_START; prog_address < BOOT_SECTION_START 
             && prog_address<=nrwwbuflast; prog_address+=SPM_PAGESIZE) {
         boot_page_erase_safe(prog_address);
         uint16_t off = prog_address - NRWW_SECTION_START;
         uint16_t j;
         for(j = 0; j<SPM_PAGESIZE; j+=2)
            boot_page_fill_safe(j, rambuf[(j+off)>>1]);
         boot_page_write_safe(prog_address);
         boot_spm_busy_wait();
      }

#ifdef BOOTLOADER_USE_LCD
      lcd_clear(); /* clear lcd */
#endif
   }

   ((void (*)(void))0)(); /* jump to app */
   return 0;
}
