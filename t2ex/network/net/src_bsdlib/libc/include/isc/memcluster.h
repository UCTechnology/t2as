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

/*	$NetBSD: memcluster.h,v 1.1.1.3.28.1 2011/01/09 20:43:28 riz Exp $	*/

/*
 * Copyright (c) 2004 by Internet Systems Consortium, Inc. ("ISC")
 * Copyright (c) 1997,1999 by Internet Software Consortium.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef MEMCLUSTER_H
#define MEMCLUSTER_H

#include <stdio.h>

#define meminit		__meminit
#ifdef MEMCLUSTER_DEBUG
#define memget(s)	__memget_debug(s, __FILE__, __LINE__)
#define memput(p, s)	__memput_debug(p, s, __FILE__, __LINE__)
#else /*MEMCLUSTER_DEBUG*/
#ifdef MEMCLUSTER_RECORD
#define memget(s)	__memget_record(s, __FILE__, __LINE__)
#define memput(p, s)	__memput_record(p, s, __FILE__, __LINE__)
#else /*MEMCLUSTER_RECORD*/
#define memget		__memget
#define memput		__memput
#endif /*MEMCLUSTER_RECORD*/
#endif /*MEMCLUSTER_DEBUG*/
#define memstats	__memstats
#define memactive	__memactive

int	meminit(size_t, size_t);
void *	__memget(size_t);
void 	__memput(void *, size_t);
void *	__memget_debug(size_t, const char *, int);
void 	__memput_debug(void *, size_t, const char *, int);
void *	__memget_record(size_t, const char *, int);
void 	__memput_record(void *, size_t, const char *, int);
void 	memstats(FILE *);
int	memactive(void);

#endif /* MEMCLUSTER_H */
/*! \file */