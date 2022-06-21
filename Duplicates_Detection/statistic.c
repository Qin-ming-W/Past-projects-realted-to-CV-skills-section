#include "duplicates.h"

// --------------------------------------------------------------------------------

int total_number_of_files  			= 0;
int total_size_of_files				= 0;
int total_number_of_unique_files	= 0;
int minimum_size_of_storage			= 0;

// --------------------------------------------------------------------------------

// Calculates the statistics
void calculate_stats(FILETABLE *filetable)
{
	// The total number of unique files = the total number of unique hashes
	total_number_of_unique_files = n_hash;

	for(int n = 0; n < n_hash; ++n)
	{
		off_t file_size = hashes[n].size;
		int   n_inode   = hashes[n].n_inode;

		// The total number of files = the number of inodes (supports hardlink)
		total_number_of_files += n_inode;

		minimum_size_of_storage += file_size;

		total_size_of_files += file_size * n_inode;
	}
}

// --------------------------------------------------------------------------------

// Reports the statistic
void report_stats(void)
{
	printf("%i\n", total_number_of_files);
	printf("%i\n", total_size_of_files);
	printf("%i\n", total_number_of_unique_files);
	printf("%i\n", minimum_size_of_storage);
}
