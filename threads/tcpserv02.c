#include	"unpthread.h"

static void	*doit(void *);		/* each thread executes this function */

static void str_echo_new(int sockfd);

int
main(int argc, char **argv)
{
	int				listenfd, *iptr;
	pthread_t		tid;
	socklen_t		addrlen, len;
	struct sockaddr	*cliaddr;

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], &addrlen);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
	else
		err_quit("usage: tcpserv01 [ <host> ] <service or port>");

	cliaddr = Malloc(addrlen);

	for ( ; ; ) {
		len = addrlen;
		iptr = Malloc(sizeof(int));
		*iptr = Accept(listenfd, cliaddr, &len);
		Pthread_create(&tid, NULL, &doit, iptr);
	}
}

static void *
doit(void *arg)
{
	int		connfd;

	connfd = *((int *) arg);
	free(arg);

	Pthread_detach(pthread_self());
	str_echo_new(connfd);		/* same function as before */
	Close(connfd);			/* done with connected socket */
	return(NULL);
}


void
str_echo_new(int sockfd)
{
	ssize_t		n;
	char		buf[MAXLINE];

again:
	while ( (n = Readline_r(sockfd, buf, MAXLINE)) > 0)
		Writen(sockfd, buf, n);

	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		err_sys("str_echo: read error");
}