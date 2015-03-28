/**************************** _read_r.c *********************************/
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
/*  Newlib support routine for reading from a file or device.  Depends 	*/
/* on the underlying drivers to do most of the actual work.  This just 	*/
/* provides a translation layer between the drivers and newlib proper.	*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 _read_r.c 30-Dec-2003,10:34:12,`RADSETT' First archival version.
*   2 _read_r.c 18-Feb-2005,12:16:16,`RADSETT' Per instance information added to
*        device driver call.
*   3 _read_r.c 01-Jan-2006,12:01:54,`RADSETT' Remove cdefs.h include.
*   TLIB revision history ends.
*/
#include <unistd.h>
#include "dev_cntrl.h"

/************************** _read_r *************************************/
/*  Support function.  Reads from a file/device given its file number.	*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  int file		-- number referring to the open file. Generally	*/
/*			obtained from a corresponding call to open.	*/
/*  void *ptr		-- memory area to place read bytes into. 	*/
/*  size_t len		-- maximum number of bytes to read.		*/
/*  Returns number of bytes read. (_ssize_t)-1 on error.		*/
_ssize_t _read_r(
    struct _reent *r, 
    int file, 
    void *ptr, 
    size_t len)
{
 static int standard_input_init = 0;	/*  Indicates if stdin has been	*/
					/* initialized.			*/

	/*  Check to see if the file handle refers to stdin.  If so	*/
	/* open it.  This is needed since stdin may be read from 	*/
	/* without opening it.  On a fully hosted system it would 	*/
	/* normally be opened as part of the program load.  We don't	*/
	/* have that luxury here.					*/
 if( file == STDIN_FILENO) {
    if( standard_input_init == 0) {

		/*  If stdin cannot be opened give up and complain.	*/
         if( device_table[DEVICE(file)]->open( r, "", 0, 0, 
              device_table[DEVICE(file)]->info ) < 0) {

	      return (_ssize_t)-1;
	      }
	 standard_input_init = 1;
	 }
    }

	/*  Pass call on to device driver.  Return result.		*/
 return device_table[DEVICE(file)]->read( r, SUB_FILE(file), ptr, len, 
    device_table[DEVICE(file)]->info );
}

