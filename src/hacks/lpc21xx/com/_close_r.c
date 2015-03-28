/**************************** _close_r.c ********************************/
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
/*  Newlib support routine for closing a file or device.  Depends on 	*/
/* underlying drivers to do the actual work.  This just provides a	*/
/* translation layer between the drivers and newlib proper.		*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 _close_r.c 30-Dec-2003,10:34:12,`RADSETT' First archival version.
*   2 _close_r.c 19-Jul-2004,15:35:12,`RADSETT' add sys prefix to include of
*        unistd.h
*   3 _close_r.c 30-Dec-2005,16:50:00,`RADSETT' Remove cdefs.h include.
*        Per instance information added to device driver call.
*   TLIB revision history ends.
*/
#include <sys/unistd.h>
#include "dev_cntrl.h"

/************************** _close_r ************************************/
/*  Support function.  Closes a file.					*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  int file		-- number referring to the open file. Generally	*/
/*			obtained from a corresponding call to open.	*/
/*  Returns 0 if successful.  Otherwise the error code may be found in 	*/
/* errno.								*/
int _close_r(
    struct _reent *r, 
    int file)
{

	/*  First check that we are not being asked to close stdin, 	*/
	/* stdout or stderr.						*/
 if( (file == STDIN_FILENO) || (file == STDOUT_FILENO) || (file == STDERR_FILENO)) {
    return -1;
    }

	/*  Now pass on request to device driver, decoding the device	*/
	/* index and the file number assigned by the device from the 	*/
	/* file number.  There is probably room here for more sanity 	*/
	/* checks.							*/
 return device_table[DEVICE(file)]->close( r, SUB_FILE(file), 
    device_table[DEVICE(file)]->info );
}
