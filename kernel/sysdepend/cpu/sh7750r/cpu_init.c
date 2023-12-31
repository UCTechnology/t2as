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
 *	cpu_init.c (SH7750R)
 *	CPU-Dependent Initialization/Finalization
 */

#include "kernel.h"
#include "task.h"
#include "cpu_insn.h"
#include <smachine.h>

EXPORT MONHDR	SaveMonHdr;	/* For saving monitor exception handler */
EXPORT VW	*int_stack_top;	/* EIT stack top */
EXPORT ATR	available_cop;	/* Enabled coprocessor */
#if USE_SH3_DSP
EXPORT TCB	*dsp_ctxtsk;	/* Task including DSP context */
#endif
#if TA_FPU
EXPORT TCB	*fpu_ctxtsk;	/* Task including FPU context */
#endif

/*
 * Check existence of coprocessor
 */
LOCAL ATR check_cop( void )
{
	UW	sr, sr_tmp;

#if USE_SH3_DSP
	/* If SR.DSP can be ON, determine DSP exists. */
	sr_tmp = get_cr();
	sr_tmp |= SR_DSP;
	set_cr(sr_tmp);
	sr = get_cr();
	sr_tmp ^= SR_DSP;
	set_cr(sr_tmp);			/* Set SR.DSP=0 */

	return ( (sr & SR_DSP) != 0 )? TA_COP0: 0;
#elif TA_FPU
	/* If SR.DSP can be ON, determine FPU exits */
	sr_tmp = get_cr();
	set_cr(sr_tmp | SR_FD);
	sr = get_cr();
	set_cr(sr_tmp);

	return ( (sr & SR_FD) != 0 )? TA_COP0: 0;
#else
	return 0;
#endif
}

/*
 * CPU-dependent initialization
 */
EXPORT ER cpu_initialize( void )
{
	IMPORT void dispatch_entry( void );	/* Dispatcher call */
	IMPORT void call_entry( void );		/* System call */
	IMPORT void _tk_ret_int( void );	/* 'tk_ret_int()' only call */
	IMPORT void dct_startup( void );	/* Delayed context trap */
	IMPORT void load_SR( void );		/* SR register load function */
	IMPORT void call_dbgspt( void );	/* Debugger support call */
	IMPORT void unavailable_cop( void );	/* Coprocessor disable exception */

	/* Save monitor exception handler */
	SaveMonHdr.trapa_hdr   = SCArea->intvec[VECNO_TRAPA];
	SaveMonHdr.break_hdr   = SCArea->intvec[VECNO_BREAK];
	SaveMonHdr.monitor_hdr = SCArea->intvec[VECNO_MONITOR];
	SaveMonHdr.default_hdr = SCArea->intvec[VECNO_DEFAULT];
	SaveMonHdr.tlbmiss_hdr = SCArea->intvec[VECNO_TLBMISS];

	/* Save EIT stack top */
	int_stack_top = getISP();

	/* Check enabled coprocessor */
	available_cop = check_cop();
#if USE_SH3_DSP
	dsp_ctxtsk = NULL;
#endif
#if TA_FPU
	fpu_ctxtsk = NULL;
#endif

	/* Register exception handler used on OS */
	define_inthdr(TRAP_SVC,      (FP)call_entry);
	define_inthdr(TRAP_RETINT,   (FP)_tk_ret_int);
	define_inthdr(TRAP_DISPATCH, (FP)dispatch_entry);
	define_inthdr(TRAP_LOADSR,   (FP)load_SR);
#if USE_MDR_DCT
	define_inthdr(TRAP_DCT,      (FP)dct_startup);
#endif
#if USE_DBGSPT
	define_inthdr(TRAP_DEBUG,    (FP)call_dbgspt);
#endif
#if TA_FPU
	/* For FPU context switch */

	/* General FPU supress exception */
	define_inthdr(0x800/0x20, (FP)unavailable_cop);
	/* Slot FPU suppress exception */
	define_inthdr(0x820/0x20, (FP)unavailable_cop);
#endif

	return E_OK;
}

/*
 * CPU-dependent finalization
 */
