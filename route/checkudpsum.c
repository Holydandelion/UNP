#include	"unproute.h"
#include	<netinet/udp.h>
/* #include	<netinet/ip_var.h>
/*#include	<netinet/udp_var.h>		/* for UDPCTL_xxx constants */

struct ipvolg{
    u_char  ih_x1 [9];  //9 bit
    u_char  ih_pr;      //1 bit
    u_short     ih_len;  //2 bytes
    struct in_addr  ih_src; //4 bytes;
    struct in_addr  ih_dst; //4 bytes;
};
struct udpiphdr{
    struct ipvolg ui_i;
    struct udphdr ui_u;
};

#define ui_x1           ui_i.ih_x1
#define ui_pr           ui_i.ih_pr
#define ui_sum          ui_i.ih_len
#define ui_src          ui_i.ih_src
#define ui_dst          ui_i.ih_dst
#define ui_sport        ui_u.source
#define ui_dport        ui_u.dest
#define ui_ulen         ui_u.len


int
main(int argc, char **argv)
{
	int		mib[4], val;
	size_t	len;

	mib[0] = CTL_NET;
	mib[1] = AF_INET;
	mib[2] = IPPROTO_UDP;
	mib[3] = UDPCTL_CHECKSUM;

	len = sizeof(val);
	Sysctl(mib, 4, &val, &len, NULL, 0);
	printf("udp checksum flag: %d\n", val);

	exit(0);
}
