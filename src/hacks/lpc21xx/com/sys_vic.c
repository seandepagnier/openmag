/**************************** sys_vic.c *********************************/
/* Copyright 2004/07/07 Aeolus Development				*/
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
/*  Wrapper around the VIC.						*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 sys_vic.c 13-Jul-2004,13:55:42,`RADSETT' Original archived version.
*   TLIB revision history ends.
*/
#include <limits.h>
#include "lpc210x.h"	/*  Use the 210x definitions to get VIC 	*/
			/* addresses.  These will be the same for 	*/
			/* multiple LPC variants.			*/
#include "lpc_sys.h"
#include "errno_lpc.h"

#define SOURCE_MASK	0x1F
#define INT_ENABLED	0x20

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
    )
{
 PROCESSOR_STATUS prev_int;	/*  Used for clearing/restoring		*/
 				/* interrupt status.			*/
 INT_PRIORITY i;


 if( defadd == 0) {		/* Check for null pointer.		*/
    return ELPC_NULL_ARG;
    }

	/*  Disable interrupts so that there are no conflicts while	*/
	/* setting up the VIC.						*/
 prev_int = DisableInterrupts( INTERRUPT_DISABLE_MASK);

	/*  Now disable all interrupts at the VIC.			*/
 VICIntEnClr = UINT_MAX;

	/*  Flag all interrupts as IRQ by clearing bits.		*/
 VICIntSelect = 0;

	/*  The lowest priority is used to indicated a non-vectored IRQ.*/
	/* This maps the way it's actually treated in hardware but the	*/
	/* setup is different.  First deal with the higher priority 	*/
	/* cases.							*/
 for( i = (INT_PRIORITY)0; i < MIN_VIC_PRIORITY; i++) {

	/*  Set up all vectors to point at the default service routine.	*/
    VICVectAddr[i] = defadd;

	/*  Disable all vectors. Tag the vector with a source number of	*/
	/* 0 and flag it as disabled.					*/
    VICVectCntl[i] = 0;
    }

	/*  For the lowest priority, set the default vector to the	*/
	/* default service routine.					*/
 VICDefVectAddr = defadd;

	/*  Finally restore interrupts to the previous state.		*/
 RestoreInterrupts( prev_int);

 return 0;
}

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
    unsigned int FIQ)		/*  Set to non-zero if interrupt wil be	*/
    				/* serviced as an FIQ.			*/

{
 PROCESSOR_STATUS prev_int;	/*  Used for clearing/restoring		*/
 				/* interrupt status.			*/

	/*  Check priority range for IRQs				*/
 if( (FIQ == 0) && (priority > MIN_VIC_PRIORITY)) {
    return ELPC_OOR;
    }

	/*  Since a null pointer for service may be correct for FIQ and	*/
	/* non-vectored IRQ sources only check for vectored IRQs.	*/
 if( (FIQ == 0) && (priority < MIN_VIC_PRIORITY)) {
    if( service == 0) {		/* Check for null pointer.		*/
         return ELPC_NULL_ARG;
	 }
    }

	/*  Disable interrupts so that there are no conflicts while	*/
	/* setting up the VIC.						*/
 prev_int = DisableInterrupts( INTERRUPT_DISABLE_MASK);

	/*  Now disable interrupts at the VIC.				*/
 VICIntEnClr = 1u << interrupt_source;

	/*  If it's to be set up as FIQ all we want to do is enable it  */
	/* and flag it as an FIQ source. The actual FIQ vector is set	*/
	/* outside of this routine, often in flash.			*/
 if( FIQ != 0) {
    VICIntSelect |= 1 << interrupt_source;
    }

	/*  If it's to be set up as an IRQ then determine which vector 	*/
	/* to set up.							*/
 else {

	/*  The lowest priority is used to indicate a non-vectored IRQ. */
	/* This maps the way it's actually treated in hardware but the	*/
	/* setup is different.  First deal with the higher priority 	*/
	/* cases.							*/
    if( priority < MIN_VIC_PRIORITY) {

		/*  Set up vector to point at service routine.		*/
         VICVectAddr[priority] = service;

		/*  Set up the source.  Tag the vector with the source	*/
		/* number and flag it as enabled.			*/
         VICVectCntl[priority] = ((unsigned int)interrupt_source & SOURCE_MASK) | 
              INT_ENABLED;
	 }

	/*  Now deal with the non-vectored IRQs.  All we do is check to	*/
	/* see if the default vector is being changed to a new value.	*/
    else {
	 if( service != 0) {
	      VICDefVectAddr = service;
	      }
         }

	/*  Flag interrupt as an IRQ by clearing the corresponding bit.	*/
    VICIntSelect &= ~(1u << interrupt_source);
    }

	/*  Interrupt has been setup, now enable it.  Interrupt may now	*/
	/* be flagged to the CPU.					*/
 VICIntEnable |= 1u << interrupt_source;

	/*  Finally restore interrupts to the previous state.  If they	*/
	/* were enabled before entry to this routine they may now be 	*/
	/* recognized by the CPU.					*/
 RestoreInterrupts( prev_int);

 return 0;
}
