/**************************** spi_int.c *********************************/
/* Copyright 2005/01/18 Aeolus Development				*/
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
/*  SPI0 device.  Runs in interrupt mode.	  			*/
/* The interrupt and baud rate setup must be done through an ioctl 	*/
/* call, there are no default values.					*/
/*  Note:  Most of the actual routines are private to this module. The	*/
/* only elements publically visible are the device table entry 		*/
/* structure and the interrupt support routine.				*/
/*									*/
/*  Contributed by Bruce Paterson 2005/01/17 -- Thank you.		*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 spi_int.c 17-Jan-2005,15:34:52,`BRUCEPATERSON' Original contributed version.
*   2 spi_int.c 01-Jan-2006,10:45:44,`RADSETT' Update comments to reflect actual
*        device being supported.
*        Update header references so they are no specific to a particular
*        installation
*        Change most magic numbers to defines.
*        Add sub-device support.
*        Add additional error checking.
*        Add support for multiple buffering methods.
*        Add ioctls for interrupt setup, number of char waiting, and device
*        selections.
*   TLIB revision history ends.
*/

#include <stdio.h>
#include <errno.h>
#include "dev_cntrl.h"	/* spi device */
#include "lpc_ioctl.h"	/* spi_param definition */
#include "errno_lpc.h"
#include "lpc210x.h"

	/**** Local Prototypes ****/
static int spi0_chars_waiting( struct _reent *r, size_t *waiting);
static _ssize_t spi0_read (struct _reent *r, int file, void *ptr, size_t len, SUB_DEVICE_INFO *info);
static _ssize_t spi0_write (struct _reent *r, int file, const void *ptr, size_t len, SUB_DEVICE_INFO *info);
static int spi0_open(struct _reent *r, const char *name, int flags, int mode, SUB_DEVICE_INFO *info);
static int spi0_close(struct _reent *r, int file, SUB_DEVICE_INFO *info);
static int spi0_device_add( struct _reent *r, SUB_DEVICE_CHAIN *dev);
static int spi0_device_select( struct _reent *r, const struct device_select *sel);
static int spi0_ioctl(struct _reent *r, int file, unsigned long request, void *ptr, const SUB_DEVICE_INFO *info);
static int spi0_setup( struct _reent *r, struct spi_param *p);
static int spi0_blocking_setup( struct _reent *r, const int *p);
static int spi0_buffer_setup( struct _reent *r, const struct buffer_param *p);
static int spi0_init( struct _reent *r);
static int spi0_interrupt_setup( struct _reent *r, const struct interrupt_param *p);
static void transmit( void);
static void transfer_next( void);
static void receive( void);
static _ssize_t unblocked_read( unsigned char *buf, _ssize_t len);
static size_t unblocked_buffer( const unsigned char * buf, size_t len);

void Spi0Service(void);		/*  Private to this driver and the 	*/
				/* interrupt shell but needs public 	*/
				/* linkage so that the interrupt shell 	*/
				/* can be linked to it.			*/
void Spi0InterruptShell(void);	/*  Private to this driver and the 	*/
				/* interrupt shell but needs public 	*/
				/* linkage so that this module can get 	*/
				/* its address for the VIC.	 	*/

	/**** Local Macros ****/

	/*  Sets bit to make SPI peripheral the master.			*/
#define SPI_MASTER		(0x20)	

	/*  Mask 2 LSBs as reserved bits.  Use this to ensure that they	*/
	/* are written as zero as required in the manual.		*/
#define SPCR_RESERVED_MASK	(0x3)

	/*  Mask to use to enable the interrupt on the SPI peripheral.	*/
#define SPI_INT_ENABLE_MASK	(0x80)

	/*  Mask to use to disable the interrupt on the SPI peripheral.	*/
	/* Note that it also ensures that the reserved bits are written	*/
	/* as zero as documented in the data sheet.			*/
#define SPI_INT_DISABLE_MASK	(~( SPI_INT_ENABLE_MASK | SPCR_RESERVED_MASK))

	/*  Maximum value for the baud rate divisor.			*/
#define MAX_BAUD_DIVISOR	(254)

	/*  Minimum value for the baud rate divisor.			*/
#define MIN_BAUD_DIVISOR	(8)

	/*  Maximum value used for chip select.  Used for bounds	*/
	/* checking.							*/
#define MAX_SPI_DEVICE_SELECT	(31u)

	/*  Sizes of the send and receive buffers.			*/
#define RECEIVE_BUFFER_SIZE	((size_t)32)
#define SEND_BUFFER_SIZE	((size_t)32)

	/*  Endpoints of the send and receive buffers.  Note that the 	*/
	/* end endpoint is one element past the last.  No pointer 	*/
	/* should ever be left with these values, they are used to 	*/
	/* detect the wrap point.					*/
#define RECEIVE_BUFFER_START	(&receive_buffer[0])
#define RECEIVE_BUFFER_END	(&receive_buffer[RECEIVE_BUFFER_SIZE])
#define SEND_BUFFER_START	(&send_buffer[0])
#define SEND_BUFFER_END		(&send_buffer[SEND_BUFFER_SIZE])

	/*  Maximum number of sub devices (of this device) that may be	*/
	/* opened simultaneously.					*/
#define NUM_SUB_DEVICES		(10)

	/**** Local Variables ****/

	/*  The receive buffer.  Includes buffer and two pointers used	*/
	/* to track the first and last characters in the buffer.	*/
