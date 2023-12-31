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
 *    Changed by UC Technology at 2013/01/29.
 *    
 *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 2.00.01
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */
/*
 *	cpu_calls.c (SH7750R)
 *	CPU-Dependent System Call
 */

#include "kernel.h"
#include "task.h"
#include "check.h"
#include "cpu_task.h"

#include <sys/sysinfo.h>
#include <tk/sysdef.h>
#include "cpu_insn.h"

#if defined(USE_FUNC_TK_DIS_DSP)
/*
 * Dispatch enable/disable 
 */
SYSCALL ER _tk_dis_dsp( void )
{
	CHECK_CTX(!in_loc());

	dispatch_disabled = DDS_DISABLE;

	return E_OK;
}
#endif /* USE_FUNC_TK_DIS_DSP */

#if defined(USE_FUNC_TK_ENA_DSP)
/*
 * Dispatch enable
 */
SYSCALL ER _tk_ena_dsp( void )
{
	CHECK_CTX(!in_loc());

	dispatch_disabled = DDS_ENABLE;
	if ( ctxtsk != schedtsk ) {
		dispatch();
	}

	return E_OK;
}
#endif /* USE_FUNC_TK_ENA_DSP */

/* ------------------------------------------------------------------------ */

/*
 * High level programming language supported
 */

#if defined(USE_FUNC_HLL_INTHDR)
/* High level programming language interrupt handler entry */
EXPORT FP hll_inthdr[N_INTVEC + 1];
#endif /* USE_FUNC_HLL_INTHDR */

#if defined(USE_FUNC_TK_DEF_INT)
/* High level programming language-supported routine (General) */
IMPORT void inthdr_startup();

/* For default handler */
IMPORT void defaulthdr_startup();

/*
 * Interrupt handler definition
 */
SYSCALL ER _tk_def_int( UINT dintno, T_DINT *pk_dint )
{
	FP	inthdr;
	INT	vecno = dintno >> 5;

	CHECK_PAR(vecno <= VECNO_TLBMISS);

	if ( pk_dint != NULL ) {
		/* Set interrupt handler */
		if ( vecno == VECNO_TLBMISS ) {
			CHECK_RSATR(pk_dint->intatr, 0);
		} else {
			CHECK_RSATR(pk_dint->intatr, TA_HLNG);
		}

		inthdr = pk_dint->inthdr;

		BEGIN_CRITICAL_SECTION;
		if ( (pk_dint->intatr & TA_HLNG) != 0 ) {
			hll_inthdr[vecno] = inthdr;
			inthdr = ( vecno == VECNO_DEFAULT )?
					defaulthdr_startup: inthdr_startup;
		}
		define_inthdr(vecno, inthdr);
		END_CRITICAL_SECTION;
	} else {
		/* Free interrupt handler */
		switch ( vecno ) {
		  case VECNO_TRAPA:	inthdr = SaveMonHdr.trapa_hdr;	 break;
		  case VECNO_BREAK:	inthdr = SaveMonHdr.break_hdr;	 break;
		  case VECNO_MONITOR:	inthdr = SaveMonHdr.monitor_hdr; break;
		  case VECNO_DEFAULT:	inthdr = SaveMonHdr.default_hdr; break;
		  case VECNO_TLBMISS:	inthdr = SaveMonHdr.tlbmiss_hdr; break;
		  default:		inthdr = NULL;
		}

		BEGIN_CRITICAL_SECTION;
		define_inthdr(vecno, inthdr);
		if ( vecno < VECNO_TLBMISS ) {
			hll_inthdr[vecno] = NULL;
		}
		END_CRITICAL_SECTION;
	}

	return E_OK;
}
#endif /* USE_FUNC_TK_DEF_INT */

/* ------------------------------------------------------------------------ */

#if defined(USE_FUNC_TK_GET_TSP)
/*
 * Get task space
 */
