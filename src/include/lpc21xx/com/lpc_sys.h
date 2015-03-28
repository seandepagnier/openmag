/**************************** lpc_sys.h *********************************/
/* Copyright 2003/12/29 Aeolus Development				*/
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
/*  System control.  Basic support that doesn't really fit as part of a */
/* device driver.							*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 lpc_sys.h 30-Dec-2003,10:34:12,`RADSETT' First archival version.
*   2 lpc_sys.h 17-Jan-2004,16:04:06,`RADSETT' Correct title line.
*   3 lpc_sys.h 29-Jan-2004,11:28:14,`RADSETT' Update Prototypes.
*        Add MinimumAchievableWait prototype.
*        Spelling corrections
*   4 lpc_sys.h 13-Jul-2004,13:55:42,`RADSETT' A few spelling corrections.
*        Remove use of struct _reent argument from some routines
*        so they can be decoupled from the newlib support.
*        Provide prototype for endless_loop.
*        Add DisableInterrupt and RestoreInterrupt protypes, typedefs and defines.
*        Add VICInit and VICSetup prototypes with supporting typedefs and defines.
*   TLIB revision history ends.
*/
/*lint -library*/
#ifndef LPC_SYS__H
#define LPC_SYS__H

#include <reent.h>

	/**** Structures, typedefs, macros  & enums. ****/

typedef enum {		/*  Enumerates the possible VPB clock dividers	*/
    VPB_DIV1,		/*  VPB clock = CPU clock			*/
    VPB_DIV2,		/*  VPB clock = CPU clock / 2			*/
    VPB_DIV4		/*  VPB clock = CPU clock / 4			*/
    } VPB_param;

typedef enum {		/*  Enumerates MAM modes.			*/
    MAM_disabled,	/*  MAM disabled.				*/
    MAM_part_enable,	/*  MAM partially enabled.			*/
    MAM_full_enable	/*  MAM fully enabled.				*/
    } MAM_CONTROL;

	/*  Holds processor status, should probably be treated as a	*/
	/* magic cookie.  						*/
typedef unsigned int PROCESSOR_STATUS;

	/*  Holds interrupt disable flags for adjusting interrupt	*/
	/* enable/disable at the processor level.  Use the defined	*/
	/* values to set. 						*/
typedef unsigned int INT_MASK;

	/* Disables all interrupts.					*/
#define INTERRUPT_DISABLE_MASK ((INT_MASK)0xC0)	

typedef unsigned int INTERRUPT_SOURCE;	/*  Interrupt source.  Defined	*/
					/* in hardware.			*/
typedef unsigned char INT_PRIORITY;	/*  Interrupt priority.		*/

	/*  Minimum priority.						*/
#define MIN_VIC_PRIORITY	((INT_PRIORITY)16)

/********************* ActualSpeed **************************************/
/*  ActualSpeed -- Returns the operating speed of the CPU.  Relies on 	*/
/* earlier call to set native oscillator speed correctly.		*/
unsigned long ActualSpeed( void);

/********************* GetUs ********************************************/
/*  GetUs -- Get the current time in uS.				*/
unsigned long long GetUs( void);

/********************* DisableInterrupts ********************************/
/*  DisableInterrupts -- Disables IRQ/FIQ using the flags passed. 	*/
/*  INT_MASK imask (r0)  -- Flag bits to be set in the processor status */
/* register.								*/
/*  Returns the previous processor status register.			*/
PROCESSOR_STATUS DisableInterrupts( INT_MASK imask);

/********************* RestoreInterrupts ********************************/
/*  RestoreInterrupts -- Restores IRQ/FIQ using the processor status	*/
/* passed (presumeably from a previous call to DisableInterrupts). 	*/
/*  PROCESSOR_STATUS stat (r0)  -- Processor status register containing	*/
/* previous interrupt state to restore.					*/
/*  Returns nothing.							*/
void RestoreInterrupts( PROCESSOR_STATUS stat);

/********************* MinimumAchievableWait ****************************/
/*  MinimumAchievableWait -- Get the shotest wait we can do.		*/
unsigned int MinimumAchievableWait(void);

/********************* SetDesiredSpeed **********************************/
/*  SetDesiredSpeed -- Set the cpu to desired frequency.  Relies on 	*/
/* earlier call to set native oscillator speed correctly.  Returns 0	*/
/* if successful.  desired_speed is set to actual speed obtained on 	*/
/* return.								*/
/*  unsigned long desired_speed	-- CPU operating frequency in kHz.	*/
/*  Returns 0 if successful.  errno will be set on error.		*/
int SetDesiredSpeed( unsigned long desired_speed);

/********************* SetMAM *******************************************/
/*  SetMAM -- Set up the MAM.  Minimal error checking, not much more 	*/
/* than a wrapper around the register.  Returns 0 if successful, 	*/
/* something else.  Sets errno in case of an error.			*/
/*  unsigned int cycle_time -- number of cycles to access the flash.	*/
/*  MAM_CONTROL ctrl	-- Mode to place MAM in.  One of:		*/
/*				MAM_disabled, MAM_part_enable, or 	*/
/*				MAM_full_enable.			*/
/*  Returns 0 if successful. Additional error/sanity checks are possible*/
int SetMAM( unsigned int cycle_time, MAM_CONTROL ctrl);