static volatile unsigned char receive_buffer[RECEIVE_BUFFER_SIZE];
static volatile unsigned char * receive_buffer_first = RECEIVE_BUFFER_START;
static volatile unsigned char * volatile receive_buffer_last = RECEIVE_BUFFER_START;

	/*  The send buffer.  Includes buffer and two pointers used to	*/
	/* track the first and last characters in the buffer.		*/
static unsigned char send_buffer[SEND_BUFFER_SIZE];
static const unsigned char * volatile send_buffer_first = SEND_BUFFER_START;
static const unsigned char * send_buffer_last = SEND_BUFFER_START;

	/* Semaphore to lock out multiple simultaneous access attempts.	*/
static int spi0_opened = 0;	/*  Only one open at a time allowed.	*/

	/* Buffering mode, defaults to internal buffering.		*/
static int buffer_mode = BUFFER_INTERNAL;

	/* IO mode, defaults to blocking.				*/
static int io_mode = BLOCKING_IO_YES;

	/* Start of chain of subdevices.				*/
static SUB_DEVICE_CHAIN *device_head = 0;
	
	/* List of open sub devices.					*/
static OPEN_DEVICE open_list[NUM_SUB_DEVICES];

/********************* spi0_setup ***************************************/
/*  spi0_setup 		-- Setup the SPI polarity and speed.		*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  speed *p		-- requested line speed, polarity, clock phase	*/
/*			and bit direction.				*/
/*  Returns 0 if successful.  Sets baud in 'p' to the actual baud rate	*/
/* achieved.								*/
static int spi0_setup( 
    struct _reent *r, 
    struct spi_param *p)
{
 unsigned long vpbrate, divisor;
 PROCESSOR_STATUS prev_int;

 vpbrate = VPBRate();		/*  Find the clock rate on the VP bus.	*/
 if( vpbrate == 0) {
    r->_errno = ELPC_INTERNAL;
    return -1;
    }

 divisor = vpbrate/p->hz;	/*  Divisor to reach requested speed	*/

	/*  The divisor has three restrictions imposed by the HW 	*/
	/* implementation.  It must be at least 8, it must fit in 8	*/
	/* bits (and so be no more than 255), and its low order bit 	*/
	/* must be zero.  The following enforces those restrictions 	*/
	/* while ensuring the resulting baud rate is no higher than	*/
	/* requested.  If the result is larger than requested an error	*/
	/* will be flagged.						*/
 if( divisor < MIN_BAUD_DIVISOR ) {	/*  Ensure at least the minimum	*/
     divisor = MIN_BAUD_DIVISOR;	/* divisor.			*/
     }
 divisor &= ~1;			/*  Low order bit forced to zero.	*/

	/*  If divisor is too large, flag as an	error we can't go this	*/
	/* slow.  Note that this is flagged as an error rather than 	*/
	/* changed to fit since going faster than the requested rate 	*/
	/* is likely to cause errors because of limitations in the 	*/
	/* interfaced HW.  This is unlike the case of going slower than	*/
	/* requested where all that is lost is bandwidth, the attached	*/
	/* peripherals are unlikely to be sensitive to signals that are	*/
	/* too slow.							*/
 if( divisor > MAX_BAUD_DIVISOR ) {
    r->_errno = ELPC_CANT;	
    return -1;
    }

 prev_int = DisableInterrupts( INTERRUPT_DISABLE_MASK);

 SPCR &= SPI_INT_DISABLE_MASK;	/* Shut off interrupt in SPI peripheral.*/

 SPCCR = (unsigned char)divisor;

	/*  Set line characteristics.					*/
 SPCR = SPI_MASTER | p->cpha | p->cpol | p->lsbf;

 RestoreInterrupts( prev_int);

	/* Restore buffering and IO modes to default.			*/
 buffer_mode = BUFFER_INTERNAL;
 io_mode = BLOCKING_IO_YES;

 p->hz = vpbrate/divisor;	/*  Return actual rate.			*/

 return 0;
}

/********************* spi0_init ****************************************/
/*  spi0_init 		-- Setup the SPI pins and data structures.	*/
/* Should be caled only once.						*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  Returns 0 if successful.						*/
/*lint -esym(715, r ) r not referenced.					*/
/*lint -esym(818, r ) r could be declared const.			*/
static int spi0_init( 
    struct _reent *r) 
{
 PROCESSOR_STATUS prev_int;

 prev_int = DisableInterrupts( INTERRUPT_DISABLE_MASK);

 SPCR &= SPI_INT_DISABLE_MASK;	/* Shut off interrupt in SPI peripheral.*/

	/*  Select spi port functions for P0.4, 0.5, P0.6 and P0.7	*/
 PINSEL0 = (PINSEL0 & ~0xFF00u) | 0x5500u;

 RestoreInterrupts( prev_int);

 device_head = 0;

 InitOpenSubDeviceList( open_list, NUM_SUB_DEVICES);

 return 0;
}

/********************* spi0_interrupt_setup *****************************/
/*  spi0_interrupt_setup -- Setup the spi's interrupt (priority and 	*/
/* FIQ/IRQ).								*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  interrupt_param *p	-- requested interrupt characteristics.		*/
/*  Returns 0 if successful						*/
static int spi0_interrupt_setup( 
    struct _reent *r, 
    const struct interrupt_param *p)
{
 int ret;

	/*  setup VIC uart interrupt.  Notethat actual enabling of the	*/
	/* interrupt in the peripheral is done during write.		*/
 ret = VICSetup( SPI, p->pri, Spi0InterruptShell, p->FIQ);

 if( ret != 0) {		/*  Save any errors.			*/
    r->_errno = ret;
    }

 return ret;
}

