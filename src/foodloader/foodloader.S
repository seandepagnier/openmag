	.file	"foodloader.c"
__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__tmp_reg__ = 0
__zero_reg__ = 1
	.global __do_copy_data
	.global __do_clear_bss
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.text
.Ltext0:
	.type	uart_putc, @function
uart_putc:
.LFB12:
.LM1:
/* prologue: frame size=0 */
/* prologue end (size=0) */
.LVL0:
	mov r25,r24
.LVL1:
.L3:
.LM2:
	lds r24,200
	sbrs r24,5
	rjmp .L3
.LM3:
	sts 206,r25
/* epilogue: frame size=0 */
	ret
/* epilogue end (size=1) */
/* function uart_putc size 8 (7) */
.LFE12:
	.size	uart_putc, .-uart_putc
	.type	uart_getc, @function
uart_getc:
.LFB14:
.LM4:
/* prologue: frame size=0 */
/* prologue end (size=0) */
.L10:
.LM5:
	lds r24,200
	sbrs r24,7
	rjmp .L10
.LM6:
	lds r24,206
.LM7:
	ldi r25,lo8(0)
/* epilogue: frame size=0 */
	ret
/* epilogue end (size=1) */
/* function uart_getc size 8 (7) */
.LFE14:
	.size	uart_getc, .-uart_getc
.global	custom_bootloader_startup
	.type	custom_bootloader_startup, @function
custom_bootloader_startup:
.LFB17:
.LM8:
/* prologue: frame size=0 */
/* prologue end (size=0) */
.LBB47:
.LM9:
	ldi r30,lo8(0)
	ldi r31,hi8(0)
/* #APP */
	lpm r18, Z+
	lpm r19, Z
	
.LVL2:
/* #NOAPP */
.LBE47:
.LM10:
	sbi 42-0x20,4
.LM11:
	sbi 37-0x20,4
.LBB48:
.LBB49:
.LBB50:
.LBB51:
.LM12:
	ldi r24,lo8(26)
.LVL3:
/* #APP */
	1: dec r24
	brne 1b
/* #NOAPP */
.LBE51:
.LBE50:
.LBE49:
.LBE48:
.LM13:
	sbis 35-0x20,4
	rjmp .L23
.LM14:
	ldi r24,lo8(0)
.LVL4:
	subi r18,lo8(-1)
	sbci r19,hi8(-1)
	brne .L18
.LVL5:
.L23:
	ldi r24,lo8(1)
.LVL6:
.L18:
.LM15:
	cbi 42-0x20,4
.LM16:
	cbi 37-0x20,4
.LM17:
	tst r24
	brne .L22
.LM18:
	out 92-0x20,__zero_reg__
.LM19:
	ldi r30,lo8(0)
	ldi r31,hi8(0)
	eicall
.LVL7:
.LVL8:
.L22:
	ret
/* epilogue: frame size=0 */
/* epilogue: noreturn */
/* epilogue end (size=0) */
/* function custom_bootloader_startup size 32 (32) */
.LFE17:
	.size	custom_bootloader_startup, .-custom_bootloader_startup
	.data
.LC0:
	.string	"FDL v"
	.text
.global	main
	.type	main, @function
main:
.LFB18:
.LM20:
/* prologue: frame size=0 */
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
/* prologue end (size=12) */
.LBB83:
.LBB84:
.LBB85:
.LM21:
	ldi r30,lo8(0)
	ldi r31,hi8(0)
/* #APP */
	lpm r18, Z+
	lpm r19, Z
	
.LVL9:
/* #NOAPP */
.LBE85:
.LM22:
	sbi 42-0x20,4
.LM23:
	sbi 37-0x20,4
.LBB86:
.LBB87:
.LBB88:
.LBB89:
.LM24:
	ldi r24,lo8(26)
.LVL10:
/* #APP */
	1: dec r24
	brne 1b
/* #NOAPP */
.LBE89:
.LBE88:
.LBE87:
.LBE86:
.LM25:
	sbis 35-0x20,4
	rjmp .L107
.LM26:
	ldi r24,lo8(0)
.LVL11:
	subi r18,lo8(-1)
	sbci r19,hi8(-1)
	brne .L27
.LVL12:
.L107:
	ldi r24,lo8(1)
.LVL13:
.L27:
.LM27:
	cbi 42-0x20,4
.LM28:
	cbi 37-0x20,4
.LM29:
	tst r24
	brne .L29
.LM30:
	out 92-0x20,__zero_reg__
.LM31:
	ldi r30,lo8(0)
	ldi r31,hi8(0)
	eicall
.LVL14:
.L29:
.LBE84:
.LBE83:
.LM32:
	ldi r24,lo8(-128)
.LVL15:
	sts 102,r24
.LBB90:
.LBB91:
.LM33:
	sts 205,__zero_reg__
.LM34:
	ldi r24,lo8(12)
	sts 204,r24
.LM35:
	ldi r24,lo8(24)
	sts 201,r24
.LBE91:
.LBE90:
.LBB92:
.LM36:
	ldi r27,lo8(.LC0)
	mov r8,r27
	ldi r27,hi8(.LC0)
	mov r9,r27
.LVL16:
.L111:
.LM37:
	call uart_getc
.LVL17:
.LM38:
	cpi r24,lo8(86)
.LVL18:
	brne .+2
	rjmp .L40
	cpi r24,lo8(87)
	brsh .L48
	cpi r24,lo8(72)
	brne .+2
	rjmp .L36
	cpi r24,lo8(73)
	brsh .L49
	cpi r24,lo8(66)
	brne .+2
	rjmp .L35
	cpi r24,lo8(67)
	brsh .L50
	tst r24
	brne .+2
	rjmp .L33
	cpi r24,lo8(65)
	breq .+2
	rjmp .L32
	rjmp .L34
.L50:
	cpi r24,lo8(69)
	breq .+2
	rjmp .L32
	rjmp .L33
.L49:
	cpi r24,lo8(80)
	brne .+2
	rjmp .L109
	cpi r24,lo8(81)
	brsh .L51
	cpi r24,lo8(76)
	breq .+2
	rjmp .L32
	rjmp .L109
.L51:
	cpi r24,lo8(83)
	brne .+2
	rjmp .L38
	cpi r24,lo8(84)
	breq .+2
	rjmp .L32
	rjmp .L39
.L48:
	cpi r24,lo8(101)
	brne .+2
	rjmp .L43
	cpi r24,lo8(102)
	brsh .L52
	cpi r24,lo8(97)
	breq .L41
	cpi r24,lo8(98)
	brne .+2
	rjmp .L42
	cpi r24,lo8(88)
	breq .+2
	rjmp .L32
	rjmp .L33
.L52:
	cpi r24,lo8(112)
	brne .+2
	rjmp .L45
	cpi r24,lo8(113)
	brsh .L53
	cpi r24,lo8(103)
	breq .+2
	rjmp .L32
	rjmp .L44
.L53:
	cpi r24,lo8(115)
	brne .+2
	rjmp .L46
	cpi r24,lo8(116)
	breq .+2
	rjmp .L32
	rjmp .L47
.L41:
.LM39:
	ldi r24,lo8(89)
.LVL19:
	rjmp .L108
