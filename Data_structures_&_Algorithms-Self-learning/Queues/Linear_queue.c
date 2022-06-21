/*
	Linked representation of linear queues

	All insertions will be done at the rear end.
	All deletions will be done at the front end.
*/

#include <stdlib.h>
#include <stdio.h>


typedef struct  _node
{
	int data;
	struct _node *next;
} NODE;


typedef struct _queue
{
	NODE *front;
	NODE *rear;
} QUEUE;


// INITIALISE AN EMPTY QUEUE
void creat_queue(QUEUE *queue)
{
	queue->front = NULL;
	queue->rear  = NULL;
}


// INSERT AN ELEMENT val AT THE REAT END
QUEUE *insert(QUEUE *queue, int val)
{
	// ATTEMPT TO ALLOCATE MEMORY TO A NEW NODE
	NODE *node = NULL;
	node = malloc( sizeof(NODE) );

	// ACCOUNT FOR malloc() FAILURE
	if(node == NULL)
	{
		fprintf(stderr, "%s\n", "malloc() failed");
		exit(EXIT_FAILURE);
	}

	// INITIALISE FIELDS
	node->data = val;
	node->next = NULL;

	// THE QUEUE IS EMPTY 
	if(queue->front == NULL)
	{
		queue->front = node;
		queue->rear  = node;
	}

	// OTHERWISE
	else
	{
		queue->rear->next = node;
		queue->rear = node;	
	}

	return queue;
}


// DELETE AN ELEMENT AT THE FRONT END
QUEUE *delete(QUEUE *queue)
{
	// THE QUEUE IS EMPTRY, NOTHING TO BE DELETED
	if(queue->front == NULL)
	{
		printf("The queue is empty\n");
	}

	// OTHERWISE
	else
	{
		// TAKE A COPY OF THE NODE TO BE DELETED
		NODE *cp = queue->front;

		// DELETE THE NODE CURRENTLY LINKING AT THE FRONT END
		queue->front = queue->front->next;

		// DEALLOCATE ITS MEMORY
		free(cp);

		// IF QUEUE BECOMES EMPTY ADTER DELETION
		// UPDATE queue->rear
		if(cp == queue->rear)
		{
			queue->rear = NULL;
		}	
	}

	return queue;
}


// PEEK THE ELEMENT AT THE FRONT END
int peek(QUEUE *queue)
{
	// WHEN peek() IS INVOKED, THE QUEUE IS GUARANTEED TO BE NON-EMPTY
	return queue->front->data;
}


// DISPLAY THE QUEUE
void display(QUEUE *queue)
{
	// THE QUEUE IS EMPTY
	if(queue->front == NULL)
	{
		printf("%s\n", "The queue is empty");
	}

	// OTHERWISE
	else
	{
		NODE *node = queue->front;
		while(node != NULL)
		{
			printf("%d––>", node->data);
			node = node->next;
		}
		printf("\n");
	}
}


int main(int argc, char *argv[])
{
	// ATTEMPT TO ALLOCATE MEMORY
	QUEUE *queue = NULL;
	queue = malloc( sizeof(queue) );

	// ACCOUNT FOR malloc() FAILURE
	if(queue == NULL)
	{
		fprintf(stderr, "%s\n", "malloc() failed");
	}

	creat_queue(queue);

	int opt;
	int val;

	do
	{
		// DISPLAT MENU PAGE
		printf("\n***** MIAN MENU *****\n");
		printf("1. INSERT\n");
		printf("2. DELETE\n");
		printf("3. PEEK\n");
		printf("4. DISPLAY\n");
		printf("5. EXIT\n");

		// REQUEST USER INPUT
		printf("Enter your option: ");

		// ACCOUNT FOR scanf() FAILURE
		if(scanf("%d", &opt) != 1)
		{
			fprintf(stderr, "%s\n", "Option conversion failed");
			exit(EXIT_FAILURE);
		}

		// PROCESS OPTION
		switch(opt)
		{
			// INSERTION
			case 1 :

				// REQUEST USER INPUT
				printf("Enter an integer value: ");

				// ACCOUNT FOR USER INPUT
				if(scanf("%d", &val) != 1)
				{
					fprintf(stderr, "%s\n", "Input conversion failed");
					exit(EXIT_FAILURE);
				}

				queue = insert(queue, val);
				break;

			// DELETION
			case 2 :
				queue = delete(queue);
				break;

			// PEEK
			case 3 :
				// THE QUEUE IS EMPTY
				if(queue->front == NULL)
				{
					printf("The queue is enmpty, nothing for you to peek!\n");
				}

				// OTHERWISE
				else
				{
					int value = peek(queue);
					printf("The value sitting at front of the queue is %d\n", value);
				}
				break;

			// DISPLAY
			case 4 :
				display(queue);
				break;

			// EXIT
			case 5 :
				break;

			default :
				printf("The option is not recognosed, please try again\n");
				break;
		}

	} while(opt != 5);

	return 0;
}