/********************* spi0_buffer_setup ********************************/
/*  spi0_buffer_setup	-- Setup the drivers buffering		 	*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  buffer_param *p	-- requested interrupt characteristics.		*/
/*  Returns 0 if successful						*/
static int spi0_buffer_setup( 
    struct _reent *r, 
    const struct buffer_param *p)
{
 PROCESSOR_STATUS prev_int;

	/* Set up the chosen buffer type.				*/
 switch( p->type) {

		/*  Internal buffering, set up pointers to the 		*/
		/* appropriate ring buffers and set the mode variable.	*/
    case BUFFER_INTERNAL:
         prev_int = DisableInterrupts( INTERRUPT_DISABLE_MASK);
         receive_buffer_first = RECEIVE_BUFFER_START;
         receive_buffer_last = RECEIVE_BUFFER_START;
         send_buffer_first = SEND_BUFFER_START;
         send_buffer_last = SEND_BUFFER_START;
	 buffer_mode = BUFFER_INTERNAL;
         RestoreInterrupts( prev_int);
	 break;

		/*  Zero buffering, set up pointers to 0 since there is	*/
		/* nothing for them to refer to yet and set the mode	*/
		/* variable.						*/
    case BUFFER_ZERO:
         prev_int = DisableInterrupts( INTERRUPT_DISABLE_MASK);
         receive_buffer_first = 0;
         receive_buffer_last = 0;
         send_buffer_first = 0;
         send_buffer_last = 0;
	 buffer_mode = BUFFER_ZERO;
         RestoreInterrupts( prev_int);
	 break;

		/*  Unknown buffer mode, treat as if internal and flag	*/
		/* as an error.						*/
    default:
         prev_int = DisableInterrupts( INTERRUPT_DISABLE_MASK);
         receive_buffer_first = RECEIVE_BUFFER_START;
         receive_buffer_last = RECEIVE_BUFFER_START;
         send_buffer_first = SEND_BUFFER_START;
         send_buffer_last = SEND_BUFFER_START;
	 buffer_mode = BUFFER_INTERNAL;
         RestoreInterrupts( prev_int);
         r->_errno = ENOSYS;	/*  Request not understood.		*/
	 return -1;
    }

 return 0;
 }

/********************* spi0_blocking_setup ******************************/
/*  spi0_blocking_setup	-- Setup the drivers blocking response.	 	*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  const unsigned int *p -- requested blocking mode.			*/
/*  Returns 0 if successful						*/
static int spi0_blocking_setup( 
    struct _reent *r, 
    const int *p)
{

	/*  Not much to do other than note the mode.			*/
 switch( *p) {
    case BLOCKING_IO_YES:
    case BLOCKING_IO_NO:
         io_mode = *p;
	 break;

	/*  Unknown mode, set to default and flag as an error.		*/
    default:
         io_mode = BLOCKING_IO_YES;
         r->_errno = ENOSYS;	/*  Request not understood.		*/
	 return -1;
    }

 return 0;	/* Success.						*/
 }

/********************* spi0_chars_waiting *******************************/
/*  spi0_chars_waiting	-- Query to see if any chars are waiting. 	*/
/*  int *waiting 	-- Where to place count of characters waiting.	*/
/*  Returns 0 if successful						*/
static int spi0_chars_waiting(
    struct _reent *r, 
    size_t *waiting)
{
 size_t  tsize;	/*  Temporary to hold result until we've 	*/
			/* finished calculating it.			*/

	/*  How the number of waiting characters are counted depends on	*/
	/* the buffering mode.						*/
 switch( buffer_mode) {

	/*  Internal buffer, we need to deal with two cases.		*/
    case BUFFER_INTERNAL:
		/*  First case, first <= last. Straightforward pointer	*/
		/* subtraction give the number of bytes in the buffer.	*/
         if( receive_buffer_first <=  receive_buffer_last) {
	      tsize = (size_t)(receive_buffer_last -  receive_buffer_first);
	      }

		/*  Second case, first > last. Buffered data is wrapped	*/
		/* around the end.  In this case the pointer 		*/
		/* subtraction gives the amount of the ring buffer that	*/
		/* is not used.  Subtract this from the buffer size to 	*/
		/* get the number of bytes waiting.			*/
	 else {
	      tsize = (size_t)(receive_buffer_first -  receive_buffer_last);
	      tsize = RECEIVE_BUFFER_SIZE - tsize;
	      }
	 *waiting =  tsize;
	 break;

	/*  Zero buffering, simply subtrac the pointers.		*/
    case BUFFER_ZERO:
	 *waiting =  (size_t)(receive_buffer_last - receive_buffer_first);
	 break;

	/*  Unknown buffering mode, nothing waiting, flag an error.	*/
    default:
	 *waiting = 0;
         r->_errno = ENOSYS;	/*  Request not understood.		*/
	 return -1;
    }

 return 0;
}

