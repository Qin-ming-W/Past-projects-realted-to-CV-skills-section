#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define CHECK_ALLOC(p) 	if(p == NULL) { perror(__func__); exit(EXIT_FAILURE); }
#define NULLBYTE		'\0'

// -------------------------------------------------------------------------------------------------

// INFORMATION SHARED BY CLIENT AND SERCER
// 1. CLINET ==> SERVER
// 2. SERVER ==> CLIENT

typedef enum {
	CMD_QUOTE_REQUEST = 0,	// 1.
	CMD_QUOTE_REPLY,		// 2.

	CMD_SEND_FILE,			// 1.
	CMD_RETURN_FILE, 		// 2.

	CMD_EXECUTE,			// 1.

	CMD_RETURN_STATUS,		// 2.
	CMD_RETURN_STDOUT,		// 2.
	CMD_RETURN_STDERR,		// 2.

	CMD_ACTIONSET,			// 2. INDICATES THE NUMBER OF COMMANDS IN AN ACTIONSET
							

	CMD_NOUTPUTS,  	 		// 2. INDICATES THE NUMBER OF RETURNED FILES IN AN ACTIONSET
							
} CMDTYPE;

typedef struct {
	CMDTYPE	type;			// TYPE OF INFORMATION COMING NEXT
	int 	len; 			// len MIGHT HAVE DIFFERENT MEANING FOR DIFFERENT CMDTYPE
	int 	len_content;	// len MIGHT HAVE DIFFERENT MEANING FOR DIFFERENT CMDTYPE
} HEADER;

// -------------------------------------------------------------------------------------------------

// DATA STRCUTURE USED TO STORE INFORMATION IN THE Rakefile

typedef struct {
	char *name;				// HOSTNAME
	char *port;				// SERVERNAME (PORT NUMBER)
	int   sd;				// SOCKET DESCRIPTOR PREVIOUSLY ALLOCATED FOR IT
} HOST;

typedef struct _cmd {
	bool is_local;	

	char *line;				// A LINE OF COMMAND

	char **cmd;				// COMMAND BROKEN INTO MULTIPLE WORDS
	int nwords;	

	char **filenames;		// NAME OF ANY ADDITIONAL FILES NEEDED 
	int  nfiles;

	struct _cmd *next;
} CMD;

extern char		*port;		

extern HOST 	*hosts;		
extern int 		nhosts;

extern CMD 		**cmds;
extern int 		nsets;

// -------------------------------------------------------------------------------------------------

// THESE FUNCTIONS ARE DEFINED IN strsplit.c
extern char  	**strsplit(const char *line, int *nwords);
extern void     free_words(char **words);


// THIS FUNCTION IS DEFINED IN Rakefile_parser.c
extern void 	parse_Rakefile();


// THIS FUNCTION IS DEFINED parser_debugger.c
extern void		dump();		


// THESE FUNCTIONS ARE DEFINED IN support.c
extern ssize_t 	read_socket(int sd, void *buf, size_t nbytes);
extern ssize_t 	write_socket(int sd, void *buf, size_t nbytes);

// THESE FUNCTIONS ARE DEFINED IN choose_server.c
extern int   	choose_server();
extern int 		reconnect(int sd);


// THIS FUNCTION IS DEFINED IN Networking.c
extern int 		create_socket_connect(HOST host);


// THE FUNCTIONS ARE DEFINED IN execute.c
extern int 		count_remote_cmds(CMD *cmd);

extern void 	send_a_request(int sd);
extern int 		recv_a_reply(int sd);

extern void 	send_a_file(int sd, char *filename);
extern void 	recv_a_file(int sd);

extern void 	send_a_cmd(int sd, CMD *cmd);

extern void 	send_an_actionset(int sd, int ncmds, int actionset);
extern bool 	next_actionset(int sd, int ncmds);
