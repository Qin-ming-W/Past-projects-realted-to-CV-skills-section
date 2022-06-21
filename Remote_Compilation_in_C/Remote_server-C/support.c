#include "server.h"


// -------------------------------------------------------------------------------------------------	

ssize_t read_socket(int sd, void *buf, size_t nbytes)
{
	ssize_t total 	  = 0;				// HOW MANY BYTES WE HAVE READ SO FAR
	size_t 	byte_left = nbytes;  		// HOW MANY BYTES WE HAVE LEFT TO READ
	int 	n; 

	while (total < nbytes)
	{
		n = read(sd, buf, byte_left);
		if (n == -1)					// SOMETHING WENT WRONG
		{
			perror("read");
			total = -1;
			break;
		}	

		total 		+= n;				
		byte_left 	-= n;
		buf 		= (char *)buf + total;	
	}

	return total;
}

// -------------------------------------------------------------------------------------------------	

ssize_t write_socket(int sd, void *buf, size_t nbytes)
{
	ssize_t total 	  = 0;			// HOW MANY BYTES WE HAVE SENT SO FAR
	size_t  byte_left = nbytes;  	// HOW MANY BYTES WE HAVE LEFT TO SENT
	int 	n; 

	while (total < nbytes)
	{
		n = write(sd, buf, byte_left);
		if (n == -1)				// SOMETHING WENT WRONG
		{
			total = -1;
			break;
		}	

		total 		+= n;
		byte_left 	-= n;
		buf 		= (char *)buf + total;
	}

	return total;
}
