#include	"unp.h"

int
main(int argc, char **argv)
{
	int				sockfd, n;
	char			recvline[MAXLINE + 1];
	socklen_t		len;
	struct sockaddr_storage	ss;

	if (argc != 3)
		err_quit("usage: daytimetcpcli <hostname/IPaddress> <service/port#>");

	sockfd = Tcp_connect(argv[1], argv[2]);

	len = sizeof(ss);
	Getpeername(sockfd, (SA *)&ss, &len);
	printf("connected to %s\n", Sock_ntop_host((SA *)&ss, len));

	
	for (; ;)
	{
		if ( (n = recv(sockfd, recvline, MAXLINE, MSG_PEEK)) == 0)
			break;
		else if (n < 0)
			err_sys("recv error: \n");
		
		ictol(sockfd, FIONREAD, &pend_read);
		printf("%d bytes from recv, %d bytes from pendind \n", n, pend_read);	
		
		n = Read(sockfd, recvline, MAXLINE);
		recvline[n] = 0;	/* null terminate */
		Fputs(recvline, stdout);
		 
	}
	exit(0);
}
