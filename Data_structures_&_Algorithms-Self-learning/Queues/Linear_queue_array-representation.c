/*
	Array representation of linear queue

	All insertions will be done at the rear end of the queue
	All deletions will be done at the front end of the queue
*/

#include <stdio.h>
#include <stdlib.h>

// DYNAMICALLY ALLOCATE MEMORY FOR QUEUE, POINTED TO BY queue
// UNLESS MEMORY IS INSUFFICINET, THE QUEUE CAN NEVER BE OVERFLOWED
int *queue = NULL;

// num HOLDS THE SIZE OF THE QUEUE
int num = 0;


void insert(int val)
{
	// REALLOCATE MEEMORY FOR QUEUE, POINTED TO BY queue
	queue = realloc(queue, sizeof(int) * (num + 1));

	// ACCOUNT FOR realloc() failure
	if(queue == NULL)
	{
		fprintf(stderr, "%s\n", "realloc failed");
		exit(EXIT_FAILURE);
	}

	// INSERT val INTO QUEUE
	queue[num] = val;

	// UPDATE THE SIZE OF THE QUEUE
	++num;
}


void delete()
{
	// THE QUEUE IS NOT EMPTY
	if(num > 0)
	{
		// DON'T KNOW HOW TO free() THE MEMORY HERE
		// IT DOES NOT SOLVE THE UNDERFLOW OF THE LINEAR QUEUE
		++queue; // ???


		// UPDATE THE SIZE OF THE QUEUE
		--num;
	}
}


int peek()
{
	// THE QUEUE IS GURANTEED TO BE NON-EMPTY, IF peek() IS INVOKED
	return queue[0];
}


void display()
{
	// THE QUEUE IS EMPTY
	if( num == 0)
	{
		printf("The queue is empty, nothing to disply\n");
	}

	// OTHERWISE
	else
	{
		for(int i = 0 ; i < num; ++i)
		{
			printf("%d  ", queue[i]);
		}
		printf("\n");
	}
}


int main(int argc, char *argv[])
{
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

				insert(val);
				break;

			// DELETION
			case 2 :
				delete();
				break;

			// PEEK
			case 3 :
				// THE QUEUE IS EMPTY
				if(num == 0)
				{
					printf("The queue is empty, nothing to peek");
				}

				// OTHERWISE
				else
				{
					int value = peek();
					printf("The value currently sitting at the front end of the queue is %d\n", value);
				}
				break;

			// DISPLAY
			case 4 :
				display();
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