.LVL20:
.L34:
.LM40:
	call uart_getc
.LVL21:
	mov r16,r24
.LVL22:
	ldi r17,lo8(0)
	mov r17,r16
	clr r16
	sts (eeprom_address)+1,r17
	sts eeprom_address,r16
.LM41:
	call uart_getc
	ldi r25,lo8(0)
	or r24,r16
	or r25,r17
	sts (eeprom_address)+1,r25
	sts eeprom_address,r24
.LM42:
	lsl r24
	rol r25
	ldi r26,lo8(0)
	ldi r27,hi8(0)
	sts flash_address,r24
	sts (flash_address)+1,r25
	sts (flash_address)+2,r26
	sts (flash_address)+3,r27
.LVL23:
.L109:
.LM43:
	ldi r24,lo8(13)
.LVL24:
	rjmp .L108
.LVL25:
.L36:
.LM44:
	call uart_getc
.LVL26:
	mov r14,r24
	clr r15
	ldi r16,lo8(0)
	ldi r17,hi8(0)
.LVL27:
	movw r16,r14
	clr r15
	clr r14
	sts flash_address,r14
	sts (flash_address)+1,r15
	sts (flash_address)+2,r16
	sts (flash_address)+3,r17
.LM45:
	call uart_getc
	mov r10,r24
.LVL28:
	clr r11
	mov r11,r10
	clr r10
	clr r12
	sbrc r11,7
	com r12
	mov r13,r12
	or r10,r14
	or r11,r15
	or r12,r16
	or r13,r17
	sts flash_address,r10
	sts (flash_address)+1,r11
	sts (flash_address)+2,r12
	sts (flash_address)+3,r13
.LM46:
	call uart_getc
	ldi r25,lo8(0)
	ldi r26,lo8(0)
	ldi r27,hi8(0)
	or r10,r24
	or r11,r25
	or r12,r26
	or r13,r27
.LM47:
	sts (eeprom_address)+1,__zero_reg__
	sts eeprom_address,__zero_reg__
.LM48:
	lsl r10
	rol r11
	rol r12
	rol r13
	sts flash_address,r10
	sts (flash_address)+1,r11
	sts (flash_address)+2,r12
	sts (flash_address)+3,r13
	rjmp .L109
.LVL29:
.L43:
.LM49:
	sts flash_address,__zero_reg__
	sts (flash_address)+1,__zero_reg__
	sts (flash_address)+2,__zero_reg__
	sts (flash_address)+3,__zero_reg__
	ldi r18,lo8(0)
	ldi r19,hi8(0)
	ldi r20,hlo8(0)
	ldi r21,hhi8(0)
	rjmp .L54
.L93:
.LM50:
	in __tmp_reg__,87-0x20
	sbrc __tmp_reg__,0
	rjmp .L93
.L94:
	sbic 63-0x20,1
	rjmp .L94
	ldi r24,lo8(3)
.LVL30:
/* #APP */
	movw r30, r18
	sts  91, r20
	sts 87, r24
	spm
	
/* #NOAPP */
	subi r18,lo8(-(256))
	sbci r19,hi8(-(256))
	sbci r20,hlo8(-(256))
	sbci r21,hhi8(-(256))
.LVL31:
.L54:
.LM51:
	cpi r18,lo8(253952)
	ldi r26,hi8(253952)
	cpc r19,r26
	ldi r26,hlo8(253952)
	cpc r20,r26
	ldi r26,hhi8(253952)
	cpc r21,r26
	brlo .L93
	sts flash_address,r18
	sts (flash_address)+1,r19
	sts (flash_address)+2,r20
	sts (flash_address)+3,r21
	rjmp .L109
.L39:
.LM52:
	call uart_getc
.LVL32:
	rjmp .L109
.LVL33:
.L46:
.LM53:
	ldi r24,lo8(2)
.LVL34:
	call uart_putc
.LM54:
	ldi r24,lo8(-104)
	call uart_putc
.LM55:
	ldi r24,lo8(30)
	rjmp .L108
.LVL35:
.L47:
.LM56:
	ldi r24,lo8(2)
.LVL36:
	rjmp .L110
.LVL37:
.L38:
.LM57:
	movw r28,r8
.LVL38:
	rjmp .L59
.LVL39:
.L60:
.LBB93:
.LBB94:
.LM58:
	call uart_putc
.LM59:
	adiw r28,1
.LVL40:
.L59:
.LM60:
	ld r24,Y
.LVL41:
	tst r24
	brne .L60
.LVL42:
.L40:
.LBE94:
.LBE93:
.LM61:
	ldi r24,lo8(48)
.LVL43:
	call uart_putc
.LM62:
	ldi r24,lo8(50)
	rjmp .L108
.LVL44:
.L45:
.LM63:
	ldi r24,lo8(83)
.LVL45:
	rjmp .L108
.LVL46:
.L33:
.LM64:
	ldi r24,lo8(13)
.LVL47:
	call uart_putc
.LBB95:
.LBB96:
.LM65:
	ldi r24,lo8(1)
	out 85-0x20,r24
.LM66:
	out 85-0x20,__zero_reg__
.LM67:
	lds r30,jump_to_application
	lds r31,(jump_to_application)+1
	eicall
	rjmp .L111
.LVL48:
.L42:
.LBE96:
.LBE95:
.LM68:
	ldi r24,lo8(89)
.LVL49:
	call uart_putc
.LM69:
	ldi r24,lo8(1)
.L110:
	call uart_putc
.LM70:
	ldi r24,lo8(0)
	rjmp .L108
.LVL50:
.L35:
.LM71:
	call uart_getc
.LVL51:
	mov r16,r24
.LVL52:
	call uart_getc
	ldi r17,lo8(0)
	mov r29,r16
	clr r28
.LVL53:
	ldi r25,lo8(0)
	or r28,r24
	or r29,r25
.LM72:
	ldi r27,hi8(257)
	cpi r28,lo8(257)
	cpc r29,r27
	brlo .+2
	rjmp .L32
.LM73:
	call uart_getc
.LM74:
	cpi r24,lo8(70)
.LVL54:
	breq .+2
	rjmp .L63
.LBB97:
.LM75:
	lds r24,flash_address
	lds r25,(flash_address)+1
	lds r26,(flash_address)+2
	lds r27,(flash_address)+3
.LVL55:
	subi r24,lo8(253953)
	sbci r25,hi8(253953)
	sbci r26,hlo8(253953)
	sbci r27,hhi8(253953)
	brlo .L65
.LM76:
	ldi r24,lo8(0)
	call uart_putc
.L65:
.LM77:
	lds r12,flash_address
	lds r13,(flash_address)+1
	lds r14,(flash_address)+2
	lds r15,(flash_address)+3
.L67:
.LM78:
	in __tmp_reg__,87-0x20
	sbrc __tmp_reg__,0
	rjmp .L67
	clr r10
	clr r11
.LM79:
	lsr r29
	ror r28
.LVL56:
	rjmp .L69
.L70:
.LM80:
	call uart_getc
	mov r16,r24
	call uart_getc
.LM81:
	ldi r18,lo8(1)
	ldi r25,lo8(0)
	mov r25,r24
	clr r24
	ldi r17,lo8(0)
	or r24,r16
	or r25,r17
