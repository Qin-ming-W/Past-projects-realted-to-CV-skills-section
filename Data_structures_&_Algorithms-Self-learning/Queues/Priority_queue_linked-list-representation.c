/*
	Sorted linked list implementation of Priority queue

	A priority queue is a data structure in which each element is assigned 
	a priority. The priority will determine the order in which the elements
	will be processed.

	• An element with higher priority is processed before an element with 
	  a lower priority.
	• Two elements with the same priority are processed on a first-come-first-served
	  (FCFS) basis.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _node
{
	char *name;
	int priority;
	struct _node *next;
} NODE;

typedef struct _queue
{
	NODE *head;
} QUEUE;


void insert(QUEUE *queue)
{
	int priority;
	char name[BUFSIZ];

	// REQUEST USER INPUT
	printf("Enter the name and its priority(separated by white-spaces): ");

	// ACCOUNT FOR scanf() FAILURE
	if(scanf("%s %d", name, &priority) != 2)
	{
		fprintf(stderr, "%s\n", "scanf() conversion failed)");
		exit(EXIT_FAILURE);
	}

	NODE *new = NULL;

	// ATTEMPTS TO ALLOCATE MEMORY FOR new
	new = malloc(sizeof(NODE));

	// ACCOUNT FOR malloc() FAILURE
	if(new == NULL)
	{
		fprintf(stderr, "%s\n", "malloc() failed");
		exit(EXIT_FAILURE);
	}

	// ATTEMPTS TO DO A COPY OF name AND INITIALISE ITS CORRESPONDING FIELD
	new->name = strdup(name);

	if(new->name == NULL)
	{
		fprintf(stderr, "%s\n", "strdup() failed");
		exit(EXIT_FAILURE);
	}

	// INITIALISE FIELDS
	new->priority = priority;
	new->next = NULL;

	// THE PROCESS OF INSERTION OFFICAILLY BEGINES

	// IF THE QUEUE IS EMPTY OR new WITH HIGHER PRIORITY THAN ANY ELEMENTS SAVED IN THE QUEUE
	if(queue->head == NULL || new->priority < queue->head->priority)
	{
		// APPEND THE HEAD OF THE QUEUE TO new
		new->next = queue->head;

		// THE HEAD OF THE QUEUE IS NOW new
		queue->head = new;
	}

	// OTHERWISE
	// THAT IS, new WITH EQUAL OR LOWER PRIORITY THAN PREVIOUS ELEMENTS SAVED IN THR QUEUE
	else
	{
		// SAVE A COPY OF queue->head
		NODE *cpy = queue->head;

		// START SEARCHING FROM THE ELEMENT RIGHT AFTER THE HEAD
		// TO FIND THE FIRST ELEMENT WITH LOWER PRIORITY THAN new
		while(cpy->next != NULL && cpy->next->priority <= new->priority)
		{
			cpy = cpy->next;
		}


		// APPEND THE ONE JUST FOUND TO new
		new->next = cpy->next;

		// new NOW TAKES THE PLACE THAT WE JUST FOUND !!!
		cpy->next = new;
	}
}


void delete(QUEUE *queue)
{
	// THE QUEUE IS EMPTY
	if(queue->head == NULL)
	{
		printf("The queue is empty");
	}

	// OTHERWISE
	else
	{
		// SAVE A COPY OF queue->head
		NODE *cpy = queue->head;

		// POP OFF THE HEAD OF THE QUEUE
		queue->head = queue->head->next;

		// DEALLOCATE MEMORY
		free(cpy);
	}
}


void dispaly(QUEUE *queue)
{
	// THE QUEUE IS EMPTY
	if(queue->head == NULL)
	{
		printf("The queue is empty, nothing to Display\n");
	}

	// OTHERWISE
	else
	{
		NODE *node = queue->head;

		while(node != NULL)
		{
			printf("%s[priority = %d]––>", node->name, node->priority);

			// UPDATE node
			node = node->next;
		}

		printf("\n");
	}
}


int main(int argc, char *argv[])
{
	int opt;

	QUEUE *queue = NULL;

	// ATTEMPTS TO ALLOCATAE MEMORY FOR queue
	queue = malloc(sizeof(QUEUE));

	// ACCOUNT FOR malloc() FAILURE
	if(queue == NULL)
	{
		fprintf(stderr, "%s\n", "malloc() failure");
		exit(EXIT_FAILURE);
	}

	// INITIALISE FIELDS
	queue->head = NULL;

	do
	{
		// DISPLAY MAIN MENU PAGE
		printf("\n ***** MIAN MENU *****\n");
		printf("1. Insert\n");
		printf("2. Delete\n");
		printf("3. Display\n");
		printf("4. Exit\n");

		// REQUEST USER INPUT
		printf("Enter your option: ");

		// ACCOUNT FOR scanf() FAILURE
		if(scanf("%d", &opt) != 1)
		{
			fprintf(stderr, "%s\n", "scanf() conversion failure");
			exit(EXIT_FAILURE);
		}

		switch(opt)
		{
			case 1 :
				insert(queue);
				break;

			case 2 :
				delete(queue);
				break;

			case 3 :
				dispaly(queue);
				break;

			case 4 :
				break;

			default :
				printf("The option is not recognized, please try again\n");
				break;
		}

	} while(opt != 4);

	return 0;
}
