/**************************** uart0_int.c *******************************/
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
/*  Uart0 device.  Runs in interrupt mode.	  			*/
/* Provides hooks needed to work as a device driver. Relatively basic 	*/
/* support.  There are no provisions for turning off the interrupts on 	*/
/* device close or for error response.  Also the interrupt and baud 	*/
/* rate setup must be done through an ioctl call, there are no default	*/
/* values.		 						*/
/*  Note:  Most of the actual routines are private to this module. The	*/
/* only elements publically visible are the device table entry 		*/
/* structure and the interrupt support routine.				*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 uart0_int.c 14-Jul-2004,10:12:40,`RADSETT' Original archived version.
*   2 uart0_int.c 06-Nov-2004,16:43:44,`RADSETT' Remove pin toggle debug lines.
*        Thanks to Cyril Holweck for
*        pointing out theywer still present
*   3 uart0_int.c 18-Feb-2005,15:40:44,`RADSETT' Add device specific information
*        to device calls.
*   4 uart0_int.c 12-Mar-2006,12:03:08,`RADSETT' Bug Fixes from Jacob Oursland.
*        Thanks.
*        Correct volatile modifiers on pointers tracking position
*        in send/receive buffers.
*        Enable FIFO.  Appears to correct previously mentioned interrupt
*        race condition.
*   TLIB revision history ends.
*/
#include <stdio.h>
#include <errno.h>
#include "lpc210x.h"
#include "dev_cntrl.h"
#include "lpc_ioctl.h"
#include "errno_lpc.h"

	/**** Local Prototypes ****/
static _ssize_t uart0_read (struct _reent *r, int file, void *ptr, size_t len, SUB_DEVICE_INFO *info);
static _ssize_t uart0_write (struct _reent *r, int file, const void *ptr, size_t len, SUB_DEVICE_INFO *info);
static int uart0_open(struct _reent *r, const char *name, int flags, int mode, SUB_DEVICE_INFO *info);
static int uart0_close(struct _reent *r, int file, SUB_DEVICE_INFO *info);
static int uart0_ioctl(struct _reent *r, int file, unsigned long request, void *ptr, SUB_DEVICE_INFO *info);
static int uart0_setup( struct _reent *r, struct serial_param *p);
static int uart0_interrupt_setup( struct _reent *r, const struct interrupt_param *p);
static void receive( void);
static void transmit( void);
void Uart0Service(void);	/*  Private to this driver and the 	*/
				/* interrupt shell but needs public 	*/
				/* linkage so that the interrupt shell 	*/
				/* can be linked to it.			*/
void Uart0InterruptShell(void);	/*  Private to this driver and the 	*/
				/* interrupt shell but needs public 	*/
				/* linkage so that this module can get 	*/
				/* its address for the VIC.	 	*/

	/**** Local Macros ****/

	/*  Sets bit to enable the divisor latch.			*/
#define UART_DLAB_EN		((unsigned char)0x80u)	

	/*  Masks for the line status register (U0LSR) flags.		*/
#define RDR_MASK	(0x1u)		/*  Received character waiting.	*/

	/*  Flags to enable interrupt sources (U0IER).			*/
#define RDA_INT_ENABLE	(0x1u)		/*  Receive Data Available	*/
#define THRE_INT_ENABLE	(0x2u)		/*  Transmit Holding Register	*/
					/* Empty			*/

	/*  Masks for the interrupt identification register (U0IIR)	*/
	/* fields.							*/
#define IDENT_MASK	(0xEu)	/*  Separates out the bit to identify 	*/
				/* the pending interrupt source.	*/

#define TRANSMIT_AVAILABLE	(0x2u)	/*  Room in TX FIFO.		*/
#define DATA_AVAILABLE		(0x4u)	/*  Data available or FIFO 	*/
					/* trigger level reached.	*/
#define LINE_STATUS		(0x6u)	/*  Line status error.	 	*/
#define DATA_TIMEOUT		(0xCu)	/*  Data in FIFO but no 	*/
					/* character added or removed	*/
					/* for 3.5 - 4.5 character	*/
					/* times.			*/

	/*  Sizes of the send and receive buffers.			*/
#define RECEIVE_BUFFER_SIZE	(32)
#define SEND_BUFFER_SIZE	(32)

	/*  Endpoints of the send and receive buffers.  Note that the 	*/
	/* end endpoint is one element past the last.  No pointer 	*/
	/* should ever be left with these values, they are used to 	*/
	/* detect the wrap point.					*/
