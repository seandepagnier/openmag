/**************************** _ioctl_r.c ********************************/
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
/*  Support routine for setting device specific characteristics such as	*/
/* serial port paramters.  Not provided by newlib but some sort of 	*/
/* mechanism is needed.  This mimics newlibs structure and the ioctl 	*/
/* call used in various flavours of unix.  The possible parameters must	*/
/* be determined for each device driver separately.			*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 _ioctl_r.c 30-Dec-2003,10:34:12,`RADSETT' First archival version.
*   2 _ioctl_r.c 18-Feb-2005,12:12:58,`RADSETT' Per instance information added to
*        device driver call.
*   TLIB revision history ends.
*/
#include <errno.h>
#include "dev_cntrl.h"
#include "lpc_ioctl.h"

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
int _ioctl_r(
    struct _reent *r, 
    int fd,
    unsigned long request, 
    void *ptr)
{

	/*  If the device supprts ioctl calls, make the call to the 	*/
	/* driver decoding the file number into device and sub file	*/
	/* components.							*/
 if( device_table[DEVICE(fd)]->ioctl != 0) {
    return device_table[DEVICE(fd)]->ioctl( r, SUB_FILE(fd), request, ptr,
         device_table[DEVICE(fd)]->info );
    }
 r->_errno = ENOSYS;	/*  Device does not support ioctl calls.	*/
 return -1;		/*  Indicate error.				*/
}

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
int ioctl(
    int fd,
    unsigned long request, 
    void *ptr)
{
	/*  Call re-entrant version with appropriate structure.		*/
  return _ioctl_r (_REENT, fd, request, ptr);
}
