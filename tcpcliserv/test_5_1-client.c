


#include "unp.h"
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

void request(FILE *fp, int sockfd);

int main(int argc, char *argv[])
{
	int sockfd = -1;
	struct sockaddr_in server_addr;
	
	if (argc != 2)
		err_quit("usage:test_5_1-client <ipaddress>");
		
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");
		
	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERV_PORT);
	if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) != 1)
		err_sys("inet_pton error");
		
	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		err_sys("connect error");
		
	request(stdin, sockfd);
	exit(0);
}

void request(FILE *fp, int sockfd)
{
	char sendline[MAXLINE] = {0};
	char recvline[MAXLINE] = {0};	
	
	while (fgets(sendline, sizeof(sendline), fp) != NULL)
	{
		writen(sockfd, sendline, strlen(sendline));
		
		if (readline(sockfd, recvline, sizeof(recvline)) == 0)
			err_quit("request:server terminated prematurely");
			
		fputs(recvline, stdout);
	}
}