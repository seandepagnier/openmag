/**************************** _lseek_r.c ********************************/
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
/*  Support routine for changing position in a file.			*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 _lseek_r.c 30-Dec-2003,10:34:12,`RADSETT' First archival version.
*   2 _lseek_r.c 11-Mar-2005,15:24:26,`RADSETT' Add support for seeks in drivers
*        via an ioctl call.
*   TLIB revision history ends.
*/
#include <errno.h>
#include "dev_cntrl.h"
#include "lpc_ioctl.h"

/************************** _lseek_r ************************************/
/*  Support function.  Changes file position.  Since all devices are	*/
/* currently treated as character devices this is a dummy function.	*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  int file		-- number referring to the open file. Generally	*/
/*			obtained from a corresponding call to open.	*/
/*  _off_t ptr		-- offset for determining nest file position.	*/
/*  int dir		-- direction to move position in file.		*/
/* Returns new position in file.					*/
_off_t _lseek_r(
    struct _reent *r, 
    int file, 
    _off_t ptr, 
    int dir)
{
 int val;
 struct device_seek seek;

 seek.off = ptr;		/*  Set up device_seek structure for	*/
 seek.dir = dir;		/* ioctl call.				*/
 val = r->_errno;		/*  Preserve current errno value.	*/

	/*  Attempt ioctl call, if it fails we need to determine why.	*/
 if( ioctl( file, DEVICE_SEEK, &seek) != 0) {

	/*  Ioctl call failed.  First check to see if it failed because	*/
	/* seek is unsupported.  If that's the case fake it by restoring*/
	/* errno and always returning 0.				*/
    if( r->_errno == ENOSYS) {
         r->_errno = val;
	 return (_off_t)0;
	 }

    return (_off_t)-1;	/*  Genuine error, pass it back to the caller.	*/
    }
 return seek.off;	/*  Success return current position.		*/
}

