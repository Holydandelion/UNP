

#include "unp.h"
#include <stdlib.h>
#include <sys/socket.h>


int main(void)
{
	int sockfd = -1;
	int size = -1;
	socklen_t len = 0;
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error"); 
	
	len = sizeof(size);	
	if (getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &size, &len) < 0)
		err_sys("getsockopt error");
	
	printf("recbuf size is:%d B %d KB\n", size, size/1024);
		
	size = -1;
	len = sizeof(size);
	if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, &len) < 0)
		err_sys("getsockopt error");		
	
	printf("recbuf size is:%d B %d KB\n", size, size/1024);
	exit(0);
}