/********************* spi0_device_select *******************************/
/*  spi0_device_select	-- Set appropriate lines to select or deselect  */
/* a physical device.  Generally used with sub-drivers so they can 	*/
/* control their associated device without needing user intervention. 	*/
/*  struct device_select *sel 	-- Information on what device to select.*/
/*  Although this routine checks that the requested select is within	*/
/* range, it does not verify that the select has been initialized.	*/
/*  Returns 0 if successful						*/
static int spi0_device_select(
    struct _reent *r, 
    const struct device_select *sel)
{

 if( spi0_opened == 0) {	/*  Only allow driver respond if open.	*/
    r->_errno = ENOENT;
    return -1;
    }

	/*  Check that select is within range.				*/
 if( sel->device_number > MAX_SPI_DEVICE_SELECT) {
    r->_errno = ELPC_OOR;
    return -1;
    }

	/*  Now perform the appropriate selection action.		*/
 switch( sel->select_action) {

    case SELECT_ACTION_SEL:	/*  Set pin low for select.		*/
         IOCLR = 1 << sel->device_number;
	 break;

    case SELECT_ACTION_DESEL:	/*  Set pin high to de-select.		*/
         IOSET = 1 << sel->device_number;
	 break;

    case SELECT_ACTION_INIT:
         IOSET =  1 << sel->device_number; /*  Default to de-selected.	*/
         IODIR |= 1 << sel->device_number; /*  Set pin as output.	*/
	 break;

    default:
         r->_errno = ENOSYS;	/*  Request not understood.	*/
         return -1;
    }

 return 0;
}

/********************* spi0_device_add **********************************/
/*  spi0_device_add	-- Add a subdevice to the list of SPI controlled*/
/* devices.								*/
/*  SUB_DEVICE_INFO *info -- Per instance information, used to provide	*/
/*			information on a per instance basis.  In this	*/
/*			case it's retained with the subdevice so that it*/
/*			can refere back to it's parent. Only one	*/
/*			instance of this device is allowed.		*/
/*  SUB_DEVICE *dev 	-- Information on sub device.			*/
/*  Returns 0 if successful						*/
static int spi0_device_add(
    struct _reent *r, 
    SUB_DEVICE_CHAIN *dev)
{

 if( dev != 0) {
    dev->next = device_head;		/*  Insert device at the head of*/
    device_head = dev;			/* the chain.			*/
    dev->info.parent = &spi;		/*  Point subdevice parent 	*/
    					/* reference back to here.	*/
    }
 else {
    r->_errno = ENOSYS;		/*  Request not understood.		*/
    return -1;
    }

 return 0;
}

/********************* spi0_ioctl ***************************************/
/*  spi0_ioctl 		-- Provides ioctl functions for spi port.  	*/
/* Essentially a cracker function, it figures out what is being 	*/
/* requested and calls the appropriate routine to do it.		*/
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
/*  Returns 0 if successful.					  	*/
/*lint -esym(715, info)							*/
/*lint -esym(818, info ) info could be declared const.			*/
static int spi0_ioctl(
    struct _reent *r, 
    int file, 
    unsigned long request, 
    void *ptr,
    const SUB_DEVICE_INFO *info)/*  Per instance information, only one	*/
    				/* instance.				*/
{
 if( file != 0) {
    return SubDeviceIoctl ( r, file, request, ptr, open_list, NUM_SUB_DEVICES);
    }

	/*  Figure out request.						*/
 switch( request) {

    case SPI_SETUP:		/*  Set up line characteristics.	*/
         return( spi0_setup( r, ptr));

    case INTERRUPT_SETUP:	/*  Set up interrupt characteristics.	*/
         return( spi0_interrupt_setup( r, ptr));

    case BUFFER_SETUP:		/*  Set up buffer characteristics.	*/
         return( spi0_buffer_setup( r, ptr));

    case BLOCKING_SETUP:	/*  Do we block on I/O?			*/
         return( spi0_blocking_setup( r, ptr));

    case UART_CHAR_WAITING:	/*  Anything waiting to be read.	*/
         return( spi0_chars_waiting( r, ptr));

    case DEVICE_SELECT:		/*  Select physical device.		*/
         return( spi0_device_select( r, ptr));

    case DEVICE_ADD:		/*  Add a subdevice to device chain.	*/
         return( spi0_device_add( r, ptr));

    case DEVICE_INIT:		/*  Initialize device.			*/
         return( spi0_init( r));

    default:
         r->_errno = ENOSYS;	/*  Request not understood.		*/
	 return -1;
    }
}


/************************** unblocked_read ******************************/
/*  Performs an unblocked read from the internal buffer to the users	*/
/* buffer.  Returns as soon as the maximum number of characters is read */
/* or there is nothing left to read.  This routine forms the core of 	*/
/* both the blocked and unblocked read mechanisms.			*/
/*  char *ptr		-- memory area to place read bytes into. 	*/
/*  size_t len		-- maximum number of bytes to read.		*/
/*  Returns number of bytes read.					*/
static _ssize_t unblocked_read( unsigned char *buf, _ssize_t len)
{
 _ssize_t len_read = 0;
 PROCESSOR_STATUS prev_int;

	/*  Interrupts disabled during the period that 			*/
	/* receive_buffer_first may not be valid. Simple buffer read, 	*/
	/* read next available byte and advance the pointer.  Wrap to 	*/
	/* start of buffer if it walks off the end.			*/
 prev_int = DisableInterrupts( INTERRUPT_DISABLE_MASK);
 while((receive_buffer_first != receive_buffer_last) && (len_read < len)) {
    *buf++ = *receive_buffer_first++;
    len_read ++;
    if( receive_buffer_first >= RECEIVE_BUFFER_END) {
         receive_buffer_first = RECEIVE_BUFFER_START;
         }
    }
 RestoreInterrupts( prev_int);

 return( len_read);
}

