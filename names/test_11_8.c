#include	"unp.h"
#include	<time.h>

int
main(int argc, char **argv)
{
	int				listenfd, connfd;
	socklen_t		len, addrlen;
	char			buff[MAXLINE];
	char			hostname[48] = {0};
	char			servname[48] = {0};
	time_t			ticks;
	struct sockaddr_storage	cliaddr;
	struct sockaddr_in6	tempaddr;
	int n = 0;

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], &addrlen);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
	else
		err_quit("usage: daytimetcpsrv2 [ <host> ] <service or port>");

	for ( ; ; ) {
		len = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *)&cliaddr, &len);
		
		if ((n = getnameinfo((struct sockaddr *)&cliaddr, len, hostname, (socklen_t)sizeof(hostname), servname, (socklen_t)sizeof(servname), 
			NI_NUMERICHOST|NI_NUMERICSERV) != 0))
			err_sys("getnameinfo error, %s\n", gai_strerror(n));
			
		printf("connection from %s:%s \n", hostname, servname);
		

		if (cliaddr.ss_family == AF_INET6)
		{
			memset(&tempaddr, 0x00, sizeof(tempaddr));
			memcpy(&tempaddr, &cliaddr, sizeof(tempaddr));		
			if (IN6_IS_ADDR_V4MAPPED(&tempaddr.sin6_addr))
				printf("address is v4mapped ipv6 address.\n");
		}
		
		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		Write(connfd, buff, strlen(buff));

		Close(connfd);
	}
}
