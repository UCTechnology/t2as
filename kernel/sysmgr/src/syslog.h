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
 *    Changes: Adapted to the UCT T-Kernel 2.0
 *    Changed by UC Technology at 2013/01/29.
 *
 *    UCT T-Kernel 2.0 DevKit Version 2.00.01
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	syslog.h (T-Kernel/SM)
 *	System Log Task Definition
 */

#ifndef _SYSLOG_
#define _SYSLOG_

#define USE_SYSLOG_CONSIO 0		/* Use console IO for system log */

#define MBF_LOG_BUFSZ	(1024*6)	/* Size of message buffer for log */
#define MBF_LOG_MAXMSZ	256		/* Maximum length of log message */

#endif /* _SYSLOG_ */
