#include "server.h"


// -------------------------------------------------------------------------------------------------

void set_up_server()
{
	struct addrinfo hints;
	struct addrinfo *res = NULL;

// BEFOER WE USE IT, WE ZERO IT OUT
	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_flags 		= AI_PASSIVE;
	hints.ai_family 	= PF_UNSPEC;
	hints.ai_socktype 	= SOCK_STREAM;
	hints.ai_protocol 	= IPPROTO_TCP; 		

	int error_code;
	if ((error_code = getaddrinfo(NULL, PORT, &hints, &res)) != 0)
	{
		fprintf(stderr, "Server: getaddrinfo: %s\n", gai_strerror(error_code));
		exit(EXIT_FAILURE);
	}

	struct 	addrinfo *ptr = res;
	int 	sd;
	int 	option_value  = 1;	

	while (ptr != NULL)
	{
		if ((sd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == -1)
		{
			perror("Server: socket");
			ptr = ptr->ai_next;	
		}

		if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int)) == -1)
		{
			perror("Server: setsockopt");
			exit(EXIT_FAILURE);
		}

		if (bind(sd, ptr->ai_addr, ptr->ai_addrlen) == -1)
		{
			close(sd);
			perror("Server: bind");
			ptr = ptr->ai_next;	
		}

		break;
	}

	if (ptr == NULL)
	{
		fprintf(stderr, "Server: Failed to bind\n");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(res); // WE DON'E NEED IT ANYMORE, WE FREE IT
	printf("The server is listening on port %s\n", PORT);

	if (listen(sd, BACKLOG) == -1)
	{
		perror("Server: listen");
		exit(EXIT_FAILURE);
	}

	int 					accepted_sd;
	struct sockaddr_storage connecting_entity_addr;
	socklen_t 				addr_len = sizeof(struct sockaddr);
	bool					keep_going = true;

	while (keep_going)
	{
		if ((accepted_sd = accept(sd, (struct sockaddr *)&connecting_entity_addr, &addr_len)) == -1)
		{
			perror("Server: accept");
			continue;
		}

		printf("A client connection is established. On socket %d\n", accepted_sd);

// BEFORE THE SERVER forK() A NEW PROCESS TO PROVIDE SERVICE,
// header.type CAN ONLY BE ==> CMD_QUOTE_REQUEST OR CMD_ACTIONSET

		HEADER header;
		read_socket(accepted_sd, &header, sizeof(HEADER));

// REPLY TO A QUERY, AND THE SERVER EXPECTS A RECONNECTION
		if (header.type == CMD_QUOTE_REQUEST)
			return_a_price(accepted_sd);

// IF WE GET TO THIS POINT, THE SERVER EXECUITES THIS ACTIONSET
		else
		{
			switch (fork())
			{	
				case -1 :	// SOMETHING WENT WRONG
				{
					keep_going = false;
					perror("fork");
					break;
				}

				case 0 :	// CHILD PROCESS ==> HANDLE CMD_EXECUTE
				{
// CHILD PROCESS EXECUTES COMMANDS IN AN ACTIONSET, IT DOES NOT NEED TO LISTEN TO OTHER CONNECTIONS
					close(sd);
					execution(accepted_sd, header.len_content, header.len);	
					shutdown(accepted_sd, SHUT_RDWR);
					close(accepted_sd);
					exit(EXIT_SUCCESS); 
					break;
				}

				default :	// PARENT PROCESS
				{
// WAIT FOR ITS CHILD PROCESS
					wait(NULL);		
// PARENT PROCESS DON'T NEED IT
					shutdown(accepted_sd, SHUT_RDWR);
					close(accepted_sd);
					break;
				}
			}
		}		
	}
}
