#include "error.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char buf[] = "test err funtions, option:%c \n";
	if (argc != 2)
	{
		printf("usage: test_err option[rsdmq]\n");
		exit(1);
	}	
	errno = ENOENT;

	switch(argv[1][0])
	{
		case 'r':
			err_ret(buf, 'r');
			break;

		case 's':
			err_sys(buf, 's');
			break;

		case 'd':
			err_dump(buf, 'd');
			break;

		case 'm':
			err_msg(buf, 'm');
			break;
			
		case 'q':
			err_quit(buf, 'q');
			break;	
		
		default:
			printf("usage: test_err option[rsdmq]\n");
			exit(1);													
	}
	exit(0);
}