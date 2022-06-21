#include "duplicates.h"

// --------------------------------------------------------------------------------

// The helper function of file_add() to make sure each itme only added once
bool file_find(FILES *file, char *pathname)
{
	while(file != NULL)
	{
		if( strcmp(file->pathname, pathname) == 0 )
		{	
			return true;
		}

		file = file->next;
	}
	return false;
}


// --------------------------------------------------------------------------------

// Creates a new _file item
FILES *file_new_item(char *new_pathname)
{
	// Allcoates memnory for this _file iten
	FILES *new = malloc( sizeof(FILES) );
	CHECK_ALLOC(new);

	new->pathname = strdup(new_pathname);
	CHECK_ALLOC(new->pathname);

	// Extracts attributes of this file and fill its fields
	struct stat stat_info;

	if( stat(new_pathname, &stat_info) != 0 )
	{
		perror(new_pathname);
		exit(EXIT_FAILURE);
	}

	new->inode 		= stat_info.st_ino;
	new ->size 		= stat_info.st_size;

	new->next = NULL;
	return new;
}


// --------------------------------------------------------------------------------

FILES *file_add(FILES *file, char *pathname)
{
	// Only adds each item once
    if( file_find(file, pathname) )
    {            
        return file;
    }

    else 
    {                                     
        FILES *new   = file_new_item(pathname);

        // Adds item to the head of list
        new->next    = file;
        return new;
    }
}
