/**************************** sbdvsp.c **********************************/
/* Copyright 2006/01/01 Aeolus Development				*/
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
/*  Newlib support routine providing support routines for sub-devices.	*/
/* Theses are devices that use other device drivers to provide some of	*/
/* the support services they need.  FOr instance A/D and non-volatile	*/
/* storage devices using SPI might use the same underlying driver 	*/
/* while providing an interface themselves.				*/
/*  These routines are support routines that if not present the drivers	*/
/* would need to implement themselves.					*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 sbdvsp.c 01-Jan-2006,10:13:00,`RADSETT' Original version.
*   TLIB revision history ends.
*/
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "dev_cntrl.h"

	/**** Local Prototypes ****/
static int confirm_table_entry( int file_num, SUB_DEVICE_INFO *inf, OPEN_DEVICE open_list[], OPEN_DEVICE_INDEX num);
static int discard_table_entry( OPEN_DEVICE_INDEX dev_in, OPEN_DEVICE open_list[], OPEN_DEVICE_INDEX num);
static OPEN_DEVICE_INDEX find_table_entry( int filenum, OPEN_DEVICE open_list[], OPEN_DEVICE_INDEX num);
static int reserve_table_entry( OPEN_DEVICE open_list[], OPEN_DEVICE_INDEX num);


/********************* find_table_entry *********************************/
/*  find_table_entry	-- Given a subdevice number and the table it	*/
/* refers to verify and look up the corresponding index.		*/
/*  int filenum		-- number referring to the subdevice we are	*/
/*			dealing with. Generally	obtained from a		*/
/*			corresponding call to open.			*/
/*  OPEN_DEVICE open_list[] -- array of OPEN_DEVICEs holding the open 	*/
/*			sub_devices of this device.		 	*/
/*  size_t num		-- the maximum number of sub_devices of this	*/
/*			(sub)device that may be open at once.		*/
/*  Returns an index into open_list if successful, otherwise returns a	*/
/* value < 0.							  	*/
static OPEN_DEVICE_INDEX find_table_entry( int filenum, OPEN_DEVICE open_list[], 
    OPEN_DEVICE_INDEX num)
{

	/*  Since 0 is reserved for the raw entry and any value over	*/
	/* num will be off the end of the list any value not in that	*/
	/* range is an error.						*/
 if( (filenum <= 0) || (filenum > (int)num)) {
    return -1;
    }

	/*  Convert the filenumber to an index and check that is open	*/
	/* before returning it.						*/
 if( open_list[FILENUM_TO_ODI( filenum)].status != DEVICE_OPEN) {
    return -1;
    }

 return FILENUM_TO_ODI( filenum);
}

/********************* reserve_table_entry ******************************/
/*  reserve_table_entry	-- Reserve an entry in an open list for opening	*/
/* a sub_device.							*/
/*  OPEN_DEVICE open_list[] -- array of OPEN_DEVICEs holding the open 	*/
/*			sub_devices of this device.		 	*/
/*  OPEN_DEVICE_INDEX num   -- the maximum number of sub_devices of	*/
/*			this (sub)device that may be open at once.	*/
/*  Returns a number that may be used as part of a valid file number if	*/
/* successful, otherwise returns a value < 0.			  	*/
static int reserve_table_entry( OPEN_DEVICE open_list[], OPEN_DEVICE_INDEX num)
{
 OPEN_DEVICE_INDEX i;

 if( (int)num > SUB_DEVICE_MAX) {/*  Verify table is of reasonable size.*/
    return -1;
    }

	/*  Walk through the list looking for an available entry.  If	*/
	/* an entry is avalable reserve it and return the index after	*/
	/* converting it to a file number.				*/
 for( i = 0; i < num; i++) {
    if( open_list[i].status == DEVICE_AVAILABLE) {
         open_list[i].status = DEVICE_CLAIMED;
	 return ODI_TO_FILENUM(i);
	 }	   
    }
 return -1;
}

