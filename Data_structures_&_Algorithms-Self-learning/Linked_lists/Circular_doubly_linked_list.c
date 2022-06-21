/*
	Implementation of a Circular two-way linked list
*/

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
		printf("11 : Exit\n");

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
				break;

			default :
				printf("Option is not recognized, try again\n");
				break;
		}

	} while(opt != 11);


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

	NODE *new = malloc( sizeof(NODE));
	CHECK_ALLOC(new);

	new->prev = new;
	new->next = new;
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
		NODE *ptr = start;

		while(ptr->next != start)
		{	
			if(ptr == start) printf("addr %lu<––", (unsigned long)ptr->prev);

			printf("%d[addr %lu]<––>", ptr->val, (unsigned long)ptr);

			ptr = ptr->next;
		}

		if(ptr == start) 
		{ 
			printf("addr %lu<––%d[addr %lu]––>addr %lu", 
					(unsigned long)ptr->prev, ptr->val, 
					(unsigned long)ptr, (unsigned long)ptr->next); 
		}

		else 
		{ 
			printf("%d[addr %lu]––>addr %lu", ptr->val, (unsigned long)ptr, 
					(unsigned long)ptr->next); 
		}

		printf("\n");
	}
}


void insert_head(void)
{
	NODE *new = get_new();

	if(start == NULL)
	{
		start = new;
	}

	else
	{
		new->next = start;
		new->prev = start->prev;
		start->prev = new;

		NODE *ptr;
		for(ptr = start ; ptr->next != start ; ptr = ptr->next) { ; }

		ptr->next = new;

		start = new;
	}
}


// Seems like something is wrong with this one
void insert_tail(void)
{
	NODE *new = get_new();

	if(start == NULL)
	{
		start = new;
	}

	else
	{
		NODE *tail = start->prev;

		tail->next = new;
		new->prev = tail;
		new->next = start;

		start->prev = new;
	}
}


void insert_before(void)
{
	if(start == NULL)
	{
		printf("The list is empty\n");
	}

	else
	{
		int cond;

		printf("Enter the value before which the new node has to be added: ");
		if(scanf("%d", &cond) != 1)
		{
			printf("scanf() failed\n");
			exit(EXIT_FAILURE);
		}

		NODE *new = get_new();

		NODE *ptr;

		for(ptr = start ; 
			ptr->val != cond && ptr->next != start ;
			ptr = ptr->next) { ; }

		if(ptr->val != cond)
		{
			printf("The conditional value is not found\n");
		}

		else
		{
			new->next = ptr;
			new->prev = ptr->prev;

			ptr->prev->next = new;
			ptr->prev = new;

			if(ptr == start) { start = new; }
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

		printf("Enter the value after which the new node has to be added: ");
		if(scanf("%d", &cond) != 1)
		{
			printf("scanf() failed\n");
			exit(EXIT_FAILURE);
		}

		NODE *new = get_new();

		NODE *ptr;
		for(ptr = start ; 
			ptr->val != cond && ptr->next != start ;
			ptr = ptr->next) { ; }

		if(ptr->val != cond)
		{
			printf("The conditional value is not found\n");
		}

		else
		{
			new->prev = ptr;
			new->next = ptr->next;

			NODE *cpy = ptr->next;
			ptr->next = new;
			cpy->prev = new;
		}
	}
}


void delete_head(void)
{
	if(start == NULL)
	{
		printf("The list is empty, nothing to be deleted\n");
	}

	else if(start == start->next)
	{
		free(start);
		start = NULL;
	}

	else
	{
		NODE *node_to_be_deleted = start;

		start = start->next;

		start->prev = node_to_be_deleted->prev;
		node_to_be_deleted->prev->next = start;

		free(node_to_be_deleted);
	}
}


void delete_tail(void)
{
	if(start == NULL)
	{
		printf("The list is empty, nothing to be deleted\n");
	}

	else
	{
		NODE *tail = start->prev;

		tail->prev->next = start;
		start->prev = tail->prev;

		free(tail);

		if(tail == start) start = NULL;
	}
}


void delete_node(void)
{
	if(start == NULL)
	{
		printf("The list is empty, nothing to be deleted\n");
	}

	else
	{
		int cond;

		printf("Enter the value of node which has to be deleted: ");
		if(scanf("%d", &cond) != 1)
		{
			printf("scanf() failed\n");
			exit(EXIT_FAILURE);
		}

		NODE *ptr;

		for(ptr = start ; 
			ptr->val != cond && ptr->next != start ;
			ptr = ptr->next) { ; }

		if(ptr->val != cond)
		{
			printf("The conditional value is not found\n");
		}

		else if(ptr->next == ptr && ptr->prev == ptr)
		{
			free(ptr);
			start = NULL;
		}

		else
		{
			ptr->prev->next = ptr->next;
			ptr->next->prev = ptr->prev;

			if(ptr == start) start = start->next;

			free(ptr);
		}
	}
}


void delete_after(void)
{
	if(start == NULL)
	{
		printf("The list is empty, nothing to be deleted\n");
	}

	else
	{
		int cond;

		printf("Enter the value of node after which has to be deleted: ");
		if(scanf("%d", &cond) != 1)
		{
			printf("scanf() failed\n");
			exit(EXIT_FAILURE);
		}

		NODE *ptr;

		for(ptr = start ;
			ptr->val != cond && ptr->next != start ;
			ptr = ptr->next) { ; }

		if(ptr->val != cond)
		{
			printf("The conditional value is not found\n");
		}

		else if(ptr->next == ptr && ptr->prev == ptr)
		{
			printf("Only one node presents in the circular two-way list");
			printf(", no operation has been performed\n");
		}

		else if(ptr->next == start) delete_head();

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
		printf("The list is empty, nothing to be deleted\n");
	}

	else
	{
		NODE *indicator = start;

		while(start->next != indicator)
		{
			NODE *node_to_be_deleted = start;

			start = start->next;

			free(node_to_be_deleted);
		}

		free(start);
		start = NULL;
	}
}
