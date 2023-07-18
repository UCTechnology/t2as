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
 *	@(#)dbgspt_common.h (tk)
 *
 *	T-Kernel Debugger Support
 */

#ifndef __TK_DBGSPT_COMMON_H__
#define __TK_DBGSPT_COMMON_H__

/*
 * System-dependent definition
 */
#if STD_SH7727
#  include <tk/sysdepend/std_sh7727/dbgspt_depend.h>
#endif
#if STD_SH7751R
#  include <tk/sysdepend/std_sh7751r/dbgspt_depend.h>
#endif
#if MIC_M32104
#  include <tk/sysdepend/mic_m32104/dbgspt_depend.h>
#endif
#if STD_S1C38K
#  include <tk/sysdepend/std_s1c38k/dbgspt_depend.h>
#endif
#if STD_MC9328
#  include <tk/sysdepend/std_mc9328/dbgspt_depend.h>
#endif
#if MIC_VR4131
#  include <tk/sysdepend/mic_vr4131/dbgspt_depend.h>
#endif
#if STD_VR5500
#  include <tk/sysdepend/std_vr5500/dbgspt_depend.h>
#endif
#if STD_MB87Q1100
#  include <tk/sysdepend/std_mb87q1100/dbgspt_depend.h>
#endif
#if STD_SH7760
#  include <tk/sysdepend/std_sh7760/dbgspt_depend.h>
#endif
#if TEF_EM1D
#  include <tk/sysdepend/tef_em1d/dbgspt_depend.h>
#endif
#if ASP_SH7750R
#  include <tk/sysdepend/asp_sh7750r/dbgspt_depend.h>
#endif

#endif /* __TK_DBGSPT_COMMON_H__ */
