#include	"unp.h"
#include	<sys/stat.h>
#include 	<netinet/tcp.h>

int main(int argc, char **argv)
{
	int					sockfd, n;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;
	struct stat stat_buff = {0};
	
	int size = 0;
	socklen_t len = 0;
	

	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	if (fstat(sockfd, &stat_buff) < 0)
		err_sys("lstat error");		

	if (S_ISSOCK(stat_buff.st_mode))
		printf("[DEBUG]socket fd is socket descriptor, fd:%d mode:%ld  user:%d\n", sockfd, stat_buff.st_mode, stat_buff.st_uid);

	if (S_ISUID & stat_buff.st_mode)
		printf("[DEBUG]set_uid\n");
		

	if (S_ISGID  & stat_buff.st_mode)
		printf("[DEBUG]set_gid\n");
		
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(13);	/* daytime server */
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);

	if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, &len) < 0)
		err_sys("getsockopt error");		
	
	printf("before connect, recbuf size is:%d B %d KB\n", size, size/1024);	
	
	size =-1;
	len = sizeof(size);
	if (getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &size, &len) < 0)
		err_sys("getsockopt error");
			
	printf("before connect, TCP_MAXSE:%d B %d KB\n", size, size/1024);	

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");

	size =-1;
	len = sizeof(size);
	if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, &len) < 0)
		err_sys("getsockopt error");		
	
	printf("after connect, recbuf size is:%d B %d KB\n", size, size/1024);	

	size =-1;
	len = sizeof(size);	
	if (getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &size, &len) < 0)
		err_sys("getsockopt error");
			
	printf("after connect, TCP_MAXSE:%d B %d KB\n", size, size/1024);	

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
