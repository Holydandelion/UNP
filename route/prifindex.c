#include <net/if.h>
#include <stdio.h>

int
main(int argc, char **argv)
{
	if (argc != 2)
		err_quit("usage: prifname <interface-name>");

	printf("interface index = %d\n", if_nametoindex(argv[1]));
	exit(0);
}
