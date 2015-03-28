/**************************** sys_mam.c *********************************/
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
/*  A shallow wrapper around the MAM device.  A little bit of error	*/
/* checking.								*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 sys_mam.c 30-Dec-2003,10:34:12,`RADSETT' First archival version.
*   2 sys_mam.c 14-Jun-2004,14:55:16,`RADSETT' Remove use of struct _reent
*        argument so this
*        can be decoupled from the newlib support.
*   TLIB revision history ends.
*/
#include <errno.h>
#include "lpc210x.h"
#include "lpc_sys.h"

/********************* SetMAM *******************************************/
/*  SetMAM -- Set up the MAM.  Minimal error checking, not much more 	*/
/* than a wrapper around the register.  Returns 0 if successful, 	*/
/* something else.  Sets errno in case of an error.			*/
/*  unsigned int cycle_time -- number of cycles to access the flash.	*/
/*  MAM_CONTROL ctrl	-- Mode to place MAM in.  One of:		*/
/*				MAM_disabled, MAM_part_enable, or 	*/
/*				MAM_full_enable.			*/
/*  Returns 0 if successful. Additional error/sanity checks are possible*/
int SetMAM( 
    unsigned int cycle_time,
    MAM_CONTROL ctrl)
{
 if( cycle_time > 7) {			/*  Cycle time is limited, 	*/
    return EINVAL;			/* complain if set too high.	*/
    }

 MAMCR = (unsigned char)ctrl;		/*lint !e930 cast from enum 	*/
 MAMTIM = (unsigned char)cycle_time;

 return 0;
}