/* #APP */
	movw  r0, r24
	movw r30, r12
	sts 91, r14
	sts 87, r18
	spm
	clr  r1
	
.LM82:
/* #NOAPP */
	ldi r24,lo8(2)
	ldi r25,hi8(2)
	ldi r26,hlo8(2)
	ldi r27,hhi8(2)
	add r12,r24
	adc r13,r25
	adc r14,r26
	adc r15,r27
.LM83:
	sec
	adc r10,__zero_reg__
	adc r11,__zero_reg__
.L69:
	cp r10,r28
	cpc r11,r29
	brlo .L70
.L91:
.LM84:
	in __tmp_reg__,87-0x20
	sbrc __tmp_reg__,0
	rjmp .L91
.L92:
	sbic 63-0x20,1
	rjmp .L92
	ldi r18,lo8(5)
	lds r24,flash_address
	lds r25,(flash_address)+1
	lds r26,(flash_address)+2
	lds r27,(flash_address)+3
/* #APP */
	movw r30, r24
	sts 91, r26
	sts 87, r18
	spm
	
/* #NOAPP */
.L74:
.LM85:
	in __tmp_reg__,87-0x20
	sbrc __tmp_reg__,0
	rjmp .L74
.LM86:
	ldi r24,lo8(17)
/* #APP */
	sts 87, r24
	spm
	
.LM87:
/* #NOAPP */
	sts flash_address,r12
	sts (flash_address)+1,r13
	sts (flash_address)+2,r14
	sts (flash_address)+3,r15
	rjmp .L109
.LVL57:
.L63:
.LBE97:
.LM88:
	cpi r24,lo8(69)
	breq .+2
	rjmp .L32
	ldi r16,lo8(0)
	ldi r17,hi8(0)
.LVL58:
	rjmp .L78
.LVL59:
.L79:
.LBB98:
.LM89:
	call uart_getc
.LVL60:
.LM90:
	lds r26,eeprom_address
	lds r27,(eeprom_address)+1
.LVL61:
.LBB99:
.LBB100:
.LM91:
/* #APP */
	mov __tmp_reg__,r24
	call __eeprom_write_byte_1F2021
/* #NOAPP */
.LBE100:
.LBE99:
.LM92:
	lds r24,eeprom_address
	lds r25,(eeprom_address)+1
	adiw r24,1
	sts (eeprom_address)+1,r25
	sts eeprom_address,r24
.LM93:
	subi r16,lo8(-(1))
	sbci r17,hi8(-(1))
.LVL62:
.L78:
	cp r16,r28
	cpc r17,r29
	brne .L79
	rjmp .L109
.LVL63:
.L44:
.LBE98:
.LM94:
	call uart_getc
.LVL64:
	mov r16,r24
.LVL65:
	call uart_getc
	ldi r17,lo8(0)
	mov r13,r16
	clr r12
.LVL66:
	ldi r25,lo8(0)
	or r12,r24
	or r13,r25
.LM95:
	call uart_getc
.LM96:
	cpi r24,lo8(70)
.LVL67:
	brne .L81
	ldi r28,lo8(0)
	ldi r29,hi8(0)
.LVL68:
	rjmp .L83
.LVL69:
.L84:
.LBB101:
.LBB102:
.LBB103:
.LM97:
	lds r30,flash_address
	lds r31,(flash_address)+1
.LVL70:
/* #APP */
	lpm r14, Z+
	lpm r15, Z
	
.LVL71:
/* #NOAPP */
.LBE103:
	ldi r16,lo8(0)
	ldi r17,hi8(0)
.LM98:
	mov r24,r14
.LVL72:
	call uart_putc
.LVL73:
.LM99:
	mov r14,r15
	mov r15,r16
	mov r16,r17
	clr r17
.LVL74:
	mov r24,r14
	call uart_putc
.LM100:
	lds r24,flash_address
	lds r25,(flash_address)+1
	lds r26,(flash_address)+2
	lds r27,(flash_address)+3
	adiw r24,2
	adc r26,__zero_reg__
	adc r27,__zero_reg__
	sts flash_address,r24
	sts (flash_address)+1,r25
	sts (flash_address)+2,r26
	sts (flash_address)+3,r27
.LBE102:
.LM101:
	adiw r28,2
.LVL75:
.L83:
	cp r28,r12
	cpc r29,r13
	brlo .L84
	rjmp .L111
.LVL76:
.L81:
.LBE101:
.LM102:
	cpi r24,lo8(69)
	brne .L32
	ldi r28,lo8(0)
	ldi r29,hi8(0)
.LVL77:
	rjmp .L87
.LVL78:
.L88:
	lds r26,eeprom_address
	lds r27,(eeprom_address)+1
.LVL79:
.LBB104:
.LBB105:
.LM103:
/* #APP */
	call __eeprom_read_byte_1F2021
	mov r24,__tmp_reg__
.LVL80:
/* #NOAPP */
	call uart_putc
.LVL81:
.LM104:
	lds r24,eeprom_address
	lds r25,(eeprom_address)+1
	adiw r24,1
	sts (eeprom_address)+1,r25
	sts eeprom_address,r24
.LVL82:
.L87:
.LBE105:
.LM105:
	mov r24,r28
.LVL83:
	ldi r25,lo8(0)
	adiw r28,1
	cp r24,r12
	cpc r25,r13
	brlo .L88
	rjmp .L111
.LVL84:
.L32:
.LBE104:
.LM106:
	ldi r24,lo8(63)
.LVL85:
.L108:
	call uart_putc
.LVL86:
	rjmp .L111
.LBE92:
/* epilogue: frame size=0 */
/* epilogue: noreturn */
/* epilogue end (size=0) */
/* function main size 599 (587) */
.LFE18:
	.size	main, .-main
.global	jump_to_application
.global	jump_to_application
	.section .bss
	.type	jump_to_application, @object
	.size	jump_to_application, 2
jump_to_application:
	.skip 2,0
	.comm flash_address,4,1
	.comm eeprom_address,2,1
	.section	.debug_frame,"",@progbits
.Lframe0:
	.long	.LECIE0-.LSCIE0
.LSCIE0:
	.long	0xffffffff
	.byte	0x1
	.string	""
	.uleb128 0x1
	.sleb128 -1
	.byte	0x24
	.byte	0xc
	.uleb128 0x20
	.uleb128 0x0
	.p2align	2
.LECIE0:
.LSFDE0:
	.long	.LEFDE0-.LASFDE0
.LASFDE0:
	.long	.Lframe0
	.long	.LFB12
	.long	.LFE12-.LFB12
	.p2align	2
.LEFDE0:
.LSFDE2:
	.long	.LEFDE2-.LASFDE2
.LASFDE2:
	.long	.Lframe0
	.long	.LFB14
	.long	.LFE14-.LFB14
	.p2align	2
.LEFDE2:
.LSFDE4:
	.long	.LEFDE4-.LASFDE4
.LASFDE4:
	.long	.Lframe0
	.long	.LFB17
	.long	.LFE17-.LFB17
	.p2align	2
.LEFDE4:
.LSFDE6:
	.long	.LEFDE6-.LASFDE6
.LASFDE6:
	.long	.Lframe0
	.long	.LFB18
	.long	.LFE18-.LFB18
	.p2align	2
