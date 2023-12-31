;/*
; *----------------------------------------------------------------------
; *    T-Kernel 2.0 Software Package
; *
; *    Copyright 2011 by Ken Sakamura.
; *    This software is distributed under the T-License 2.0.
; *----------------------------------------------------------------------
; *
; *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
; *
; *----------------------------------------------------------------------
; *    Changes: Adapted to the ASP-SH7750R Board.
; *    Changed by UC Technology at 2012/12/25.
; *    
; *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
; *    Copyright (c) 2012 UC Technology. All Rights Reserved.
; *----------------------------------------------------------------------
; */

;/*
; *	T-Kernel system call branch table [HEW]
; *
; *	   created from syscall.h
; */

	.include "machine.inc"

	.macro _SVC_ENTRY name
	.aif	"\name" NE "_no_support"	;; avoid 801 (W) MULTIPLE SYMBOLS
	.aif	"\name" NE "__tk_ret_int"	;; avoid 801 (W) MULTIPLE SYMBOLS
	.import	\name
	.aendi
	.aendi
	.data.l	\name
	.endm

N_TFN:	.equ	132

	_SVC_ENTRY	__tk_cre_tsk
	_SVC_ENTRY	__tk_del_tsk
	_SVC_ENTRY	__tk_sta_tsk
	_SVC_ENTRY	__tk_ext_tsk
	_SVC_ENTRY	__tk_exd_tsk
	_SVC_ENTRY	__tk_ter_tsk
	_SVC_ENTRY	__tk_dis_dsp
	_SVC_ENTRY	__tk_ena_dsp
	_SVC_ENTRY	__tk_chg_pri
	_SVC_ENTRY	__tk_chg_slt
	_SVC_ENTRY	__tk_rot_rdq
	_SVC_ENTRY	__tk_rel_wai
	_SVC_ENTRY	__tk_get_tid
	_SVC_ENTRY	__tk_get_tsp
	_SVC_ENTRY	__tk_set_tsp
	_SVC_ENTRY	__tk_get_rid
	_SVC_ENTRY	__tk_set_rid
	_SVC_ENTRY	__tk_get_reg
	_SVC_ENTRY	__tk_set_reg
	_SVC_ENTRY	__tk_get_cpr
	_SVC_ENTRY	__tk_set_cpr
	_SVC_ENTRY	__tk_inf_tsk
	_SVC_ENTRY	__tk_ref_tsk
	_SVC_ENTRY	__tk_def_tex
	_SVC_ENTRY	__tk_ena_tex
	_SVC_ENTRY	__tk_dis_tex
	_SVC_ENTRY	__tk_ras_tex
	_SVC_ENTRY	__tk_end_tex
	_SVC_ENTRY	__tk_ref_tex
	_SVC_ENTRY	__tk_sus_tsk
	_SVC_ENTRY	__tk_rsm_tsk
	_SVC_ENTRY	__tk_frsm_tsk
	_SVC_ENTRY	__tk_slp_tsk
	_SVC_ENTRY	__tk_wup_tsk
	_SVC_ENTRY	__tk_can_wup
	_SVC_ENTRY	__tk_sig_tev
	_SVC_ENTRY	__tk_wai_tev
	_SVC_ENTRY	__tk_dis_wai
	_SVC_ENTRY	__tk_ena_wai
	_SVC_ENTRY	__tk_cre_sem
	_SVC_ENTRY	__tk_del_sem
	_SVC_ENTRY	__tk_sig_sem
	_SVC_ENTRY	__tk_wai_sem
	_SVC_ENTRY	__tk_ref_sem
	_SVC_ENTRY	__tk_cre_mtx
	_SVC_ENTRY	__tk_del_mtx
	_SVC_ENTRY	__tk_loc_mtx
	_SVC_ENTRY	__tk_unl_mtx
	_SVC_ENTRY	__tk_ref_mtx
	_SVC_ENTRY	__tk_cre_flg
	_SVC_ENTRY	__tk_del_flg
	_SVC_ENTRY	__tk_set_flg
	_SVC_ENTRY	__tk_clr_flg
	_SVC_ENTRY	__tk_wai_flg
	_SVC_ENTRY	__tk_ref_flg
	_SVC_ENTRY	__tk_cre_mbx
	_SVC_ENTRY	__tk_del_mbx
	_SVC_ENTRY	__tk_snd_mbx
	_SVC_ENTRY	__tk_rcv_mbx
	_SVC_ENTRY	__tk_ref_mbx
	_SVC_ENTRY	__tk_cre_mbf
	_SVC_ENTRY	__tk_del_mbf
	_SVC_ENTRY	__tk_snd_mbf
	_SVC_ENTRY	__tk_rcv_mbf
	_SVC_ENTRY	__tk_ref_mbf
	_SVC_ENTRY	__tk_cre_por
	_SVC_ENTRY	__tk_del_por
	_SVC_ENTRY	__tk_cal_por
	_SVC_ENTRY	__tk_acp_por
	_SVC_ENTRY	__tk_fwd_por
	_SVC_ENTRY	__tk_rpl_rdv
	_SVC_ENTRY	__tk_ref_por
	_SVC_ENTRY	__tk_def_int
	_SVC_ENTRY	__tk_ret_int
	_SVC_ENTRY	__tk_cre_mpl
	_SVC_ENTRY	__tk_del_mpl
	_SVC_ENTRY	__tk_get_mpl
	_SVC_ENTRY	__tk_rel_mpl
	_SVC_ENTRY	__tk_ref_mpl
	_SVC_ENTRY	__tk_cre_mpf
	_SVC_ENTRY	__tk_del_mpf
	_SVC_ENTRY	__tk_get_mpf
	_SVC_ENTRY	__tk_rel_mpf
	_SVC_ENTRY	__tk_ref_mpf
	_SVC_ENTRY	__tk_set_tim
	_SVC_ENTRY	__tk_get_tim
	_SVC_ENTRY	__tk_get_otm
	_SVC_ENTRY	__tk_dly_tsk
	_SVC_ENTRY	__tk_cre_cyc
	_SVC_ENTRY	__tk_del_cyc
	_SVC_ENTRY	__tk_sta_cyc
	_SVC_ENTRY	__tk_stp_cyc
	_SVC_ENTRY	__tk_ref_cyc
	_SVC_ENTRY	__tk_cre_alm
	_SVC_ENTRY	__tk_del_alm
	_SVC_ENTRY	__tk_sta_alm
	_SVC_ENTRY	__tk_stp_alm
	_SVC_ENTRY	__tk_ref_alm
	_SVC_ENTRY	__tk_ref_ver
	_SVC_ENTRY	__tk_ref_sys
	_SVC_ENTRY	__tk_def_ssy
	_SVC_ENTRY	__tk_sta_ssy
	_SVC_ENTRY	__tk_cln_ssy
	_SVC_ENTRY	__tk_evt_ssy
	_SVC_ENTRY	__tk_ref_ssy
	_SVC_ENTRY	__tk_cre_res
	_SVC_ENTRY	__tk_del_res
	_SVC_ENTRY	__tk_get_res
	_SVC_ENTRY	__tk_set_pow
	_SVC_ENTRY	__tk_chg_slt_u
	_SVC_ENTRY	__tk_inf_tsk_u
	_SVC_ENTRY	__tk_ref_tsk_u
	_SVC_ENTRY	__tk_slp_tsk_u
	_SVC_ENTRY	__tk_wai_tev_u
	_SVC_ENTRY	__tk_dly_tsk_u
	_SVC_ENTRY	__tk_wai_sem_u
	_SVC_ENTRY	__tk_wai_flg_u
	_SVC_ENTRY	__tk_rcv_mbx_u
	_SVC_ENTRY	__tk_loc_mtx_u
	_SVC_ENTRY	__tk_snd_mbf_u
	_SVC_ENTRY	__tk_rcv_mbf_u
	_SVC_ENTRY	__tk_cal_por_u
	_SVC_ENTRY	__tk_acp_por_u
	_SVC_ENTRY	__tk_get_mpl_u
	_SVC_ENTRY	__tk_get_mpf_u
	_SVC_ENTRY	__tk_set_tim_u
	_SVC_ENTRY	__tk_get_tim_u
	_SVC_ENTRY	__tk_get_otm_u
	_SVC_ENTRY	__tk_cre_cyc_u
	_SVC_ENTRY	__tk_ref_cyc_u
	_SVC_ENTRY	__tk_sta_alm_u
	_SVC_ENTRY	__tk_ref_alm_u
