/**************************** interrupt.inc *****************************/
/* Copyright 2004/07/05 Aeolus Development				*/
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
/*  Provides support for interrupt routines.				*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 interrupt.inc 13-Jul-2004,14:02:38,`RADSETT' Original archive version
*   2 interrupt.inc 06-Nov-2004,15:33:44,`RADSETT' Correct header (to refer to
*        correct file name).
*   TLIB revision history ends.
*/


/********************* InterruptEntry ***********************************/
/*  InterruptEntry -- Macro to encapsulate the setup needed at the 	*/
/* start of an interrupt.  Use as a companion to InterruptExit.		*/
/* Will save sufficient registers so that is a call is made to a 	*/
/* standard function enough has been saved that no registers will be 	*/
/* corrupted on interrupt return.  Any registers beyond r0-r3, and r12	*/
/* that will be used must be saved explicitly.  Interrupts are left as	*/
/* they were (IE if this is an IRQ, IRQs will be disabled and the FIQ	*/
/* will be in the state it was when the interrupt occured.). 		*/
.macro InterruptEntry
	sub	r14, r14, #4		/*  Get the correct return	*/
					/* address.			*/
	stmfd	r13!, {r0-r3, r12, r14}	/*  Save the register that can 	*/
					/* be lost during a procedure 	*/
					/* call.			*/
	mrs	r0, spsr		/*  Get the status register of 	*/
					/* the interrupted code	and	*/
	stmfd	r13!, {r0}		/* save it.			*/
.endm

/********************* InterruptExit ************************************/
/*  InterruptExit -- Macro to encapsulate the teardown needed at the 	*/
/* end of an interrupt.  Use as a companion to InterruptEntry.		*/
/* Disables interrupts, then restores the registers and the processor 	*/
/* state saved by InterruptEntry before returning to the interrupted 	*/
/* program. 								*/
.macro InterruptExit
	mrs	r0, cpsr		/*  Disable interrupts.		*/
	orr	r0, r0, #0xC0
	msr	cpsr_c, r0

	ldmfd	r13!, {r0}		/*  Get the interrupted 	*/
					/* processor state from the 	*/
					/* stack and restore it.	*/
	msr	spsr_cxsf, r0

	ldmfd	r13!, {r0-r3, r12, PC}^	/*  Restore the registers saved */
					/* by InterruptEntry and return	*/
					/* to the interrupted program	*/
					/* restoring the processor's 	*/
					/* status register. 		*/
.endm
