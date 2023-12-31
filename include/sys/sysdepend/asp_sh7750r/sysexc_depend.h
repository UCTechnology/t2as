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
 *    Changed by UC Technology at 2012/12/20.
 *    
 *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
 *    Copyright (c) 2012 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)sysexc_depend.h (sys/ASP-SH7750R)
 *
 *	System exception processing (SH7750R)
 */

#ifndef __SYS_SYSEXC_DEPEND_H__
#define __SYS_SYSEXC_DEPEND_H__

#ifdef __cplusplus
extern "C" {
#endif

/* System exception message */
typedef struct {
	W	type;		/* Message type (MS_SYS1) */
	W	size;		/* Message size */
	ID	taskid;		/* Exception generated task ID */
	ID	procid;		/* Exception generated process ID */
	UW	exccode;	/* Exception code (EXPEVT, INTEVT2) */
	UW	excinfo;	/* Exception information (TRA, INTEVT) */
	UW	excaddr;	/* Exception address (TEA) */
} EXCMESG;

#ifdef __cplusplus
}
#endif

#endif /* __SYS_SYSEXC_DEPEND_H__ */
