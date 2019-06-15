#include	"unp.h"

int main(int argc, char **argv)
{
	int					sockfd, n;
	int ret = -1;
	socklen_t len = 0;
	char				recvline[MAXLINE + 1];
	char				buff[MAXLINE] = {0};
	struct sockaddr_in	servaddr;
	struct sockaddr_in  cliaddr;
	
	memset(&cliaddr, 0, sizeof(cliaddr));
	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(9999);	/* daytime server */
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");
	
	len = sizeof(cliaddr);
	ret = getsockname(sockfd, (struct sockaddr *)&cliaddr, &len);
	if(ret < 0)
		printf("[ERROR]:getsockname error:%s\n", strerror(errno));
	len = sizeof(cliaddr);
	printf("[DEBUG]:local:%s:%d \n", inet_ntop(AF_INET, (void *)&cliaddr.sin_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));
	
	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) 
	{
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF)
			err_sys("fputs error");
	}
	if (n < 0)
		err_sys("read error");

	Close(sockfd);	
	printf("close connection before exit.\n");
	
	exit(0);
}
