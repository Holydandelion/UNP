#include	"error.h"

#include	<stdarg.h>		/* ANSI C header file */
#include	<syslog.h>		/* for syslog() */
#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<string.h>
#include 	<errno.h>

int		daemon_proc;		/* set nonzero by daemon_init() */

#define ERRNO_PRINT_ON 1
#define ERRNO_PRINT_OFF 0
#define MAXLINE 4096

static void err_doit(int errnoflag, int level, const char *fmt, va_list ap);

/* Nonfatal error related to system call
 * Print message and return */

void err_ret(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(ERRNO_PRINT_ON, LOG_INFO, fmt, ap);
	va_end(ap);
	return;
}

/* Fatal error related to system call
 * Print message and terminate */

void err_sys(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(ERRNO_PRINT_ON, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(1);
}

/* Fatal error related to system call
 * Print message, dump core, and terminate */

void err_dump(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(ERRNO_PRINT_ON, LOG_ERR, fmt, ap);
	va_end(ap);
	abort();		/* dump core and terminate */
	exit(1);		/* shouldn't get here */
}

/* Nonfatal error unrelated to system call
 * Print message and return */

void err_msg(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(ERRNO_PRINT_OFF, LOG_INFO, fmt, ap);
	va_end(ap);
	return;
}

/* Fatal error unrelated to system call
 * Print message and terminate */

void err_quit(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(ERRNO_PRINT_OFF, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(1);
}

/* Print message and return to caller
 * Caller specifies "errnoflag" and "level" */

static void err_doit(int errno_flag, int level, const char *fmt, va_list ap)
{
	int		errno_save, n;
	char	buf[MAXLINE + 1] = {0};

	errno_save = errno;		/* value caller might want printed */

#ifdef	HAVE_VSNPRINTF
	vsnprintf(buf, MAXLINE, fmt, ap);	/* safe */
#else
	vsprintf(buf, fmt, ap);					/* not safe */
#endif

	n = strlen(buf);
	if (errno_flag)
		snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));
	
	strcat(buf, "\n");

	if (daemon_proc) 
	{
		/*daemon process, use syslog*/
		syslog(level, buf);
	} 
	else 
	{
		fflush(stdout);		/* in case stdout and stderr are the same */
		fputs(buf, stderr);
		fflush(stderr);
	}
	return;
}