/************************** spi0_read ***********************************/
/*  Reads from buffer and sets buffer.					*/
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
/*  Returns number of bytes read. -1 on error.				*/
/*lint -esym(715, info ) info not referenced.				*/
/*lint -esym(818, info ) info could be declared const.			*/
static _ssize_t spi0_read (
    struct _reent *r, 		/*  Re-entrancy structure, used to make	*/
				/* thread safe.				*/
    int file,			/*  File handle.  Used to distinguish	*/
				/* multiple instances.  		*/
    void *ptr,		 	/*  Where to place data.		*/
    size_t len,			/*  Max data to read.			*/
    SUB_DEVICE_INFO *info)	/*  Per instance information, only one	*/
    				/* instance so not used.		*/
{
 _ssize_t len_read;		/* Number of bytes read.		*/
 unsigned char *p;

 if( file != 0) {
    return SubDeviceRead ( r, file, ptr, len, open_list, NUM_SUB_DEVICES);
    }

 if( spi0_opened == 0) {	/*  Only allow driver respond if open.	*/
    r->_errno = ENOENT;
    return (_ssize_t)-1;
    }

 p = ptr;			/*  Conversion from void pointer to 	*/
				/* character pointer.			*/
 len_read = 0;

	/*  Read is treated differently for the different buffering 	*/
	/* modes.							*/
 switch( buffer_mode) {

     case BUFFER_ZERO:

		/*  In zero buffer mode, a requested read length of	*/
		/* is used to set up the user's read buffer.  This 	*/
		/* buffer must large enough to hold the incoming data	*/
		/* since there is no overflow check.			*/
	 if( len == 0) {
              receive_buffer_first = receive_buffer_last = p;
	      return 0;
	      }

		/*  In zero buffer mode reading actually does nothing	*/
		/* other than return the number of characters in the	*/
		/* buffer. The nature of SPI means that in order to	*/
		/* read a byte you must write a byte so all the really	*/
		/* heavy lifting is done at the write interface.	*/
		/*  If the receive buffer is set to 0 then all incoming */
		/* bytes are discarded and this call will always return	*/
		/* 0.							*/
	 if( receive_buffer_last == 0) {
	      len_read = 0;
	      }
	 else {
		/*  Check length by simple pointer arithmetic.  If in	*/
		/* blocking mode wait until at least the required 	*/
		/* number of bytes have been read, otherwise return	*/
		/* immediately with the number of bytes already read.	*/
	      if( io_mode == BLOCKING_IO_YES) {
	           while ( (size_t)(receive_buffer_last - receive_buffer_first) < len) {
		        }
		   len_read = (_ssize_t)(receive_buffer_last - receive_buffer_first);
		   }
	      else {
 		   len_read = (_ssize_t)(receive_buffer_last - receive_buffer_first);
		   }
	      }
	 break;

    case BUFFER_INTERNAL:

		/*  In internal buffer mode we copy from the internal	*/
		/* buffer to the user buffer.  In blocking mode we will	*/
		/* stay in this loop until we have all the requested 	*/
		/* bytes.  In non-blocking mode we return immediately 	*/
		/* with whatever has been received (up to a max of len)	*/
		/* up to this point.					*/
	 if( io_mode == BLOCKING_IO_YES) {
	      while(len_read < (_ssize_t)len) {
                   len_read += unblocked_read( p+len_read, ((_ssize_t)len-len_read));
		   }
	      }
	 else {
              if( receive_buffer_first != receive_buffer_last) {
                   len_read = unblocked_read( p, (_ssize_t)len); 
                   }
	      }
	 break;

    default:
         r->_errno = ENOSYS;	/*  Request not understood.		*/
	 return (_ssize_t)-1;
    }

 return len_read;
}

