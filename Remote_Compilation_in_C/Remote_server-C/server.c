#include "server.h"


// -------------------------------------------------------------------------------------------------

int main(int argc, char const *argv[])
{
	if (argc != 1)
	{
		fprintf(stderr, "No argument should be given\n");
		exit(EXIT_FAILURE);
	}

// RUN THE SERVER
	set_up_server();

	return 0;
}
