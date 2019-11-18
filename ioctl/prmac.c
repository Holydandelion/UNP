#include	"unpifi.h"
#include	<net/if_arp.h>

int
main(int argc, char **argv)
{
	int sockfd	= -1;
	int i		= 0; 	
	struct ifi_info	*ifi, *ifihead;
	struct ifreq ifr;
	unsigned char *ptr = NULL;
	
	struct sockaddr_in *sin = NULL;
	struct sockaddr_in6 *sin6 = NULL;	
	char 	str[128] = {0};

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
		err_sys("socket error");

	for (ifihead = ifi = Get_ifi_info(AF_INET, 1); ifi != NULL; ifi = ifi->ifi_next) 
	{
		printf("%s: ", ifi->ifi_name);
		memset(&ifr, 0x00, sizeof(ifr));
		
		strncpy(ifr.ifr_name, ifi->ifi_name, sizeof(ifr.ifr_name));
		/*
		if (ioctl(sockfd, SIOCGIFADDR, &ifr) < 0)
			err_sys("1-ioctl error");
		
		if (ifr.ifr_addr.sa_family == AF_INET)
		{
			sin = (struct sockaddr_in *)&ifr.ifr_addr;
			memset(str, 0x00, sizeof(str));
			printf("ipv4 address:%s ", inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) );
		}
		else if (ifr.ifr_addr.sa_family == AF_INET6)
		{
			sin6 = (struct sockaddr_in6 *)&ifr.ifr_addr;
			memset(str, 0x00, sizeof(str));
			printf("ipv6 address:%s  ", inet_ntop(AF_INET6, &sin6->sin6_addr, str, sizeof(str)) );
		}
		*/
		
		ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;		
		if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0)
			err_sys("2-ioctl error");

		ptr = (unsigned char *)ifr.ifr_hwaddr.sa_data;
		for (i=0;i<6;i++)
		{
			printf("%02x%s", ptr[i], i == 5 ? "":":");
		}
		printf("\n");
	}
	free_ifi_info(ifihead);
	exit(0);
}
