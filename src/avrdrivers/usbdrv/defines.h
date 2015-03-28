// AT90USB/defines.h
// Adapt the constants in this file to your project
// S. Salewski 29-JAN-2007

#ifndef _DEFINES_H_
#define _DEFINES_H_ 

//#defineUSB_DEBUG // send debug messages over serial port
//#undefUSB_DEBUG

#define NOUART // do not use usart_drv.c at all
#undef NOUART

#define F_XTAL		8000000	// XTAL frequency (Hz). Use a crystal with 2, 4, 6, 8, 12 or 16 MHz if USB is used. 
#define F_CPU		8000000UL	// mpu frequency (may be divided by prescaler, default prescaler is 8) 
#define USART_BAUD	38400UL		// baud(rate) for serial port
#define UART_BAUD 	USART_BAUD	// alias

#endif
