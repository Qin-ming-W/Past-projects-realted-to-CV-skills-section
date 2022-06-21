#include "rake-c.h"


// -------------------------------------------------------------------------------------------------	

static void print_hosts()
{
	printf("\nDEBUGGING: WHICH HOST LISTENS ON WHICH PORT?\n");
	for (int i=0 ; i<nhosts ; ++i)
		printf("%s LISTENS ON PORT %s\n", hosts[i].name, hosts[i].port);
}

// -------------------------------------------------------------------------------------------------	

static void print_cmdsAndFiles()
{
	printf("\nDEBUGGING: COMMANDS AND POSSIBLY REQUIRED FILES:\n");
	for (int i=0 ; i<nsets ; ++i)
    {
    	printf("ACTIONSET%d: \n", i+1);
    	if (cmds[i] != NULL)
    	{
    		CMD *cpy = cmds[i];	// TAKE A COPY, DON'T MODIFY cmds[i]
    		int k = 0;

    		while (cpy != NULL)
    		{
    			if (cpy->is_local)
    				printf("cmds[%d][%d] STORES A LOCAL COMMAND: ", i, k);
    			else
    				printf("cmds[%d][%d] STORES A REMOTE COMMAND: ", i, k);
    			++k;

    			for (int j=0 ; j<cpy->nwords ; ++j)
    				printf("%s ", cpy->cmd[j]);

    			if (cpy->nfiles == 0)
    				printf("No additional files are required\n");
    			else
    			{	
    				printf("REQUIRES: ");
    				for (int j=0 ; j<cpy->nfiles ; ++j)
    					printf("%s ", cpy->filenames[j]);
    			}
    			printf("\n");
    			cpy = cpy->next; // GO TO THE NEXT ONE
    		}
    	}
    }
}

// -------------------------------------------------------------------------------------------------	

// DUMP OUT DATA STRUCTURE TO CHECK WHAT HAS BEEN STORED 
void dump()
{
	print_hosts();
	print_cmdsAndFiles();
	printf("**********************************************************\n\n");
}
