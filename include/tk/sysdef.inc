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
; *    Changed by UC Technology at 2012/12/20.
; *    
; *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
; *    Copyright (c) 2012 UC Technology. All Rights Reserved.
; *----------------------------------------------------------------------
; */

;/*
; *	@(#)sysdef.inc
; *
; *	System dependencies definition.
; *	Included also from assembler program.
; */

	.aifdef __TK_SYSDEF_H__
	.aelse
__TK_SYSDEF_H__: .define " "

	.include "tk/sysdepend/sysdef_common.inc"

	.aendi ;/* __TK_SYSDEF_H__ */