#define RECEIVE_BUFFER_START	(&receive_buffer[0])
#define RECEIVE_BUFFER_END	(&receive_buffer[RECEIVE_BUFFER_SIZE])
#define SEND_BUFFER_START	(&send_buffer[0])
#define SEND_BUFFER_END		(&send_buffer[SEND_BUFFER_SIZE])

	/**** Local Variables ****/

	/*  The receive buffer.  Includes buffer and two pointers used	*/
	/* to track the first and last characters in the buffer.	*/
static unsigned char volatile receive_buffer[RECEIVE_BUFFER_SIZE];
static unsigned char volatile * volatile receive_buffer_first = RECEIVE_BUFFER_START;
static unsigned char volatile * volatile receive_buffer_last = RECEIVE_BUFFER_START;

	/*  The send buffer.  Includes buffer and two pointers used to	*/
	/* track the first and last characters in the buffer.		*/
static unsigned char send_buffer[SEND_BUFFER_SIZE];
static unsigned char * volatile send_buffer_first = SEND_BUFFER_START;
static unsigned char *send_buffer_last = SEND_BUFFER_START;

static int volatile prime = 1;	/*  Set whenever the transmit interrupt	*/
				/* did not find anything to send.  When	*/
				/* that happens the send routine needs	*/
				/* to send the next character directly	*/
				/* to 'prime the pump'.			*/

/********************* uart0_setup **************************************/
/*  uart0_setup -- Setup the uarts line characteristics, baud rate,	*/
/* bits per character, parity, stop.					*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  serial_param *p	-- requested serial line characteristics.	*/
/*  Returns 0 if successful.  Sets baud in 'p' to the actual baud rate	*/
/* achieved.								*/
static int uart0_setup( 
    struct _reent *r, 
    struct serial_param *p)
{
 unsigned long vpbrate, divisor;
 PROCESSOR_STATUS prev_int;

 vpbrate = VPBRate();		/*  Find the clock rate on the VP bus.	*/
 if( vpbrate == 0) {
    r->_errno = ELPC_INTERNAL;
    return -1;
    }

 prev_int = DisableInterrupts( INTERRUPT_DISABLE_MASK);

	/*  Select serial port functions for P0.0 and P0.1		*/
 PINSEL0 = (PINSEL0 & ~ 0xFu) | 0x5u;	
 U0LCR = UART_DLAB_EN;			/*  Enable the Divisor latch.	*/

 divisor = vpbrate/(p->baud *16);	/*  Divisor to reach requested	*/
					/* baud rate.			*/

 if( divisor > 0xFFFFu) {		/*  Divisor too large, can't go	*/
    r->_errno = ELPC_CANT;		/* this slow.			*/
    return -1;
    }

	/*  Use next highest divisor if it will get us closer to the	*/
	/* requested baud rate.						*/
 if( (divisor < 0xFFFFu) && ((vpbrate%(p->baud*16)) > (p->baud*8))) {
    divisor ++;
    }

 U0DLM = (unsigned char)(divisor >> 8u);/* Set the baud rate divider.	*/
 U0DLL = (unsigned char)(divisor & 0xFFu);

	/*  Disable divisor latch, set to desired line styles.		*/
 U0LCR = (unsigned char)(p->length | p->parity | p->stop);

 	/* Finally enable the FIFOs.  Even though they are not used	*/
	/* they must be enabled for proper operation.			*/
 U1FCR = 1;

 RestoreInterrupts( prev_int);

 p->baud = vpbrate/(divisor * 16);	/* Return actual baud rate.	*/
 return 0;
}

/********************* uart0_interrupt_setup ****************************/
/*  uart0_interrupt_setup -- Setup the uarts interrupt (priority and 	*/
/* FIQ/IRQ).								*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  interrupt_param *p	-- requested interrupt characteristics.		*/
/*  Returns 0 if successful						*/
static int uart0_interrupt_setup( 
    struct _reent *r, 
    const struct interrupt_param *p)
{
 int ret;
	/* setup VIC uart interrupt					*/
 ret = VICSetup( UART0, p->pri, Uart0InterruptShell, p->FIQ);

	/*  Enable the interrupts in the uart peripheral.		*/
 if( ret == 0) {
    U0IER = RDA_INT_ENABLE | THRE_INT_ENABLE;
    }
 else {
    r->_errno = ret;
    }
 return ret;
}

