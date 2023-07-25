/*
 *----------------------------------------------------------------------
 *    T2EX Software Package
 *
 *    Copyright 2012 by Ken Sakamura.
 *    This software is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2012/12/12.
 *
 *----------------------------------------------------------------------
 */
/*
 * This software package is available for use, modification, 
 * and redistribution in accordance with the terms of the attached 
 * T-License 2.0.
 * If you want to redistribute the source code, you need to attach 
 * the T-License 2.0 document.
 * There's no obligation to publish the content, and no obligation 
 * to disclose it to the T-Engine Forum if you have modified the 
 * software package.
 * You can also distribute the modified source code. In this case, 
 * please register the modification to T-Kernel traceability service.
 * People can know the history of modifications by the service, 
 * and can be sure that the version you have inherited some 
 * modification of a particular version or not.
 *
 *    http://trace.t-engine.org/tk/?lang=en
 *    http://trace.t-engine.org/tk/?lang=ja
 *
 * As per the provisions of the T-License 2.0, T-Engine Forum 
 * ensures that the the portion of the software that is copyrighted 
 * by Ken Sakamura or the T-Engine Forum does not infringe the 
 * copyrights of a third party.
 * However, it does not make any warranty other than this.
 * DISCLAIMER: T-Engine Forum and Ken Sakamura shall not be held
 * responsible for any consequences or damages caused directly or
 * indirectly by the use of this software package.
 *
 * Materials copyrighted by third parties are included in the 
 * software package. Please use them according to the individual 
 * copyright notice and license for these parts.
 */

/*      $OpenBSD: citrus_ctype_local.h,v 1.2 2010/07/27 16:59:03 stsp Exp $       */
/*      $NetBSD: citrus_ctype_local.h,v 1.2 2003/03/05 20:18:15 tshiozak Exp $  */

/*-
 * Copyright (c)2002 Citrus Project,
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
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#ifndef _CITRUS_CTYPE_LOCAL_H_
#define _CITRUS_CTYPE_LOCAL_H_

#define _CITRUS_CTYPE_DECLS(_e_)					\
size_t	_citrus_##_e_##_ctype_mbrtowc(wchar_t * __restrict,		\
				      const char * __restrict, size_t,	\
				      void * __restrict);		\
int	_citrus_##_e_##_ctype_mbsinit(const void * __restrict);		\
size_t	_citrus_##_e_##_ctype_mbsrtowcs(wchar_t * __restrict,		\
					const char ** __restrict,	\
					size_t, void * __restrict);	\
size_t	_citrus_##_e_##_ctype_wcrtomb(char * __restrict, wchar_t,	\
				      void * __restrict);		\
size_t	_citrus_##_e_##_ctype_wcsrtombs(char * __restrict,		\
					const wchar_t ** __restrict,	\
					size_t, void * __restrict);	\

#define _CITRUS_CTYPE_DEF_OPS(_e_)					\
struct _citrus_ctype_ops_rec _citrus_##_e_##_ctype_ops = {		\
	/* co_mbrtowc */	&_citrus_##_e_##_ctype_mbrtowc,		\
	/* co_mbsinit */	&_citrus_##_e_##_ctype_mbsinit,		\
	/* co_mbsrtowcs */	&_citrus_##_e_##_ctype_mbsrtowcs,	\
	/* co_wcrtomb */	&_citrus_##_e_##_ctype_wcrtomb,		\
	/* co_wcsrtombs */	&_citrus_##_e_##_ctype_wcsrtombs,	\
}

typedef size_t	(*_citrus_ctype_mbrtowc_t)
	(wchar_t * __restrict, const char * __restrict,
	size_t, void * __restrict);
typedef int	(*_citrus_ctype_mbsinit_t) (const void * __restrict);
typedef size_t	(*_citrus_ctype_mbsrtowcs_t)
	(wchar_t * __restrict, const char ** __restrict,
	 size_t, void * __restrict);
typedef size_t	(*_citrus_ctype_wcrtomb_t)
	(char * __restrict, wchar_t, void * __restrict);
typedef size_t	(*_citrus_ctype_wcsrtombs_t)
	(char * __restrict, const wchar_t ** __restrict,
	 size_t, void * __restrict);

struct _citrus_ctype_ops_rec {
	_citrus_ctype_mbrtowc_t		co_mbrtowc;
	_citrus_ctype_mbsinit_t		co_mbsinit;
	_citrus_ctype_mbsrtowcs_t	co_mbsrtowcs;
	_citrus_ctype_wcrtomb_t		co_wcrtomb;
	_citrus_ctype_wcsrtombs_t	co_wcsrtombs;
};

#define _CITRUS_DEFAULT_CTYPE_NAME	"NONE"

struct _citrus_ctype_rec {
	struct _citrus_ctype_ops_rec	*cc_ops;
};

#endif