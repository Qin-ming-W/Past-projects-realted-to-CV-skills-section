#include "duplicates.h"

// --------------------------------------------------------------------------------

char **pathnames    = NULL;
int n_path	  	 	= 0;

HASH *hashes 	 	= NULL;
int n_hash		 	= 0;

// --------------------------------------------------------------------------------

// The function scan_directory() supports option -a 
void scan_directory(char *dirname)
{
	DIR *dirp;
	struct dirent *dp;

	dirp = opendir(dirname);

	if(dirp == NULL)
	{
		exit(EXIT_FAILURE);
	}

	while( (dp = readdir(dirp)) != NULL )
	{
		// If the pathname starts with a '.' character, it is consideredas a hidden file by convention
		// Skips the scanning, if -a is not provided
		if(!run_option_a && dp->d_name[0] == '.')
		{
			continue;
		}
		
		// Constructs the path
		char fullpath[MAXPATHLEN] = { 0 };
		sprintf(fullpath, "%s/%s", dirname, dp->d_name);

		// Extracts attributes of the file
		struct stat 	stat_info;

		if( stat(fullpath, &stat_info) != 0 )
		{
			perror(fullpath);
			exit(EXIT_FAILURE);
		}

		// Adds to 'char **pathnames', if it is a regular file(including the hardlink)
		if(S_ISREG(stat_info.st_mode))
		{
			pathnames = realloc(pathnames, (n_path + 1) * sizeof(pathnames[0]));
			CHECK_ALLOC(pathnames);

			pathnames[n_path] = strdup(fullpath);
			CHECK_ALLOC(pathnames[n_path]);

			++n_path;	
		}
		
		// If 1. the file is of type directory
		//    2. the file is not the current directory
		//    3. the file is not the parent directory
		// Recursively scans the directory
		if( S_ISDIR(stat_info.st_mode) && strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 )
		{
			scan_directory(fullpath);	
		}
	}
	closedir(dirp);
}


// --------------------------------------------------------------------------------

// The helper-function of scan_pathnames()
void add_hash_and_size(char *hash, char *pathname)
{
	// Each unique hash is only added once
	for(int n = 0; n < n_hash; ++n)
	{
		if( strcmp(hash, hashes[n].hash) == 0 )
		{
			return;
		}					
	}

	hashes = realloc(hashes, (n_hash + 1) * sizeof(hashes[0]));
	CHECK_ALLOC(hashes);

	hashes[n_hash].hash = strdup(hash);
	CHECK_ALLOC(hashes[n_hash].hash);

	// Extracts attributes of the file
	struct stat stat_info;

	if( stat(pathname, &stat_info) != 0 )
	{
		perror(pathname);
		exit(EXIT_FAILURE);
	}

	hashes[n_hash].size = stat_info.st_size;

	++n_hash;
}


// --------------------------------------------------------------------------------

// Scans all the pathnames that stored in 'char **pathnames'
// Fills the one of the fields(i.e. char *hash) in struct 'HASH'
// Puts all HASHes into 'HASH *hashes'
void scan_pathnames(void)
{
	for(int n = 0; n < n_path; ++n)
	{
		char *pathname = pathnames[n];
		char *hash = strSHA2(pathname);

		add_hash_and_size(hash, pathname);
	}
}


// --------------------------------------------------------------------------------

// The helper-function of scan_filetable()
int add_chain_length(FILETABLE *filetable, int entry)
{
	int length = 0;

	FILES *file = filetable[entry];

	if(file != NULL)
	{
		while(file != NULL)
		{
			++length;
			file = file->next;
		}
	}

	return length;
}


// --------------------------------------------------------------------------------

// The heler-function of add_n_inode()
int count_distinct(ino_t inodes[], int n)
{
	int i; 
	int j;
	int count = 1;
   
   	for (i = 1; i < n; ++i)      
   	{
      	for (j = 0; j < i; ++j)   
      	{
         	if (inodes[i] == inodes[j])    
         	{
            	break;            
         	}
      	}

      	if (i == j)
      	{
        	++count;    
      	}
   }
   return count;      
}


// --------------------------------------------------------------------------------

// The helper-function of scan_filetable()
int add_n_inode(FILETABLE *filetable, int entry)
{
	FILES *file = filetable[entry];

	int chain_len = hashes[entry].chain_len;

	ino_t inodes[chain_len];

	int n = 0;

	while(file != NULL)
	{
		inodes[n] = file->inode;
		++n;
		file = file->next;
	}

	return count_distinct(inodes, chain_len);
}


// --------------------------------------------------------------------------------

// Scans the filetable that is already built up
// Fills the one of the fields(i.e. int chain_len) in struct 'HASH'
// Fills the one of the fields(i.e. int n_inode) in struct 'HASH'
void scan_filetable(FILETABLE *filetable)
{
	for(int n = 0; n < n_hash; ++n)
	{
		int chain_len = add_chain_length(filetable, n);

		hashes[n].chain_len = chain_len;

		int n_inode = add_n_inode(filetable, n);

		hashes[n].n_inode = n_inode;
	}
}