SYSCALL ER _tk_get_tsp( ID tskid, T_TSKSPC *pk_tskspc )
{
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_TSKID_SELF(tskid);

	tcb = get_tcb_self(tskid);

	BEGIN_CRITICAL_SECTION;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		pk_tskspc->uatb = tcb->tskctxb.uatb;
		pk_tskspc->lsid = tcb->tskctxb.lsid;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_GET_TSP*/

#if defined(USE_FUNC_TK_SET_TSP)
/*
 * Set task space
 */
SYSCALL ER _tk_set_tsp( ID tskid, T_TSKSPC *pk_tskspc )
{
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_TSKID_SELF(tskid);

	tcb = get_tcb_self(tskid);

	BEGIN_CRITICAL_SECTION;
	if ( tcb->state == TS_NONEXIST) {
		ercd = E_NOEXS;
	} else {
		tcb->tskctxb.uatb = pk_tskspc->uatb;
		tcb->tskctxb.lsid = pk_tskspc->lsid;

		/* When it is the currently running task,
		   switch the current space */
		if ( tcb == ctxtsk ) {
			change_space(tcb->tskctxb.uatb, tcb->tskctxb.lsid);
		}
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_SET_TSP */

/* ------------------------------------------------------------------------ */

#if defined(USE_FUNC_TK_SET_REG)||defined(USE_FUNC_TD_SET_REG)
/*
 * Set task register contents
 */
LOCAL void set_reg( TCB *tcb, T_REGS *regs, T_EIT *eit, T_CREGS *cregs )
{
	const UW	SR_Mask		/* SR unchangeable bit */
		= 0x8fff7c0c | SR_MD | SR_RB | SR_BL | SR_FD | SR_I(15);
	SStackFrame	*ssp;
	UW		sr;
	INT		i;

	ssp = tcb->tskctxb.ssp;
	sr = ssp->rng0_3.ssr;

	if ( cregs != NULL ) {
		ssp = cregs->ssp;
	}

	if ( regs != NULL ) {
		for ( i = 0; i < 15; ++i ) {
			ssp->rng0_3.r[i] = regs->r[i];
		}
		ssp->rng0_3.mach = regs->mach;
		ssp->rng0_3.macl = regs->macl;
		ssp->rng0_3.gbr  = regs->gbr;
		ssp->rng0_3.pr   = regs->pr;
	}

	if ( eit != NULL ) {
		ssp->rng0_3.spc = eit->pc;
		ssp->rng0_3.ssr = (sr & SR_Mask) | (eit->sr & ~SR_Mask);
		ssp->rng0_3.mdr = eit->mdr;
	}

	if ( cregs != NULL ) {
		tcb->tskctxb.ssp     = cregs->ssp;
		tcb->tskctxb.uatb    = cregs->uatb;
		tcb->tskctxb.lsid    = cregs->lsid;
		ssp->rng0_3.bank1_r0 = cregs->r0;
		ssp->rng0_3.bank1_r1 = cregs->r1;

		if ( (tcb->tskatr & TA_RNG3) != 0 ) {
			/* RNG 1-3 */
			((void **)tcb->isstack)[-1] = cregs->usp;
		}
	}
}
#endif /* USE_FUNC_TK_SET_REG or USE_FUNC_TD_SET_REG */

#if defined(USE_FUNC_TK_SET_REG)
/*
 * Set task register contents
 */
SYSCALL ER _tk_set_reg( ID tskid,
		T_REGS *pk_regs, T_EIT *pk_eit, T_CREGS *pk_cregs )
{
	TCB		*tcb;
	ER		ercd = E_OK;

	CHECK_INTSK();
	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		set_reg(tcb, pk_regs, pk_eit, pk_cregs);
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_SET_REG */

#if defined(USE_FUNC_TK_GET_REG)||defined(USE_FUNC_TD_GET_REG)
/*
 * Get task register contents
 */
LOCAL void get_reg( TCB *tcb, T_REGS *regs, T_EIT *eit, T_CREGS *cregs )
{
	SStackFrame	*ssp;
	INT		i;

	ssp = tcb->tskctxb.ssp;

	if ( regs != NULL ) {
		for ( i = 0; i < 15; ++i ) {
			regs->r[i] = ssp->rng0_3.r[i];
		}
		regs->mach = ssp->rng0_3.mach;
		regs->macl = ssp->rng0_3.macl;
		regs->gbr  = ssp->rng0_3.gbr;
		regs->pr   = ssp->rng0_3.pr;
	}

	if ( eit != NULL ) {
		eit->pc  = ssp->rng0_3.spc;
		eit->sr  = ssp->rng0_3.ssr;
		eit->mdr = ssp->rng0_3.mdr;
	}

	if ( cregs != NULL ) {
		cregs->sstop = tcb->isstack;
		cregs->ssp   = tcb->tskctxb.ssp;
		cregs->usp   = ((void **)tcb->isstack)[-1];
		cregs->uatb  = tcb->tskctxb.uatb;
		cregs->lsid  = tcb->tskctxb.lsid;
		cregs->r0    = ssp->rng0_3.bank1_r0;
		cregs->r1    = ssp->rng0_3.bank1_r1;
	}
}
#endif /* USE_FUNC_TK_GET_REG or USE_FUNC_TD_GET_REG */

#if defined(USE_FUNC_TK_GET_REG)
/*
 * Get task register contents
 */
SYSCALL ER _tk_get_reg( ID tskid,
		T_REGS *pk_regs, T_EIT *pk_eit, T_CREGS *pk_cregs )
{
	TCB		*tcb;
	ER		ercd = E_OK;

	CHECK_INTSK();
	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		get_reg(tcb, pk_regs, pk_eit, pk_cregs);
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_GET_REG */

#if defined(USE_FUNC_TK_SET_CPR)
/*
 * Set task coprocessor register
 */
SYSCALL ER _tk_set_cpr( ID tskid, INT copno, T_COPREGS *pk_copregs )
{
	ATR	copatr = TA_COP0 << copno;
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_INTSK();
	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);
	CHECK_PAR((copatr & available_cop) != 0);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else if ( (tcb->tskatr & copatr) == 0 ) {
		ercd = E_PAR;
	} else {
#if USE_SH3_DSP
		/* If the current DSP context belongs to 'tcb', disable it */
		if ( dsp_ctxtsk == tcb ) {
			dsp_ctxtsk = NULL;
		}

		/* Set DSP register */
		memcpy(tcb->isstack, pk_copregs, sizeof(T_COP0REGS));
#endif
#if TA_FPU
		/* If the current FPU context belongs to 'tcb,' disable it. */
		if ( fpu_ctxtsk == tcb ) {
			fpu_ctxtsk = NULL;
		}

		/* Set FPU register */
		memcpy(tcb->isstack, pk_copregs, sizeof(T_COP0REGS));
#endif
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_SET_CPR */

#if defined(USE_FUNC_TK_GET_CPR)
/*
 * Get task coprocessor register
 */
SYSCALL ER _tk_get_cpr( ID tskid, INT copno, T_COPREGS *pk_copregs )
{
IMPORT void save_dspctx( T_COP0REGS *save_area );
IMPORT void save_fpuctx( T_COP0REGS *save_area );

	ATR	copatr = TA_COP0 << copno;
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_INTSK();
	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);
	CHECK_PAR((copatr & available_cop) != 0);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else if ( (tcb->tskatr & copatr) == 0 ) {
		ercd = E_PAR;
	} else {
#if USE_SH3_DSP
		/* If the current DSP context belongs to 'tcb,'
		   save the context */
		if ( dsp_ctxtsk == tcb ) {
			save_dspctx(tcb->isstack);
		}

		/* Get DSP register */
		memcpy(pk_copregs, tcb->isstack, sizeof(T_COP0REGS));
#endif
#if TA_FPU
		/* If the current FPU context belongs to 'tcb,'
		   Save the context */
		if ( fpu_ctxtsk == tcb ) {
			save_fpuctx(tcb->isstack);
		}

		/* Get FPU register */
		memcpy(pk_copregs, tcb->isstack, sizeof(T_COP0REGS));
#endif
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_GET_CPR */

/* ------------------------------------------------------------------------ */
/*
 *	Debugger support function
 */
#if USE_DBGSPT

#if defined(USE_FUNC_TD_SET_REG)
/*
 * Set task register
 */
SYSCALL ER _td_set_reg( ID tskid, T_REGS *regs, T_EIT *eit, T_CREGS *cregs )
{
	TCB		*tcb;
	ER		ercd = E_OK;

	CHECK_TSKID(tskid);
#if 0 /* (030206) This is not an error. Please ignore. */
	if ( eit != NULL ) {
		if ( (eit->sr & (SR_BL|SR_I(15))) != 0 ) {
			return E_PAR;
		}
	}
#endif

	tcb = get_tcb(tskid);
	if ( tcb == ctxtsk ) {
		return E_OBJ;
	}

	BEGIN_DISABLE_INTERRUPT;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		set_reg(tcb, regs, eit, cregs);
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_SET_REG */

#if defined(USE_FUNC_TD_GET_REG)
/*
 * Get task register
 */
SYSCALL ER _td_get_reg( ID tskid, T_REGS *regs, T_EIT *eit, T_CREGS *cregs )
{
	TCB		*tcb;
	ER		ercd = E_OK;

	CHECK_TSKID(tskid);

	tcb = get_tcb(tskid);
	if ( tcb == ctxtsk ) {
		return E_OBJ;
	}

	BEGIN_DISABLE_INTERRUPT;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		get_reg(tcb, regs, eit, cregs);
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_GET_REG */

#endif /* USE_DBGSPT */