/********************* uart0_ioctl **************************************/
/*  uart0_ioctl -- Provides ioctl functions for serial port.  Really	*/
/* just sets the uarts line characteristics, baud rate,	bits per 	*/
/* character, parity, stop.  Essentially a cracker function, it figures	*/
/* out what is being requested and calls the appropriate routine to do	*/
/* it.									*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  int file		-- number referring to the open file. Generally	*/
/*			obtained from a corresponding call to open.	*/
/*			Only one file number (0) is supported.		*/
/*  unsigned long request -- simple ordinal indicating what the 	*/
/*			requested action is.				*/
/*  void *ptr		-- pointer to data to read and or write to	*/
/*			perform request.				*/
/*  SUB_DEVICE_INFO *info -- Per instance information, used to provide	*/
/*			information on a per instance basis.  Only one	*/
/*			instance of this device is allowed.		*/
/*  Returns 0 if successful.  Sets baud in 'p' to the actual baud rate	*/
/* achieved.								*/
/*lint -esym(715, info ) info not referenced.				*/
/*lint -esym(818, info ) info could be declared const.			*/
static int uart0_ioctl(
    struct _reent *r, 
    int file, 
    unsigned long request, 
    void *ptr,
    SUB_DEVICE_INFO *info)	/*  Per instance information, only one	*/
    				/* instance so not used.		*/
{
 if( file != 0) {		/* Only one device.			*/
    r->_errno = EBADF;		/* Bad file number.			*/
    return -1;
    }

	/*  Figure out request, only setup supported for now.		*/
 switch( request) {
    case UART_SETUP:
         return( uart0_setup( r, ptr));

    case INTERRUPT_SETUP:
         return( uart0_interrupt_setup( r, ptr));

    default:
         r->_errno = ENOSYS;	/*  Request not understood.		*/
	 return -1;
    }
}


/************************** uart0_read **********************************/
/*  Reads from uart0 (polled fashion).					*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  int file		-- number referring to the open file. Generally	*/
/*			obtained from a corresponding call to open.	*/
/*			Only one file number (0) is supported.		*/
/*  void *ptr		-- memory area to place read bytes into. 	*/
/*  size_t len		-- maximum number of bytes to read.		*/
/*  SUB_DEVICE_INFO *info -- Per instance information, used to provide	*/
/*			information on a per instance basis.  Only one	*/
/*			instance of this device is allowed.		*/
/*  Note:  will only return a single byte on every call. Blocks until	*/
/* that byte is ready. 							*/
/*  Returns number of bytes read. (_ssize_t)-1 on error.		*/
/*lint -esym(715, info, len ) info & len not referenced.		*/
/*lint -esym(818, info ) info could be declared const.			*/
static _ssize_t uart0_read (
    struct _reent *r, 		/*  Re-entrancy structure, used to make	*/
				/* thread safe.				*/
    int file,			/*  File handle.  Used to distinguish	*/
				/* multiple instances.  		*/
    void *ptr,		 	/*  Where to place data.		*/
    size_t len,			/*  Max data to read.			*/
    SUB_DEVICE_INFO *info)	/*  Per instance information, only one	*/
    				/* instance so not used.		*/
{
 unsigned char *p;
 PROCESSOR_STATUS prev_int;

 if( file != 0) {		/* Only one device.			*/
    r->_errno = EBADF;		/* Bad file number.			*/
    return (_ssize_t)-1;
    }

 p = ptr;			/*  Conversion from void pointer to 	*/
				/* character pointer.			*/

	/*  Wait for a character to arrive.				*/
 while( receive_buffer_first == receive_buffer_last) {
    }

	/*  Interrupts disabled during the period that 			*/
	/* receive_buffer_first may not be valid. Simple buffer read, 	*/
	/* read next available byte and advance the pointer.  Wrap to 	*/
	/* start of buffer if it walks off the end.			*/
 prev_int = DisableInterrupts( INTERRUPT_DISABLE_MASK);
 *p = *receive_buffer_first++;
 if( receive_buffer_first >= RECEIVE_BUFFER_END) {
    receive_buffer_first = RECEIVE_BUFFER_START;
    }
 RestoreInterrupts( prev_int);

 return (_ssize_t)1;		/*  One byte read.			*/
}

