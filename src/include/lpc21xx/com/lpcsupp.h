/**************************** lpc_supp.h ********************************/
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
/*  Various useful extensions.						*/
/************************************************************************/
/*
*   TLIB revision history:
*   1 lpcsupp.h 06-Nov-2004,17:18:54,`RADSETT' Original archived version.
*   TLIB revision history ends.
*/
#ifndef LPCSUPP__H
#define LPCSUPP__H

/************************** chars_waiting *******************************/
/*  Support function.  How many characters waiting to be read.		*/
/*  int fd		-- number referring to the open file. Generally	*/
/*			obtained from a corresponding call to open.	*/
/*  Returns 0 if no characters are waiting.  If the device does not 	*/
/* support the call then it will always indicated that a character is	*/
/* waiting which may lead to blocking behaviour.  In that case errno	*/
/* will be set to indicated that an unsupported call was made.		*/
int chars_waiting(
    int fd);


#endif /*LPCSUPP__H*/
