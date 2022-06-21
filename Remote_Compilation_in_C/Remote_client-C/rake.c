#include "rake-c.h"


// -------------------------------------------------------------------------------------------------	

int main(int argc, char const *argv[])
{
	if (argc != 1)
	{
		fprintf(stderr, "No argument should be given\n");
		exit(EXIT_FAILURE);
	}

// PARSE THE Rakefile AND STORE ITS INFORMATION IN THE DATA STRUCTURE FOR FUTURE USE
	parse_Rakefile();

// DUMP IT OUT TO SEE WHAT WE HAVE STORED
	dump();

// FOR EACH ACTIONSET
	for (int i=0 ; i<nsets ; ++i)
	{
// CHOOSE THE REMOTE SERVER WITH LOWEST QUTOTATION AND RECONNECT TO IT
		int sd_with_lowest_quotation = choose_server(); 
		int sd 						 = reconnect(sd_with_lowest_quotation);	

		CMD *ptr = cmds[i];	// TAKE A COPY, ENSURE WE DON'T MODIFY cmds[i]	

// COUNT THE NUMBER OF COMMANDS IN AN ACTIONSET
		int ncmds = count_remote_cmds(ptr);

// SEND A HEADER OF TYPE CMD_ACTIONSET TO INFORM THE SERVER THE NUMBER OF INCOMING COMMANDS 
		send_an_actionset(sd, ncmds, i+1);				

// SEND EACH COMMANDS IN AN ACTIONSET
		while (ptr != NULL)
		{
			if (!ptr->is_local)					
				send_a_cmd(sd, ptr);	
			ptr = ptr->next;	// GO TO THE NEXT ONE
		}

// WERE ALL COMMANDS EXECUTED SUCCESSFULLY ? 
// CAN WE GO TO THE NEXT ACTIONSET
		bool next_one = next_actionset(sd, ncmds);

// FOR EACH COMMAND, ANYTHING WILL BE PRINTED TO stdout, stderr
		for (int i=0 ; i<ncmds ; ++i)
		{
			recv_a_file(sd);	// CMD_RETURN_STDOUT
			recv_a_file(sd);	// CMD_RETURN_STDERR
		}

// READ A HEADER OF TYPE CMD_NOUTPUTS, TO KNOW THE NUMBER OF INCOMING OUTPUT FILES (*.O ..)
		HEADER h;
		read_socket(sd, &h, sizeof(HEADER)); 

// RECV EACH ONE OF THEM
		for (int i=0 ; i<h.len ; ++i)
			recv_a_file(sd);

// GO TO THE NEXT ACTIONSET OR NOT ?
		if (!next_one) 
		{
			fprintf(stderr, "Something went wrong is the privous actionset\n");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}
