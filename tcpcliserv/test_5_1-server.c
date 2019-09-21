


#include "unp.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

void serve(int sockfd);

int main(void)
{
	int listen_fd = -1;
	int connect_fd = -1;
	pid_t child_pid = -1;
	socklen_t addr_len = 0;
	struct sockaddr_in	server_addr;
	struct sockaddr_in	client_addr;
	
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd < 0)
		err_sys("socket error ");
	
	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERV_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		err_sys("bind error ");
	
	if (listen(listen_fd, LISTENQ) < 0)
		err_sys("listen error");
	
	for (; ;)
	{
		addr_len = sizeof(client_addr);
		connect_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addr_len);
		
		if (connect_fd < 0)
			err_sys("accept error ");
			
		if ((child_pid = fork()) == 0)
		{
			close(listen_fd);
			serve(connect_fd);
			exit(0);
		}
		else if (child_pid < 0)
			err_sys("fork error");
			
		close(connect_fd);
	}
	
	return 0;
}

void serve(int sockfd)
{
	ssize_t n = 0;
	char buf[MAXLINE] = {0};
	
again:
	while ((n=read(sockfd, buf, sizeof(buf))) > 0)
		writen(sockfd, buf, n);
	
	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		err_sys("read error ");
}