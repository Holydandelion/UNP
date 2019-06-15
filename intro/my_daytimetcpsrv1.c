#include "unp.h"
#include <time.h>

int main(int argc, char **argv)
{
	int ret = -1;
	int listenfd = -1;
	int connfd = -1;
	int len = 0;
	int listenq_len = 0;
	char buff[MAXLINE] = {0};
	char *ptr = NULL;
	time_t ticks;
	struct sockaddr_in servaddr, cliaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));	
	memset(&ticks, 0, sizeof(ticks));	
		
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if  (listenfd < 0)
	{	
		printf("[ERROR]:socket error:%s \n", strerror(errno));
		exit(-1);
	}
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(9999);

	printf("[DEBUG]:servaddr.sin_port:%d \n", ntohs(servaddr.sin_port));		
	
	/*
	ret = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if (ret < 0)
	{	
		printf("[ERROR]:bind error:%s \n", strerror(errno));			
		exit(-1);	
	}
	*/
	ptr = getenv("UNP_LISTENQ");
	if (ptr == NULL)
		listenq_len = LISTENQ;
	else
		listenq_len	= atoi(ptr);
	
	ret = listen(listenfd, listenq_len);		
	if (ret < 0)
	{
		printf("[ERROR]:listen error:%s \n", strerror(errno));					
		exit(-1);	
	}
	
	printf("[DEBUG]:listenq_len:%d \n", listenq_len);		
	
	for (;;)
	{
		len = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, (socklen_t *)&len);	
		if (connfd < 0 )
		{
			printf("[ERROR]:accept error:%s \n", strerror(errno));
			exit(-1);	
		}	
		printf("connected from %s:%d \n",inet_ntop(AF_INET, &cliaddr.sin_addr,  buff, sizeof(buff)), ntohs(cliaddr.sin_port));		
		memset(&buff, 0, sizeof(buff));
		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		write_n(connfd, &buff, strlen(buff));
		close(connfd);
	}
	
	exit(0);
}