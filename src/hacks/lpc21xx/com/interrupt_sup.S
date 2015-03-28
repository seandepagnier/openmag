/************************ interrupt_sup.s *******************************/
/* Copyright 2004/07/16 Aeolus Development				*/
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
/*  Support for enabling and disabling interrupts on an ARM processor	*/
/* at the processor level. The disable routine returns the previous	*/
/* interrupt state so that it can be restored.. This allows these	*/
/* routines to be used in an environment where nested calls could occur.*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 interrupt_sup.s 16-Jul-2004,11:49:34,`RADSETT' Original archived version.
*   TLIB revision history ends.
*/

 /* .text is used instead of .section .text so it works with
arm-aout too. */ 

	.text 
	.code 32 

	.align 	0



	.global DisableInterrupts
	.global RestoreInterrupts

/********************* DisableInterrupts ********************************/
/*  DisableInterrupts -- Disables IRQ/FIQ using the flags passed. 	*/
/*  INT_MASK imask (r0)  -- Flag bits to be set in the processor status */
/* register.								*/
/*  Returns the previous processor status register.			*/
DisableInterrupts:
	mov	r1, r0		/* Save mask for further work.		*/
	mrs	r0, cpsr	/* Get status register.			*/
	orr	r1, r1, r0	/* Or in passed in mask to disable 	*/
				/*  interrupts.				*/
	msr	cpsr_c, r1	/* Update status register to disable 	*/
				/*  interrupts.				*/
	mov	pc, r14		/* Return to caller with prev interrupt	*/
				/*  state in r0.			*/

/********************* RestoreInterrupts ********************************/
/*  RestoreInterrupts -- Restores IRQ/FIQ using the processor status	*/
/* passed (presumeably from a previous call to DisableInterrupts). 	*/
/*  PROCESSOR_STATUS stat (r0)  -- Processor status register containing	*/
/* previous interrupt state to restore.					*/
/*  Returns nothing.							*/
RestoreInterrupts:
	msr	cpsr_c, r0	/* Restore interrupt portion of the	*/
				/*  status register.			*/
	mov	pc, r14		/* Return.				*/

