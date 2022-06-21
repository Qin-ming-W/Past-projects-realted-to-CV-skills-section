#include "server.h"


// -------------------------------------------------------------------------------------------------

// GENERATE A IMAGINARY QUOTATION, RANGES FROM 0 .. 99
static int make_a_quotation()
{
	srand(getpid());
	return rand() % 100;
}

// -------------------------------------------------------------------------------------------------

// SEND BACK THE QUOTATION BACK TO CLIENT
void return_a_price(int sd)
{
	int quotation = make_a_quotation();

	printf("Reply %d\n", quotation);

	// SLEEP FOR A RANDOM PERIOD OF TIME BEFORE ECHOING BACK TEXT
	sleep(getpid() % 5 + 2);

// PREPARE A HEADER OF TYPE CMD_QUOTE_REPLU
	HEADER header;

	header.type 		= CMD_QUOTE_REPLY;
	header.len  		= sizeof(int);
	header.len_content 	= 0;

	write_socket(sd, &header, sizeof(HEADER));	// SEND THE HEADER
	write_socket(sd, &quotation, header.len);	// SEND THE QUOTATION

// CLOSE THE CONNECTION, IF I AM THE ONE WITH THE LOWEST PRICE, CLIENT WILL RECONNECT TO ME
	shutdown(sd, SHUT_WR);
	close(sd);
}

// -------------------------------------------------------------------------------------------------

// FIND OUT THE SIZE OF A FILE
static long find_file_size(char *filename)
{
	if (filename == NULL)
		exit(EXIT_FAILURE);

	struct stat statinfo;

	//  ENSURE THAT WE CAN GET THE ATTRIBUTES OF THIS PATHNAME
	if (stat(filename, &statinfo) != 0) 
	{        
		perror("stat");
		exit(EXIT_FAILURE);
    }

    return statinfo.st_size;
}

// -------------------------------------------------------------------------------------------------

// RETURN A FILE BACK TO CLIENT, THE RETURNED FILE CAN BE OF THREE TYPES
// CMD_RETURN_FILE, CMD_RETURN_STDOUT, CMD_RETURN_STDERR
void return_a_file(int sd, char *filename, CMDTYPE type)
{
	if (filename == NULL)
		exit(EXIT_FAILURE);

	char *fullpath = filename;	// TAKE A COPY HERE

// IF IT IS AN OUTPUT FILE, THEN STRIP OF /tmp/x-cmd-x/ BEFORE SENDING IT BACK
	if (type == CMD_RETURN_FILE)
	{
		filename = strrchr(filename, '/');
		++filename;
	}

// PREPARE A HEADER OF APPROPRIATE TYPE
	HEADER header;

	header.type 		= type;
	header.len 			= strlen(filename) + 1; 	// RESERVER ONE BYTE FOR '\0'
	header.len_content 	= find_file_size(fullpath);

	write_socket(sd, &header, sizeof(HEADER));		// SEND THE HEADER
	write_socket(sd, filename, header.len);			// SEND THE FILENAME

	int fd = open(fullpath, O_RDONLY);

	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	char 	buf[BUFSIZ];	// SEND THE FILE ONE CHUNCK AT A TIME
	size_t 	got;

	// SEND THE ACTUAL CONTENT OF A FILE
	while ((got = read(fd, buf, sizeof(buf))) > 0)	
	{
		if ((write_socket(sd, buf, got)) != got)	// SOMETHING WENT WRONG
		{
			close(fd);
			shutdown(sd, SHUT_WR);
			close(sd);
			exit(EXIT_FAILURE);
		}
	}

// WE OPENED IT, WE CLOSE IT
	close(fd);
}

// -------------------------------------------------------------------------------------------------

// RETURN A STATUS TO INFORM CLIENT IF EXECUTION WAS SUCCESSFUL
void return_status(int sd, int status)
{
// PREPARE A HEADER OF CMD_RETURN_STATUS
	HEADER header;

	header.type 		= CMD_RETURN_STATUS;
	header.len 			= sizeof(int);
	header.len_content 	= 0;

	write_socket(sd, &header, sizeof(HEADER));	// SEND BACK HEADER
	write_socket(sd, &status, header.len);		// SEND BACK STATUS
}

// -------------------------------------------------------------------------------------------------