/************************** unblocked_buffer ****************************/
/*  Performs an unblocked transfer from the users buffer to the 	*/
/* internal buffer.  Returns as soon as the all of the bytes have been	*/
/* buffered or there is no room left in the internal buffer.  This 	*/
/* routine forms the core of both the blocked and unblocked write 	*/
/* mechanisms.								*/
/*  char *buf		-- bytes to buffer.			 	*/
/*  size_t len		-- maximum number of bytes to write into buffer.*/
/*  Returns number of bytes read.					*/
static size_t unblocked_buffer( const unsigned char * buf, size_t len)
{
 unsigned char *put;
 size_t todo = len;
 PROCESSOR_STATUS prev_int;

 for( ; todo != 0; todo--) {	/* Loop through string.			*/

 	/*  Put byte in first available spot in the buffer using a 	*/
 	/* temporary pointer.  The temporary pointer is used since we	*/
 	/* don't know if we can advance until we have.  Doing the	*/
 	/* advance in a temporary we can advance and then update the	*/
 	/* record or simply forget it.  Otherwise we would need to do	*/
 	/* an undo.							*/

	/*  Cast away constness.  The only place we don't view the send	*/
	/* buffer as constant is here where we fill it.  So everywhere	*/
	/* else we use the const definition.  Note:  It is only the 	*/
	/* pointers that have the const qualification.  The buffer 	*/
	/* itself must obviously not be const.				*/
    put = (unsigned char *)send_buffer_last;	/*lint !e926		*/
    *put++ = *buf;

    if( put == SEND_BUFFER_END) {	/*  Check for wraparound.	*/
	 put = SEND_BUFFER_START;
	 }

	/*  There was room in the buffer, advance the appropriate 	*/
	/* pointers.  Protect updates on the internal buffer by 	*/
	/* disabling interrupts for a short period.			*/
    if( put != send_buffer_first) {
         buf++;
         prev_int = DisableInterrupts( INTERRUPT_DISABLE_MASK);
         send_buffer_last = put;
         RestoreInterrupts( prev_int);
	 }
    else {
		/* No room left in the buffer, time to exit.		*/
	 break;		/*lint !e960 58: Non-switch break.		*/
         }
    }

	/*  Check to see if sending is active, if not then restart it.	*/
 if( (SPCR & SPI_INT_ENABLE_MASK) == 0) {

	/*  Clear any possible remaining interupt source.  A 		*/
	/* conservative and defensive approach.				*/
    (void)SPSR;		/*  Clear status register (clears most 		*/
    			/* interrupt sources).				*/
    (void)SPDR;		/*  Read data register to clear data ready 	*/
    			/* interrupt source.				*/
    SPINT = 1;		/*  Finally clear the interrupt flag.		*/

	/*  Now re-enable interrupts.					*/
    SPCR = (SPCR & ~SPCR_RESERVED_MASK) | SPI_INT_ENABLE_MASK;

    prev_int = DisableInterrupts( INTERRUPT_DISABLE_MASK);
    transmit();		/*  Kick start the transmission.		*/
    RestoreInterrupts( prev_int);
    }

 return( len - todo);	/*  Let caller know how many bytes were 	*/
 			/* actually buffered				*/
}

/************************** spi0_write **********************************/
/*  Writes to  spi0.							*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  int file		-- number referring to the open file. Generally	*/
/*			obtained from a corresponding call to open.	*/
/*			Only one file number (0) is supported.		*/
/*  const void *ptr	-- memory area to place send bytes from. 	*/
/*  size_t len		-- maximum number of bytes to write.		*/
/*  SUB_DEVICE_INFO *info -- Per instance information, used to provide	*/
/*			information on a per instance basis.  Only one	*/
/*			instance of this device is allowed.		*/
/*  Returns number of bytes written. -1 on error.			*/
/*lint -esym(715, info ) info not referenced.				*/
/*lint -esym(818, info ) info could be declared const.			*/
static _ssize_t spi0_write (
    struct _reent *r,  		/*  Re-entrancy structure, used to make	*/
				/* thread safe.				*/
    int file, 			/*  File handle.  Used to distinguish	*/
				/* multiple instances.  		*/
    const void *ptr,		/*  Pointer to data to write.		*/
    size_t len,			/*  Amount of data to write.		*/
    SUB_DEVICE_INFO *info)	/*  Per instance information, only one	*/
    				/* instance so not used.		*/
{
 PROCESSOR_STATUS prev_int;
 const unsigned char *p;/*  Keeps track of position in buffer.		*/
 size_t todo = len;	/*  Working var, keeps track of how many bytes	*/
			/* are left to write.				*/

 if( file != 0) {
    return SubDeviceWrite( r, file, ptr, len, open_list, NUM_SUB_DEVICES);
    }

 if( spi0_opened == 0) {	/*  Only allow driver respond if open.	*/
    r->_errno = ENOENT;
    return (_ssize_t)-1;
    }

 if((SPCR & SPI_INT_ENABLE_MASK) != 0) {	/*  Only allow one write*/
    r->_errno = EBUSY;				/* at a time.		*/
    return (_ssize_t)-1;
    }

	/*  Special case a write of length zero.  If a write of zero is	*/
	/* made while a write is already in progress a -1 will be	*/
	/* returned (and errno set).  See above.  If a write is not in	*/
	/* progress we will return 0 here.  This behaviour can be used	*/
	/* to poll for completion.					*/
 if( len == 0 ) {
    return (_ssize_t)0;
    }

	/*  Perform the actual write.  Behaviour will be different 	*/
	/* depending on buffering and blocking modes.			*/
 switch( io_mode) {

	/*  Zero buffering, the users send and receive buffers are used	*/
	/* without internal buffering.  Note that in order for this to	*/
	/* work the user MUST perform a read before writing so that the	*/
	/* read buffer is set up.  Also there will be no overrun checks	*/
	/* on the receive buffer.					*/
    case BUFFER_ZERO:

		/*  Set up the send an receive buffers, note that this	*/
		/* method allows the reuse of the receive buffer	*/
		/* without needing multiple reads.			*/
         send_buffer_first = ptr;
         send_buffer_last = send_buffer_first + len;
         receive_buffer_last = receive_buffer_first;

		/*  Clear any possible remaining interupt source.  A	*/
		/* conservative and defensive approach.			*/
         (void)SPSR;	/*  Clear status register (clears most 		*/
    			/* interrupt sources).				*/
         (void)SPDR;	/*  Read data register to clear data ready 	*/
    			/* interrupt source.				*/
         SPINT = 1;	/*  Finally, clear the interrupt flag.		*/

		/*  Now re-enable interrupts.				*/
         SPCR = (SPCR & ~SPCR_RESERVED_MASK) | SPI_INT_ENABLE_MASK;

         prev_int = DisableInterrupts( INTERRUPT_DISABLE_MASK);
         transmit();		/*  Kick start the transmission.	*/
         RestoreInterrupts( prev_int);

	 todo = 0;
	 break;

	/*  Internally buffered.  If write fits in the internal buffer	*/
	/* it will return immediately.  Buffered I/O may be blocked or	*/
	/* unblocked, unblocked I/O returns as soon as it's finished	*/
	/* buffering or the internal buffer is full whichever happens	*/
	/* first. Blocked I/O waits until it has placed all of the data */
	/* into the internal buffer before returning.			*/
    case BUFFER_INTERNAL:
         p = ptr;	

         if( io_mode == BLOCKING_IO_YES){
              size_t written;

	      while( todo != 0) {
                   written = unblocked_buffer( p, todo);
		   todo -= written;
		   p += written;
		   }
	      }
	 else {
              todo -= unblocked_buffer( p, todo);
	      }
	 break;

    default:
         r->_errno = ENOSYS;	/*  Request not understood.		*/
	 return (_ssize_t)-1;
    }

 return (_ssize_t)(len-todo);		/* Number of bytes written.	*/
}

