/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *
 *----------------------------------------------------------------------
 *    Changes: Adapted to the ASP-SH7750R Board.
 *    Changed by UC Technology at 2013/01/28.
 *    
 *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)malloc.c (libtk)
 *
 *	Non resident system memory allocation
 */

#include "libtk.h"
#include <sys/util.h>

EXPORT void* malloc( size_t size )
{
#if	USE_MEM_PROTECT
	return Smalloc(size);
#else
	return Vmalloc(size);
#endif
}

EXPORT void* calloc( size_t nmemb, size_t size )
{
#if	USE_MEM_PROTECT
	return Scalloc(nmemb, size);
#else
	return Vcalloc(nmemb, size);
#endif
}

EXPORT void* realloc( void *ptr, size_t size )
{
#if	USE_MEM_PROTECT
	return Srealloc(ptr, size);
#else
	return Vrealloc(ptr, size);
#endif
}

EXPORT void free( void *ptr )
{
#if	USE_MEM_PROTECT
	Sfree(ptr);
#else
	Vfree(ptr);
#endif
}