.LEFDE6:
	.text
.Letext0:
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST1:
	.long	.LVL0-.Ltext0
	.long	.LVL1-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL1-.Ltext0
	.long	.LFE12-.Ltext0
	.word	0x1
	.byte	0x69
	.long	0x0
	.long	0x0
.LLST4:
	.long	.LVL4-.Ltext0
	.long	.LVL7-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL8-.Ltext0
	.long	.LFE17-.Ltext0
	.word	0x1
	.byte	0x68
	.long	0x0
	.long	0x0
.LLST5:
	.long	.LVL2-.Ltext0
	.long	.LVL7-.Ltext0
	.word	0x6
	.byte	0x6e
	.byte	0x93
	.uleb128 0x1
	.byte	0x6f
	.byte	0x93
	.uleb128 0x1
	.long	.LVL8-.Ltext0
	.long	.LFE17-.Ltext0
	.word	0x6
	.byte	0x6e
	.byte	0x93
	.uleb128 0x1
	.byte	0x6f
	.byte	0x93
	.uleb128 0x1
	.long	0x0
	.long	0x0
.LLST6:
	.long	.LVL2-.Ltext0
	.long	.LVL7-.Ltext0
	.word	0x6
	.byte	0x62
	.byte	0x93
	.uleb128 0x1
	.byte	0x63
	.byte	0x93
	.uleb128 0x1
	.long	.LVL8-.Ltext0
	.long	.LFE17-.Ltext0
	.word	0x6
	.byte	0x62
	.byte	0x93
	.uleb128 0x1
	.byte	0x63
	.byte	0x93
	.uleb128 0x1
	.long	0x0
	.long	0x0
.LLST7:
	.long	.LVL3-.Ltext0
	.long	.LVL4-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL5-.Ltext0
	.long	.LVL6-.Ltext0
	.word	0x1
	.byte	0x68
	.long	0x0
	.long	0x0
.LLST9:
	.long	.LVL16-.Ltext0
	.long	.LVL17-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL23-.Ltext0
	.long	.LVL24-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL54-.Ltext0
	.long	.LVL55-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL57-.Ltext0
	.long	.LVL60-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL62-.Ltext0
	.long	.LVL63-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL67-.Ltext0
	.long	.LVL72-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL75-.Ltext0
	.long	.LVL78-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL82-.Ltext0
	.long	.LVL83-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL84-.Ltext0
	.long	.LVL85-.Ltext0
	.word	0x1
	.byte	0x68
	.long	0x0
	.long	0x0
.LLST10:
	.long	.LVL16-.Ltext0
	.long	.LVL23-.Ltext0
	.word	0x6
	.byte	0x5c
	.byte	0x93
	.uleb128 0x1
	.byte	0x5d
	.byte	0x93
	.uleb128 0x1
	.long	.LVL23-.Ltext0
	.long	.LVL25-.Ltext0
	.word	0x6
	.byte	0x6c
	.byte	0x93
	.uleb128 0x1
	.byte	0x6d
	.byte	0x93
	.uleb128 0x1
	.long	.LVL25-.Ltext0
	.long	.LVL53-.Ltext0
	.word	0x6
	.byte	0x5c
	.byte	0x93
	.uleb128 0x1
	.byte	0x5d
	.byte	0x93
	.uleb128 0x1
	.long	.LVL53-.Ltext0
	.long	.LVL56-.Ltext0
	.word	0x6
	.byte	0x6c
	.byte	0x93
	.uleb128 0x1
	.byte	0x6d
	.byte	0x93
	.uleb128 0x1
	.long	.LVL57-.Ltext0
	.long	.LVL63-.Ltext0
	.word	0x6
	.byte	0x6c
	.byte	0x93
	.uleb128 0x1
	.byte	0x6d
	.byte	0x93
	.uleb128 0x1
	.long	.LVL63-.Ltext0
	.long	.LVL84-.Ltext0
	.word	0x6
	.byte	0x5c
	.byte	0x93
	.uleb128 0x1
	.byte	0x5d
	.byte	0x93
	.uleb128 0x1
	.long	.LVL84-.Ltext0
	.long	.LFE18-.Ltext0
	.word	0x6
	.byte	0x6c
	.byte	0x93
	.uleb128 0x1
	.byte	0x6d
	.byte	0x93
	.uleb128 0x1
	.long	0x0
	.long	0x0
.LLST11:
	.long	.LVL11-.Ltext0
	.long	.LVL14-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL14-.Ltext0
	.long	.LVL15-.Ltext0
	.word	0x1
	.byte	0x68
	.long	0x0
	.long	0x0
.LLST12:
	.long	.LVL9-.Ltext0
	.long	.LVL14-.Ltext0
	.word	0x6
	.byte	0x6e
	.byte	0x93
	.uleb128 0x1
	.byte	0x6f
	.byte	0x93
	.uleb128 0x1
	.long	.LVL14-.Ltext0
	.long	.LVL17-.Ltext0
	.word	0x6
	.byte	0x6e
	.byte	0x93
	.uleb128 0x1
	.byte	0x6f
	.byte	0x93
	.uleb128 0x1
	.long	0x0
	.long	0x0
.LLST13:
	.long	.LVL9-.Ltext0
	.long	.LVL14-.Ltext0
	.word	0x6
	.byte	0x62
	.byte	0x93
	.uleb128 0x1
	.byte	0x63
	.byte	0x93
	.uleb128 0x1
	.long	.LVL14-.Ltext0
	.long	.LVL17-.Ltext0
	.word	0x6
	.byte	0x62
	.byte	0x93
	.uleb128 0x1
	.byte	0x63
	.byte	0x93
	.uleb128 0x1
	.long	0x0
	.long	0x0
.LLST14:
	.long	.LVL10-.Ltext0
	.long	.LVL11-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL12-.Ltext0
	.long	.LVL13-.Ltext0
	.word	0x1
	.byte	0x68
	.long	0x0
	.long	0x0
.LLST15:
	.long	.LVL18-.Ltext0
	.long	.LVL19-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL20-.Ltext0
	.long	.LVL21-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL23-.Ltext0
	.long	.LVL24-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL25-.Ltext0
	.long	.LVL26-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL29-.Ltext0
	.long	.LVL30-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL31-.Ltext0
	.long	.LVL32-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL33-.Ltext0
	.long	.LVL34-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL35-.Ltext0
	.long	.LVL36-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL37-.Ltext0
	.long	.LVL39-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL40-.Ltext0
	.long	.LVL41-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL42-.Ltext0
	.long	.LVL43-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL44-.Ltext0
	.long	.LVL45-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL46-.Ltext0
	.long	.LVL47-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL48-.Ltext0
	.long	.LVL49-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL50-.Ltext0
	.long	.LVL51-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL63-.Ltext0
	.long	.LVL64-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL84-.Ltext0
	.long	.LVL85-.Ltext0
	.word	0x1
	.byte	0x68
	.long	0x0
	.long	0x0
