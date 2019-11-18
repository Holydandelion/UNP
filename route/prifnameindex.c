#include <stdio.h>
#include <net/if.h>
#include "unp.h"
#include <sys/socket.h>

int
main(int argc, char **argv)
{
	int					n;
	int					i;
	int 				sockfd;
	char				ifname[IFNAMSIZ];
	unsigned char		*ptr = NULL;
	struct if_nameindex	*ifptr, *save;
	struct ifreq ifr;

	if (argc != 1)
		err_quit("usage: prifnameindex");

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
		/* print all the interface names and indexes */
	for (save = ifptr = if_nameindex(); ifptr->if_index > 0; ifptr++) 
	{
		printf("name = %s, index = %d ", ifptr->if_name, ifptr->if_index);;

		if ( (n = if_nametoindex(ifptr->if_name)) != ifptr->if_index)
			err_quit("if_nametoindex returned %d, expected %d, for %s",
						n, ifptr->if_index, ifptr->if_name);

		if_indextoname(ifptr->if_index, ifname);
		if (strcmp(ifname, ifptr->if_name) != 0)
			err_quit("if_indextoname returned %s, expected %s, for %d",
					 ifname, ifptr->if_name, ifptr->if_index);
		
		memset(&ifr, 0x00, sizeof(ifr));		 
		strncpy(ifr.ifr_name, ifptr->if_name, sizeof(ifr.ifr_name));
		if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0)
			printf("ioctl error:%s", strerror(errno));
			

		ptr = (unsigned char *)ifr.ifr_hwaddr.sa_data;
		for (i=0;i<6;i++)
		{
			printf("%02x%s", ptr[i], i == 5 ? "":":");
		}
		printf("\n");			
		
	}

	n = if_nametoindex("fkjhkjhgjhgjhgdjhguyetiuyiuyhkjhkjdh");
	if (n != 0)
		err_quit("if_nametoindex returned %d for fkjh...", n);
		
	n = if_nametoindex("");
	if (n != 0)
		err_quit("if_nametoindex returned %d for (null)", n);

	if (if_indextoname(0, ifname) != NULL)
		err_quit("if_indextoname error for 0");
		
	if (if_indextoname(888888, ifname) != NULL)
		err_quit("if_indextoname error for 888888");

	if_freenameindex(save);
	exit(0);
}
