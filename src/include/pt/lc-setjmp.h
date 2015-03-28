/*
 * Copyright (c) 2009, Sean D'Epagnier
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Author: Sean D'Epagnier <geckosenator@gmail.com>
 *
 * $Id: lc-setjmp.h,v 1.4 2006/06/03 11:29:43 adam Exp $
 */

/**
 * \addtogroup lc
 * @{
 */

/**
 * \file
 * Implementation of local continuations based on switch() statement
 * which invokes goto to allow switch statements in protothreads.
 * \author Sean D'Epagnier <geckosenator@gmail.com>
 *
 * This implementation of local continuations uses setjmp and longjmp
 * functions to resume execution of a function inside the function's body.
 *
 * The advantage is that switch statements can be freely used in protothreads
 * without compiler support for address labels.  The disadvantage is this
 * method clearly has more overhead.
 */

#ifndef __LC_SETJMP_H__
#define __LC_SETJMP_H__

#include <setjmp.h>

/** \hideinitializer */
typedef struct {jmp_buf buf; char set; } lc_t;

#define LC_INIT(s) s.set = 0;

#define LC_RESUME(s) do { if(s.set) longjmp(s.buf, 0); } while(0)

#define LC_SET(s) do { s.set = 1; setjmp(s.buf); } while(0)

#define LC_END(s)

#endif /* __LC_SETJMP_H__ */

/** @} */
