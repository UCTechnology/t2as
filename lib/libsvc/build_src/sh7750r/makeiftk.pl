#! /usr/local/bin/perl
#
# ----------------------------------------------------------------------
#     T-Kernel 2.0 Software Package
#
#     Copyright 2011 by Ken Sakamura.
#     This software is distributed under the T-License 2.0.
# ----------------------------------------------------------------------
#
#     Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
#     Modified by T-Engine Forum at 2012/11/07.
#
# ----------------------------------------------------------------------
#     Changes: Adapted to the ASP-SH7750R Board.
#     Changed by UC Technology at 2012/12/20.
#     
#     UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
#     Copyright (c) 2012 UC Technology. All Rights Reserved.
# ----------------------------------------------------------------------

#
#	makeiftk.pl
#
#	generate interface library for SH7750R [HEW]
#

sub makelib
{
	print LIB <<EndOfIfLibBody;
	.include "machine.inc"
	.include "tk/sysdef.inc"
	.include "sys/svc/$fn_inc"

	.aifdef	TFN_${Func}

	.section P,code
	.align	2
	.global	_${func}
_${func}:
	mov.l	fno, r0
	trapa	#TRAP_SVC
	rts
	nop

	.align	4
fno:	.data.l	TFN_${Func}

	.aendi

	.end

EndOfIfLibBody
}

1;
