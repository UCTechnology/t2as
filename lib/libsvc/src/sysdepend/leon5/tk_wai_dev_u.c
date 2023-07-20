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
 *    UCT T2AS DevKit tuned for LEON5 Version 1.00.00
 *    Copyright (c) 2021 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	T-Kernel extended SVC interface library (leon5)
 *
 *	   created from <tk/devmgr.h>
 */


#include <machine.h>
#include <tk/sysdef.h>
#include <sys/svc/ifdevmgr.h>

#ifdef DEVICE_TK_WAI_DEV_U_FN

EXPORT ID tk_wai_dev_u( ID dd, ID reqid, W *asize, ER *ioer, TMO_U tmout_u )
{
	volatile DEVICE_TK_WAI_DEV_U_PARA	para;
	register int	o0	asm("%o0");
	register int	g2	asm("%g2") = DEVICE_TK_WAI_DEV_U_FN;

	para.dd = dd;
	para.reqid = reqid;
	para.asize = asize;
	para.ioer = ioer;
	para.tmout_u = tmout_u;
	o0 = (int)&para;

	Asm("ta	%2": "+r"(o0): "r"(g2), "i"(TRAP_SVC));

	return o0;
}

#endif
