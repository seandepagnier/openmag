	.file	"debug.c"
	.arch atmega644
__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__tmp_reg__ = 0
__zero_reg__ = 1
	.global __do_copy_data
	.global __do_clear_bss
	.text
.global	uart_write
	.type	uart_write, @function
uart_write:
/* prologue: frame size=0 */
/* prologue end (size=0) */
	mov r25,r24
.L3:
	lds r24,192
	sbrs r24,5
	rjmp .L3
	sts 198,r25
	ldi r24,lo8(0)
	ldi r25,hi8(0)
/* epilogue: frame size=0 */
	ret
/* epilogue end (size=1) */
/* function uart_write size 10 (9) */
	.size	uart_write, .-uart_write
.global	uart_init
	.type	uart_init, @function
uart_init:
/* prologue: frame size=0 */
/* prologue end (size=0) */
	sts 197,__zero_reg__
	sts 196,r24
	ldi r24,lo8(24)
	sts 193,r24
	ldi r24,lo8(uart_str)
	ldi r25,hi8(uart_str)
	sts (__iob+2)+1,r25
	sts __iob+2,r24
/* epilogue: frame size=0 */
	ret
/* epilogue end (size=1) */
/* function uart_init size 14 (13) */
	.size	uart_init, .-uart_init
.global	__vector_1
	.type	__vector_1, @function
__vector_1:
/* prologue: frame size=0 */
	push __zero_reg__
	push __tmp_reg__
	in __tmp_reg__,__SREG__
	push __tmp_reg__
	clr __zero_reg__
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r30
	push r31
/* prologue end (size=17) */
	ldi r30,lo8(-8192)
	ldi r31,hi8(-8192)
	icall
/* epilogue: frame size=0 */
	pop r31
	pop r30
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop __tmp_reg__
	out __SREG__,__tmp_reg__
	pop __tmp_reg__
	pop __zero_reg__
	reti
/* epilogue end (size=17) */
/* function __vector_1 size 37 (3) */
	.size	__vector_1, .-__vector_1
.global	usbloadable_init
	.type	usbloadable_init, @function
usbloadable_init:
/* prologue: frame size=0 */
/* prologue end (size=0) */
	ldi r24,lo8(1)
	out 61-0x20,r24
	ldi r30,lo8(105)
	ldi r31,hi8(105)
	ld r24,Z
	ori r24,lo8(3)
	st Z,r24
/* epilogue: frame size=0 */
	ret
/* epilogue end (size=1) */
/* function usbloadable_init size 8 (7) */
	.size	usbloadable_init, .-usbloadable_init
.global	uart_read
	.type	uart_read, @function
uart_read:
/* prologue: frame size=0 */
/* prologue end (size=0) */
.L16:
	lds r24,192
	sbrs r24,7
	rjmp .L16
	lds r24,198
	clr r25
/* epilogue: frame size=0 */
	ret
/* epilogue end (size=1) */
/* function uart_read size 8 (7) */
	.size	uart_read, .-uart_read
.global	__vector_20
	.type	__vector_20, @function
__vector_20:
/* prologue: frame size=0 */
	push __zero_reg__
	push __tmp_reg__
	in __tmp_reg__,__SREG__
	push __tmp_reg__
	clr __zero_reg__
	push r24
/* prologue end (size=6) */
	lds r24,198
/* epilogue: frame size=0 */
	pop r24
	pop __tmp_reg__
	out __SREG__,__tmp_reg__
	pop __tmp_reg__
	pop __zero_reg__
	reti
/* epilogue end (size=6) */
/* function __vector_20 size 14 (2) */
	.size	__vector_20, .-__vector_20
.global	starttimer
	.type	starttimer, @function
starttimer:
/* prologue: frame size=0 */
/* prologue end (size=0) */
/* #APP */
	cli
/* #NOAPP */
	in r24,69-0x20
	ori r24,lo8(1)
	out 69-0x20,r24
	ldi r30,lo8(110)
	ldi r31,hi8(110)
	ld r24,Z
	ori r24,lo8(1)
	st Z,r24
/* #APP */
	sei
/* #NOAPP */
/* epilogue: frame size=0 */
	ret
/* epilogue end (size=1) */
/* function starttimer size 13 (12) */
	.size	starttimer, .-starttimer
	.data
.LC0:
	.string	"the value is: %d\n"
	.text
.global	__vector_18
	.type	__vector_18, @function
__vector_18:
/* prologue: frame size=0 */
	push __zero_reg__
	push __tmp_reg__
	in __tmp_reg__,__SREG__
	push __tmp_reg__
	clr __zero_reg__
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r30
	push r31
/* prologue end (size=17) */
/* #APP */
	mov r24, r24
	
/* #NOAPP */
	clr r25
	push r25
	push r24
	ldi r24,lo8(.LC0)
	ldi r25,hi8(.LC0)
	push r25
	push r24
	call printf
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
/* epilogue: frame size=0 */
	pop r31
	pop r30
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop __tmp_reg__
	out __SREG__,__tmp_reg__
	pop __tmp_reg__
	pop __zero_reg__
	reti
/* epilogue end (size=17) */
/* function __vector_18 size 51 (17) */
	.size	__vector_18, .-__vector_18
	.data
.LC1:
	.string	"init"
	.text
.global	main
	.type	main, @function
main:
/* prologue: frame size=0 */
/* prologue end (size=0) */
	sts 197,__zero_reg__
	ldi r24,lo8(12)
	sts 196,r24
	ldi r24,lo8(24)
	sts 193,r24
	ldi r24,lo8(uart_str)
	ldi r25,hi8(uart_str)
	sts (__iob+2)+1,r25
	sts __iob+2,r24
	ldi r24,lo8(1)
	out 61-0x20,r24
	lds r24,105
	ori r24,lo8(3)
	sts 105,r24
/* #APP */
	sei
/* #NOAPP */
	ldi r24,lo8(input_str)
	ldi r25,hi8(input_str)
	sts (__iob)+1,r25
	sts __iob,r24
	lds r24,193
	ori r24,lo8(-128)
	sts 193,r24
	ldi r24,lo8(.LC1)
	ldi r25,hi8(.LC1)
	call puts
	call starttimer
	call testasm
.L28:
	sbi 43-0x20,7
	cbi 43-0x20,7
	rjmp .L28
/* epilogue: frame size=0 */
/* epilogue: noreturn */
/* epilogue end (size=0) */
/* function main size 45 (45) */
	.size	main, .-main
	.data
	.type	input_str, @object
	.size	input_str, 14
input_str:
	.skip 3,0
	.byte	3
	.skip 4,0
	.word	0
	.word	gs(uart_read)
	.word	0
	.type	uart_str, @object
	.size	uart_str, 14
uart_str:
	.skip 3,0
	.byte	3
	.skip 4,0
	.word	gs(uart_write)
	.word	0
	.word	0
/* File "debug.c": code  200 = 0x00c8 ( 115), prologues  40, epilogues  45 */
