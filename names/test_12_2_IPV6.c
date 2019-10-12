#include	"unp.h"

int
main(int argc, char **argv)
{
	int					listenfd;
	const int 			on = 1;
	struct sockaddr_in6	servaddr;

	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");

	listenfd = Socket(AF_INET6, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin6_family      = AF_INET6;
	servaddr.sin6_port        = htons(9999);	
	if (inet_pton(AF_INET6, argv[1], &servaddr.sin6_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);

	Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	Setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));	
	
	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);
	
	pause();
	exit(0);
}
