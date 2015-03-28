/**************************** _fstat_r.c ********************************/
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
/*  Newlib support routine for determining the type of a file or device.*/
/* Simple minded at the moment, all devices are treated as character 	*/
/* devices.								*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 _fstat_r.c 30-Dec-2003,10:34:12,`RADSETT' First archival version.
*   TLIB revision history ends.
*/
#include <sys/stat.h>
#include "dev_cntrl.h"

/************************** _fstat_r ************************************/
/*  Support function.  Determines file status.				*/
/*  struct _reent *r	-- re-entrancy structure, used by newlib to 	*/
/*			support multiple threads of operation.		*/
/*  int file		-- number referring to the open file. Generally	*/
/*			obtained from a corresponding call to open.	*/
/*  struct stat *st	-- Structure to store status info in for caller.*/
/* Returns 0.  Always successful.					*/
/*lint -e{715} not referenced.  Never refer to r			*/
/*lint -e818 pointer parameter could be declared const, but need to	*/
/*  maintain consistency with newlib.					*/
int _fstat_r(
    struct _reent *r, 
    int file, 
    struct stat *st)
{

	/*  Always set as character device.				*/
 st->st_mode = S_IFCHR;	/*lint !e960 !e632 !e915 octal constant 	*/
			/* assigned to strong type with implicit 	*/
			/* signed/unsigned conversion.  Required by 	*/
			/* newlib.					*/
 return 0;
}

