/* Implementation of doubly linked list */

#include <stdio.h>
#include <stdlib.h>

#define CHECK_ALLOC(ptr) if(ptr == NULL) { perror(__func__); exit(EXIT_FAILURE); }

typedef struct _node
{
	struct _node *prev;
	struct _node *next;
	int val;	
} NODE;

NODE *start = NULL;	

NODE *get_new(void);

void display(void);

void insert_head(void);

void insert_tail(void);

void insert_before(void);

void insert_after(void);

void delete_head(void);

void delete_tail(void);

void delete_node(void);

void delete_after(void);

void delete_list(void);

void sort_list(void);

int main(void)
{
	int opt;

	do
	{
		// DISPLAY MAIN MENU PAGE
		printf("\n     ***** MAIN MENU *****\n");
		printf("1  : Display the list\n");
		printf("2  : Add a node at the beginning\n");
		printf("3  : Add a node at the end\n");
		printf("4  : Add a node before a given node\n");
		printf("5  : Add a node after a given node\n");
		printf("6  : Delete a node from the beginning\n");
		printf("7  : Delete a node from the end\n");
		printf("8  : Delete a given node\n");
		printf("9  : Delete a node after a given node\n");
		printf("10 : Delete the entire list\n");
		printf("11 : Sort the list\n");
		printf("12 : Exit\n");

		// REQUEST USER INPUT
		printf("Enter your option: ");

		// ACCOUNT FOR scanf() FAILURE
		if( scanf("%d", &opt) != 1 )
		{
			fprintf(stderr, "%s\n", "scanf() failed");
			exit(EXIT_FAILURE);
		}

		switch(opt)
		{
			case 1 :
				display();
				break;

			case 2 :
				insert_head();
				break;

			case 3 :
				insert_tail();
				break;

			case 4 :
				insert_before();
				break;

			case 5 :
				insert_after();
				break;

			case 6 :
				delete_head();
				break;

			case 7 :
				delete_tail();
				break;

			case 8 :
				delete_node();
				break;

			case 9 :
				delete_after();
				break;

			case 10 :
				delete_list();

			case 11 :
				sort_list();
				break;

			case 12 :
				break;

			default :
				printf("Option is not recognized, try again\n");
				break;
		}

	} while(opt != 12);


	return 0;
}


NODE *get_new(void)
{
	int i;

	printf("Enter an integer value: ");
	if(scanf("%d", &i) != 1)
	{
		fprintf(stderr, "%s\n", "scanf failed");
		exit(EXIT_FAILURE);
	}

	NODE *new = calloc(1, sizeof(NODE));
	CHECK_ALLOC(new);

	new->val = i;


	return new;
}


void display(void)
{
	if(start == NULL)
	{
		printf("Nothing to display\n");
	}

	else
	{
		NODE *curr = start;

		while(curr != NULL)
		{
			if(curr->prev == NULL) printf("NULL<––");

			printf("%d", curr->val);

			if(curr->next != NULL) printf("<––>");
			else printf("––>NULL");

			curr = curr->next;
		}

		printf("\n");
	}
}


void insert_head(void)
{
	NODE *new = get_new();

	new->next = start;
	if(start != NULL) start->prev = new;
	start = new;
}


void insert_tail(void)
{
	NODE *new = get_new();

	if(start == NULL)
	{
		start = new;
	}

	else
	{
		NODE *ptr = start;

		while(ptr->next != NULL)
		{
			ptr = ptr->next;
		}

		ptr->next = new;

		new->prev = ptr;
	}
}


void insert_before(void)
{
	if(start == NULL)
	{
		printf("Empty list\n");
	}

	else
	{
		int cond;

		printf("Enter the value before which the new integer is added: ");
		if(scanf("%d", &cond) != 1)
		{
			printf("scanf failed\n");
			exit(EXIT_FAILURE);
		}

		NODE *new = get_new();
		NODE *preptr, *ptr;

		for(preptr = NULL, ptr = start ; ptr->next != NULL && ptr->val != cond ; 
			preptr = ptr, ptr = ptr->next)
		{
			;
		}

		if(ptr->val != cond) printf("The conditional value is not found\n");

		else if(preptr == NULL)
		{
			new->next = start;
			start->prev = new;
			start = new;
		}

		else
		{
			preptr->next = new;
			new->prev = preptr;

			new->next = ptr;
			ptr->prev = new;
		}
	}
}


