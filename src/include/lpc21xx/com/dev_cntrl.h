/**************************** dev_cntrl.h *******************************/
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
/*  Provides definitions and prototypes for device drivers to run with	*/
/* newlib.								*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 dev_cntrl.h 30-Dec-2003,10:34:12,`RADSETT' First archival version.
*   2 dev_cntrl.h 13-Jul-2004,11:32:26,`RADSETT' Add interrupt driven com1 device.
*   3 dev_cntrl.h 06-Nov-2004,15:33:44,`RADSETT' Add support for UART1 device
*        drivers contributed by Cyril Holweck.
*   4 dev_cntrl.h 01-Jan-2006,11:59:04,`RADSETT' Add suport for sub devices.
*   TLIB revision history ends.
*/

#ifndef DEV_CNTRL__H
#define DEV_CNTRL__H

#include <reent.h>

	/*  Structure used to define per instance information for a 	*/
	/* device.							*/
typedef struct {
	/*  Device driver description table.				*/
    const struct device_table_entry *entry;
	/*  Device driver description table of the device that this	*/
	/* device uses for support.					*/
    const struct device_table_entry *parent;
	/*  Device number.  Chip select, index or some other 		*/
	/* distinguishing information.  Interpretation is device	*/
	/* specific.							*/
    unsigned int device_number;
    void *device_info;	/*  Any other instance specific data needed.	*/
    } SUB_DEVICE_INFO;

	/*  Structure used to define a device driver.			*/
struct device_table_entry {
    const char *name;		/*  Device name.			*/

	/*  Device open method.						*/
    int (*open)(struct _reent *r,const char *name, int flags, int mode, SUB_DEVICE_INFO *info);

	/*  Device close method.					*/
    int (*close)(struct _reent *r,int file, SUB_DEVICE_INFO *info);

	/*  Device read method.						*/
    _ssize_t (*read)(struct _reent *r,int file, void *ptr, size_t len, SUB_DEVICE_INFO *info);

	/*  Device write method.					*/
    _ssize_t (*write)(struct _reent *r,int file, const void *ptr, size_t len, SUB_DEVICE_INFO *info);

	/*  Device ioctl method (controls device operating parameters.	*/
    int (*ioctl)(struct _reent *r, int file, unsigned long request, void *ptr, SUB_DEVICE_INFO *info);

    SUB_DEVICE_INFO *info;
    };

	/*  List of devices.  Newlib adapatation uses this to determine	*/
	/* what devices are available and how to control them.  List	*/
	/* is terminated by a null pointer entry.			*/
	/*  Note:  Entries 0, 1, 2 are reserved to address stdin, 	*/
	/* stdout and stderr.  These must be able to work from a 	*/
	/* standard open w/o needing further setup if they are to be	*/
	/* transparent, alternatively they must be setup before any I/O	*/
	/* is done.							*/
extern const struct device_table_entry *device_table[];

	/*  Macros to split file number into two pieces.  The low order	*/
	/* n bits are used to index the specific device or sub-device 	*/
	/* being addressed (hopefully 2^n devices is an unreasonably 	*/
	/* large limit). The remaining bits are used in a device	*/
	/* specific fashion to allow each device driver to address	*/
	/* multiple devices or files.  These macros can be used by	*/
	/* successive drivers to split the file number into multiple	*/
	/* pieces. 							*/
#define BITS_PER_DEVICE_FIELD	(8)	/*  8 bits (256) per field.	*/
	/*  Mask off lowest order field.				*/
#define DEVICE(x)	((x) & ((1 << BITS_PER_DEVICE_FIELD)-1))
	/*  Remove lowest order field.					*/
	/*lint -emacro(702,SUB_FILE) right shift of signed qty		*/
#define SUB_FILE(x)	((x) >> BITS_PER_DEVICE_FIELD) 
	/*  Build up a file number from it's components.  Can be used in*/
	/* succesive stages if all devices and subdevices used the field*/
	/* size defined here.						*/
	/*  Since file number is holding both error and non-error 	*/
	/* information it ends up being signed.				*/
	/*lint -emacro(701,BUILD_FILE_NUMBER) left shift of signed qty	*/
#define BUILD_FILE_NUMBER( file_no, device_no)	\
		(((file_no) << BITS_PER_DEVICE_FIELD) | (device_no))
	/*  Maximum number of open subdevices allowed.  Any more than	*/
	/* this will overflow the subdevice field.			*/
#define SUB_DEVICE_MAX	((1 << BITS_PER_DEVICE_FIELD)-2)

	/*  Device status, the state a device entry may be in.		*/
typedef enum {
    DEVICE_AVAILABLE,	/*  Available for use.				*/
    DEVICE_CLAIMED,	/*  Something is in the process of setting it up*/
			/* or tearing it down.				*/
    DEVICE_OPEN		/*  Open.					*/
    } DEVICE_STATUS;


	/* Device chain.  Used to create lists of sub devices.		*/
typedef struct device_chain SUB_DEVICE_CHAIN;

	/* Structure to implement a device chain link.			*/
