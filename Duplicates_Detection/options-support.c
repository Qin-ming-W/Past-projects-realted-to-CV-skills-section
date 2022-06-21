#include "duplicates.h"

// --------------------------------------------------------------------------------

// Supports option -q 
void duplicates_testing(void)
{
	// If no duplciates 
	if(n_path == n_hash)
	{
		exit(EXIT_SUCCESS);
	}

	// Otherwise
	exit(EXIT_FAILURE);
}


// --------------------------------------------------------------------------------

// The helper-function of list_duplicates_l()
void print_pathnames(FILES *file)
{
    if(file != NULL) 
    {
        while(file != NULL) 
        {
	    	printf("%s", file->pathname);

	    	if(file->next != NULL) {
	        	printf("\t");
            }

            // Move to the next file
	   		file = file->next;
        }
		printf("\n");
    }
}


// Supports option -l
void list_duplicates_l(FILETABLE *filetable)
{
	for(int entry = 0; entry < n_hash; ++entry)
	{
		// If two or more files have the same strSHA2 hash
		if( hashes[entry].chain_len > 1 )
		{
			FILES *file = filetable[entry];
			print_pathnames(file);	
		}
	}
}


// --------------------------------------------------------------------------------

// Supports option -h
void list_pathnames_h(FILETABLE *filetable, char *hash)
{
	int entry = filetable_entry(hash);

	// The provided hash is not found
	if(entry == -1) 
	{
		exit(EXIT_FAILURE);
	}

	// The hash is found, do option -h
	FILES *file = filetable[entry];

	if(file != NULL)
	{
		while(file != NULL)
		{
			printf("%s\n", file->pathname);

			// Move to the next one
			file = file->next;
		}
	}
	exit(EXIT_SUCCESS);
}


// --------------------------------------------------------------------------------

// Supports option -f
void list_pathnames_f(FILETABLE *filetable, char *pathname)
{
	char *hash = strSHA2(pathname);

	int entry = filetable_entry(hash);

	// The provided pathname is not found
	if(entry == -1)
	{
		exit(EXIT_FAILURE);
	}

	// The pathname is found, do option -f
	FILES *file = filetable[entry];

	if(file != NULL)
	{
		while(file != NULL)
		{
			if( strcmp(file->pathname, pathname) != 0)
			{
				printf("%s\n", file->pathname);
			}

			// Move to the next one
			file = file->next;
		}
	}
	exit(EXIT_SUCCESS);
}