.LLST16:
	.long	.LVL16-.Ltext0
	.long	.LVL53-.Ltext0
	.word	0x6
	.byte	0x6c
	.byte	0x93
	.uleb128 0x1
	.byte	0x6d
	.byte	0x93
	.uleb128 0x1
	.long	.LVL63-.Ltext0
	.long	.LVL68-.Ltext0
	.word	0x6
	.byte	0x6c
	.byte	0x93
	.uleb128 0x1
	.byte	0x6d
	.byte	0x93
	.uleb128 0x1
	.long	.LVL76-.Ltext0
	.long	.LVL77-.Ltext0
	.word	0x6
	.byte	0x6c
	.byte	0x93
	.uleb128 0x1
	.byte	0x6d
	.byte	0x93
	.uleb128 0x1
	.long	.LVL84-.Ltext0
	.long	.LFE18-.Ltext0
	.word	0x6
	.byte	0x6c
	.byte	0x93
	.uleb128 0x1
	.byte	0x6d
	.byte	0x93
	.uleb128 0x1
	.long	0x0
	.long	0x0
.LLST17:
	.long	.LVL16-.Ltext0
	.long	.LVL28-.Ltext0
	.word	0x6
	.byte	0x5a
	.byte	0x93
	.uleb128 0x1
	.byte	0x5b
	.byte	0x93
	.uleb128 0x1
	.long	.LVL29-.Ltext0
	.long	.LFE18-.Ltext0
	.word	0x6
	.byte	0x5a
	.byte	0x93
	.uleb128 0x1
	.byte	0x5b
	.byte	0x93
	.uleb128 0x1
	.long	0x0
	.long	0x0
.LLST18:
	.long	.LVL16-.Ltext0
	.long	.LVL66-.Ltext0
	.word	0xc
	.byte	0x5c
	.byte	0x93
	.uleb128 0x1
	.byte	0x5d
	.byte	0x93
	.uleb128 0x1
	.byte	0x5e
	.byte	0x93
	.uleb128 0x1
	.byte	0x5f
	.byte	0x93
	.uleb128 0x1
	.long	.LVL84-.Ltext0
	.long	.LFE18-.Ltext0
	.word	0xc
	.byte	0x5c
	.byte	0x93
	.uleb128 0x1
	.byte	0x5d
	.byte	0x93
	.uleb128 0x1
	.byte	0x5e
	.byte	0x93
	.uleb128 0x1
	.byte	0x5f
	.byte	0x93
	.uleb128 0x1
	.long	0x0
	.long	0x0
.LLST19:
	.long	.LVL16-.Ltext0
	.long	.LVL22-.Ltext0
	.word	0x6
	.byte	0x60
	.byte	0x93
	.uleb128 0x1
	.byte	0x61
	.byte	0x93
	.uleb128 0x1
	.long	.LVL23-.Ltext0
	.long	.LVL27-.Ltext0
	.word	0x6
	.byte	0x60
	.byte	0x93
	.uleb128 0x1
	.byte	0x61
	.byte	0x93
	.uleb128 0x1
	.long	.LVL29-.Ltext0
	.long	.LVL52-.Ltext0
	.word	0x6
	.byte	0x60
	.byte	0x93
	.uleb128 0x1
	.byte	0x61
	.byte	0x93
	.uleb128 0x1
	.long	.LVL58-.Ltext0
	.long	.LVL65-.Ltext0
	.word	0x6
	.byte	0x60
	.byte	0x93
	.uleb128 0x1
	.byte	0x61
	.byte	0x93
	.uleb128 0x1
	.long	.LVL84-.Ltext0
	.long	.LFE18-.Ltext0
	.word	0x6
	.byte	0x60
	.byte	0x93
	.uleb128 0x1
	.byte	0x61
	.byte	0x93
	.uleb128 0x1
	.long	0x0
	.long	0x0
.LLST20:
	.long	.LVL23-.Ltext0
	.long	.LVL25-.Ltext0
	.word	0x6
	.byte	0x6a
	.byte	0x93
	.uleb128 0x1
	.byte	0x6b
	.byte	0x93
	.uleb128 0x1
	.long	.LVL59-.Ltext0
	.long	.LVL60-.Ltext0
	.word	0x6
	.byte	0x6a
	.byte	0x93
	.uleb128 0x1
	.byte	0x6b
	.byte	0x93
	.uleb128 0x1
	.long	.LVL61-.Ltext0
	.long	.LVL63-.Ltext0
	.word	0x6
	.byte	0x6a
	.byte	0x93
	.uleb128 0x1
	.byte	0x6b
	.byte	0x93
	.uleb128 0x1
	.long	.LVL85-.Ltext0
	.long	.LVL86-.Ltext0
	.word	0x6
	.byte	0x6a
	.byte	0x93
	.uleb128 0x1
	.byte	0x6b
	.byte	0x93
	.uleb128 0x1
	.long	0x0
	.long	0x0
.LLST21:
	.long	.LVL16-.Ltext0
	.long	.LVL38-.Ltext0
	.word	0x6
	.byte	0x6c
	.byte	0x93
	.uleb128 0x1
	.byte	0x6d
	.byte	0x93
	.uleb128 0x1
	.long	.LVL42-.Ltext0
	.long	.LVL53-.Ltext0
	.word	0x6
	.byte	0x6c
	.byte	0x93
	.uleb128 0x1
	.byte	0x6d
	.byte	0x93
	.uleb128 0x1
	.long	.LVL63-.Ltext0
	.long	.LVL77-.Ltext0
	.word	0x6
	.byte	0x6c
	.byte	0x93
	.uleb128 0x1
	.byte	0x6d
	.byte	0x93
	.uleb128 0x1
	.long	.LVL84-.Ltext0
	.long	.LFE18-.Ltext0
	.word	0x6
	.byte	0x6c
	.byte	0x93
	.uleb128 0x1
	.byte	0x6d
	.byte	0x93
	.uleb128 0x1
	.long	0x0
	.long	0x0
.LLST22:
	.long	.LVL70-.Ltext0
	.long	.LVL73-.Ltext0
	.word	0x6
	.byte	0x6e
	.byte	0x93
	.uleb128 0x1
	.byte	0x6f
	.byte	0x93
	.uleb128 0x1
	.long	0x0
	.long	0x0