/********************* SetNativeSpeed ***********************************/
/*  SetNativeSpeed -- Set the oscillator frequency for the external 	*/
/* oscillator.  This is used to inform the routines that deal with cpu	*/
/* frequencies what the starting point is.  Any error here will be 	*/
/* multiplied later.							*/
/*  unsigned long speed	-- external oscillator/crystal frequency in kHz.*/
/*  Note:  There is no way to determine or verify this value so we have	*/
/* to trust the caller to get it right.					*/
/*  Returns 0 if successful.						*/
int SetNativeSpeed( unsigned long speed);

/********************* StartClock ***************************************/
/*  StartClock -- Starts up the clock used for internal timing.  	*/
/* Attempts to match the desired clock speed (CLOCK_SPEED) and 		*/
/* initializes timing_scale_factor to a compensating scale.  Returns 0	*/
/* if successful, otherwise error code will be retained in errno.	*/
/*  Note:  Should be called only after all clocks have been set up.	*/
/* Otherwise time scale will not be correct.				*/
/*  Returns 0 if successful.						*/
int StartClock( void);


/********************* VICInit ******************************************/
/*  VICInit -- Initializes the VIC.  Does anything required for initial	*/
/* setup.  In particular it ensures that the default vector is set up	*/
/* correctly and all interrupt sources are disabled.	 		*/
/*  void (*defadd)(void) -- Address of the default interrupt service 	*/
/* routine.								*/
/* Returns 0 if successful, otherwise an error.				*/
/*lint -sem(VICInit, 1p)						*/
int VICInit( 
    void (*defadd)(void)	/*  Default interrupt service routine.	*/
    );

/********************* VICSetup *****************************************/
/*  VICSetup -- Sets up an interrupt source in the Vectored Interrupt	*/
/* Controller.  Although it checks against some errors there is no 	*/
/* check against the interrupt source since the allowable range varies	*/
/* from variant to variant.						*/
/*  Returns 0 if successful, otherwise an error.			*/
/*  The lint semantic check is useful more in the header file than in   */
/* sys_vic.c.  It provides a check (if you use PC-LINT) that the 	*/
/* arguments passed are in the correct range.				*/
/*  Lint requires that this be a single line.				*/
/*lint -sem(VICSetup, (1n <= MAX_INTERRUPT_SOURCE) && (((4n == 0) && ((2n < MIN_VIC_PRIORITY) && 3p ) || (2n == MIN_VIC_PRIORITY)) || (4n != 0))) */
int VICSetup( 
    INTERRUPT_SOURCE interrupt_source,	/*  Source of the interrupt to	*/
    					/* setup.  This is the interrupt*/
    					/* number assigned to the	*/
    					/* peripheral in hardware.	*/
    INT_PRIORITY priority, 	/*  The priority of the interrupt if 	*/
    				/* assigned to the IRQ.  This also ends	*/
    				/* up being the index to its vector. 	*/
    				/* Not used if the interrupt is assigned*/
    				/* to the FIQ.  If the interrupt is 	*/
    				/* assigned the lowest priority the 	*/
    				/* service routine passed in (if not 0)	*/
    				/* will replace the current default	*/
    				/* vector routine. 			*/
    void (*service)(), 		/*  A pointer to the service routine to */
				/* use for this vector.  Should be 0 for*/
				/* FIQ, may be 0 for non-vectored IRQs	*/
    unsigned int FIQ);		/*  Set to non-zero if interrupt wil be	*/
    				/* serviced as an FIQ.			*/

/********************* VPBControl ***************************************/
/*  VPBControl -- Control the clock divider on the peripheral bus.	*/
/* Returns the actual divider in ptr (requested is also passed in ptr.	*/
/*  VPB_parm p		-- requested VPB to CPU freq rate.		*/
/*  Returns 0 if successful.						*/
int VPBControl( VPB_param p);

/********************* VPBRate ******************************************/
/*  VPBRate -- Finds and returns the rate of the clock on the 		*/
/* peripheral bus (in Hz).  						*/
unsigned long VPBRate( void);

/********************* UsToCounts ***************************************/
/*  UsToCounts -- converts to internal units in counts from uS.  Other 	*/
/* Modules use this counter for a timebase so this needs to be 		*/
/* available to them.							*/
/*  unsigned int us	-- microseconds to convert to counts.	 	*/
/*  Returns number of counts corresponding to us.  Saturates on		*/
/* overflow so for large time periods it is possible to get a result	*/
/* lower than requested.						*/
unsigned int UsToCounts( unsigned int us);

/********************* WaitUs *******************************************/
/*  WaitUs -- Wait for 'wait_time' us					*/
/*  unsigned int wait_time	-- microseconds to convert to counts. 	*/
/*  Will break wait into multiple waits if needed to avoid saturation.	*/
void WaitUs( unsigned int wait_time);

/********************* endless_loop *************************************/
/*  endless_loop -- As it says, just sits there forever.		*/
/*lint -sem(endless_loop, r_no)						*/
void endless_loop( void);

#endif /* LPC_SYS__H */