void insert_after(void)
{
	if(start == NULL)
	{
		printf("The list is empty\n");
	}

	else
	{
		int cond;

		printf("Enter the value after which the new integer is added: ");
		if(scanf("%d", &cond) != 1)
		{
			printf("scanf failed\n");
			exit(EXIT_FAILURE);
		}

		NODE *new = get_new();
		NODE *ptr;

		for(ptr = start ; ptr->next != NULL && ptr->val != cond ; 
			ptr = ptr->next)
		{
			;
		}

		if(ptr->val != cond)
		{
			printf("The conditional value is not found\n");
		}

		else if(ptr->next == NULL)
		{
			ptr->next = new;
			new->prev = ptr;
		}

		else
		{
			new->next = ptr->next;
			ptr->next->prev = new;

			ptr->next = new;
			new->prev = ptr;
		}
	}
}


void delete_head(void)
{
	if(start == NULL)
	{
		printf("The list is empty\n");
	}

	else
	{
		NODE *cpy = start;

		start = start->next;
		if(start != NULL) start->prev = NULL;

		free(cpy);
	}
}


void delete_tail(void)
{
	if(start == NULL)
	{
		printf("The list is empty\n");
	}

	else
	{
		NODE *preptr;
		NODE *ptr;

		for(preptr = NULL, ptr = start ; ptr->next != NULL ;
			preptr = ptr, ptr = ptr->next)
		{
			;
		}

		if(preptr == NULL)
		{
			free(start);
			start = NULL;
		}

		else
		{
			free(ptr);
			preptr->next = NULL;
		}
	}
}


void delete_node(void)
{
	if(start == NULL)
	{
		printf("The list is empty\n");
	}

	else
	{
		int cond;

		printf("Enter the value of node which has to be deleted: ");
		if(scanf("%d", &cond) != 1)
		{
			printf("scanf failed\n");
			exit(EXIT_FAILURE);
		}

		NODE *preptr, *ptr;

		for(preptr = NULL, ptr = start ; ptr->next != NULL && ptr->val != cond ;
			preptr = ptr, ptr = ptr->next)
		{
			;
		}

		if(ptr->val != cond)
		{
			printf("The conditional value is not found\n");
		}

		else if(preptr == NULL)
		{	
			delete_head();
			ptr = NULL;
		}

		else
		{
			preptr->next = ptr->next;
			if(ptr->next != NULL) ptr->next->prev = preptr;

			free(ptr);
		}
	}
}


void delete_after(void)
{
	if(start == NULL)
	{
		printf("The list is empty\n");
	}

	else
	{
		int cond;

		printf("Enter the value of node after which has to be deleted: ");
		if(scanf("%d", &cond) != 1)
		{
			printf("scanf failed\n");
			exit(EXIT_FAILURE);
		}

		NODE *ptr;

		for(ptr = start ; ptr->next != NULL && ptr->val != cond ;
			ptr = ptr->next)	
		{
			;
		}

		if(ptr->val != cond)
		{
			printf("The conditional value is not found\n");
		}

		else if(ptr->next == NULL)
		{
			printf("Nothing to delete\n");
		}

		else if(ptr->next->next == NULL)
		{
			delete_tail();
			ptr->next = NULL;
		}

		else
		{
			NODE *node_to_be_deleted = ptr->next;

			ptr->next = node_to_be_deleted->next; 
			node_to_be_deleted->next->prev = ptr;

			free(node_to_be_deleted);
		}
	}
}


void delete_list(void)
{
	if(start == NULL)
	{
		printf("The list is empty\n");
	}

	else
	{
		while(start != NULL)
		{
			NODE *node_to_be_deleted = start;

			start = start->next;

			free(node_to_be_deleted);
		}
	}
}


void sort_list(void)
{
	if(start != NULL)
	{
		NODE *preptr = start;
		NODE *ptr = start;

		int temp;

		while(preptr->next != NULL)
		{
			ptr = preptr->next;

			while(ptr != NULL)
			{
				if(preptr->val > ptr->val)
				{
					temp = preptr->val;

					preptr->val = ptr->val;
					ptr->val = temp;
				}

				ptr = ptr->next;
			}				

			preptr = preptr->next;
		}
	}
}