/************************** uart0_write *********************************/
/*  Writes to  uart0 (polled fashion).					*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  int file		-- number referring to the open file. Generally	*/
/*			obtained from a corresponding call to open.	*/
/*			Only one file number (0) is supported.		*/
/*  const void *ptr	-- memory area to place read bytes from. 	*/
/*  size_t len		-- maximum number of bytes to write.		*/
/*  SUB_DEVICE_INFO *info -- Per instance information, used to provide	*/
/*			information on a per instance basis.  Only one	*/
/*			instance of this device is allowed.		*/
/*  Returns number of bytes written. (_ssize_t)-1 on error.		*/
/*lint -esym(715, info ) info not referenced.				*/
/*lint -esym(818, info ) info could be declared const.			*/
static _ssize_t uart0_write (
    struct _reent *r,  		/*  Re-entrancy structure, used to make	*/
				/* thread safe.				*/
    int file, 			/*  File handle.  Used to distinguish	*/
				/* multiple instances.  		*/
    const void *ptr,		/*  Pointer to data to write.		*/
    size_t len,			/*  Amount of data to write.		*/
    SUB_DEVICE_INFO *info)	/*  Per instance information, only one	*/
    				/* instance so not used.		*/
{
 size_t todo;
 const unsigned char *p;
 unsigned char *put;

 if( file != 0) {		/* Only one device.			*/
    r->_errno = EBADF;		/* Bad file number.			*/
    return (_ssize_t)-1;
    }

 todo = len;		/*  Working var, keeps track of how many bytes	*/
			/* are left to write.				*/
 p = ptr;		/*  Keeps track of position in buffer.		*/
 for( ; todo != 0; todo--) {		/* Loop through string.		*/
    do {
		/*  Put byte in first available spot in the buffer 	*/
		/* using a temporary pointer.  The temporary pointer is	*/
		/* used since we don't know if we can advance until we	*/
		/* have.  Doing the advance in a temporary we can 	*/
		/* advance and then update the record or simply forget	*/
		/* it.  Otherwise we would need to do an undo.		*/
         put = send_buffer_last;
	 *put++ = *p;
	 if( put == SEND_BUFFER_END) {
	      put = SEND_BUFFER_START;
	      }
	 } while( put == send_buffer_first);	/*  Keep trying until 	*/
	 					/* there is room.	*/
    send_buffer_last = put;
    if( prime != 0) {
	 prime = 0;
	 transmit();
	 }
    p++;
    }

 return (_ssize_t)len;			/* Number of bytes written.	*/
}

/************************** uart0_open **********************************/
/*  Opens uart0.							*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  const char *name	-- name of file/device to open. Since sys 	*/
/*			supports no sub devices this should be an empty	*/
/*			string.						*/
/*  int flags		-- Flags to control open.  Not used at the 	*/
/*			moment.						*/
/*  int mode		-- Mode to open in.  Not used at the moment.	*/
/*  SUB_DEVICE_INFO *info -- Per instance information, used to provide	*/
/*			information on a per instance basis.  Only one	*/
/*			instance of this device is allowed.		*/
/*  Returns file number >= 0 if successful.  Otherwise the error code 	*/
/* may be found in errno.						*/
/*lint -esym(715, o_flags, o_mode ) o_flags, o_mode not referenced.	*/
/*lint -esym(715, info ) info not referenced.				*/
/*lint -esym(818, info ) info could be declared const.			*/
static int uart0_open(		
    struct _reent *r,		/*  Re-entrancy structure, used to make	*/
				/* thread safe.				*/
    const char *name, 		/*  Name to open.			*/
    int o_flags,		/*  Flags to control open.		*/
				/* Read, write binary etc...		*/
				/* See flags.c for values generated by	*/
				/* newlib.				*/
    int o_mode,			/*  Mode to open in.  This is a 	*/
    				/* security or permissions value.  	*/
    				/* Newlib uses the classic 0666 for all */
    				/* fopens.  See fopen.c			*/
    SUB_DEVICE_INFO *info)	/*  Per instance information, only one	*/
    				/* instance so not used.		*/
{

	/*  Check against null pointer.  Also no sub-devices available	*/
	/* so make sure we aren't asked to open one.			*/
 if( (name == 0) || (*name != '\0')) {
    r->_errno = ENOENT;		/* No such file or directory.		*/
    return( -1);
    }

 return( 0);			/*  Always sub-handle 0.  Note we never	*/
				/* fail on this open.			*/
}

/************************** uart0_close *********************************/
/*  Close uart0.							*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  int file		-- number referring to the open file. Generally	*/
/*			obtained from a corresponding call to open.	*/
/*			Only one file number (0) is supported.		*/
/*  SUB_DEVICE_INFO *info -- Per instance information, used to provide	*/
/*			information on a per instance basis.  Only one	*/
/*			instance of this device is allowed.		*/
/*  Returns 0 if successful.  Otherwise the error code may be found in 	*/
/* errno.								*/
/*lint -esym(715, info ) info not referenced.				*/
/*lint -esym(818, info ) info could be declared const.			*/
static int uart0_close(
    struct _reent *r, 		/*  Re-entrancy structure, used to make	*/
				/* thread safe.				*/
    int file,			/*  File/device sub handle.		*/
    SUB_DEVICE_INFO *info)	/*  Per instance information, only one	*/
    				/* instance so not used.		*/
{

 if( file != 0) {		/* Only one device.			*/
    r->_errno = EBADF;		/* Bad file number.			*/
    return -1;
    }
 return( 0);			/* Always succeeds.			*/
}

