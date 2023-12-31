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
 *      @(#)str_align_common.h (sys)
 *
 *	Bit alignment definitions for structure
 */

#ifndef __SYS_STR_ALIGN_COMMON_H__
#define __SYS_STR_ALIGN_COMMON_H__

#if STD_SH7727
#  include <sys/sysdepend/std_sh7727/str_align_depend.h>
#endif
#if STD_SH7751R
#  include <sys/sysdepend/std_sh7751r/str_align_depend.h>
#endif
#if MIC_M32104
#  include <sys/sysdepend/mic_m32104/str_align_depend.h>
#endif
#if STD_S1C38K
#  include <sys/sysdepend/std_s1c38k/str_align_depend.h>
#endif
#if STD_MC9328
#  include <sys/sysdepend/std_mc9328/str_align_depend.h>
#endif
#if MIC_VR4131
#  include <sys/sysdepend/mic_vr4131/str_align_depend.h>
#endif
#if STD_VR5500
#  include <sys/sysdepend/std_vr5500/str_align_depend.h>
#endif
#if STD_MB87Q1100
#  include <sys/sysdepend/std_mb87q1100/str_align_depend.h>
#endif
#if STD_SH7760
#  include <sys/sysdepend/std_sh7760/str_align_depend.h>
#endif
#if TEF_EM1D
#  include <sys/sysdepend/tef_em1d/str_align_depend.h>
#endif
#if ASP_SH7750R
#  include <sys/sysdepend/asp_sh7750r/str_align_depend.h>
#endif

#endif /* __SYS_STR_ALIGN_COMMON_H__ */