/********************* confirm_table_entry ******************************/
/*  confirm_table_entry	-- Given a subdevice number and the table it	*/
/* refers to change the devices status to open.				*/
/*  int file_num	-- number referring to the subdevice we are	*/
/*			dealing with. Generally	obtained from a		*/
/*			corresponding call to open.			*/
/*  SUB_DEVICE_INFO *inf    -- The information needed to describe a	*/
/*			sub-device.					*/
/*  OPEN_DEVICE open_list[] -- array of OPEN_DEVICEs holding the open 	*/
/*			sub_devices of this device.		 	*/
/*  OPEN_DEVICE_INDEX num   -- the maximum number of sub_devices of	*/
/*			this (sub)devices that may be open at once.	*/
/*  Returns 0 if successful, otherwise returns a value < 0.	  	*/
static int confirm_table_entry( 
    int file_num, 
    SUB_DEVICE_INFO *inf,
    OPEN_DEVICE open_list[], 
    OPEN_DEVICE_INDEX num)
{
	/*  Since 0 is reserved for the raw entry and any value over	*/
	/* num will be off the end of the list any value not in that	*/
	/* range is an error.						*/
 if( (file_num <= 0) || (file_num > (int)num)) {
    return -1;
    }

	/*  Not much to do really.  Just fill in the remaining field	*/
	/* and mark as open.						*/
 open_list[FILENUM_TO_ODI(file_num)].device = inf;
 open_list[FILENUM_TO_ODI(file_num)].status = DEVICE_OPEN;
 return 0;
}

/********************* discard_table_entry ******************************/
/*  discard_table_entry	-- Given a subdevice number and the table it	*/
/* refers to change the devices status to available.  Called either as	*/
/* a result of an error during an open call or as the cleanup during a	*/
/* close.				 				*/
/*  OPEN_DEVICE_INDEX dev_in -- number referring to the subdevice we	*/
/*			are dealing with. Generally obtained from a	*/
/*			corresponding call to find_table_entry.		*/
/*  OPEN_DEVICE open_list[] -- array of OPEN_DEVICEs holding the open 	*/
/*			sub_devices of this device.		 	*/
/*  OPEN_DEVICE_INDEX num   -- the maximum number of sub_devices of	*/
/*			this (sub)device that may be open at once.	*/
/*  Returns 0 if successful, otherwise returns a value < 0.	  	*/
static int discard_table_entry( OPEN_DEVICE_INDEX dev_in, OPEN_DEVICE open_list[], 
    OPEN_DEVICE_INDEX num)
{
 if( (dev_in < 0) || (dev_in >= num)) {
    return -1;
    }

 open_list[dev_in].status = DEVICE_AVAILABLE;

 return 0;
}

/********************* InitOpenSubDeviceList ****************************/
/*  InitOpenSubDeviceList -- Initializes an open sub-device list.	*/
/* Basically marks all entries as available.				*/
/*  OPEN_DEVICE open_list[] -- array of OPEN_DEVICEs holding the open 	*/
/*			sub_devices of this device.		 	*/
/*  OPEN_DEVICE_INDEX num   -- the maximum number of sub_devices of	*/
/*			this (sub)device that may be open at once.	*/
void InitOpenSubDeviceList( 
    OPEN_DEVICE open_list[], 
    OPEN_DEVICE_INDEX num)
{
 OPEN_DEVICE_INDEX i;

 for( i = 0; i < num; i++) {
    open_list[i].device = 0;
    open_list[i].status = DEVICE_AVAILABLE;
    }
}

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
    OPEN_DEVICE_INDEX num)	/*  Number of entries in above list.	*/
{
 OPEN_DEVICE_INDEX i;

	/*  Find which entry in the open list refers to the sub-device.	*/
 i = find_table_entry(DEVICE(file), open_list, num);
 if( i < 0) {
    r->_errno = EBADF;		/* Bad file number.			*/
    return (_ssize_t)-1;
    }

 file = SUB_FILE( file);	/*  Remove callers portion of the file 	*/
 				/* leaving all subdevices.		*/

	/*  Check for and call the read operation for the sub-device.	*/
 if( open_list[i].device->entry->read != 0) {
    return open_list[i].device->entry->read( r, file, ptr, len, open_list[i].device);
    }

 r->_errno = ENODEV;	/*  Device does not support read calls.		*/
 return (_ssize_t)-1;	/*  Indicate error.				*/
}

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
    OPEN_DEVICE_INDEX num)	/*  Number of entries in above list.	*/
{
 OPEN_DEVICE_INDEX i;

	/*  Find which entry in the open list refers to the sub-device.	*/
 i = find_table_entry(DEVICE(file), open_list, num);
 if( i < 0) {
    r->_errno = EBADF;		/* Bad file number.			*/
    return (_ssize_t)-1;
    }

 file = SUB_FILE( file);	/*  Remove callers portion of the file 	*/
 				/* leaving all subdevices.		*/

	/*  Check for and call the write operation for the sub-device.	*/
 if( open_list[i].device->entry->write != 0) {
    return open_list[i].device->entry->write( r, file, ptr, len, open_list[i].device);
    }

 r->_errno = ENODEV;	/*  Device does not support write calls.	*/
 return (_ssize_t)-1;	/*  Indicate error.				*/
}

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
    OPEN_DEVICE_INDEX num)	/*  Number of entries in above list.	*/
{
 OPEN_DEVICE_INDEX i;

	/*  Find which entry in the open list refers to the sub-device.	*/
 i = find_table_entry(DEVICE(file), open_list, num);
 if( i < 0) {
    r->_errno = EBADF;		/* Bad file number.			*/
    return -1;
    }

 file = SUB_FILE( file);	/*  Remove callers portion of the file 	*/
 				/* leaving all subdevices.		*/

	/*  Check for and call the ioctl operation for the sub-device.	*/
 if( open_list[i].device->entry->ioctl != 0) {
    return open_list[i].device->entry->ioctl( r, file, request, ptr, 
         open_list[i].device);
    }

 r->_errno = ENODEV;	/*  Device does not support ioctl calls.	*/
 return -1;		/*  Indicate error.				*/
}

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
    OPEN_DEVICE_INDEX num)	/*  Number of entries in above list.	*/