EXPORT void cpu_shutdown( void )
{
	/* Restore saved monitor exception handler */
	SCArea->intvec[VECNO_TRAPA]   = SaveMonHdr.trapa_hdr;
	SCArea->intvec[VECNO_BREAK]   = SaveMonHdr.break_hdr;
	SCArea->intvec[VECNO_MONITOR] = SaveMonHdr.monitor_hdr;
	SCArea->intvec[VECNO_DEFAULT] = SaveMonHdr.default_hdr;
	SCArea->intvec[VECNO_TLBMISS] = SaveMonHdr.tlbmiss_hdr;
}

/* ------------------------------------------------------------------------- */

#if USE_MDR_DCT
/*
 * Task exception handler startup reservation
 *	isstack    ->	+-------+
 *		-1	| USP	|
 *		-2	| MDR	| <-Set DCT here
 *		-3	| SSR	|
 *		-4	| SPC	|
 *		-5	| R0	|
 *			|  :	|
 *
 *	MDR DCT is used only by SH3
 */
EXPORT void request_tex( TCB *tcb )
{
	UW	*ssp = tcb->isstack;

	/* Cannot set to the task operating at protected level 0 */
	if ( tcb->isysmode == 0 ) {
		*(ssp - 2) |= MDR_DCT;
	}
}
#else
/*
 * Task exception handler startup reservation
 */
EXPORT void request_tex( TCB *tcb )
{
	/* Cannot set to the task operating at protected level 0 */
	if ( tcb->isysmode == 0 ) {
		tcb->reqdct = 1;
	}
}
#endif

/*
 * Task exception handler startup setting
 *
 *		User stack			EIT stack
 *		+---------------+		+---------------+
 *	USP ->	|(xxxxxxxxxxxxx)|  +0		| USP		|  +4
 *		+---------------+		| MDR		|  +3
 *						| SSR		|  +2
 *						| SPC		|  +1
 *					SSP ->	| R0_BANK1	|  +0
 *						+---------------+
 *
 *	Coordinate the stack as shown below (Change *)
 *		+---------------+		+---------------+
 *		|(xxxxxxxxxxxxx)|  +0		| USP		|* +4
 *		+---------------+		| MDR		|  +3
 *		| texcd		|* -1		| SSR		|  +2
 *		| retadr	|* -2		| SPC		|* +1
 *	USP ->	| save SR	|* -3	SSP ->	| R0_BANK1	|  +0
 *		+---------------+		+---------------+
 */
EXPORT void setup_texhdr( UW *ssp )
{
	FP	texhdr;
	INT	texcd;
	UINT	m;
	UW	*usp;

	/* Called in interrupt disable state */

#if USE_MDR_DCT
	*(ssp + 3) &= ~MDR_DCT;	/* Free DCT */
#else
	ctxtsk->reqdct = 0;	/* Free DCT */
#endif

	/* Get exception code */
	m = 0x00000001;
	for ( texcd = 0; texcd <= 31; texcd++ ) {
		if ( (ctxtsk->exectex & m) != 0 ) {
			break;
		}
		m <<= 1;
	}

	/* Exception does not occur /Exception is freed */
	if ( texcd > 31 ) {
		return;
	}

	ctxtsk->exectex = 0;
	ctxtsk->pendtex &= ~m;
	ctxtsk->texflg |= ( texcd == 0 )? TEX0_RUNNING: TEX1_RUNNING;
	texhdr = ctxtsk->texhdr;

	/* If the exception code is 0, return the user stack to the
	   initial value */
	usp = ( texcd == 0 )? ctxtsk->istack: (UW*)*(ssp + 4);

	ENABLE_INTERRUPT;

	/* Stack coordination
	 *	A page fault may occur 
	 *	due to access to the user stack.
	 */
	*(usp - 1) = texcd;
	*(usp - 2) = *(ssp + 1);	/* retadr */
	*(usp - 3) = *(ssp + 2);	/* save SR */
	*(ssp + 1) = (UW)texhdr;
	*(ssp + 4) = (UW)(usp - 3);	/* new USP */
}
