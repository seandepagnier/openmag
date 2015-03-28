/**************************** sys_pll.c *********************************/
/* Copyright 2003/12/27 Aeolus Development				*/
/* All rights reserved.							*/
/*									*/
/* Redistribution and use in source and binary forms, with or without	*/
/* modification, are permitted provided that the following conditions	*/
/* are met:								*/
/* 1. Redistributions of source code must retain the above copyright	*/
/*   notice, this list of conditions and the following disclaimer.	*/
/* 2. Redistributions in binary form must reproduce the above copyright	*/
/*   notice, this list of conditions and the following disclaimer in the*/
/*   documentation and/or other materials provided with the 		*/
/*   distribution.							*/
/* 3. The name of the Aeolus Development or its contributors may not be	*/
/* used to endorse or promote products derived from this software 	*/
/* without specific prior written permission.				*/
/*									*/
/* THIS SOFTWARE IS PROVIDED BY THE AEOULUS DEVELOPMENT "AS IS" AND ANY	*/
/* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE	*/
/* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR	*/
/* PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AEOLUS DEVELOPMENT BE	*/
/* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR	*/
/* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF	*/
/* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR 	*/
/* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,*/
/* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE */
/* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 	*/
/* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.			*/
/*									*/
/*  A wrapper around the PLL device.  Adds little bit of error checking	*/
/* and does the arithmetic needed to determione the register values for */
/* a particular clock frequency.  Also provides hooks so other parts	*/
/* of the application can determine the operating frequency.		*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 sys_pll.c 30-Dec-2003,10:34:12,`RADSETT' First archival version.
*   2 sys_pll.c 14-Jun-2004,15:02:40,`RADSETT' Remove use of struct _reent
*        argument so this
*        can be decoupled from the newlib support.
*   TLIB revision history ends.
*/
#include "lpc210x.h"
#include "lpc_sys.h"
#include "errno_lpc.h"

	/**** Local Macros ****/
#define PLL_ENABLE		((unsigned char)1u)	/* enable pll 	*/
#define PLL_CONNECT		((unsigned char)3u)	/* connect pll 	*/
#define PLL_FEED_SEQ1		((unsigned char)0xAAu)	/*  First feed 	*/
							/* value.	*/
#define PLL_FEED_SEQ2		((unsigned char)0x55u)	/*  Second feed	*/
							/* value.	*/
#define PLL_LOCK_MASK		((unsigned short)0x400u)/*  Masks PLL 	*/
							/* lock bit.	*/
#define MIN_OSCILLATOR_IN	(10000uL)
#define MAX_OSCILLATOR_IN	(25000uL)
#define MAX_OPERATING_SPEED	(60000uL)

	/**** Local Variables ****/
static unsigned long native_speed = 10000uL;	/* Default to 10MHz	*/

/********************* SetNativeSpeed ***********************************/
/*  SetNativeSpeed -- Set the oscillator frequency for the external 	*/
/* oscillator.  This is used to inform the routines that deal with cpu	*/
/* frequencies what the starting point is.  Any error here will be 	*/
/* multiplied later.							*/
/*  unsigned long speed	-- external oscillator/crystal frequency in kHz.*/
/*  Note:  There is no way to determine or verify this value so we have	*/
/* to trust the caller to get it right.					*/
/*  Returns 0 if successful.						*/
int SetNativeSpeed( 
    unsigned long speed)
{

 if( (speed < MIN_OSCILLATOR_IN) || (speed > MAX_OSCILLATOR_IN)) {
    return ELPC_OOR;
    }
 native_speed = speed;
 return 0;
}

/********************* SetDesiredSpeed **********************************/
/*  SetDesiredSpeed -- Set the cpu to desired frequency.  Relies on 	*/
/* earlier call to set native oscillator speed correctly.  Returns 0	*/
/* if successful.  desired_speed is set to actual speed obtained on 	*/
/* return.								*/
/*  unsigned long desired_speed	-- CPU operating frequency in kHz.	*/
/*  Returns 0 if successful.  errno will be set on error.		*/
int SetDesiredSpeed( 
    unsigned long desired_speed)
{
 unsigned long multiplier, remainder, pll_speed, i;

	/*  First check that the requested speed is within allowable 	*/
	/* limits.							*/
 if( (desired_speed < native_speed) ||  (desired_speed > MAX_OPERATING_SPEED)) {
    return ELPC_OOR;
    }

	/*  Find the nearest multiple between the native speed and the	*/
	/* desired speed.						*/
 multiplier = desired_speed/native_speed;
 remainder = desired_speed%native_speed;
 if( remainder > (native_speed/2)) {
    multiplier++;
	
	/*  If nearest speed is higher than maximum reduce to next 	*/
	/* closest.							*/
    if( (native_speed * multiplier) > MAX_OPERATING_SPEED) {
	 multiplier--;
	 }
    }

	/*  Finally limit multiplier range.  Note: we are unlikely to 	*/
	/* exceed the maximum.						*/
 if( multiplier < 1uL) {
    multiplier = 1uL;
    }
 if( multiplier > 32uL) {
    multiplier = 32uL;
    }

	/*  Now we need a PLL divider that will keep the PLL frequency	*/
	/* within the allowable range.  Simply ramp up through dividers */
	/* until we find one that works.					*/
 pll_speed = native_speed * multiplier;
 for( i = 0uL; i < 4uL; i++) {
    if( (pll_speed * 2 * (1 << i)) > 156000uL) {
	 if((pll_speed * 2 * (1 << i)) > 320000uL) {
	      return ELPC_CANT;
	      }
	 break;		/*lint !e960 non switch break.			*/
	 }
    }
 if( i >= 4) {			/*  Check to make sure we found a 	*/
    return ELPC_CANT;		/* divisor that works.			*/
    }

	/* Set PLL divisor, multiplier 					*/
 PLLCFG = (unsigned char)((multiplier -1) | (i << 5));
 PLLCON = PLL_ENABLE;		/* Enables PLL				*/
 PLLFEED = PLL_FEED_SEQ1;	/*  Change to setting above.  These two	*/
 PLLFEED = PLL_FEED_SEQ2;	/* updates MUST occur in sequence on 	*/
 				/* the VPB bus.				*/

 	/* wait for lock */
 while( (PLLSTAT & PLL_LOCK_MASK) == 0) {
    }

 	
 PLLCON = PLL_CONNECT;		/* Enables and connects PLL		*/
 PLLFEED = PLL_FEED_SEQ1;	/*  Change to setting above.  These two	*/
 PLLFEED = PLL_FEED_SEQ2;	/* updates MUST occur in sequence on 	*/
 				/* the VPB bus.				*/
 return 0;
}

/********************* ActualSpeed **************************************/
/*  ActualSpeed -- Returns the operating speed of the CPU.  Relies on 	*/
/* earlier call to set native oscillator speed correctly.		*/
unsigned long ActualSpeed( void)
{

	/*  Use PLL multiplier if connected.				*/
 if( (PLLCON & PLL_CONNECT) == PLL_CONNECT) {
    return( native_speed * ((PLLCFG & 0x1F) + 1));
    }
 return native_speed;	/*  No multiplier, just return native speed.	*/
}
