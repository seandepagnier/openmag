/**************************** charwait.c ********************************/
/* Copyright 2004/09/16 Aeolus Development				*/
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
/*  Support routine for determining if any characters are waiting to be	*/
/* read. Used to prevent blocking in a single threaded environment.	*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 charwait.c 06-Nov-2004,15:12:28,`RADSETT' First archived version.
*   TLIB revision history ends.
*/
#include <errno.h>
#include "dev_cntrl.h"
#include "lpc_ioctl.h"
#include "lpcsupp.h"

/************************** chars_waiting *******************************/
/*  Support function.  How many characters waiting to be read.		*/
/*  int fd		-- number referring to the open file. Generally	*/
/*			obtained from a corresponding call to open.	*/
/*  Returns 0 if no characters are waiting.  If the device does not 	*/
/* support the call then it will return < 0.  In that case errno	*/
/* will be set to indicated that an unsupported call was made.		*/
int chars_waiting(
    int fd)
{
int num_waiting;

	/* Make call to driver. In case of error arbitrarily set number */
	/* of waiting characters to -1.					*/
if( ioctl( fd,  UART_CHAR_WAITING, &num_waiting) != 0) {
    return -1;
    }
return num_waiting;
}