.LLST23:
	.long	.LVL71-.Ltext0
	.long	.LVL74-.Ltext0
	.word	0x6
	.byte	0x5e
	.byte	0x93
	.uleb128 0x1
	.byte	0x5f
	.byte	0x93
	.uleb128 0x1
	.long	0x0
	.long	0x0
	.section	.debug_info
	.long	0x55b
	.word	0x2
	.long	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.long	.LASF41
	.byte	0x1
	.long	.LASF42
	.long	.LASF43
	.long	.Ltext0
	.long	.Letext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF0
	.uleb128 0x3
	.long	.LASF2
	.byte	0x4
	.byte	0x7a
	.long	0x37
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF1
	.uleb128 0x4
	.byte	0x2
	.byte	0x5
	.string	"int"
	.uleb128 0x3
	.long	.LASF3
	.byte	0x4
	.byte	0x7c
	.long	0x50
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF4
	.uleb128 0x2
	.byte	0x4
	.byte	0x5
	.long	.LASF5
	.uleb128 0x3
	.long	.LASF6
	.byte	0x4
	.byte	0x7e
	.long	0x69
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF7
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF8
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF9
	.uleb128 0x5
	.byte	0x1
	.byte	0x6
	.uleb128 0x5
	.byte	0x1
	.byte	0x8
	.uleb128 0x6
	.long	.LASF10
	.byte	0x2
	.byte	0x68
	.byte	0x1
	.byte	0x3
	.long	0x9d
	.uleb128 0x7
	.long	.LASF12
	.byte	0x2
	.byte	0x67
	.long	0x45
	.byte	0x0
	.uleb128 0x6
	.long	.LASF11
	.byte	0x5
	.byte	0x8f
	.byte	0x1
	.byte	0x3
	.long	0xce
	.uleb128 0x7
	.long	.LASF13
	.byte	0x5
	.byte	0x8e
	.long	0xce
	.uleb128 0x8
	.long	.LASF14
	.byte	0x5
	.byte	0x90
	.long	0x45
	.uleb128 0x8
	.long	.LASF15
	.byte	0x5
	.byte	0x91
	.long	0xce
	.uleb128 0x9
	.uleb128 0x9
	.byte	0x0
	.uleb128 0x2
	.byte	0x4
	.byte	0x4
	.long	.LASF16
	.uleb128 0x6
	.long	.LASF17
	.byte	0x2
	.byte	0x52
	.byte	0x1
	.byte	0x3
	.long	0xee
	.uleb128 0x7
	.long	.LASF12
	.byte	0x2
	.byte	0x51
	.long	0x2c
	.byte	0x0
	.uleb128 0x6
	.long	.LASF18
	.byte	0x5
	.byte	0x6d
	.byte	0x1
	.byte	0x3
	.long	0x132
	.uleb128 0x7
	.long	.LASF19
	.byte	0x5
	.byte	0x6c
	.long	0xce
	.uleb128 0x8
	.long	.LASF14
	.byte	0x5
	.byte	0x6e
	.long	0x2c
	.uleb128 0x8
	.long	.LASF15
	.byte	0x5
	.byte	0x6f
	.long	0xce
	.uleb128 0xa
	.long	0x130
	.uleb128 0xb
	.uleb128 0xc
	.long	0xb5
	.uleb128 0xc
	.long	0xc0
	.uleb128 0x9
	.uleb128 0x9
	.byte	0x0
	.byte	0x0
	.uleb128 0x9
	.byte	0x0
	.uleb128 0xd
	.long	.LASF44
	.byte	0x1
	.byte	0x37
	.byte	0x1
	.long	.LFB12
	.long	.LFE12
	.byte	0x2
	.byte	0x90
	.uleb128 0x20
	.long	0x159
	.uleb128 0xe
	.long	.LASF45
	.byte	0x1
	.byte	0x36
	.long	0x2c
	.long	.LLST1
	.byte	0x0
	.uleb128 0xf
	.long	.LASF46
	.byte	0x1
	.byte	0x4f
	.byte	0x1
	.long	0x2c
	.long	.LFB14
	.long	.LFE14
	.byte	0x2
	.byte	0x90
	.uleb128 0x20
	.uleb128 0x10
	.byte	0x1
	.long	.LASF47
	.byte	0x1
	.byte	0x7b
	.byte	0x1
	.byte	0x1
	.long	0x1e9
	.uleb128 0x8
	.long	.LASF20
	.byte	0x1
	.byte	0x7c
	.long	0x2c
	.uleb128 0xa
	.long	0x1a5
	.uleb128 0x8
	.long	.LASF21
	.byte	0x1
	.byte	0x7f
	.long	0x45
	.uleb128 0x8
	.long	.LASF22
	.byte	0x1
	.byte	0x7f
	.long	0x45
	.byte	0x0
	.uleb128 0xb
	.uleb128 0x11
	.long	0xfb
	.uleb128 0xb
	.uleb128 0xc
	.long	0x106
	.uleb128 0xc
	.long	0x111
	.uleb128 0xa
	.long	0x1df
	.uleb128 0x11
	.long	0xaa
	.uleb128 0xb
	.uleb128 0xc
	.long	0xb5
	.uleb128 0xc
	.long	0xc0
	.uleb128 0xa
	.long	0x1d6
	.uleb128 0x11
	.long	0x91
	.byte	0x0
	.uleb128 0xb
	.uleb128 0x11
	.long	0x91
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0xb
	.uleb128 0x11
	.long	0xe2
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0x12
	.long	0x170
	.long	.LFB17
	.long	.LFE17
	.byte	0x2
	.byte	0x90
	.uleb128 0x20
	.long	0x269
	.uleb128 0x13
	.long	0x17e
	.long	.LLST4
	.uleb128 0x14
	.long	.LBB47
	.long	.LBE47
	.long	0x226
	.uleb128 0x13
	.long	0x18e
	.long	.LLST5
	.uleb128 0x13
	.long	0x199
	.long	.LLST6
	.byte	0x0
	.uleb128 0x15
	.long	0xee
	.long	.LBB48
	.long	.LBE48
	.byte	0x1
	.byte	0x92
	.uleb128 0x11
	.long	0xfb
	.uleb128 0x16
	.long	.LBB49
	.long	.LBE49
	.uleb128 0xc
	.long	0x106
	.uleb128 0xc
	.long	0x111
	.uleb128 0x15
	.long	0xd5
	.long	.LBB50
	.long	.LBE50
	.byte	0x5
	.byte	0x79
	.uleb128 0x17
	.long	0xe2
	.long	.LLST7
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0x18
	.long	.LASF25
	.byte	0x1
	.byte	0x5b
	.byte	0x1
	.byte	0x3
	.uleb128 0x6
	.long	.LASF23
	.byte	0x1
	.byte	0x43
	.byte	0x1
	.byte	0x3
	.long	0x28b
	.uleb128 0x7
	.long	.LASF24
	.byte	0x1
	.byte	0x42
	.long	0x28b
	.byte	0x0
	.uleb128 0x19
	.byte	0x2
	.long	0x2c
	.uleb128 0x18
	.long	.LASF26
	.byte	0x1
	.byte	0x6b
	.byte	0x1
	.byte	0x3
	.uleb128 0x1a
	.long	.LASF27
	.byte	0x3
	.word	0x11e
	.byte	0x1
	.byte	0x3
	.long	0x2c1
	.uleb128 0x1b
	.long	.LASF28
	.byte	0x3
	.word	0x11d
	.long	0x28b
	.uleb128 0x1b
	.long	.LASF29
	.byte	0x3
	.word	0x11d
	.long	0x2c
	.byte	0x0
	.uleb128 0x1c
	.long	.LASF48
	.byte	0x3
	.byte	0xbf
	.byte	0x1
	.long	0x2c
	.byte	0x3
	.long	0x2e9
	.uleb128 0x7
	.long	.LASF28
	.byte	0x3
	.byte	0xbe
	.long	0x2e9
	.uleb128 0x8
	.long	.LASF30
	.byte	0x3
	.byte	0xc0
	.long	0x2c
	.byte	0x0
	.uleb128 0x19
	.byte	0x2
	.long	0x2ef
	.uleb128 0x1d
	.long	0x2c
	.uleb128 0x1e
	.byte	0x1
	.long	.LASF49
	.byte	0x1
	.byte	0xa3
	.byte	0x1
	.long	0x3e
	.long	.LFB18
	.long	.LFE18
	.byte	0x2
	.byte	0x90
	.uleb128 0x20
	.long	0x520
	.uleb128 0x1f
	.long	.LASF31
	.byte	0x1
	.byte	0xab
	.long	0x2c
	.long	.LLST9
	.uleb128 0x1f
	.long	.LASF32
	.byte	0x1
	.byte	0xae
	.long	0x45
	.long	.LLST10
	.uleb128 0x20
	.long	0x170
	.long	.LBB83
	.long	.LBE83
	.byte	0x1
	.byte	0xa6
	.long	0x3b7
	.uleb128 0x16
	.long	.LBB84
	.long	.LBE84
	.uleb128 0x13
	.long	0x17e
	.long	.LLST11
	.uleb128 0x14
	.long	.LBB85
	.long	.LBE85
	.long	0x373
	.uleb128 0x13
	.long	0x18e
	.long	.LLST12
	.uleb128 0x13
	.long	0x199
	.long	.LLST13
	.byte	0x0
	.uleb128 0x15
	.long	0xee
	.long	.LBB86
	.long	.LBE86
	.byte	0x1
	.byte	0x92
	.uleb128 0x11
	.long	0xfb
	.uleb128 0x16
	.long	.LBB87
	.long	.LBE87
	.uleb128 0xc
	.long	0x106
	.uleb128 0xc
	.long	0x111
	.uleb128 0x15
	.long	0xd5
	.long	.LBB88
	.long	.LBE88
	.byte	0x5
	.byte	0x79
	.uleb128 0x17
	.long	0xe2
	.long	.LLST14
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0x21
	.long	0x269
	.long	.LBB90
	.long	.LBE90
	.byte	0x1
	.byte	0xb0
	.uleb128 0x16
	.long	.LBB92
	.long	.LBE92
	.uleb128 0x1f
	.long	.LASF33
	.byte	0x1
	.byte	0xba
	.long	0x2c
	.long	.LLST15
	.uleb128 0x22
	.long	0x272
	.long	.LBB93
	.long	.LBE93
	.byte	0x1
	.word	0x115
	.long	0x3fc
	.uleb128 0x17
	.long	0x27f
	.long	.LLST16
	.byte	0x0
	.uleb128 0x23
	.long	0x291
	.long	.LBB95
	.long	.LBE95
	.byte	0x1
	.word	0x126
	.uleb128 0x14
	.long	.LBB97
	.long	.LBE97
	.long	0x444
	.uleb128 0x24
	.string	"i"
	.byte	0x1
	.word	0x143
	.long	0x45
	.long	.LLST17
	.uleb128 0x25
	.long	.LASF34
	.byte	0x1
	.word	0x144
	.long	0x45
	.uleb128 0x26
	.long	.LASF35
	.byte	0x1
	.word	0x14a
	.long	0x5e
	.long	.LLST18
	.byte	0x0
	.uleb128 0x14
	.long	.LBB98
	.long	.LBE98
	.long	0x48b
	.uleb128 0x25
	.long	.LASF36
	.byte	0x1
	.word	0x16c
	.long	0x2c
	.uleb128 0x24
	.string	"i"
	.byte	0x1
	.word	0x16d
	.long	0x45
	.long	.LLST19
	.uleb128 0x27
	.long	0x29a
	.long	.LBB99
	.long	.LBE99
	.byte	0x1
	.word	0x171
	.uleb128 0x11
	.long	0x2b4
	.uleb128 0x17
	.long	0x2a8
	.long	.LLST20
	.byte	0x0
	.byte	0x0
	.uleb128 0x14
	.long	.LBB101
	.long	.LBE101
	.long	0x4f4
	.uleb128 0x24
	.string	"i"
	.byte	0x1
	.word	0x18e
	.long	0x45
	.long	.LLST21
	.uleb128 0x16
	.long	.LBB102
	.long	.LBE102
	.uleb128 0x28
	.long	.LASF34
	.byte	0x1
	.word	0x18f
	.long	0x5e
	.byte	0xc
	.byte	0x5e
	.byte	0x93
	.uleb128 0x1
	.byte	0x5f
	.byte	0x93
	.uleb128 0x1
	.byte	0x60
	.byte	0x93
	.uleb128 0x1
	.byte	0x61
	.byte	0x93
	.uleb128 0x1
	.uleb128 0x16
	.long	.LBB103
	.long	.LBE103
	.uleb128 0x26
	.long	.LASF21
	.byte	0x1
	.word	0x192
	.long	0x45
	.long	.LLST22
	.uleb128 0x26
	.long	.LASF22
	.byte	0x1
	.word	0x192
	.long	0x45
	.long	.LLST23
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0x16
	.long	.LBB104
	.long	.LBE104
	.uleb128 0x29
	.string	"i"
	.byte	0x1
	.word	0x1a1
	.long	0x2c
	.uleb128 0x16
	.long	.LBB105
	.long	.LBE105
	.uleb128 0x25
	.long	.LASF37
	.byte	0x1
	.word	0x1a2
	.long	0x2c
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0x2a
	.long	.LASF38
	.byte	0x1
	.byte	0x28
	.long	0x5e
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	flash_address
	.uleb128 0x2a
	.long	.LASF39
	.byte	0x1
	.byte	0x29
	.long	0x45
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	eeprom_address
	.uleb128 0x2b
	.byte	0x1
	.uleb128 0x2a
	.long	.LASF40
	.byte	0x1
	.byte	0x2c
	.long	0x558
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	jump_to_application
	.uleb128 0x19
	.byte	0x2
	.long	0x544
	.byte	0x0
	.section	.debug_abbrev
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0x0
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x16
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0x0
	.byte	0x0
	.uleb128 0x5
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x6
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x7
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x8
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x9
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0xa
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0xc
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xd
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xe
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0xf
	.uleb128 0x2e
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x10
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x11
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x12
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x13
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x14
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x15
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x16
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x17
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x18
	.uleb128 0x2e
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x20
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x19
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1a
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1b
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1c
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1d
	.uleb128 0x26
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1e
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1f
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x20
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x21
	.uleb128 0x1d
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x22
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0x5
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x23
	.uleb128 0x1d
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0x5
	.byte	0x0
	.byte	0x0
	.uleb128 0x24
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x25
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x26
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x27
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0x5
	.byte	0x0
	.byte	0x0
	.uleb128 0x28
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x29
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x2a
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x2b
	.uleb128 0x15
	.byte	0x0
	.uleb128 0x27
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.section	.debug_pubnames,"",@progbits
	.long	0x72
	.word	0x2
	.long	.Ldebug_info0
	.long	0x55f
	.long	0x1e9
	.string	"custom_bootloader_startup"
	.long	0x2f4
	.string	"main"
	.long	0x520
	.string	"flash_address"
	.long	0x532
	.string	"eeprom_address"
	.long	0x546
	.string	"jump_to_application"
	.long	0x0
	.section	.debug_aranges,"",@progbits
	.long	0x1c
	.word	0x2
	.long	.Ldebug_info0
	.byte	0x4
	.byte	0x0
	.word	0x0
	.word	0x0
	.long	.Ltext0
	.long	.Letext0-.Ltext0
	.long	0x0
	.long	0x0
	.section	.debug_line
	.long	.LELT0-.LSLT0
