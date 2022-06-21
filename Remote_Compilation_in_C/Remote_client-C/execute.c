#include "rake-c.h"


// -------------------------------------------------------------------------------------------------

// COUNT THE NUBER OF REMOTE COMMANDS IN ACTION SET
int count_remote_cmds(CMD *cmd)
{
	if (cmd == NULL)
		exit(EXIT_FAILURE);

	int ncmds = 0;
	CMD *ptr = cmd;			// TAKE A COPY, ENSURE THAT WE DON'T CHANHE WHERE cmd POINTS TO

	while (ptr != NULL)
	{
		if (!ptr->is_local)
			++ncmds;
		ptr = ptr->next;	// GO TO THE NEXT ONE
	}

	return ncmds;
}

// SEND A HEADER OF TYPE CMD_ACTIONSET TO INFORM THE SERVER THE NUMBER OF INCOMING COMMANDS 
void send_an_actionset(int sd, int ncmds, int actionset)
{
	HEADER header;

	header.type 		= CMD_ACTIONSET;	
	header.len 			= ncmds;
	header.len_content 	= actionset;	// THE ORDERING OF ACTIONSETS (1 ==> THE FIRST ACTIONSET ..)

	write_socket(sd, &header, sizeof(HEADER));
}

// -------------------------------------------------------------------------------------------------

// SEND A HEADER OF TYPE CMD_QUOTE_REQUEST TO QUERY A REMOTE SERVER
void send_a_request(int sd)
{
// PREPARE A HEADER OF TYPE CMD_QUOTE_REQUEST
	HEADER header;

	header.type 		= CMD_QUOTE_REQUEST;
	header.len  	   	= 0;
	header.len_content 	= 0;

	write_socket(sd, &header, sizeof(HEADER));	// SEND THE HEADER
}

// -------------------------------------------------------------------------------------------------

// RECV A QUOTATION REPLT FROM A REMOTE SERVER
int recv_a_reply(int sd)
{
	int 	quotation;
	HEADER 	header;

	read_socket(sd, &header, sizeof(HEADER));	
	read_socket(sd, &quotation, header.len);

	return quotation;
}

// -------------------------------------------------------------------------------------------------

// FIND THE SIZE OF A FILE, stat() COULD BE USED AS AN ALTERNATIVE WAY
static long find_file_size(char *filename)
{
	if (filename == NULL)
		exit(EXIT_FAILURE);

	long size;
	FILE *fp = fopen(filename, "r");

	if (fp == NULL)
	{
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	fclose(fp);

	return size;
}

// -------------------------------------------------------------------------------------------------

// SEND A FILE TO A REMOTE SERVER
void send_a_file(int sd, char *filename)
{
	if (filename == NULL)
		exit(EXIT_FAILURE);

// PREPARE A HEADER OF TYPE CMD_SEND_FILE
	HEADER header;

	header.type 		= CMD_SEND_FILE;
	header.len 			= strlen(filename) + 1; 	// RESERVER ONE BYTE FOR '\0'
	header.len_content 	= find_file_size(filename);

	write_socket(sd, &header, sizeof(HEADER));		// SEND THE HEADER
	write_socket(sd, filename, header.len);			// SEND THE FILENAME

// THEN WE SEND THE ACTUAL CONTENT OF A FILE
	int fd = open(filename, O_RDONLY);

	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	char 	buf[BUFSIZ];	// SEND ONE CHUNK (1024) BYTES AT A TIME
	size_t 	got;

	// SEND THE ACTUAL CONTENT OF A FILE
	while ((got = read(fd, buf, sizeof(buf))) > 0)	
	{
		if ((write_socket(sd, buf, got)) != got) // SOMETHING WENT WRONG !
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

// RECV A FILE FROM A REMOTE SERVER
// THE FILE RETURED BACK TO CLIENT COULD BE OF TYPE: 
// CMD_RETURN_STDOUT, CMD_RETURN_STDERR, OR CMD_RETURN_FILE 
void recv_a_file(int sd)
{
	HEADER header;
	read_socket(sd, &header, sizeof(HEADER));

	char filename[header.len];					
	long len_content = header.len_content;	// THE SIZE OF THE INCOMING FILE

	read_socket(sd, filename, header.len);	// GET FILENAME
	filename[header.len] = NULLBYTE;		

	int fd; 

// RETURNED FILE OF TYPE CMD_RETURN_FILE
	if (header.type == CMD_RETURN_FILE)
	{
		fd = open(filename, O_WRONLY | O_CREAT, 0777);

		if (fd == -1)
		{
			perror("open");
			exit(EXIT_FAILURE);
		}	
	}

// RETURNED FILE OF TYPE CMD_RETURN_STDOUT
	else if (header.type == CMD_RETURN_STDOUT) 
		fd = STDOUT_FILENO;

// RETURNED FILE OF TYPE CMD_RETURN_STDERR
	else 
		fd = STDERR_FILENO;

	char buf[len_content];	// BUFFER WHAT IS COMING
	long total 	= 0;		// HOW MANY BYTES WE HAVE READ SO FAR
	long got 	= 0;		// HOW MANY BYTES WE HAVE READ IN ONE read_socket()

	while ((got = read_socket(sd, buf, len_content)) > 0)
	{
		if (write(fd, buf, got) != got) 
		{
			close(fd);	// SOCKET DESCRIPTOR IS NOT IMMEDIATELY CLOSED
			exit(EXIT_FAILURE);
		}

		total += got;

		if (total == len_content)	// WE DONE ?
			break;
	}

// ENSURE THAT WE DO NOT CLOSE stdout, stderr
	if (fd != STDERR_FILENO && fd != STDOUT_FILENO)
		close(fd);
}

// -------------------------------------------------------------------------------------------------

void send_a_cmd(int sd, CMD *cmd)
{
	if (cmd == NULL)
		exit(EXIT_FAILURE);

// PREPARE A HEADER OF TYPE CMD_EXECUTE
	HEADER header;

	header.type 		= CMD_EXECUTE;
	header.len 			= strlen(cmd->line) + 1; // RESERVER ONE BYTE FOR '\0'
	header.len_content 	= cmd->nfiles;			 // THE NUMBER OF FILES REQUIRED FOR THIS COMMAND 

	write_socket(sd, &header, sizeof(HEADER));	 // SEND THE HEADER
	write_socket(sd, cmd->line, header.len);	 // SEND THE COMMAND

// DO WE NEED TO SEND ANY REQUIRED FILES FOR THIS COMMAND ?

	for (int i=0 ; i<cmd->nfiles ; ++i)
		send_a_file(sd, cmd->filenames[i]);		 
}

// -------------------------------------------------------------------------------------------------

// RECV A STATUS FROM A REMOTE SERVER
// WAS EXECUTION THERE SUCCESSFUL ?
static int recv_a_status(int sd)
{
	int status;
	HEADER header;

	read_socket(sd, &header, sizeof(HEADER));
	read_socket(sd, &status, header.len);

	return status;
}

// -------------------------------------------------------------------------------------------------

// CAN WE GO TO THE NEXT ACTIONSET
bool next_actionset(int sd, int ncmds)
{
	int nsuccesses = 0;
	for (int i=0; i<ncmds ; ++i)
	{
		if (recv_a_status(sd) == 0)		// WAS SUCCESSFUL !
			++nsuccesses;
	}

	return nsuccesses == ncmds ? true : false;
}
