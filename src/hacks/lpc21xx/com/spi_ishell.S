/************************ spi_ishell.s ********************************/

 /* .text is used instead of .section .text so it works with
arm-aout too. */ 

	.text 
	.code 32 

	.align 	0


.include "interrupt.inc"

	.extern	Spi0Service
	.extern	VICVectAddrRead
	.global Spi0InterruptShell

Spi0InterruptShell:
	InterruptEntry

	bl 	Spi0Service		/*  Call routine that does the 	*/
						/* actual work			*/

	ldr	r0, =VICVectAddrRead	/*  Let VIC know we are done.	*/
	str	r0,[r0]

	InterruptExit

