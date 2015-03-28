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

/* this driver requires the ad7799 driver, it is designed for an ad7799 hooked up to
   a 3 axis magnetometer along with an io pin ready to pulse set/reset straps.

   It is interrupt driven, and reads data from the ad7799 putting the result in
   hmc_output
*/

#include <ad7799.h>
#include <stdlib.h>

#if defined(__AVR_ATmega644__)
#define HMC_PCIE PCIE1
#elif defined(__AVR_AT90USB1287__) || defined(__AVR_ATmega2561__)
#define HMC_PCIE PCIE0
#else
#error "Must define HMC_PCIE in hmc105x.h"
#endif

/* because there is a 2x time cost to change channels, it is not always
   desireable to switch channels after every sample.
   Without any channel switching at 123 samples per second,
   123 samples can be obtained, but only on 1 channel.

   With switching after n samples:
   1 58hz : 2 79hz : 4 96hz : 8 108hz : 16 115hz : 32 119hz
*/
uint8_t hmc_samples = 2;
uint8_t hmc_rate = AD7799_123_HZ;
uint8_t hmc_gain;

uint8_t hmc_saturated[3];

uint8_t hmc_initstate;
uint8_t hmc_setresetfailed[3];

static volatile int32_t hmc_data[3];
static volatile uint16_t hmc_datacount[3];

static volatile uint8_t hmc_chan;
static volatile int8_t hmc_count;
static volatile int32_t hmc_val;

const uint8_t hmc_chans[] = { AD7799_AIN1_CHAN, AD7799_AIN2_CHAN, AD7799_AIN3_CHAN};

void inline hmc_disable_interrupt(void)
{
    PCICR &= ~_BV(HMC_PCIE);
}

void inline hmc_enable_interrupt(void)
{
    PCICR |= _BV(HMC_PCIE);
}

static void hmc105x_reset_measurements(void)
{
    uint8_t i;
    for(i = 0; i<3; i++) {
       hmc_data[i] = 0;
       hmc_datacount[i] = 0;
    }
    hmc_val = 0;
    hmc_count = -1;
}

void hmc105x_powerup(void)
{
   hmc105x_reset_measurements();

   HMC_SSDDR |= _BV(HMC_SS); /* slave select pin */
   HMC_SSPORT &= ~_BV(HMC_SS); /* slave select pin */

   ad7799_init();

   /* if the initial status is zero, then we have failed, bail out
      and never enable the interrupt, this might occur if the ad7799 is not
      populated */
   if(!ad7799_init_status)
      return;

   HMC_SET;
   _delay_us(6);
   HMC_RESET;
   _delay_us(60);

   hmc_initstate = 6; /* 4 iterations for each axis */

   hmc_chan = 0;
   ad7799_write_config(0, 0, hmc_gain, 0, 1, AD7799_AIN1_CHAN);

   /* put the adc in conversion mode */
   ad7799_set_mode(AD7799_CONTINUOUS_CONVERSION_MODE, 1, hmc_rate);

   PCMSK0 |= _BV(PCINT3);    /* enable HMC_INT here */
}

void hmc105x_powerdown(void)
{
    /* disable interrupt */
    PCMSK0 &= ~_BV(PCINT3);    /* disable HMC_INT here */

    /* put the adc in powerdown mode */
    ad7799_set_mode(AD7799_POWERDOWN_MODE, 0, 0);

    HMC_SSPORT |= _BV(HMC_SS); /* enable weak pullup so the ad7799 doesn't
                                  get confused */
    HMC_SSDDR &= ~_BV(HMC_SS); /* turn off to reduce power consumption */
}

/* set range */
void hmc105x_setrange(uint8_t range)
{
   hmc_gain = range + 4;
   hmc105x_reset_measurements();
}

void hmc105x_setbandwidth(uint8_t samples, uint8_t rate)
{
   hmc_samples = samples;
   hmc_rate = rate;

   if(HMC_SSDDR & _BV(HMC_SS)) /* we are running and selected */
      ad7799_set_mode(AD7799_CONTINUOUS_CONVERSION_MODE, 1, hmc_rate);
}

void hmc105x_init(void)
{
    HMC_INITSR;
    hmc_enable_interrupt();
    hmc105x_powerup();
}

/* this isr is used for the ad7799 to trigger an interrupt when the MISO line
   goes low, therefore it must be PCINT0 */
ISR(PCINT0_vect)
{
    if(!(PCMSK0 & _BV(PCINT3))) /* ignore button wakeups */
        return;

    if(!ad7799_data_ready())
        return;

    /* toggle slave select line to ensure we are syncronized,
       don't allow it to generate an interrupt for this */
    PCMSK0 &= ~_BV(PCINT3);
    HMC_SSPORT |= _BV(HMC_SS);
    HMC_SSPORT &= ~_BV(HMC_SS);
    PCMSK0 |= _BV(PCINT3);

    ad7799_request_data(0);
    int32_t data = ad7799_read_data();

    /* if count is set to -1, drop this measurement and rewrite the config */
    if(hmc_count == -1) {
       hmc_count = 0;
       goto writeconfig;
    }

    /* are we still testing each axis for set/reset? */
    static int32_t lastdata;
    if(hmc_initstate) {
       if(hmc_initstate-- & 1) {
	  HMC_RESET;
	  if(labs(data-lastdata) < 20000L)
	     hmc_setresetfailed[hmc_chan] = 1;
	  if(++hmc_chan == 3)
	     hmc_chan = 0;
	  goto writeconfig;
       }

       HMC_SET;
       lastdata = data;
       return;
    }

    hmc_val += data;
    hmc_count++;

    /* don't move this line, the compiler has a bug and will generate wrong code */
    hmc_saturated[hmc_chan] = (data > 8386560L) || (data < -8386560L);

    if(hmc_count < hmc_samples)
        return;
    
    hmc_data[hmc_chan] += hmc_val;
    hmc_datacount[hmc_chan] += hmc_count;
    
    hmc_val = 0;
    hmc_count = 0;
    
    if(++hmc_chan == 3) {
        hmc_chan = 0;
        /* set and reset */
        HMC_SET;
        _delay_us(6); /* delay 6uS, if using a smaller
                         cap than the 10uF cap as a reservoir,
                         for set/reset current,
                         use a much longer delay (around 500uS for 1uF cap)
                      */
        HMC_RESET;
    }
    
writeconfig:
    ad7799_write_config(0, 0, hmc_gain, 0, 1, hmc_chans[hmc_chan]);
}

static int hmc105x_filter_data(int32_t data[3])
{
    if(!hmc_datacount[0] || !hmc_datacount[1] || !hmc_datacount[2])
        return 0;

    hmc_disable_interrupt();

    int32_t d[3] = {0, 0, 0};
    uint16_t count[3];

    uint8_t i;
    for(i = 0; i<3; i++) {
        d[i] = hmc_data[i];
        count[i] = hmc_datacount[i];
        hmc_data[i] = 0;
        hmc_datacount[i] = 0;
    }

    hmc_enable_interrupt();

    for(i = 0; i<3; i++)
       data[i] = (d[i] / count[i]) >> hmc_gain;  /* shift based on gain */

#if defined(__AVR_ATmega2561__)
    /* screwed up the board somehow and managed to reverse the polarity
       on the z axis */
    data[2] = -data[2];
#endif

    return 1;
}
