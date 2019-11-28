#include	"unpthread.h"

void	*copyto(void *);

static int	sockfd;		/* global for both threads to access */
static FILE	*fp;

static int stdineof;
static pthread_mutex_t eof_mutex = PTHREAD_MUTEX_INITIALIZER;

void
str_cli(FILE *fp_arg, int sockfd_arg)
{
	char		recvline[MAXLINE];
	pthread_t	tid;

	sockfd = sockfd_arg;	/* copy arguments to externals */
	fp = fp_arg;

	Pthread_create(&tid, NULL, copyto, NULL);

	while (Readline(sockfd, recvline, MAXLINE) > 0)
		Fputs(recvline, stdout);
		
	Pthread_mutex_lock(&eof_mutex);
	if (!stdineof)
	{
		Pthread_mutex_unlock(&eof_mutex);
		err_msg("server terminated prematurely");
	}	
	Pthread_mutex_unlock(&eof_mutex);
}

void *
copyto(void *arg)
{
	char	sendline[MAXLINE];

	while (Fgets(sendline, MAXLINE, fp) != NULL)
		Writen(sockfd, sendline, strlen(sendline));

	Shutdown(sockfd, SHUT_WR);	/* EOF on stdin, send FIN */
	Pthread_mutex_lock(&eof_mutex);
	stdineof = 1;
	Pthread_mutex_unlock(&eof_mutex);
	return(NULL);
		/* 4return (i.e., thread terminates) when EOF on stdin */
}
