

#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <sys/socket.h>

int main(int argc, char **argv)
{
	char	name[16];
	struct ifreq ifr;
	unsigned char *ptr = NULL;
	int i;
	int sockfd;

	if (argc != 2)
		printf("usage: prifname <interface-index>");

	printf("interface name = %s\n", if_indextoname(atoi(argv[1]), name));

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&ifr, 0x00, sizeof(ifr));
 	strncpy(ifr.ifr_name, name, sizeof(ifr.ifr_name)); 

	ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;		
	if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0)
		printf("ioctl error:%s", strerror(errno));

	ptr = (unsigned char *)ifr.ifr_hwaddr.sa_data;
	for (i=0;i<6;i++)
	{
		printf("%02x%s", ptr[i], i == 5 ? "":":");
	}
	printf("\n");	

	exit(0);
}