struct device_chain {
    SUB_DEVICE_INFO info;	/*  Information on a specific device.	*/
    SUB_DEVICE_CHAIN *next;	/*  Next device in the list.		*/
    };

	/* Open Device. A structure that defines an open device.	*/
typedef struct {
    SUB_DEVICE_INFO *device;	/*  Pointer to information describing 	*/
    				/* the device.				*/
    DEVICE_STATUS status;	/*  Status of the device.  Note that 	*/
    				/* the device field can only be 	*/
    				/* considered valid if this is 		*/
    				/* DEVICE_OPEN.				*/
    } OPEN_DEVICE;

	/* Type used to index arrays of OPEN_DEVICE.			*/
	/*lint -strong(AzcXJce, OPEN_DEVICE_INDEX)	*/
typedef int OPEN_DEVICE_INDEX;

	/* Set up lint to check indexing of OPEN_DEVICE arrays.		*/
	/*lint -index( d, OPEN_DEVICE_INDEX, OPEN_DEVICE) */


	/*  Macros to convert between indexes into arrays of 		*/
	/* OPEN_DEVICE and (sub)file numbers.				*/
#define FILENUM_TO_ODI( num)	((OPEN_DEVICE_INDEX)((num)-1))
#define ODI_TO_FILENUM( di)	((int)(di)+1)

/********************* InitOpenSubDeviceList ****************************/
/*  InitOpenSubDeviceList -- Initializes an open sub-device list.	*/
/* Basically marks all entries as available.				*/
/*  OPEN_DEVICE open_list[] -- array of OPEN_DEVICEs holding the open 	*/
/*			sub_devices of this device.		 	*/
/*  OPEN_DEVICE_INDEX num   -- the maximum number of sub_devices of	*/
/*			this (sub)device that may be open at once.	*/
void InitOpenSubDeviceList( 
    OPEN_DEVICE open_list[], 
    OPEN_DEVICE_INDEX num);

/************************** SubDeviceOpen *******************************/
/*  Support function.  Opens a file/device given its name.		*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  const char *name	-- name of file/device to open. The name is of	*/
/*			the form dev_name or dev_name/sub_dev.  Anything*/
/*			before the optional forward slash is compared 	*/
/*			against the devices in the driver table.  If a	*/
/*			match is found anything after thr first forward	*/
/*			slash is passed on to the device driver for it	*/
/*			to interpret.					*/
/*  int flags		-- Flags to control open.  Not used at the 	*/
/*			moment.						*/
/*  int mode		-- Mode to open in.  Not used at the moment.	*/
/*  SUB_DEVICE_CHAIN *head -- linked list of subdevices available to 	*/
/*			open.						*/
/*  OPEN_DEVICE open_list[] -- array of OPEN_DEVICEs holding the open 	*/
/*			sub_devices of this device.		 	*/
/*  size_t num		-- the maximum number of sub_devices of this	*/
/*			(sub)devices that may be open at once.		*/
/*  Returns file number >= 0 if successful.  Otherwise the error code 	*/
/* may be found in errno.						*/
int SubDeviceOpen( 		
    struct _reent *r,		/*  Re-entrancy structure, used to make	*/
				/* thread safe.				*/
    const char *name, 		/*  Name to open.			*/
    int flags,			/*  Flags to control open.		*/
				/* Read, write binary etc...		*/
				/* See flags.c for values generated by	*/
				/* newlib.				*/
    int mode,			/*  Mode to open in.  This is a 	*/
    				/* security or permissions value.  	*/
    				/* Newlib uses the classic 0666 for all */
    				/* fopens.  See fopen.c			*/
    SUB_DEVICE_CHAIN *head,	/*  List of subdevices to search for a	*/
				/* match in.				*/
    OPEN_DEVICE open_list[], 	/*  List used to hold open files/devices.*/
    OPEN_DEVICE_INDEX num);	/*  Number of entries in above list.	*/

/********************** CheckFilenameSubMatch ***************************/
/*  Support function.  Checks subsection of a string to see if it	*/
/* matches a device name.						*/
/*  const char *requested_start	-- start of string to check	 	*/
/*  const char *requested_end	-- end of string to check		*/
/*  const char *dev_name	-- name to check string against		*/
/* Returns 0 if string matches dev_name, otherwise non-zero.		*/
int CheckFilenameSubMatch( 
    const char *requested_start, 
    const char *requested_end,
    const char *dev_name);

/************************** SubDeviceClose ******************************/
/*  Support function.  Close an opens file/device.			*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  int file		-- number refering to the file to close.	*/
/*  OPEN_DEVICE open_list[] -- array of OPEN_DEVICEs holding the open 	*/
/*			sub_devices of this device.		 	*/
/*  size_t num		-- the maximum number of sub_devices of this	*/
/*			(sub)devices that may be open at once.		*/
/*  Returns 0 if successful.  Otherwise the error code may be found in	*/
/* errno.								*/
int SubDeviceClose( 		
    struct _reent *r,		/*  Re-entrancy structure, used to make	*/
				/* thread safe.				*/
    int file,	 		/*  File to close.			*/
    OPEN_DEVICE open_list[], 	/*  List used to hold open files/devices.*/
    OPEN_DEVICE_INDEX num);	/*  Number of entries in above list.	*/

