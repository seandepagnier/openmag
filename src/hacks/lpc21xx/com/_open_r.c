/**************************** _open_r.c *********************************/
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
/*  Newlib support routine for open a file or device.  Depends on 	*/
/* underlying drivers to do most of the actual work.  This just 	*/
/* provides a translation layer between the drivers and newlib proper.	*/
/* In addition it separates out the device name from any provided 	*/
/* subdevice name or file.						*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 _open_r.c 30-Dec-2003,10:34:12,`RADSETT' First archival version.
*   2 _open_r.c 18-Feb-2005,12:14:32,`RADSETT' Move file number construction to a
*        macro.
*        Per instance information added to device driver call.
*   TLIB revision history ends.
*/
#include <string.h>
#include <errno.h>
#include "dev_cntrl.h"
//#include "lpc210x.h"

	/**** Local Prototypes ****/
static int check_match( const char *requested_start, const char *requested_end,
    const char *dev_name);

/************************** check_match *********************************/
/*  Local support function.  Checks subsection of a string to see if it */
/* matches a device name.						*/
/*  const char *requested_start	-- start of string to check	 	*/
/*  const char *requested_end	-- end of string to check		*/
/*  const char *dev_name	-- name to check string against		*/
/* Returns 0 if string matches dev_name, otherwise non-zero.		*/
static int check_match( 
    const char *requested_start, 
    const char *requested_end,
    const char *dev_name)
{

	/*  Walk through the string comparing it to dev_name.  Loop	*/
	/* continues until we reach the end of the substring to match	*/
	/* or we have a mismatch.  Pointer comparison in loop is OK	*/
	/* since both pointers must be from the same block.		*/
 while( requested_start <= requested_end) {/*lint !e946 pointer comp	*/
    if( *requested_start != *dev_name) {
         return -1;
	 }
    requested_start++;
    dev_name++;
    }
 if( *dev_name != '\0') {	/*  If we are not at the end of 	*/
    return -1;			/* dev_name then we only matched the 	*/
    }				/* beginning not the whole thing.	*/
 return 0;			
}

/************************** _open_r *************************************/
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
/*  Returns file number >= 0 if successful.  Otherwise the error code 	*/
/* may be found in errno.						*/
int _open_r(		
    struct _reent *r,		/*  Re-entrancy structure, used to make	*/
				/* thread safe.				*/
    const char *name, 		/*  Name to open.			*/
    int flags,			/*  Flags to control open.		*/
				/* Read, write binary etc...		*/
				/* See flags.c for values generated by	*/
				/* newlib.				*/
    int mode)			/*  Mode to open in.  This is a 	*/
    				/* security or permissions value.  	*/
    				/* Newlib uses the classic 0666 for all */
    				/* fopens.  See fopen.c			*/
{
 const char *dev_end;
 int i, sub_file;

	/*  First make sure that we have been passed an actual name to 	*/
	/* use.								*/
 if( (name == 0) || (*name == '\0')) {
    r->_errno = ENOENT;		/* No such file or directory.		*/
    return( -1);
    }

	/*  Now find first / in name.  If there isn't one point at the	*/
	/* terminating 0.  In either case we are now one past the last	*/
	/* character in the device name so we back up one.		*/
 dev_end = strchr( name, '/');
 if( dev_end == 0) {
    dev_end = name + strlen( name);
    }
 dev_end--;

	/*  Now search through the device table to see if there is a 	*/
	/* matching device.  Note:  It is the responsibility of the 	*/
	/* application to provide an appropriate device table.		*/
	/*  When a device in the table matches the name we have been 	*/
	/* given pass our argument (with device name stripped from the 	*/
	/* name) to the device drivers open function.  The result from 	*/
	/* this call is combined with the device drivers table index to */
	/* form a file number for subsequent operations.		*/
 for( i = 0; device_table[i] != 0; i++) {

    if( check_match( name, dev_end, device_table[i]->name) == 0) {
	 dev_end++;
	 if( *dev_end == '/') {
	      dev_end++;
	      }
         sub_file = device_table[i]->open( r, dev_end, flags, mode, 
              device_table[i]->info );
	 if( sub_file < 0) {
	      return( sub_file);
	      }
	 return( BUILD_FILE_NUMBER( sub_file, i));
	 }
    }
 r->_errno = ENOENT;		/*  No such file or directory.		*/
 return( -1);			/*  Couldn't find device.		*/
}

