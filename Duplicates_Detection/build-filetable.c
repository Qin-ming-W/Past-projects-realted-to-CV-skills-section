#include "duplicates.h"

// --------------------------------------------------------------------------------

/*
	 FILETABLE        
					 	                hashes               filetable
					 	 				------				 ------
		typedef strcut hash  --------> |      | 0 --------> |      | 0 --------> typedef struct _file
		{								------               ------				 {
			char *hash;				   |      | 1           |      | 1                char         *pathname;
			int  chain_len;				------               ------                   inot_t       inode;
			int  n_inode			   |      | 2           |      | 2                
		}HASH;							------               ------                   struct _file *next;      --------> typedef struct _file
	 								   |  •   | •           |  •   | •			 }										 {
									   |  •   | •           |  •   | •														char  	     *pathname;
									   |  •   | •           |  •   | •       												inot_t	     inode;
									   |      |             |      |														
										------               ------															struct _file *next;      --------> NULL
									   |      | n_hash      |      | n_hash												  }
									    ------	             ------
*/									

// --------------------------------------------------------------------------------

FILETABLE *filetable_new(void)
{
	// Allocats memory space for a new filetable and checks if the allocation was successful
	FILETABLE *new = malloc(n_hash * sizeof(FILES));
	CHECK_ALLOC(new);

	// Initialises all its entries as NULL, since the filetable is empty initially
	for(int i = 0; i < n_hash; ++i)
	{
		new[i] = NULL;
	}

	return new;
}


// --------------------------------------------------------------------------------

// Generates the filetable entry
int filetable_entry(char *hash)
{ 
	// The entry is initialsed to be -1, if the provided hash is not found, -1 is returned
	int entry = -1;

	// hash is the key into the filetable
	for(int n = 0; n < n_hash; ++n)
	{
		if( strcmp(hash, hashes[n].hash) == 0 )
		{
			entry = n;
		}
	}

	return entry;
}


// --------------------------------------------------------------------------------

// Adds _file itenm to filetable
void filetable_add(FILETABLE *filetable, char *pathname)
{ 
	char *hash 	= strSHA2(pathname);

	int entry   = filetable_entry(hash);

	filetable[entry] = file_add(filetable[entry], pathname);
}


// --------------------------------------------------------------------------------

FILETABLE *build_filetable(void)
{
	FILETABLE *filetable = filetable_new();

	for(int n = 0; n < n_path; ++n)
	{
		char *pathname = pathnames[n];
		filetable_add(filetable, pathname);
	}

	return filetable;
}
x