// RECV A FILE FROM CLIENT 
void recv_a_file(int sd)
{
	HEADER header;
	read_socket(sd, &header, sizeof(HEADER));

	char filename[header.len];
	long len_content = header.len_content;		// THE SIZE OF INCOMING FILE

	read_socket(sd, filename, header.len);		// GET FILENAME
	filename[header.len] = NULLBYTE;

// ATTEMPTS TO CREATE A FILE FOR WRITING
	int fd = open(filename, O_WRONLY | O_CREAT, 0777);

	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	char buf[len_content];	// BUFFERING THE INCOMING FILE
	long total 	= 0;		// HOW MANY BYTES WE HAVE READ SO FAR
	long got 	= 0;		// HOW MANY BYTES WE HAVE READ IN ONE read_socket()

	while ((got = read_socket(sd, buf, len_content)) > 0)
	{
		if (write(fd, buf, got) != got) // SOMETHING WENT WRONG
		{
			close(fd);	// sd IS NOT IMMEDIATELY CLOSED
			exit(EXIT_FAILURE);
		}

		total += got;

		if (total == len_content) 	// WE DONE ?
			break;
	}

// WE OPENED IT, WE CLOSE IT
	close(fd);
}

// -------------------------------------------------------------------------------------------------

// THE SERVER EXECUTES A COMMAND SENT BY A CLIENT
void execute_a_cmd(int sd, int actionset, int order)
{
	HEADER header;
// GET HINTS ABOUT THIS COMMAND
	read_socket(sd, &header, sizeof(HEADER)); 

	char cmd[header.len];
// GET COMMAND
	read_socket(sd, cmd, header.len);			
	cmd[header.len] = NULLBYTE;	

// DOES THIS COMMAND NEED ANY FILE ?
	int nfiles = header.len_content;

// PREPARE ARGUMENTS VECTOR FOR FUTURE CALL OF execvp()
	int nwords;
	char **words = strsplit(cmd, &nwords);

	char *path = strdup(words[0]);

	char **argv = malloc(sizeof(char *) * nwords+1);
	CHECK_ALLOC(argv);

	int j;
	for (j=0 ; j<nwords ; ++j)
		argv[j] = strdup(words[j]);
	argv[j] = NULL;

	free_words(words); // WE DON'E NEED IT ANYMORE, WE FREE IT

// CONSTRUC THE NAME OF A TEMPORARY DIRECTORT
	char dirname[MAXPATHLEN];
	sprintf(dirname, "/tmp/%d-cmd-%d", actionset, order);

// MAEK IT AND CHANGE INTO IT
	mkdir(dirname, 0777);
	chdir(dirname);

// WE CHANGED INTO OUR OWN CLEAN DIRECTORY, RECV ANY ADDITIONAL FILES FROM HERE
	for (int i=0; i<nfiles ; ++i)
		recv_a_file(sd);

// CREATE A FILE "STDOUT.txt" FOR POSSIBLE 
	int fd_1 = open("STDOUT.txt", O_WRONLY | O_CREAT, 0777);

	if (fd_1 == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

// DUPLICATE FILE DESCRIPTOR
	dup2(fd_1, STDOUT_FILENO);

// SAME HERE
	int fd_2 = open("STDERR.txt", O_WRONLY | O_CREAT, 0777);

	if (fd_2 == -1)
	{
		close(fd_1);
		perror("open");
		exit(EXIT_FAILURE);
	}

	dup2(fd_2, STDERR_FILENO);

// IF WE HAVE ALL REQUIRED FILES, THEN EXECUTION CAN THEN BEGINS
	int err = execvp(path, argv);
	if (err == -1)
	{
		perror("execvp");
		exit(EXIT_FAILURE);
	}
}

// -------------------------------------------------------------------------------------------------

// WAIT FOR ALL CHILDREN TO FINISH AND CAPTURE THEIR EXIT STATUS
static void wait_for_children(int sd, int ncmds)
{
// FOR EACH COMMANMD IN AN ACTIONSET
	for (int i=0 ; i<ncmds ; ++i)
	{
		int status;
		wait(&status);		

		if (WIFEXITED(status)) // CHILD PROCESS TERMINATED NORMALLY
			return_status(sd, status);	
	}
}

// -------------------------------------------------------------------------------------------------

// EXECUTE EACH COMMAND IN AN ACTIONSET
static void execute_an_actionset(int sd, int actionset, int ncmds)
{
// FOR EACH COMMAND
	for (int i=1 ; i<=ncmds ; ++i)
	{
		switch (fork())
		{
			case -1 : // SOMETHING WENT WRONG
			{
				perror("fork");
				exit(EXIT_FAILURE);
			}

			case 0 : // CHILD PROCESS
			{
				execute_a_cmd(sd, actionset, i);
				break;
			}
		}
	}
}

// THE WORKFLOW OF EXECUTION
void execution(int sd, int actionset, int ncmds)
{
	execute_an_actionset(sd, actionset, ncmds);
	wait_for_children(sd, ncmds);
	store_dirnames(actionset, ncmds);
	return_stdout_stderr(sd);
	return_output_files(sd);
	remove_directories();
}