/************************** spi0_open ***********************************/
/*  Opens spi0.								*/
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
/*lint -esym(715, o_flags, o_mode, info ) o_flags, o_mode, info not	*/
/* referenced.								*/
/*lint -esym(818, info ) info could be declared const.			*/
static int spi0_open(		
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

	/*  Check against null pointer.					*/
 if( name == 0) {
    r->_errno = ENOENT;		/* No such file or directory.		*/
    return -1;
    }
 else {
    if( *name != '\0') {	/*  We have a subdevice to check for	*/
    				/* open.				*/
	 (void)iprintf("Attempting to Open %s\r\n", name);
         return SubDeviceOpen( r, name, o_flags, o_mode, device_head, 
              open_list, NUM_SUB_DEVICES);
	 }  
    }

 if( spi0_opened != 0) {	/* Only allow one open.			*/
    r->_errno = EEXIST;
    return -1;
    }

 SPCR &= SPI_INT_DISABLE_MASK;	/* Shut off interrupt in SPI peripheral.*/

	/*  Set up default buffer and IO modes.				*/
 receive_buffer_first = RECEIVE_BUFFER_START;
 receive_buffer_last = RECEIVE_BUFFER_START;
 send_buffer_first = SEND_BUFFER_START;
 send_buffer_last = SEND_BUFFER_START;
 buffer_mode = BUFFER_INTERNAL;
 io_mode = BLOCKING_IO_YES;

 spi0_opened = 1;		/* Flag device as opened.		*/

 return(0);		
}

/************************** spi0_close **********************************/
/*  Close spi0.								*/
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
static int spi0_close(
    struct _reent *r, 		/*  Re-entrancy structure, used to make	*/
				/* thread safe.				*/
    int file,			/*  File/device sub handle.		*/
    SUB_DEVICE_INFO *info)	/*  Per instance information, only one	*/
    				/* instance so not used.		*/
{

#if 0
 if( file != 0) {		/* Only one device.			*/
    r->_errno = EBADF;		/* Bad file number.			*/
    return (_ssize_t)-1;
    }
#else
 if( file != 0) {
    return SubDeviceClose( r, file, open_list, NUM_SUB_DEVICES);
    }
#endif

	/*  Shutting down the interrupt will be redundant if the 	*/
	/* previous write was completed.				*/
 SPCR &= SPI_INT_DISABLE_MASK;	/* Shut off interrupt in SPI peripheral.*/

 spi0_opened = 0;

 return 0;			/* Always succeeds.			*/
}

/************************** receive *************************************/
/*  Get the next byte from the SPI port.  Since a byte must be received	*/
/* for every byte sent in order to avoid overrun errors this will 	*/
/* continue as long as there is data to be sent or received.  As well, 	*/
/* reading the data register clears the completion flag from the status */
/* register.								*/
static void receive( void)
{
 volatile unsigned char *put;

	/*  Use a temporary pointer for placing	the received byte. This */
	/* way if we fill the receive buffer we don't need an undo.	*/
	/* If receive_buffer is NULL then that indicates this is a 	*/
	/* write only operation and incoming data will be discarded.	*/
 put = receive_buffer_last;	

 if( put != 0 ) {	/*  Read and store the incoming byte.		*/
    *put++ = SPDR;
    }
 else {
    (void)SPDR;		/*  Read and discard the incoming byte.		*/
    return;
    }

 switch( io_mode) {
	/*  Buffered IO mode, buffered check for wraparound and 	*/
	/* overflow cases before updating.  				*/
    case  BUFFER_INTERNAL:
         if( put >= RECEIVE_BUFFER_END) {
              put = RECEIVE_BUFFER_START;
	      }
         if( put != receive_buffer_first) {
              receive_buffer_last = put;
	      }
	 break;

	/*  Non-buffered IO, just update.  Note that there is no 	*/
	/* overflow check in this case.  It is up to the user to ensure */
	/* that there is enough room to hold the result.		*/
    case BUFFER_ZERO:
         receive_buffer_last = put;
	 break;

    default:		/* Unknown buffering, ignore.			*/
         break;
    }
}

