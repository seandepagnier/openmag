/**************************** uart0_poll.c ******************************/
/* Copyright 2003/12/28 Aeolus Development				*/
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
/*  Uart0 device.  Runs in polled mode.		  			*/
/* Provides hooks needed to work as a device driver.		 	*/
/* Note:  All the actual routines are private to this module.  The only */
/* element publically visible is the device table entry structure.	*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 uart0_poll.c 30-Dec-2003,10:34:12,`RADSETT' First archival version.
*   2 uart0_poll.c 13-Jul-2004,09:03:40,`RADSETT' lpc_sys.h now included
*        implicictly.
*   3 uart0_poll.c 06-Nov-2004,15:22:16,`RADSETT' Add support for character
*        waiting ioctl.
*        Change transmit ready check so that is check both the TEMT and THRE bits.
*   4 uart0_poll.c 18-Feb-2005,15:35:34,`RADSETT' Add instance specific
*        information to device calls.
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
static int uart0_chars_waiting( int *serial_waiting);

	/**** Local Macros ****/

	/*  Sets bit to enable the divisor latch.			*/
#define UART_DLAB_EN		((unsigned char)0x80u)	
						
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

 vpbrate = VPBRate();		/*  Find the clock rate on the VP bus.	*/
 if( vpbrate == 0) {
    r->_errno = ELPC_INTERNAL;
    return -1;
    }

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

 p->baud = vpbrate/(divisor * 16);	/* Return actual baud rate.	*/
 return 0;
}

/********************* uart0_chars_waiting *******************************/
/*  uart0_char_waiting -- Query to see if any chars are waiting.	*/
/*  int *serial_waiting	-- Where to place count of characters waiting.	*/
/*			Note that for this version the only valid	*/
/*			numbers here are 0 and 1.			*/
/*  Returns 0 if successful.						*/
static int uart0_chars_waiting( 
    int *serial_waiting)
{
 if( (U0LSR & 0x1) == 0) {	/* Is a character waiting?		*/
    *serial_waiting = 0;
    }
 else {
    *serial_waiting = 1;
    }
 return 0;
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
/*  Returns 0 if successful.  						*/
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

    case UART_CHAR_WAITING:
         return( uart0_chars_waiting( ptr));

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
/*lint -esym(715, info, len ) info and len not referenced.		*/
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

 if( file != 0) {		/* Only one device.			*/
    r->_errno = EBADF;		/* Bad file number.			*/
    return (_ssize_t)-1;
    }

 p = ptr;			/*  Conversion from void pointer to 	*/
				/* character pointer.			*/
 while( (U0LSR & 0x1) == 0) {	/* Is a character waiting?		*/
    }
 *p = U0RBR;			/*  Read character from uart.		*/
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

 if( file != 0) {		/* Only one device.			*/
    r->_errno = EBADF;		/* Bad file number.			*/
    return (_ssize_t)-1;
    }

 todo = len;		/*  Working var, keeps track of how many bytes	*/
			/* are left to write.				*/
 p = ptr;		/*  Keeps track of position in buffer.		*/
 for( ; todo != 0; todo--) {		/* Loop through string.		*/
    while( (U0LSR & 0x60) == 0) {	/* Is transmitter ready?	*/
         }
    U0THR = *p++;			/* Send character.		*/
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

/************************** com1 ****************************************/
/*  Device table entry used to add this device.				*/
const struct device_table_entry com1 = {
	"com1",				/*  Device name.		*/
	uart0_open,  			/*  Open method.		*/
	uart0_close,			/*  Close method.		*/
	uart0_read,			/*  Read method.		*/
	uart0_write,			/*  Write method.		*/
	uart0_ioctl,			/*  Ioctl method.		*/
	0 };				/*  No per-instance data.	*/