{
 const char *name_end;
 int open_val, dev_entry;

	/*  First make sure that we have been passed an actual name to 	*/
	/* use.								*/
 if( (name == 0) || (*name == '\0')) {
    r->_errno = ENOENT;		/* No such file or directory.		*/
    return( -1);
    }

	/*  Reserve an entry in the open list.  If there is no room 	*/
	/* declare an error.						*/
 dev_entry = reserve_table_entry( open_list, num);
 if( dev_entry < 0) {
    r->_errno = ENFILE;
    return -1;
    }

	/*  Now find first / in name.  If there isn't one point at the	*/
	/* terminating 0.  In either case we are now one past the last	*/
	/* character in the device name so we back up one.		*/
 name_end = strchr( name, '/');
 if( name_end == 0) {
    name_end = name + strlen( name);
    }
 name_end--;

	/*  Now search through the linked list to see if there is a 	*/
	/* matching device.  						*/
	/*  When a device in the list matches the name we have been 	*/
	/* given pass our argument (with device name stripped from the 	*/
	/* name) to the device drivers open function.  The result from 	*/
	/* this call is combined with the open table number to form a	*/
	/* file number for subsequent operations.			*/
 for( ; head != 0; head = head->next) {

    if( CheckFilenameSubMatch( name, name_end, head->info.entry->name) == 0) {
	 name_end++;
	 if( *name_end == '/') {
	      name_end++;
	      }

		/*  Attempt to open found sub-device.			*/
	 open_val = head->info.entry->open( r, name_end, flags, mode, &(head->info));
	 if( open_val < 0) {
	      (void)discard_table_entry( FILENUM_TO_ODI(dev_entry), open_list, num);
	      return open_val;
	      }

         (void)fflush( stdout);
		/*  Confirm device open in the open table and build	*/
		/* file number.						*/
	 if( confirm_table_entry( dev_entry, &(head->info), open_list, num) < 0) {
              r->_errno = EBADF;	/*  Something is seriously wrong*/
              				/* if we get here and an error	*/
              				/* occurs.			*/
              return -1;
	      }
	 open_val = BUILD_FILE_NUMBER( open_val, dev_entry);

         (void)fflush( stdout);
	 return( open_val);
	 }
    }

 r->_errno = ENOENT;		/*  No such file or directory.		*/
 return( -1);			/*  Couldn't find device.		*/
}

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
    OPEN_DEVICE_INDEX num)	/*  Number of entries in above list.	*/
{
 int ret_val;
 OPEN_DEVICE_INDEX i;

	/*  Find which entry in the open list refers to the sub-device.	*/
 i = find_table_entry(DEVICE(file), open_list, num);
 if( i < 0) {
    r->_errno = EBADF;		/* Bad file number.			*/
    return -1;
    }

 file = SUB_FILE( file);	/*  Remove callers portion of the file 	*/
 				/* leaving all subdevices.		*/

	/*  Check for and call the close operation for the sub-device.	*/
 if( open_list[i].device->entry->close != 0) {
    ret_val = open_list[i].device->entry->close( r, file, open_list[i].device);
    (void)discard_table_entry( i, open_list, num);
    return ret_val;
    }

 r->_errno = ENODEV;	/*  Device does not support close calls.	*/
 return -1;		/*  Indicate error.				*/
}
