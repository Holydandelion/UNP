#include	"unp.h"

//get the LSB byte, if it is char[0], then little-endian
#define ENDIANNESS ((char) endians.i)    
static union {char list[sizeof(int)]; int i;} endians = {{'L', '?', '?', 'B'}};

int
main(int argc, char **argv)
{
	union {
	  short  s;
      char   c[sizeof(short)];
    } un;

	un.s = 0x0102;
	printf("%s: ", CPU_VENDOR_OS);
	if (sizeof(short) == 2) {
		if (un.c[0] == 1 && un.c[1] == 2)
			printf("big-endian\n");
		else if (un.c[0] == 2 && un.c[1] == 1)
			printf("little-endian\n");
		else
			printf("unknown\n");
	} else
		printf("sizeof(short) = %d\n", sizeof(short));

	printf("ENDIANNESS is %c \n", ENDIANNESS);

	exit(0);
}