/********************* SubDeviceRead ************************************/
/*  SubDeviceRead	-- Given a partial file number (left over from	*/
/* the caller) and a table of open subdevices perform a read of a 	*/
/* subdevice.								*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  int file		-- number referring to the open file. Generally	*/
/*			obtained from a corresponding call to open.	*/
/*  void *ptr		-- memory area to place read bytes into. 	*/
/*  size_t len		-- maximum number of bytes to read.		*/
/*  OPEN_DEVICE open_list[] -- array of OPEN_DEVICEs holding the open 	*/
/*			sub_devices of this device.		 	*/
/*  OPEN_DEVICE_INDEX num   -- the maximum number of sub_devices of	*/
/*			this (sub)device that may be open at once.	*/
/*  Returns number of bytes read. (_ssize_t)-1 on error.		*/
_ssize_t SubDeviceRead (
    struct _reent *r, 		/*  Re-entrancy structure, used to make	*/
				/* thread safe.				*/
    int file,			/*  File handle.  Used to distinguish	*/
				/* multiple instances.  		*/
    void *ptr,		 	/*  Where to place data.		*/
    size_t len,			/*  Max data to read.			*/
    OPEN_DEVICE open_list[], 	/*  List used to hold open files/devices.*/
    OPEN_DEVICE_INDEX num);	/*  Number of entries in above list.	*/

/********************* SubDeviceWrite ***********************************/
/*  SubDeviceWrite	-- Given a partial file number (left over from	*/
/* the caller) and a table of open subdevices perform a write of a 	*/
/* subdevice.								*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  int file		-- number referring to the open file. Generally	*/
/*			obtained from a corresponding call to open.	*/
/*			Only one file number (0) is supported.		*/
/*  const void *ptr	-- memory area to place send bytes from. 	*/
/*  size_t len		-- maximum number of bytes to write.		*/
/*  OPEN_DEVICE open_list[] -- array of OPEN_DEVICEs holding the open 	*/
/*			sub_devices of this device.		 	*/
/*  OPEN_DEVICE_INDEX num   -- the maximum number of sub_devices of	*/
/*			this (sub)device that may be open at once.	*/
/*  Returns number of bytes written. -1 on error.			*/
_ssize_t SubDeviceWrite (
    struct _reent *r,  		/*  Re-entrancy structure, used to make	*/
				/* thread safe.				*/
    int file, 			/*  File handle.  Used to distinguish	*/
				/* multiple instances.  		*/
    const void *ptr,		/*  Pointer to data to write.		*/
    size_t len,			/*  Amount of data to write.		*/
    OPEN_DEVICE open_list[], 	/*  List used to hold open files/devices.*/
    OPEN_DEVICE_INDEX num);	/*  Number of entries in above list.	*/

/********************* SubDeviceIoctl ***********************************/
/*  SubDeviceIoctl	-- Given a partial file number (left over from	*/
/* the caller) and a table of open subdevices perform an ioctl call to 	*/
/* the subdevice.							*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  int file		-- number referring to the open file. Generally	*/
/*			obtained from a corresponding call to open.	*/
/*			Only one file number (0) is supported.		*/
/*  unsigned long request -- simple ordinal indicating what the 	*/
/*			requested action is.				*/
/*  void *ptr		-- pointer to data to read and or write to	*/
/*			perform request.				*/
/*  OPEN_DEVICE open_list[] -- array of OPEN_DEVICEs holding the open 	*/
/*			sub_devices of this device.		 	*/
/*  OPEN_DEVICE_INDEX num   -- the maximum number of sub_devices of	*/
/*			this (sub)device that may be open at once.	*/
/*  Returns 0 if successful.					  	*/
int SubDeviceIoctl (
    struct _reent *r, 
    int file, 
    unsigned long request, 
    void *ptr,
    OPEN_DEVICE open_list[], 	/*  List used to hold open files/devices.*/
    OPEN_DEVICE_INDEX num);	/*  Number of entries in above list.	*/

	/**** Existing device drivers that may be used.	****/
extern const struct device_table_entry com1;	/*  UART0.		*/
extern const struct device_table_entry com1_int;/*  UART0, w interrupt	*/
extern const struct device_table_entry com2;	/*  UART1.		*/
extern const struct device_table_entry com2_int;/*  UART1, w interrupt	*/
extern const struct device_table_entry spi;	/*  SPI0.		*/
extern const struct device_table_entry sys;	/*  SYS -- Essentially 	*/
						/* a dummy.		*/

	/*  Sub-device list */

/*	Device		|	Parent Drivers	|	Header		*/
/*----------------------+-----------------------+-----------------------*/
/*	FM2504		|		spi	|	FM2504.h	*/
/*	FRAM  		|		spi	|	fram.h		*/

#endif /* DEV_CNTRL__H */
