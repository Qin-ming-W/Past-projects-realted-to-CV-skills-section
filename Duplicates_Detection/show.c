#include "duplicates.h"


// --------------------------------------------------------------------------------

void show_stats(void)
{
	printf("The total number of files is \t\t%i\n", 	 total_number_of_files);
	printf("The total size of files is \t\t%i\n", 		 total_size_of_files);
	printf("The total number of unique files is \t%i\n", total_number_of_unique_files);
	printf("The minimum size of storage is \t\t%i\n", 	 minimum_size_of_storage);
}


// --------------------------------------------------------------------------------

void show(FILETABLE *filetable)
{
	printf("\n%s\n", "All pathnames under consideration");
	for(int n = 0; n < n_path; ++n)
	{
		printf("%s\n", pathnames[n]);
	}

	printf("\n%s\n", "ALL hashes");	
	for(int n = 0; n < n_path; ++n)
	{
		printf("%s\n", strSHA2(pathnames[n]));
	}

	printf("\n%s\n", "ALL unique hashes");
	for(int n = 0; n < n_hash; ++n)
	{
		printf("%s\n", hashes[n].hash);
	}

	printf("\n%s\n", "n copy");
	for(int n = 0; n < n_hash; ++n)
	{
		printf("%i\n", hashes[n].chain_len);
	}


	printf("\n%s\n", "n inode");
	for(int n = 0; n < n_hash; ++n)
	{
		printf("%i\n", hashes[n].n_inode);
	}

	printf("\n%s\n", "Linked list structure");
	for(int n = 0; n < n_hash; ++n)
	{
		FILES *file = filetable[n];

		if(file != NULL)
		{
			while(file != NULL) 
        	{
	    		printf("%s", file->pathname);   		
	    		if(file->next != NULL) 
	    		{
	        		printf("-->");
            	}
	   			file = file->next;
        	}
			printf("\n");
		}
	}

	printf("\n%s\n", "Report Stats");
	calculate_stats(filetable);
	show_stats();	
}
