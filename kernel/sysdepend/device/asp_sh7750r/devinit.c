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
 *	devinit.c (SH7750R)
 *	Device-Dependent Initialization 
 */

#include "sysinit.h"
#include "kernel.h"
#include <tk/sysdef.h>
#include <sys/sysinfo.h>
#include <sys/rominfo.h>
#include <device/devconf.h>
#include <libstr.h>

EXPORT	UB	*BootDeviceName;	/* Boot device name */
LOCAL	UB	bootdevnm[L_DEVNM + 1];

/* Saved data of system common information */
EXPORT	SysCommonInfo	SaveSCInfo;

/*
 * Display the progress of start processing
 *	0x10 - 0x1F : Initialization (before kernel starts)
 *	0x20 - 0x2F : Kernel start processing
 *	0x30 - 0x3F : Start processing (after kernel starts)
 */
EXPORT void DispProgress( W n )
{
}

/* ------------------------------------------------------------------------ */

/*
 * Initialization at ROM startup
 *	This function is called before 'main()'.
 *	(same as 'SBOOT' for disk boot)
 */
EXPORT ER ROM_startup( void )
{
	W	val[L_DEVCONF_VAL];
	W	n;

	/* Set SYSCONF/DEVCONF */
	SCInfo.sysconf = ROMInfo->sysconf;
	SCInfo.devconf = ROMInfo->devconf;

	/* Set debug mode */
	n = GetDevConf(DCTAG_DEBUGMODE, val);
	SCInfo.bm.c.debug = ( n >= 1 && val[0] > 0 )? 1: 0;

	/* Set boot device (no boot device) */
	SCInfo.bootdev[0] = '\0';

	return E_OK;
}

/*
 * Initialization before T-Kernel starts
 */
EXPORT ER init_device( void )
{
IMPORT	void	CountWaitUsec( void );	/* cntwus.c */

	/* Compute loop count of micro second wait */
	CountWaitUsec();

	/* Save system shared information */
	SaveSCInfo = SCInfo;

	/* Set boot device */
	if ( SCInfo.bootdev[0] != '\0' ) {
		strncpy((char*)bootdevnm, (char*)SCInfo.bootdev, L_DEVNM+1);
		BootDeviceName = bootdevnm;
	} else {
		BootDeviceName = NULL; /* No boot device */
	}

	return E_OK;
}

/* ------------------------------------------------------------------------ */
/*
 * Start processing after T-Kernel starts
 *	Called from the initial task contexts.
 */
EXPORT ER start_device( void )
{
	return E_OK;
}

/* ------------------------------------------------------------------------ */
/*
 * System finalization
 *	Called just before system shutdown.
 *	Execute finalization that must be done before system shutdown.
 */
EXPORT ER finish_device( void )
{
	return E_OK;
}

/* ------------------------------------------------------------------------ */
/*
 *	Re-starting processing
 */

/*
 * Re-starting processing
 *	mode = -1		Reset and re-start	(cold boot)
 *	mode = -2		Re-start		(warm boot)
 *	mode = -3		Reboot			(normal boot)
 *	mode = 0xFFhhmmss	Re-start at hh:mm:ss
 *				0 <= hh < 24, 0 <= mm,ss < 60
 */
EXPORT ER restart_device( W mode )
{
	if ( mode == -1 ) {
		/* Reset and re-start (cold boot) */
#if USE_KERNEL_MESSAGE
		tm_putstring((UB*)"\n<< SYSTEM RESTART >>\n");
#endif
		tm_exit(-1);  /* no return */
		return E_OBJ;
	}

	if ( mode == -3 ) {
		/* Reboot (normal boot) */
		static UB bdcmd[4 + L_DEVNM] = "bd ";
#if USE_KERNEL_MESSAGE
		tm_putstring((UB*)"\n<< SYSTEM REBOOT >>\n");
#endif
		strncat((char*)bdcmd, (char*)SCInfo.bootdev, L_DEVNM);
		tm_command(bdcmd);	/* Normally no return */
		return E_OBJ;		/* When the BD command is an error */
	}

	if ( mode == -2 ) {
		return E_NOSPT; /* Unsupported */
	}

	if ( (mode & 0xff000000) == 0xff000000 ) {
		/* Re-start at specified time */
		return E_NOSPT;	/* Unsupported */
	}

	return E_PAR;
}
