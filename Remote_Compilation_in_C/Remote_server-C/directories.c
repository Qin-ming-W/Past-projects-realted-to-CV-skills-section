#include "server.h"


char	**dirnames = NULL;
int 	ndirs	   = 0;

// -------------------------------------------------------------------------------------------------

// REMOVE A POSSIBLY NON-EMPTY DIRECTORY
static void remove_a_directory(char *dirname)
{
	if (dirname == NULL)
		exit(EXIT_FAILURE);

// ATTEMPTS TO OPEN THE DIRECTORY
	DIR *dirp = opendir(dirname);

	if (dirp != NULL)
	{
		struct dirent *dp;

// READ EACH ENTRY OF THE DIRECTORY
		while ((dp = readdir(dirp)) != NULL)
		{
//  ALWAYS IGNORE THIS DIRECTORY (dot) AND ITS PARENT (dotdot)
			if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) 
				continue;

//  RECONSTRUCT THE PATHNAME <- dir+entry 
			char fullpath[MAXPATHLEN];
	    	sprintf(fullpath, "%s/%s", dirname, dp->d_name);

	    	struct stat statinfo;

//  ENSURE THAT WE CAN GET THE ATTRIBUTES OF THIS PATHNAME
	    	if (stat(fullpath, &statinfo) != 0) 
	    	{        
				perror("stat");
				exit(EXIT_FAILURE);
            }

// IS IT A REGULAR FILE ?
            if (S_ISREG(statinfo.st_mode))
            {
            	if (unlink(fullpath) == -1) // UNLINK IT (DELETE IT FROM THE DIRECTORY)
            	{
            		perror("unlink");
            		exit(EXIT_FAILURE);
            	}
            }

// IS IT A DIRECTORY ?
            if (S_ISDIR(statinfo.st_mode))
            	remove_a_directory(fullpath);		// RECURSION
		}

// WE OPENED IT, WE CLOSE IT
		closedir(dirp);

// NOW, WE ENSURE THAT THE GIVEN DIRETORY IS EMPTY
		if (rmdir(dirname) == -1)
		{
			perror("rmdir");
			exit(EXIT_FAILURE);
		}
	}	
}


void remove_directories()
{
// FOR EACH TEMPORARY DIRECTORY CREATED FOR AN ACTIONSET
	for (int i=0 ; i<ndirs ; ++i)
	{
		remove_a_directory(dirnames[i]);	// REMOVE T
		free(dirnames[i]);					// DEALLOCATE MEMEMORY
	}

	free(dirnames);							// DEALLOCATE MEMEMORY
	ndirs = 0;
}

// -------------------------------------------------------------------------------------------------

// STORE THE NAME OF A TEMPORARY DIRECTORIES IN DATA STRUCTURE
void store_dirnames(int actionset, int ncmds)
{
// FOR EACH COMMANDS IN AN ACTIONSET
	for (int i=1 ; i<=ncmds ; ++i)	
	{
		char dirname[MAXPATHLEN];

// CONSTRUCT THE DIRECTORY NAME
		sprintf(dirname, "/tmp/%d-cmd-%d", actionset, i);

// STORE IT 
		dirnames = realloc(dirnames, sizeof(char *) * (ndirs + 1));
		CHECK_ALLOC(dirnames);
		dirnames[ndirs] = strdup(dirname);	
		CHECK_ALLOC(dirnames[ndirs]);
		++ndirs;
	}
}

// -------------------------------------------------------------------------------------------------

// RETURN STDOUT.txt, STDERR.txt FILE
void return_stdout_stderr(int sd)
{
	char filename_stdout[MAXPATHLEN];
	char filename_stderr[MAXPATHLEN];

// FOR EACH TEMPORARY DIRECTORY CREATED FOR AN ACTIONSET
	for (int i=0 ; i<ndirs ; ++i)
	{
// CONSTRUCT THE FULLPATH	
		sprintf(filename_stdout, "%s/%s", dirnames[i], "STDOUT.txt");
		return_a_file(sd, filename_stdout, CMD_RETURN_STDOUT);	// SEND IT BACK TO CLIENT

// CONSTRUCT THE FULLPATH	
		sprintf(filename_stderr, "%s/%s", dirnames[i], "STDERR.txt");		
		return_a_file(sd, filename_stderr, CMD_RETURN_STDOUT);	// SEND IT BACK TO CLIENT
	}
}

