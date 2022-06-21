#include "rake-c.h"


// -------------------------------------------------------------------------------------------------	

// I/O MULTIPLEXING, CHOOSE A SERVER WITH LOWEST QUOTATION
// RETURN SOCKET_DESCRIPTOR THAT CLIENT USED TO CONTACT THAT SERVER
int choose_server()
{
	fd_set all_fds_read;	// KEEP TRACK OF ALL DESCRIPTORS WE HAVE SEEN
	fd_set read_fds;		// KEEP TRACK OF DESCRIPTORS THAT AER READY TO READ	

// BEFORE WE USE THEM, WE ZERO THEM OUT
	FD_ZERO(&all_fds_read);	
	FD_ZERO(&read_fds);	

	int sd;
	int fd_max = -1;	

// FOR EACH REMOTE HOST
	for (int i=0 ; i<nhosts ; ++i)
	{
// CLIENT GETS A SOCKET DESCRIPTOR AND CONNECT TO THAT REMOTE SERVER
		sd = create_socket_connect(hosts[i]);	
		hosts[i].sd = sd;
// QUERY 
		send_a_request(sd);
		FD_SET(sd, &all_fds_read);		

		if (sd > fd_max)
			fd_max = sd;
	}

	int quotation;
	int lowest_quotation 			= 100;	// QUTATION RANGES FROM 0 .. 99, 100 IS OKAY TO USE
	int sd_with_lowest_quotation 	= -1; 
	int nconnections 				= nhosts;

// WAIT FOR REPLIES FROM ALL REMOTE SERVERS
	while (true)
	{
		read_fds = all_fds_read;	

		if (select(fd_max+1, &read_fds, NULL, NULL, NULL) == -1)
		{
			perror("select");
			exit(EXIT_FAILURE);
		}

// ITERATE THROUGH EACH POSSIBLE DESCRIPTORS
		for (int i=0 ; i<=fd_max ; ++i)
		{
			if (FD_ISSET(i, &read_fds))		// READY TO READ
			{
// RECV A QUERY REPLY FROM A SERVER
				quotation = recv_a_reply(i);
				printf("Client: On socket %d recv a reply <== quotation %d\n", i, quotation);

// KICK IT OUT OF aall_fds_read
				FD_CLR(i, &all_fds_read); 	

// FOUND A BETTER ONE ?
				if (quotation < lowest_quotation)
				{
					lowest_quotation = quotation;

					if (sd_with_lowest_quotation != -1) // ACCOUNT FOR OUR INITIALIZATION
					{
						close(sd_with_lowest_quotation);
						printf("Socket descriptor %d is closed\n", sd_with_lowest_quotation);
						--nconnections;	
					}

					sd_with_lowest_quotation = i;	// I AM THE BEST ONE NOW
				}

// OTHERWISE
				else
				{
					close(i);
					printf("Socket descriptor %d is closed\n", i);
					--nconnections;
				}
				
			}
		}

// CLIENT HAS DETERMINED WHICH SERVER IS THE ONE WITH LOWEST QUTATION
		if (nconnections == 1)
				break;
	}

	printf("SOCKET_DESCRIPTOR THAT CLIENT USED TO CONTACT THE SERVER WITH THE LOWEST QUOTATION: %d\n", 
		sd_with_lowest_quotation);
	return sd_with_lowest_quotation;
}


// -------------------------------------------------------------------------------------------------	

// CLINET ATTEMPTS TO RECONNECT TO THE SERVER WITH THE LOWEST QUTATION REPLY
int reconnect(int sd_with_lowest_quotation)
{
	int sd; 

// FOR EAHC REMOTE HOSTS
	for (int i=0 ; i<nhosts ; ++i)
	{
// ARE YOU THE ONE I FOUND JSUT NOW ?
		if (hosts[i].sd == sd_with_lowest_quotation)
		{
			sd = create_socket_connect(hosts[i]);	// RECONNECT TO THAT SERVER
			printf("Client reconnects to server listening on port %s\n", hosts[i].port);
		}		
	}

	return sd;
}
