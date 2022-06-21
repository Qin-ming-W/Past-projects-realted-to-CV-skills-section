#include "rake-c.h"


// -------------------------------------------------------------------------------------------------	

char	 	*port 	= NULL;			

HOST 		*hosts 	= NULL;			
int  		nhosts 	= 0;			

CMD 		**cmds 	= NULL;
int 		nsets 	= 0;				

// -------------------------------------------------------------------------------------------------	

// ZERO OUT EXTRA MEMORY ACQUIRED BY realloc()
static void *realloc_zero(void *pBuffer, size_t old_size, size_t new_size)
{
	void *pNew = realloc(pBuffer, new_size);

	if (new_size > old_size && pNew)
	{
		size_t diff = new_size - old_size;
		void *pStart = ((char *)pNew) + old_size;
		memset(pStart, 0, diff);
	}
	return pNew;
}

// -------------------------------------------------------------------------------------------------	

// TRIM THE PESKY '\n'
static void trim_line(char *line)
{
	if (line == NULL)
		return;

	int i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\r' || line[i] == '\n')
		{
			line[i] = '\0';
			break;
		}

		++i;
	}
}

// -------------------------------------------------------------------------------------------------	

void parse_Rakefile()
{
	FILE *fp = fopen("Rakefile", "r");

	if (fp == NULL)
	{
		fprintf(stderr, "Unable to open Rakefile\n");
		exit(EXIT_FAILURE);
	}

	char line[BUFSIZ];
	CMD  *last_cmd = NULL;	// REMEMBER WHAT THE LAST COMMAND

	while (fgets(line, sizeof(line), fp) != NULL)
	{
		// BEFORE WE EVEN PROCESS IT, WE TRIM THE PESKY NEW LINE CHARACTER
		trim_line(line);	//  OR line[strlen(line)-1] = '\0';

		int nwords;
		char **words = strsplit(line, &nwords);
	
		// 1. LINES STARTING WITH # CAN BE SAFELY IGNORED
		// 2. BLANK LINES CAN BE SAFELY IGNORED
		if (nwords == 0 || strcmp(words[0], "#") == 0)
			continue;

		// 3. DEFAULT PORT
		else if (strcmp(words[0], "PORT") == 0)
			port = strdup(words[2]);

		// 4. HOSTS
		else if (strcmp(words[0], "HOSTS") == 0)
		{
			nhosts = nwords - 2;	
			hosts = malloc(sizeof(HOST) * nhosts);
			CHECK_ALLOC(hosts);

			for (int i=0; i<nhosts ; ++i)
			{
				// THIS HOST DOES NOT SPECIFY A PORT TO LISTEN ON
				if (strchr(words[i+2], ':') == NULL)
				{
					hosts[i].name = strdup(words[i+2]);
					hosts[i].port = port;	// DEFAULT PORT IS USED
				}

				// OTHERWISE
				else
				{
					char *hostname = strsep(&words[i+2], ":");
					hosts[i].name = strdup(hostname); 

					// words[i+2] NOW CONTAINS THE SPECIFIC PORT NUMBER
					hosts[i].port = strdup(words[i+2]); 
				}
			}
		}

		// 5. A NEW ACTIONSET IS FOUND
		else if (strstr(words[0], "actionset") != NULL)
		{
			cmds = realloc_zero(cmds, nsets * sizeof(cmds[0]), 
				(nsets+1) * sizeof(cmds[0]));
			CHECK_ALLOC(cmds);

			++nsets;
		}

		// 7. REQUIRED FILES FOR THE LAST COMMAND REMEMBERED
		else if (strcmp(words[0], "requires") == 0)
		{
			// ATTEMPT TO ALLOCATE MEMORY
			last_cmd->filenames = malloc(sizeof(char *) * (nwords-1));
			CHECK_ALLOC(last_cmd->filenames);	

			last_cmd->nfiles = nwords - 1;	

			for (int i=0 ; i<last_cmd->nfiles ; ++i)
				last_cmd->filenames[i] = words[i+1];
		}

		// 8. COMMANDS, EITHER LCOAL OR REMOTE
		else
		{
			// PREPARE AN OBJECT OF TYPE CMD
			CMD *cmd = malloc(sizeof(CMD));
			CHECK_ALLOC(cmd);

			cmd->cmd = malloc(sizeof(char *) * nwords);
			CHECK_ALLOC(cmd->cmd);

			// IS IT A LOCAL COMMAND OR A REMOTE COMMAND ?
			if (strstr(words[0], "remote-") != NULL) // REMOTE COMMAND
				cmd->is_local = false;
			else 
				cmd->is_local = true;				 // LOCAL COMMAND

			int i = 0;
			while (isspace(line[i]))
				++i;

			if (cmd->is_local)
				cmd->line = strdup(&line[i]);
			else 
				cmd->line = strdup(&line[i+7]);

			cmd->nwords = nwords;

			// WE DON'T KNOW IF THIS COMMAND REQUIRES ADDITIONAL FILES YET 
			cmd->filenames = NULL;	
			cmd->nfiles = 0;	
			cmd->next = NULL;	// EXPLICITLY SET TO NULL FOR COMPLETENESS	

			for (int i=0 ; i<nwords ; ++i)
				cmd->cmd[i] = strdup(words[i]);

			last_cmd = cmd; 	// THIS cmd IS REGISTERED AS THE LAST COMMAND

			// PREPARED! ADD IT TO THE DATABASE
			if (cmds[nsets-1] == 0)		// FIRST ONE IN THIS ACTIONSET
				cmds[nsets-1] = cmd;
			else						// OTHERWISE, ADD TO THE TAIL
			{
				CMD *cpy = cmds[nsets-1];
				while (cpy->next != NULL)
					cpy = cpy->next;
				cpy->next = cmd;
			}
			free_words(words);			// DEALLOCATE MEMORY
		}
	}
	fclose(fp);		// WE OPENED IT, WE CLOSE IT
}
