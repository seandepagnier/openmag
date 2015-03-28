/**************************** sys_dev.c *********************************/
/* Copyright 2003/12/27 Aeolus Development				*/
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
/*  System device.  Right now mostly a place holder.  			*/
/* Provides hooks needed to work as a device driver.		 	*/
/* Note:  All the actual routines are private to this module.  The only */
/* element publically visible is the device table entry structure.	*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 sys_dev.c 30-Dec-2003,10:34:12,`RADSETT' First archival version.
*   2 sys_dev.c 18-Feb-2005,15:29:06,`RADSETT' Add device specific info pointer to
*        device calls.
*   TLIB revision history ends.
*/
#include <errno.h>
#include <limits.h>
//#include "lpc210x.h"
#include "dev_cntrl.h"
#include <stdio.h>

	/**** Local Prototypes ****/
static _ssize_t sys_read ( struct _reent *r, int file, void *ptr, size_t len, SUB_DEVICE_INFO *info);
static _ssize_t sys_write ( struct _reent *r, int file, const void *ptr, size_t len, SUB_DEVICE_INFO *info);
static int sys_open( struct _reent *r, const char *name, int o_flags, int o_mode, SUB_DEVICE_INFO *info);
static int sys_close( struct _reent *r, int file, SUB_DEVICE_INFO *info);

/************************** sys_read ************************************/
/*  Reads from 'sys'.							*/
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
/*  Note:  will only return a single byte on every call.  That byte is 	*/
/* just a dummy.							*/
/*  Returns number of bytes read. (_ssize_t)-1 on error.		*/
/*lint -e{715} multiple arguments not referenced since this is 		*/
/* basically a placeholder.						*/
/*lint -esym(818, info ) info could be declared const.			*/
static _ssize_t sys_read (
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

 p = ptr;
 *p = (unsigned char)'a';	/* Dummy operation for now.		*/
 return (_ssize_t)1;
}

/************************** sys_write ***********************************/
/*  Writes to 'sys'.							*/
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
/*  Note:  simply returns indicating it wrote everything requested. IE	*/
/*lint -e{715} multiple arguments not referenced since this is 		*/
/* basically a placeholder.						*/
/*lint -esym(818, info ) info could be declared const.			*/
static _ssize_t sys_write (
    struct _reent *r,  		/*  Re-entrancy structure, used to make	*/
				/* thread safe.				*/
    int file, 			/*  File handle.  Used to distinguish	*/
				/* multiple instances.  		*/
    const void *ptr,		/*  Pointer to data to write.		*/
    size_t len,			/*  Amount of data to write.		*/
    SUB_DEVICE_INFO *info)	/*  Per instance information, only one	*/
    				/* instance so not used.		*/
 {

 if( file != 0) {		/* Only one device.			*/
    r->_errno = EBADF;		/* Bad file number.			*/
    return (_ssize_t)-1;
    }

 return (_ssize_t)len;
}

/************************** sys_open ************************************/
/*  Opens 'sys'								*/
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
/*lint -e{715} multiple arguments not referenced since this is 		*/
/* basically a placeholder.						*/
/*lint -esym(818, info ) info could be declared const.			*/
static int sys_open(		
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

/************************** sys_close ***********************************/
/*  Close sys.								*/
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
static int sys_close(
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

/************************** sys *****************************************/
/*  Device table entry used to add this device.				*/
const struct device_table_entry sys = {
	"sys",				/*  Device name.		*/
	sys_open, 			/*  Open method.		*/
	sys_close,			/*  Close method.		*/
	sys_read,			/*  Read method.		*/
	sys_write,			/*  Write method.		*/
	0,				/*  ioctl not supported.	*/
	0 };				/*  No per-instance data.	*/
