/************************ uart0_ishell.s ********************************/
/* Copyright 2004/07/13 Aeolus Development				*/
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
/*  Shell routine for the UART # 0 interrupt.  Just makes sure entry 	*/
/* and exit are taken care of properly.  The hard work of responding to */
/* the interrupt is left to another routine called by this shell.	*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 uart0_ishell.s 13-Jul-2004,10:42:46,`RADSETT' Original archived version
*   TLIB revision history ends.
*/

 /* .text is used instead of .section .text so it works with
arm-aout too. */ 

	.text 
	.code 32 

	.align 	0


.include "interrupt.inc"

	.extern	Uart0Service
	.extern	VICVectAddrRead
	.global Uart0InterruptShell

Uart0InterruptShell:
	InterruptEntry

	bl 	Uart0Service		/*  Call routine that does the 	*/
					/* actual work			*/

	ldr	r0, =VICVectAddrRead	/*  Let VIC know we are done.	*/
	str	r0,[r0]

	InterruptExit