// -------------------------------------------------------------------------------------------------

// FIND THE OUTPUT FILE IN A TEMPORARY DIRECTORY (*.O OR EXCTUTABLE FILE)
static char *find_output(char *dirname)
{
	if (dirname == NULL)
		exit(EXIT_FAILURE);
// ATTEMPTS TO OPEN THE DIRECTORY

	DIR *dirp = opendir(dirname);

// MODIFICATION TIME IS CHOSEN AS A MEASURE TO DETECT (*.O OR EXCTUTABLE FILE)
	long		mtime = 0;

	char *path = malloc(MAXPATHLEN);
    CHECK_ALLOC(path);
    bool found_one = false;

	if (dirp != NULL)
	{
		struct dirent *dp;

// READ EACH ENTRY OF THE DIRECTORY
		while ((dp = readdir(dirp)) != NULL)
		{
			//  ALWAYS IGNORE THIS DIRECTORY (dot), ITS PARENT (dotdot), "STDOUT.txt", "STDERR.txt"
			if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0
				|| strcmp(dp->d_name, "STDOUT.txt") == 0 || strcmp(dp->d_name, "STDERR.txt") == 0) 
				continue;

// WE GET HERE, THEN WE FOUND ONE
			found_one = true;

//  RECONSTRUCT THE PATHNAME <- dir+entry 
			char fullpath[MAXPATHLEN];
	    	sprintf(fullpath, "%s/%s", dirname, dp->d_name);

	    	struct stat statinfo;

//  ENSURE THAT WE CAN GET THE ATTRIBUTES OF THIS PATHNAME
	    	if (stat(fullpath, &statinfo) != 0) 
	    	{        
				perror("stat");
				exit(EXIT_FAILURE);
            }

// IS IT A REGULAR FILE ?
            if (S_ISREG(statinfo.st_mode))
            {
            	if (statinfo.st_mtime > mtime)
            	{
            		mtime = statinfo.st_mtime;
            		path = strdup(fullpath);
            		CHECK_ALLOC(path);
            	}
            }

// IS IT A DIRECTORY ?
            if (S_ISDIR(statinfo.st_mode))
            	find_output(fullpath);	// RECURSION
		}

// WE OPENED IT, WE CLOSE IT
		closedir(dirp);
	}

	if (found_one)
		return path;

	return NULL;
}

// -------------------------------------------------------------------------------------------------

// COUNT THE NUMBER OF OUTPUT FILES (*.O OR EXCTUTABLE FILE) FOR ONE ACTIONSET
int number_of_outputs()
{
	int num = 0;

// FOR EACH TEMPORARY DIRECTORY CREATED FOR AN ACTIONSET
	for (int i=0 ; i<ndirs ; ++i)
	{
// SEE IF WE CAN FIND ONE
		if (find_output(dirnames[i]) != NULL)
			++num;
	}

	return num;
}

// -------------------------------------------------------------------------------------------------

// SEND BACK A HEADER PF TYPE CMD_NOUTPUTS TO INFORM CLIENT THE NUMBER OF INCOMING OUTPUT FILES
static void send_noutpus(int sd)
{
// PREPARE A HEADER OF TYPE CMD_NOUTPUTS
	HEADER header;

	header.type 		= CMD_NOUTPUTS;
	header.len 			= number_of_outputs();
	header.len_content 	= 0;

	write_socket(sd, &header, sizeof(HEADER)); 
}

// -------------------------------------------------------------------------------------------------

// SEND BACK ALL OUTPUTS FILES BACK TO CLIENT
void return_output_files(int sd)
{
// FIRST INFORM CLIENT THE NUMBER OF INCOMING OUTPUT FILES
	send_noutpus(sd);

	char *path;

// FOR EACH TEMPORARY DIRECTORY CREATED FOR AN ACTIONSET
	for (int i=0 ; i<ndirs ; ++i)
	{
// FIND IT
		path = find_output(dirnames[i]);
		if (path != NULL)
			return_a_file(sd, path, CMD_RETURN_FILE);	// SEND IT
	}
}