.LSLT0:
	.word	0x2
	.long	.LELTP0-.LASLTP0
.LASLTP0:
	.byte	0x1
	.byte	0x1
	.byte	0xf6
	.byte	0xf5
	.byte	0xa
	.byte	0x0
	.byte	0x1
	.byte	0x1
	.byte	0x1
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.byte	0x1
	.ascii	"/usr/local/lib/gcc/avr/4.2.2/../../../../avr/include"
	.byte	0
	.ascii	"/usr/local/lib/gcc/avr/4.2.2/../../../../avr/include/avr"
	.byte	0
	.ascii	"/usr/local/lib/gcc/avr/4.2.2/../../../../avr/include/util"
	.byte	0
	.byte	0x0
	.string	"foodloader.c"
	.uleb128 0x0
	.uleb128 0x0
	.uleb128 0x0
	.string	"util/delay_basic.h"
	.uleb128 0x1
	.uleb128 0x0
	.uleb128 0x0
	.string	"avr/eeprom.h"
	.uleb128 0x1
	.uleb128 0x0
	.uleb128 0x0
	.string	"stdint.h"
	.uleb128 0x1
	.uleb128 0x0
	.uleb128 0x0
	.string	"util/delay.h"
	.uleb128 0x1
	.uleb128 0x0
	.uleb128 0x0
	.byte	0x0
