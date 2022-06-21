#include "rake-c.h"


// -------------------------------------------------------------------------------------------------	

// CREAT A SOCKET DESCRIPTOR AND USE IT TO CONNECT TO A REMOTE HOST
int create_socket_connect(HOST host)
{
	struct addrinfo hints;
	struct addrinfo *res = NULL;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_family 	= PF_UNSPEC;
	hints.ai_socktype 	= SOCK_STREAM;
	hints.ai_protocol 	= IPPROTO_TCP;

	int error_code;
	if ((error_code = getaddrinfo(host.name, host.port, &hints, &res)) != 0)
	{
		fprintf(stderr, "Client: getaddrinfo: %s\n", gai_strerror(error_code));
		exit(EXIT_FAILURE);
	}

	struct 	addrinfo *ptr = res;
	int 	sd;

	while (ptr != NULL)
	{
		if ((sd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == -1)
		{
			perror("socket");
			ptr = ptr->ai_next;		
		}

		if (connect(sd, ptr->ai_addr, ptr->ai_addrlen) == -1)
		{
			perror("connect");
			ptr = ptr->ai_next;		
		}

		break;
	}

	if (ptr == NULL)
	{
		fprintf(stderr, "Client: failed to connect\n");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(res);	
	return sd;
}