/********************* receive ******************************************/
/*  receive -- called from the serial interrupt service routine to 	*/
/* empty the UART FIFO and send it through a pipe to the the normal 	*/
/* task level.  If it overruns the pipe the incoming data will simply	*/
/* disappear.								*/
static void receive( void)
{
 unsigned char volatile *put;

	/*  Read bytes into the buffer from the uart.  Note that if the	*/
	/* buffer is full any newly received characters are simply 	*/
	/* thrown away.  As in the write routine a temporary pointer is	*/
	/* used to advance so we don't need an undo if the buffer 	*/
	/* cannot take any more characters.				*/
 while( (U0LSR & RDR_MASK) != 0) {
    put = receive_buffer_last;
    *put++ = U0RBR;
    if( put >= RECEIVE_BUFFER_END) {
         put = RECEIVE_BUFFER_START;
	 }
    if( put != receive_buffer_first) {
         receive_buffer_last = put;
	 }
    }
}

/********************* transmit *****************************************/
/*  transmit -- called from the serial interrupt service routine to 	*/
/* fill the UART transmit FIFO using data from the transmit pipe from 	*/
/* the normal task level.  If it empties the pipe it stops trying to 	*/
/* send.  If it is called when the pipe is empty it set the priming	*/
/* flag so that it can be restarted.					*/
static void transmit( void)
{
	/*  Check for an empty pipe and set the prime flag if there is	*/
	/* nothing to send.						*/
 if( send_buffer_first == send_buffer_last) {
    prime = 1;
    return;
    }

	/*  Send the first available byte and advance the first 	*/
	/* available byte pointer.					*/
 U0THR = *send_buffer_first++;
 if(send_buffer_first >= SEND_BUFFER_END) {
    send_buffer_first = SEND_BUFFER_START;
    }
}

/********************* Uart0Service *************************************/
/*  Uart0Service -- called from the assembly shell to do the actual 	*/
/* work in responding to the interrupt.  Determines which serial 	*/
/* interrupt request are pending and calls the appropriate work	routine */
/* to do the actual heavy lifting.  					*/
void Uart0Service(void)
{
 unsigned char temp_id;

	/*  U0IIR is read into a separate local temporary so that it 	*/
	/* can be referred to several times.  Each read of may return a */
	/* different source and in some cases will actually clear the 	*/
	/* interrupt so we read it only once per interrupt.		*/
	/*  Next the value of U0IIR is masked to determine the source 	*/
	/* of the highest priority interrupt.  The handler for that 	*/
	/* interrupt source is called to deal with the condition and 	*/
	/* clear it before we return to the assembly wrapper.		*/
	/*  Note: the usual procedure of reading the IIR multiple times */
	/* to clear all interrupt sources is not used here since there 	*/
	/* appears to be a (unconfirmed) race condition that causes 	*/
	/* THRE interrupts to be cleared before they are actually read 	*/
	/* when that happens. This will work while occaisionally 	*/
	/* requiring an extra interrupt.				*/
 temp_id = U0IIR;
 switch(temp_id & IDENT_MASK) {

    	/*  DATA_AVAILABLE occurs when data has been received.		*/
	/*  DATA_TIMEOUT only occurs if the FIFO is enabled.		*/
    case DATA_AVAILABLE:
    case DATA_TIMEOUT:
	 receive();
	 break;

	/*  TRANSMIT_AVAILABLE occurs when the THR is empty.		*/
    case TRANSMIT_AVAILABLE:
	 transmit();
	 break;


	/* The line status interrupt is ignored at the moment.		*/
    case LINE_STATUS:
    default:
	 break;
    }
}


/************************** com1_int ************************************/
/*  Device table entry used to add this device.				*/
const struct device_table_entry com1_int = {
	"com1",				/*  Device name.		*/
	uart0_open,  			/*  Open method.		*/
	uart0_close,			/*  Close method.		*/
	uart0_read,			/*  Read method.		*/
	uart0_write,			/*  Write method.		*/
	uart0_ioctl,			/*  Ioctl method.		*/
	0 };				/*  No per-instance data.	*/