/************************** transmit ************************************/
/*  Send next byte out the SPI port.  Since a byte must be sent for 	*/
/* every byte received this will continue as long as there is data to 	*/
/* be sent or received.  						*/
/*  Called from the interrupt portion of the driver for sending.  	*/
/* Called from the non-interrupt portion of the driver to prime the	*/
/* tranmission interrupt.						*/
static void transmit( void)
{
 	/*  First check to see if we are done.  If so then turn off the	*/
 	/* interrupt and flag sending as done. 				*/
 if( send_buffer_first == send_buffer_last) {
    SPCR &= SPI_INT_DISABLE_MASK;	/* Shut off interrupt in SPI	*/
    					/* peripheral.			*/
    switch( io_mode) {
	 case BUFFER_ZERO:
	      send_buffer_first = send_buffer_last = 0;
	      break;

	 case BUFFER_INTERNAL:		/*  Only modify pointers in	*/
	 default:			/* zero buffering mode.		*/
	      break;
	 }
    return;
    }

 SPDR = *send_buffer_first++;	/* Send next byte 			*/

	/*  Perform any buffer wrapping or cleanup.			*/
 switch( io_mode) {

	/*  Circular buffer, wrap to beginning if we pass the end.	*/
    case BUFFER_INTERNAL:
         if(send_buffer_first >= SEND_BUFFER_END) {
              send_buffer_first = SEND_BUFFER_START;
              }
	 break;

    case BUFFER_ZERO:	/* Only wrap for internal circular buffer.	*/
    default:
	 break;
    }
}

/************************** transfer_next *******************************/
/*  Receive and send the next byte via the SPI port.  Really just a 	*/
/* shell to combine the two operations for convience.		 	*/
static void transfer_next( void)
{

 receive();
 transmit();
}

/********************* Spi0Service *************************************/
/*  Spi0Service -- called from the assembly shell to do the actual 	*/
/* work in responding to the interrupt.  Determines which spi interrupt	*/
/* requests are pending and calls the appropriate work	routine to do 	*/
/* the actual heavy lifting.  						*/
void Spi0Service(void)
{
 unsigned char temp_id;

	/*  SPSR is read into a separate local temporary so that it 	*/
	/* can be referred to several times.  Each read of the status 	*/
	/* register clears most pending interrupt sources so we read it	*/
	/* only once.  Once all sources have been checked and dealt 	*/
	/* with the status register is re-checked again.  This 		*/
	/* continues until the staus register indicates no interrupt	*/
	/* source is pending.  This process of re-reading the status	*/
	/* register is probably not necessary but costs little.		*/
	/*  The value of SPSR is masked to determine the source of the 	*/
	/* highest priority interrupt.  The handler for that interrupt 	*/
	/* source is called to deal with the condition and clear it 	*/
	/* before we return to the assembly wrapper.  In many cases 	*/
	/* the code to handle the source is simple nough it is handled	*/
	/* in line.							*/
 for( temp_id = SPSR & 0xf8; temp_id != 0; temp_id = SPSR & 0xf8) {

    while( temp_id != 0) {

		/*  Slave abort, SSEL was deselected we are no longer a	*/
		/* slave.  This should never be allowed to happen since	*/
		/* this is a master only driver and will not react	*/
		/* correctly to being selected as a slave.		*/
 	 if( temp_id & 8) {
	      temp_id &= ~8;	/* Just clear it for now.		*/
	      }

		/*  Mode fault, SSEL was selected we are no longer a	*/
		/* master.  This should never be allowed to happen	*/
		/* since this is a master only driver and will not	*/
		/* react correctly to being selected as a slave.	*/
	 if( temp_id & 0x10 ) {
		/*  Force master mode, unlikely to really fix the	*/
		/* problem but if was a	transient issue this will clear	*/
		/* the interrupt and maybe allows us to continue again.	*/
	      SPCR = (SPCR & ~SPCR_RESERVED_MASK) | 0x20;
	      temp_id &= ~0x10;
	      }

		/* Read Overrun, incoming data not read in time. 	*/
	 if( temp_id & 0x20 ) {	
	      temp_id &= ~0x20;	/* Just clear it for now.		*/
	      }

		/*  Write collision, write attempted before shift 	*/
		/* register was empty.  This should only occur in the 	*/
		/* case of a major fault such as the user's program	*/
		/* writing to the data register directly as well as	*/
		/* using the driver.					*/
	 if( temp_id & 0x40 ) {	

	      	/*  Abort transaction, We may want to flag this for 	*/
	      	/* reporting later. 					*/
              SPCR &= SPI_INT_DISABLE_MASK;	/*  Shut off interrupt 	*/
              					/* in SPI peripheral.	*/
	      temp_id &= ~0x40;
	      }

	 if( temp_id & 0x80) {	/*  Transfer complete.			*/
	      transfer_next();	/*  Send and receive next byte.		*/
	      temp_id &= ~0x80;
	      }
 	 }
    }
 SPINT = 1;			/*  Clear interrupt source.		*/
}



/************************** spi *****************************************/
/*  Device table entry used to add this device.				*/
const struct device_table_entry spi = {
	"spi",				/*  Device name.		*/
	spi0_open,	  		/*  Open method.		*/
	spi0_close,			/*  Close method.		*/
	spi0_read,			/*  Read method.		*/
	spi0_write,			/*  Write method.		*/
	spi0_ioctl,			/*  Ioctl method.		*/
	0};				/*  No per-instance data.	*/
