

#include "unp.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <stdlib.h>

#ifdef OPEN_MAX 
static long open_max = OPEN_MAX;
#else
static long open_max = 0;
#endif

#define OPEN_MAX_GUESS 1024

int main(void)
{
	int listen_fd = -1;
	int connect_fd = -1;
	int sock_fd = -1;
	int i = -1;
	int maxi = -1;
	int ready_num = -1;
	int n = 0;
	
	struct sockaddr_in client_addr;
	struct sockaddr_in server_addr;
	socklen_t addr_len;
	struct pollfd *client;
	char buf[MAXLINE] = {0};
	char temp[128] = {0};
	
	if (open_max == 0)
	{
		if ((open_max = sysconf(_SC_OPEN_MAX)) < 0)
		{
			if (errno == 0)
				open_max = OPEN_MAX_GUESS;
			else
				err_sys("sysconf error");
		}
	}
	
	client = (struct pollfd *)malloc(sizeof(struct pollfd) * open_max);
	
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		free(client);
		err_sys("socket error");
	}
	
	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(SERV_PORT);
	
	if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		free(client);
		err_sys("bind error");
	}
	
	if (listen(listen_fd, LISTENQ) < 0)
	{
		free(client);
		err_sys("listen error");
	}
	
	client[0].fd = listen_fd;
	client[0].events = POLLRDNORM;
	for (i = 1; i < open_max; i++)
		client[i].fd = -1;		/* -1 indicates available entry */
	maxi = 0;					/* max index into client[] array */

	for ( ; ; ) 
	{
		if ((ready_num = poll(client, maxi+1, INFTIM)) < 0)
		{
			free(client);
			err_sys("poll error");
		}			

		if (client[0].revents & POLLRDNORM) 
		{	/* new client connection */
			addr_len = sizeof(client_addr);
			connect_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &addr_len);
#ifdef	NOTDEF
			printf("new client: %s\n", inet_ntop(AF_INET, (struct sockaddr *) &client_addr.sin_addr, temp, addr_len));
#endif

			for (i = 1; i < open_max; i++)
			{
				if (client[i].fd < 0) 
				{
					client[i].fd = connect_fd;	/* save descriptor */
					break;
				}
			}
			
			if (i == open_max)
			{
				free(client);
				err_quit("too many clients");
			}
			
			client[i].events = POLLRDNORM;
			if (i > maxi)
				maxi = i;				/* max index in client[] array */

			if (--ready_num <= 0)
				continue;				/* no more readable descriptors */
		}

		for (i = 1; i <= maxi; i++) 
		{	/* check all clients for data */
			if ( (sock_fd = client[i].fd) < 0)
				continue;
				
			if (client[i].revents & (POLLRDNORM | POLLERR)) 
			{
				if ( (n = read(sock_fd, buf, MAXLINE)) < 0) 
				{
					if (errno == ECONNRESET) 
					{
							/*4connection reset by client */
#ifdef	NOTDEF
						printf("client[%d] aborted connection\n", i);
#endif
						close(sock_fd);
						client[i].fd = -1;
					} else
					{
						free(client);	
						err_sys("read error");
					}	
				} else if (n == 0) 
				{
						/*4connection closed by client */
#ifdef	NOTDEF
					printf("client[%d] closed connection\n", i);
#endif
					close(sock_fd);
					client[i].fd = -1;
				} else
					writen(sock_fd, buf, n);

				if (--ready_num <= 0)
					break;				/* no more readable descriptors */
			}
		}
	}
		
	exit(0);
	
}