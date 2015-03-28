/* Copyright (C) 2007, 2008 Sean D'Epagnier <sean@depagnier.com>
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

/* simple driver for the sca3000-d01, support only for reading each axis,
   temp data, motion detection and dumping buffered data, no support for
   8bit mode or free-fall detection */

#define SCA3000_REVID      0x00
#define SCA3000_STATUS     0x02
#define SCA3000_X_LSB      0x04
#define SCA3000_X_MSB      0x05
#define SCA3000_Y_LSB      0x06
#define SCA3000_Y_MSB      0x07
#define SCA3000_Z_LSB      0x08
#define SCA3000_Z_MSB      0x09
#define SCA3000_BUF_DATA   0x0f
#define SCA3000_TEMP_LSB   0x12
#define SCA3000_TEMP_MSB   0x13
#define SCA3000_MODE       0x14
#define SCA3000_BUF_COUNT  0x15
#define SCA3000_INT_STATUS 0x16
#define SCA3000_CTRL_SEL   0x18
#define SCA3000_UNLOCK     0x1e
#define SCA3000_INT_MASK   0x21
#define SCA3000_CTRL_DATA  0x22

static uint8_t sca3000_readreg(int reg)
{
   SCA_SETSS;
   spi_transfer(reg<<2);
   reg = spi_transfer(0);
   SCA_CLEARSS;

   return reg;
}

static void sca3000_writereg(int reg, uint8_t val)
{
   SCA_SETSS;
   spi_transfer(reg<<2 | 2);
   spi_transfer(val);
   SCA_CLEARSS;
}

#define SCA3000_MODE_BUFFERED 0x80
#define SCA3000_MODE_BYPASS 0x02
#define SCA3000_MODE_MOTION_DETECTION 0x03

void sca3000_unlock(void)
{
   sca3000_writereg(SCA3000_UNLOCK, 0x00);
   sca3000_writereg(SCA3000_UNLOCK, 0x50);
   sca3000_writereg(SCA3000_UNLOCK, 0xA0);
}

void sca3000_lock(void)
{
   sca3000_writereg(SCA3000_UNLOCK, 0xcc);
}

/* set for all 3 channels to the same */
void sca3000_set_motion_detection_levels(uint8_t level)
{
   sca3000_unlock();
   sca3000_writereg(SCA3000_CTRL_SEL, 0x03);
   sca3000_writereg(SCA3000_CTRL_DATA, level);
   sca3000_writereg(SCA3000_CTRL_SEL, 0x04);
   sca3000_writereg(SCA3000_CTRL_DATA, level);
   sca3000_writereg(SCA3000_CTRL_SEL, 0x05);
   sca3000_writereg(SCA3000_CTRL_DATA, level);
   sca3000_lock();
}

/* enter motion detection mode (power down)
   this will trigger the interrupt if set waking us up from a sleep mode */
void sca3000_enter_motion_detection(void)
{
   SCA_BEFORE_SS;
   sca3000_writereg(SCA3000_MODE, SCA3000_MODE_MOTION_DETECTION);
   SCA_ENABLE_INT;
   SCA_AFTER_SS;
}

void sca3000_exit_motion_detection(void)
{
   SCA_BEFORE_SS;
   sca3000_writereg(SCA3000_MODE, SCA3000_MODE_BUFFERED);
   SCA_DISABLE_INT;
   SCA_AFTER_SS;
}

void sca3000_powerdown(void)
{
#ifdef SCA_RESETPORT
    SCA_RESETPORT &= ~_BV(SCA_RESET);
#endif
}

void sca3000_startup(void)
{
#ifdef SCA_RESETPORT
   SCA_RESETPORT &= ~_BV(SCA_RESET);
   SCA_RESETDDR |= _BV(SCA_RESET);
#endif
}

/* initialize the sca3000, set the mode using a logical OR of the above modes */
uint8_t sca3000_detected;
void sca3000_init(void)
{
#ifdef SCA_RESETPORT
   SCA_RESETPORT |= _BV(SCA_RESET);
#endif
#ifdef __AVR__
   for(int i=0; i<20; i++) _delay_ms(1);
#endif

   SCA_BEFORE_SS;

   SCA_INITSS;

   sca3000_writereg(SCA3000_MODE, SCA3000_MODE_BUFFERED);

   /* check */
   uint8_t m = sca3000_readreg(SCA3000_MODE);
   /* if we don't read it back, then the device isn't working,
      don't bother to talk to it */
   if(m != SCA3000_MODE_BUFFERED) {
      SCA_AFTER_SS;
      return;
   }

   sca3000_detected = 1;

#if defined(SCA_AVR_INT_vect)
   /* make sure INT is an input */
   SCA_INTDDR &= ~_BV(SCA_INTBV);
   /* make a pullup */
   SCA_INTPORT |= _BV(SCA_INTBV);

   sca3000_writereg(SCA3000_INT_MASK, 0x0); /* interrupt active low */
#endif

   /* set to a small value for motion detection */
   sca3000_set_motion_detection_levels(2);
   SCA_AFTER_SS;
}

#define SCA3000_PLUS_OVERFLOW 0x6BFF
#define SCA3000_MINUS_OVERFLOW 0x9400
static int sca3000_readaxis(int axis)
{
   SCA_SETSS;
   spi_transfer((5+(axis<<1))<<2);
   int x = spi_transfer(0);
   int y = spi_transfer(0);
   SCA_CLEARSS;
   return ((x<<8) + y) >> 3;
}

/* get temperature in celcius */
static int16_t sca3000_read_temp(void)
{
   if(!sca3000_detected)
      return 0;

   SCA_BEFORE_SS;
   int x = sca3000_readreg(SCA3000_TEMP_MSB);
   int y = sca3000_readreg(SCA3000_TEMP_LSB);
   SCA_AFTER_SS;

   return ((x<<8) + y) >> 5;
}

#ifdef __AVR__
#if defined(SCA_AVR_INT_vect)
ISR(SCA_AVR_INT_vect)
{
   SCA_BEFORE_SS;
   uint8_t intstatus  = sca3000_readreg(SCA3000_INT_STATUS);
#if 0
   if(intstatus & 7) /* motion detected */
      sca3000_motion_detected = 1;
#endif
   SCA_AFTER_SS;
}
#endif
#endif

/* reads data from the sca3000, then if there is any data in the internal
   buffer, dump it too and average it with the data just read */
int sca3000_filter_data(int32_t data[3])
{
   if(!sca3000_detected)
      return 0;

   SCA_BEFORE_SS;
   
   /* read some data */
   data[0] = sca3000_readaxis(0);
   data[1] = sca3000_readaxis(1);
   data[2] = sca3000_readaxis(2);

   uint8_t c = sca3000_readreg(SCA3000_BUF_COUNT) / 3;

   /* no buffered data? then done */
   if(!c) {
      SCA_AFTER_SS;
      return 1;
   }

   /* dump buffer and divide by count */
   int32_t d[3] = {0, 0, 0};

   SCA_SETSS;
   spi_transfer(SCA3000_BUF_DATA<<2);
   uint8_t i, j;
   for(i = 0; i<c; i++)
      for(j = 0; j<3; j++) {
         int16_t x1 = spi_transfer(0);
         int16_t x0 = spi_transfer(0);
         d[j] += ((int16_t)((x1 << 8) + x0)) >> 3;
      }
   
   SCA_CLEARSS;
   SCA_AFTER_SS;
   
   for(i = 0; i<3; i++)
      data[i] = (data[i]+d[i]) / (c+1);
   
   return 1;
}
