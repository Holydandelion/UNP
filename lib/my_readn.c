#include "unp.h"

ssize_t read_n(int fd, void *vptr, size_t n)
{
	char *ptr = vptr;
	size_t num_left = n;
	ssize_t num_read = 0;	

	printf("[DEBUG]: call %s.\r\n", __FUNCTION__);	
	while (0 < num_left)
	{
		if ( (num_read=read(fd, ptr, num_left)) < 0)
		{
			if (EINTR == errno)
			{
				num_read = 0;
			}
			else 
				return (-1);
		}
		else if (num_read ==0)
			break;

		printf("[DEBUG]: num_read: %d.\r\n", num_read);				
		num_left -= num_read;
		ptr += num_read;
	}
	printf("[DEBUG]: call %s success. num_left:%d \r\n", __FUNCTION__, num_left);
	return (n - num_left);
}