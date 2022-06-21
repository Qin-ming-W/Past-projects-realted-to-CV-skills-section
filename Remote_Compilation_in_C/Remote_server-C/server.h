#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <netdb.h>
#include <time.h>
#include <dirent.h>

#define PORT		"8000"
#define BACKLOG 	20

#define NULLBYTE	'\0'

#define CHECK_ALLOC(p) if(p == NULL) { perror(__func__); exit(EXIT_FAILURE); }

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

// STORE THE NAME OF TEMPORARY DIRECTORIES CREATED FOR EACH COMMAND IN AN ACTIONSET
extern char 	**dirnames;	
extern int 		ndirs;		// NUMBER OF DIRECTORIES CREATED IN AN ACTIONSET

// -------------------------------------------------------------------------------------------------

// THESE FUNCTIONS ARE DEFINED IN strsplit.c
extern char  	**strsplit(const char *line, int *nwords);
extern void     free_words(char **words);

// THESE FUNCTIONS ARE DEFINED IN support.c
extern ssize_t 	read_socket(int sd, void *buf, size_t nbytes);
extern ssize_t 	write_socket(int sd, void *buf, size_t nbytes);

// THIS FUNCTION IS DEFINED IN Networking.c
extern void 	set_up_server();

// THESE FUNCTION ARE DEFINED IN execute.c
extern void 	return_a_price(int sd);
extern void 	recv_a_file(int sd);
extern void 	return_a_file(int sd, char *filename, CMDTYPE type);
extern void 	return_status(int sd, int status);
extern void 	execute_a_cmd(int sd, int actionset, int order);
extern void 	execution(int sd, int actionset, int ncmds);

// THESE FUNCTIONS ARE DEFINED IN directories.c
extern void 	remove_directories();
extern void 	store_dirnames(int actionset, int ncmds);
extern void 	return_stdout_stderr();
extern void 	return_output_files(int sd);
extern int 		number_of_outputs();
