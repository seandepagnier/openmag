/**************************** sys_vpb.c *********************************/
/* Copyright 2003/12/28 Aeolus Development				*/
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
/*  Wrapper around VPB clock rate.					*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 sys_vpb.c 30-Dec-2003,10:34:12,`RADSETT' First archival version.
*   2 sys_vpb.c 30-Jan-2004,14:13:14,`RADSETT' Add note on double read of VPBDiv.
*   3 sys_vpb.c 14-Jun-2004,15:03:44,`RADSETT' Remove use of struct _reent
*        argument so this
*        can be decoupled from the newlib support.
*   TLIB revision history ends.
*/
#include <stdio.h>
#include "lpc210x.h"
#include "lpc_sys.h"
#include "errno.h"

	/**** Local Macros ****/
#define VPB_DIV_QUARTER	((unsigned char)0)	/*  Sets VPB clock to 	*/
						/* 1/4 cclk.		*/
#define VPB_DIV_HALF	((unsigned char)2)	/*  Sets VPB clock to 	*/
						/* 1/2 cclk.		*/
#define VPB_DIV_ONE	((unsigned char)1)	/*  Sets VPB clock to 	*/
						/* cclk.		*/

	/**** Local Prototypes ****/
static VPB_param VPBQuery( void);

/********************* VPBQuery *****************************************/
/*  VPBQuery -- Finds and returns the divider of the clock from the CPU */
/* to the peripheral bus.  						*/
static VPB_param VPBQuery( void)
{

 (void)VPBDIV;	/*  Dummy read, single doesn't always work?		*/
		/*  Apparently a real chip problem but no publically	*/
		/* available documentation yet.  			*/
		/* See http://groups.yahoo.com/group/lpc2100/ A search	*/
		/* for VPB should reveal the discussion. 		*/

	/*  Read divisor and convert to internal representation.	*/
 switch( VPBDIV & 0x3) {
    default:
    case VPB_DIV_ONE:
	 return( VPB_DIV1);

    case VPB_DIV_HALF:
	 return( VPB_DIV2);

    case VPB_DIV_QUARTER:
	 return( VPB_DIV4);
    }
}

/********************* VPBControl ***************************************/
/*  VPBControl -- Control the clock divider on the peripheral bus.	*/
/* Returns the actual divider in ptr (requested is also passed in ptr.	*/
/*  VPB_parm p		-- requested VPB to CPU freq rate.		*/
/*  Returns 0 if successful.						*/
int VPBControl( VPB_param p) 
{

 switch( p) {
    case VPB_DIV1:
	 VPBDIV = VPB_DIV_ONE;
	 break;

     case VPB_DIV2:
	 VPBDIV = VPB_DIV_HALF;
	 break;

     case VPB_DIV4:
	 VPBDIV = VPB_DIV_QUARTER;
	 break;

     default:
    	 return EINVAL;
    }
 return 0;
}


/********************* VPBRate ******************************************/
/*  VPBRate -- Finds and returns the rate of the clock on the 		*/
/* peripheral bus (in Hz).  						*/
unsigned long VPBRate( void)
{
 VPB_param vpb;

 vpb = VPBQuery();	/*  Find current divisor.			*/

	/*  Use divisor to convert from CPU speed in kHz to VPB speed 	*/
	/* in Hz.							*/
 switch( vpb) {
    case VPB_DIV1:
         return( ActualSpeed()*1000uL);

    case VPB_DIV2:
         return( ActualSpeed()*500uL);

    case VPB_DIV4:
         return( ActualSpeed()*250uL);
    }
return 0;
}