.LELTP0:
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM1
	.byte	0x4a
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM2
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM3
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM4
	.byte	0x26
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM5
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM6
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM7
	.byte	0x16
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM8
	.byte	0x38
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM9
	.byte	0x18
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM10
	.byte	0x24
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM11
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM12
	.byte	0x4
	.uleb128 0x2
	.byte	0x3
	.sleb128 -61
	.byte	0x1
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM13
	.byte	0x4
	.uleb128 0x1
	.byte	0x54
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM14
	.byte	0x1
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM15
	.byte	0x18
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM16
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM17
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM18
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM19
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM20
	.byte	0x1a
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM21
	.byte	0x3
	.sleb128 -36
	.byte	0x1
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM22
	.byte	0x24
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM23
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM24
	.byte	0x4
	.uleb128 0x2
	.byte	0x3
	.sleb128 -61
	.byte	0x1
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM25
	.byte	0x4
	.uleb128 0x1
	.byte	0x54
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM26
	.byte	0x1
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM27
	.byte	0x18
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM28
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM29
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM30
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM31
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM32
	.byte	0x20
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM33
	.byte	0x3
	.sleb128 -75
	.byte	0x1
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM34
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM35
	.byte	0x1a
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM36
	.byte	0xc4
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM37
	.byte	0x3
	.sleb128 -88
	.byte	0x1
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM38
	.byte	0x16
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM39
	.byte	0x1f
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM40
	.byte	0x1b
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM41
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM42
	.byte	0x18
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM43
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM44
	.byte	0x1c
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM45
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM46
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM47
	.byte	0x16
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM48
	.byte	0x18
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM49
	.byte	0x21
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM50
	.byte	0x16
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM51
	.byte	0x12
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM52
	.byte	0x23
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM53
	.byte	0x19
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM54
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM55
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM56
	.byte	0x18
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM57
	.byte	0x19
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM58
	.byte	0x3
	.sleb128 -206
	.byte	0x1
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM59
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM60
	.byte	0x12
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM61
	.byte	0xe6
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM62
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM63
	.byte	0x18
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM64
	.byte	0x1c
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM65
	.byte	0x3
	.sleb128 -184
	.byte	0x1
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM66
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM67
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM68
	.byte	0xd0
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM69
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM70
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM71
	.byte	0x1b
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM72
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM73
	.byte	0x1a
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM74
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM75
	.byte	0x1a
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM76
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM77
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM78
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM79
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM80
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM81
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM82
	.byte	0x18
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM83
	.byte	0xa
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM84
	.byte	0x22
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM85
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM86
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM87
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM88
	.byte	0x19
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM89
	.byte	0x1c
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM90
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM91
	.byte	0x4
	.uleb128 0x3
	.byte	0x3
	.sleb128 -82
	.byte	0x1
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM92
	.byte	0x4
	.uleb128 0x1
	.byte	0x68
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM93
	.byte	0x10
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM94
	.byte	0x29
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM95
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM96
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM97
	.byte	0x1c
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM98
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM99
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM100
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM101
	.byte	0x3
	.sleb128 -11
	.byte	0x1
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM102
	.byte	0x24
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM103
	.byte	0x1c
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM104
	.byte	0x16
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM105
	.byte	0xd
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.LM106
	.byte	0x34
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.long	.Letext0
	.byte	0x0
	.uleb128 0x1
	.byte	0x1
.LELT0:
	.section	.debug_str,"MS",@progbits,1
.LASF48:
	.string	"eeprom_read_byte"
.LASF41:
	.string	"GNU C 4.2.2"
.LASF43:
	.string	"/home/sean/openmag/src/old/foodloader"
.LASF21:
	.string	"__addr16"
.LASF32:
	.string	"buffer_size"
.LASF29:
	.string	"value"
.LASF35:
	.string	"temp_address"
.LASF33:
	.string	"command"
.LASF19:
	.string	"__us"
.LASF25:
	.string	"init_uart"
.LASF28:
	.string	"addr"
.LASF34:
	.string	"temp_word_buffer"
.LASF45:
	.string	"data"
.LASF1:
	.string	"unsigned char"
.LASF47:
	.string	"custom_bootloader_startup"
.LASF39:
	.string	"eeprom_address"
.LASF17:
	.string	"_delay_loop_1"
.LASF10:
	.string	"_delay_loop_2"
.LASF7:
	.string	"long unsigned int"
.LASF11:
	.string	"_delay_ms"
.LASF42:
	.string	"foodloader.c"
.LASF46:
	.string	"uart_getc"
.LASF22:
	.string	"__result"
.LASF38:
	.string	"flash_address"
.LASF16:
	.string	"double"
.LASF15:
	.string	"__tmp"
.LASF49:
	.string	"main"
.LASF27:
	.string	"eeprom_write_byte"
.LASF4:
	.string	"unsigned int"
.LASF40:
	.string	"jump_to_application"
.LASF18:
	.string	"_delay_us"
.LASF9:
	.string	"long long unsigned int"
.LASF2:
	.string	"uint8_t"
.LASF37:
	.string	"temp_buffer"
.LASF30:
	.string	"result"
.LASF36:
	.string	"temp_data"
.LASF8:
	.string	"long long int"
.LASF31:
	.string	"memory_type"
.LASF20:
	.string	"stay_in_bootloader"
.LASF12:
	.string	"__count"
.LASF24:
	.string	"buffer"
.LASF3:
	.string	"uint16_t"
.LASF14:
	.string	"__ticks"
.LASF44:
	.string	"uart_putc"
.LASF6:
	.string	"uint32_t"
.LASF5:
	.string	"long int"
.LASF0:
	.string	"signed char"
.LASF13:
	.string	"__ms"
.LASF23:
	.string	"uart_puts"
.LASF26:
	.string	"start_application"
/* File "foodloader.c": code  647 = 0x0287 ( 633), prologues  12, epilogues   2 */
