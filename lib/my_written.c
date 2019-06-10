#include "unp.h"

ssize_t  write_n(int fd, const void *vptr, size_t n)
{
	size_t num_left = n;
	ssize_t num_written = 0;
	char *ptr = vptr;
	
	while (num_left > 0)
	{
		num_written = write(fd, ptr, num_left);
		if (num_written <= 0)
		{
			if (EINTR == errno && num_written < 0) //write 0 byte is an error
				num_written = 0;
			else
				return (-1);
		}
		num_left -= num_written;
		ptr += num_written; 	
		
		printf("[DEBUG]:num_written:%d num_left:%d\r\n", num_written, num_left);
	} 
	return(n);
}