/**************************** lpc_ioctl.h *******************************/
/* Copyright 2003/12/29 Aeolus Development				*/
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
/*  I/O control.  Routines, enums and structures to support device	*/
/* control.								*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 lpc_ioctl.h 30-Dec-2003,10:34:14,`RADSETT' First archival version.
*   2 lpc_ioctl.h 13-Jul-2004,09:01:56,`RADSETT' Add structure and define for
*        interrupt ioctl.
*        Add include of lpc_sys.h for definitions needed in some structures.
*   3 lpc_ioctl.h 16-Sep-2004,14:21:38,`RADSETT' Add support for char waiting
*        ioctl
*   4 lpc_ioctl.h 11-Mar-2005,10:37:10,`RADSETT' Add support for SPI and zero
*        buffering.
*   TLIB revision history ends.
*/
#ifndef LPC_IOCTL__H
#define LPC_IOCTL__H

#include "lpc_sys.h"

	/**** Request defintions. These define the requested actions.****/
#define UART_SETUP		(0x2100uL)
#define UART_CHAR_WAITING	(0x2105uL)
#define SPI_SETUP		(0x210AuL)
#define INTERRUPT_SETUP		(0x2110uL)
#define BUFFER_SETUP		(0x2115uL)
#define BLOCKING_SETUP		(0x211AuL)
#define DEVICE_SELECT		(0x2120uL)
#define DEVICE_INIT		(0x2125uL)
#define DEVICE_ADD		(0x212AuL)
#define DEVICE_SEEK		(0x2130uL)


	/**** UART Defintions. ****/

	
#define UART_STOP_BITS_2	(4u)	/*  Sets to provide 2 stop bits */
#define UART_STOP_BITS_1	(0u)	/*  Sets to provide 1 stop bit	*/

	
#define UART_PARITY_NONE	(0u)	/*  Set to parity None		*/
#define UART_PARITY_ODD		(8u)	/*  Set to parity Odd		*/
#define UART_PARITY_EVEN	(0x18u)	/*  Set to parity Even		*/
#define UART_PARITY_STICK1	(0x28u)	/*  Set to parity stuck on	*/
#define UART_PARITY_STICK0	(0x38u)	/*  Set to parity stuck off	*/

#define UART_WORD_LEN_5		(0u)	/*  5 bit serial byte.		*/
#define UART_WORD_LEN_6		(1u)	/*  6 bit serial byte.		*/
#define UART_WORD_LEN_7		(2u)	/*  7 bit serial byte.		*/
#define UART_WORD_LEN_8		(3u)	/*  8 bit serial byte.		*/

	/*  serial_param -- structure to pass with ioctl request 	*/
	/* UART_SETUP to control the serial line characteristics.	*/
struct serial_param {
    unsigned long baud;
    unsigned int length;
    unsigned int parity;
    unsigned int stop;
    };

	/*  interrupt_param -- structure to pass with ioctl request 	*/
	/* INTERRUPT_SETUP to setup interrupt characteristics		*/
struct interrupt_param {
    INT_PRIORITY pri;
    unsigned int FIQ;
    };


	/**** SPI Definitions. ****/

	
#define SPI_CPHA_EDGE1		(0)	/*  Data clocked in on first 	*/
					/* clock edge.			*/
#define SPI_CPHA_EDGE2		(8)	/*  Data clocked in on second 	*/
					/* clock edge.			*/
#define SPI_CPOL_HIGH		(0)	/*  Clock polarity high.	*/
#define SPI_CPOL_LOW		(0x10)	/*  Clock polarity low.		*/
#define SPI_BIT_DIR_MSBF	(0)	/*  Data shifted MSB first.	*/
#define SPI_BIT_DIR_LSBF	(0x40)	/*  Data shifted LSB first.	*/

	/*  spi_param -- structure to pass with ioctl request SPI_SETUP	*/
	/* to control the SPI line characteristics.			*/
struct spi_param {
	unsigned char cpha;
	unsigned char cpol;
	unsigned char lsbf;
	unsigned long hz;
	};



	/**** Device Selection Definitions ****/

	/*  device_select -- structure to pass with ioctl request	*/
	/* DEVICE_SELECT to select an external device.  Meant to be	*/
	/* used by a sub-device (or user) when calling a low level	*/
	/* device to do I/O.						*/
struct device_select{
	/*  Number of the device.  May be an I/O line to act as a /CS 	*/
	/* (i.e. SPI) or a device number to be used by the low level	*/
	/* protocol (i.e. I2C).						*/
    unsigned int device_number;
    unsigned char select_action;	/*  Select or de-select.	*/
    };

#define SELECT_ACTION_SEL	(1u)	/*  Select the device.		*/
#define SELECT_ACTION_DESEL	(2u)	/*  De-select the device.	*/
#define SELECT_ACTION_INIT	(3u)	/*  Do any preparation or setup */
					/* needed to use this select	*/
					/* line.			*/


	/**** Buffering Definitions. ****/

	
#define BUFFER_INTERNAL		(0)	/*  Default internal buffer.	*/

		/*  Zero buffering.  Note any driver specific cautions,	*/
		/* particularly with respect to when the data is 	*/
		/* considered modifiable by the driver.			*/
#define BUFFER_ZERO		(1)	

					
	/*  buffer_param -- structure to pass with ioctl request 	*/
	/* BUFFER_SETUP to control the SPI line characteristics.	*/
struct buffer_param {
	unsigned int type;
	};

	/**** Blocking Definitions. ****/


		/*  I/O blocks until finished or completely buffered	*/
		/* internally.						*/
#define BLOCKING_IO_YES		(0)	
#define BLOCKING_IO_NO		(1)	/* I/O returns immediately.	*/
					


	/**** Seek Definitions. ****/


struct device_seek {
    _off_t off;		/* Offset for determining nest file position.	*/
    int dir;		/* Direction to move position in file.		*/
    };

/************************** _ioctl_r ************************************/
/*  Support function.  Device specific control.				*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  int fd		-- number referring to the open file. Generally	*/
/*			obtained from a corresponding call to open.	*/
/*  unsigned long request -- Number to indicate the request being made 	*/
/*			of the driver.					*/
/*  void *ptr		-- pointer to data that is either used by or 	*/
/*			set by the driver.  Varies by request.		*/
/*  Returns 0 if successful, otherwise errno will be set to indicate 	*/
/* the source of the error.						*/
int _ioctl_r( struct _reent *r, int fd, unsigned long request, void *ptr);

/************************** ioctl ***************************************/
/*  Support function.  Device specific control.  A shell to convert	*/
/* requests into a re-entrant form.					*/
/*  int fd		-- number referring to the open file. Generally	*/
/*			obtained from a corresponding call to open.	*/
/*  unsigned long request -- Number to indicate the request being made 	*/
/*			of the driver.					*/
/*  void *ptr		-- pointer to data that is either used by or 	*/
/*			set by the driver.  Varies by request.		*/
/*  Returns 0 if successful, otherwise errno will be set to indicate 	*/
/* the source of the error.						*/
int ioctl( int fd, unsigned long request, void *ptr);

#endif /* LPC_IOCTL__H */

