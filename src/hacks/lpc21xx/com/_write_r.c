/**************************** _write_r.c ********************************/
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
/*  Newlib support routine for writing to a file or device.  Depends 	*/
/* on the underlying drivers to do most of the actual work.  This just 	*/
/* provides a translation layer between the drivers and newlib proper.	*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 _write_r.c 30-Dec-2003,10:34:12,`RADSETT' First archival version.
*   2 _write_r.c 31-Dec-2005,14:56:28,`RADSETT' Per instance information added to
*        device driver call.
*        Remove cdefs.h include.
*   TLIB revision history ends.
*/
#include <unistd.h>
#include "dev_cntrl.h"

/************************** _write_r ************************************/
/*  Support function.  Writes to a file/device given its file number.	*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  int file		-- number referring to the open file. Generally	*/
/*			obtained from a corresponding call to open.	*/
/*  const void *ptr	-- memory area to place read bytes into. 	*/
/*  size_t len		-- maximum number of bytes to write.		*/
/*  Returns number of bytes written. (_ssize_t)-1 on error.		*/
_ssize_t _write_r (
    struct _reent *r, 
    int file, 
    const void *ptr, 
    size_t len)
{
	/*  Indicates if stdout and stderr have been initialized.	*/
 static int standard_output_init = 0;
 static int standard_error_init = 0;

	/*  Check to see if the file handle refers to stdout.  If so	*/
	/* open it.  This is needed since stdout may be written to 	*/
	/* without opening it.  On a fully hosted system it would 	*/
	/* normally be opened as part of the program load.  We don't	*/
	/* have that luxury here.					*/
 if( file == STDOUT_FILENO) {
    if( standard_output_init == 0) {

		/*  If stdout cannot be opened give up and complain.	*/
         if( device_table[DEVICE(file)]->open( r, "", 0, 0, 
              device_table[DEVICE(file)]->info) < 0) {

	      return (_ssize_t)-1;
	      }
	 standard_output_init = 1;
	 }
    }

	/*  Check to see if the file handle refers to stderr.  If so	*/
	/* open it.  This is needed since stderr may be written to 	*/
	/* without opening it.  On a fully hosted system it would 	*/
	/* normally be opened as part of the program load.  We don't	*/
	/* have that luxury here.					*/
 if( file == STDERR_FILENO) {
    if( standard_error_init == 0) {

		/*  If stderr cannot be opened give up and complain.	*/
         if( device_table[DEVICE(file)]->open( r, "", 0, 0, 
              device_table[DEVICE(file)]->info) < 0) {

	      return (_ssize_t)-1;
	      }
	 standard_error_init = 1;
	 }
    }

	/*  Pass call on to device driver.  Return result.		*/
 return device_table[DEVICE(file)]->write( r, SUB_FILE(file), ptr, len, 
    device_table[DEVICE(file)]->info);
}
