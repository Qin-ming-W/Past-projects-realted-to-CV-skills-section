/*
	New elements can only be inserted or deleted at one end of the
	Linear queue and Circular queue, either front or rear end.

	In particular, new elements are added(deleted) at the rear(front) end of the queue,
	one at a time


	Deques

	IN GENERAL

	Elements can be inserted or deleted at either end, that is, elements
	can be added to or removed from either the front(head) or the back(tail)
	end.

	TWO VARIANTS

	1. Input restricted deque, in which insertions can only be done at one of
	the ends, while deletions can be done from both ends

	2. Output restricted deque, in which deletions can only be done at one of
	the ends, while insertions can be done from both ends


	This program implements Input restricted deque and Output restricted deque 
	by using circular array

	New elements can only be added at the right end of the Input restrcited end,
	and removed from left end of the Output restricted queue.
*/


#include <stdio.h>
#include <stdlib.h>

// DEFINE THE MAXIMUM SIZE OF THE DEQUE
#define MAX 10

int deque[MAX];

// left HOLDS THE POSITION OF LEFT END OF THE DEQUE
int left = -1;

// right HOLDS THE POSITION OF RIGHT END OF THE DEQUE
int right = -1;


void insert_left(int val)
{
	// THE QUEUE IS FULL
	if( (left + MAX - 1) % MAX == right )
	{
		printf("The queue is full\n");
	}

	// OTHERWISE
	else
	{
		// THE QUEUE IS EMPTY
		if(left == -1 && right == -1)
		{
			left = 0;
			right = 0;
		}

		// OTHERWISE
		else
		{
			left = (left + MAX - 1) % MAX;
		}

		deque[left] = val;	
	}
}


void insert_right(int val)
{
	// THE QUEUE IS FULL
	if ( (right + 1) % MAX == left )
	{
		printf("The queue is full");
	}

	// OTHERWISE
	else
	{
		// THE QUEUE IS EMPTY
		if(left == -1 && right == -1)
		{
			left = 0;
			right = 0;
		}

		// OTHERWISE
		else
		{
			right = (right + 1) % MAX;
		}

		deque[right] = val;
	}
}


void delete_left(void)
{
	// THE QUEUE IS EMPTY
	if(right == -1 && left == -1)
	{
		printf("The queue is empty");
	}

	// OTHERWISE
	else
	{
		// PUSH left ONE POSITION TO THE RIGHT
		// POSSIBLY ROLLS BACK
		left = (left + 1) % MAX;

		// AFTER DELETION, IF THE QUEUE THEN BECOMES EMPTY
		if(left == (right + 1) && right < (MAX - 1))
		{
			// RESET right AND left
			right = -1;
			left = -1;
		}

		// AFTER DELETION, IF THE QUEUE THEN BECOMES EMPTY
		if( left == 0 && right == (MAX - 1))
		{
			// RESET right AND left
			right = -1;
			left = -1;
		}
	}
}


void delete_right(void)
{
	// THE QUEUE IS EMPTY
	if(left == -1 && right == -1)
	{
		printf("The queue is empty\n");
	}

	// OTHERWISE
	else
	{
		// PUSH right ONE POSITION TO THE LEFT
		// POSSIBLY ROLLS BACK
		right = (right + MAX - 1) % MAX;

		// AFTER DELETION, IF THE QUEUE THEN BECOMES EMPTY
		if(left > 0 && left == (right + 1))
		{
			// RESET right AND left
			right = -1;
			left = -1;	
		}

		// AFTER DELETION, IF THE QUEUE THEN BECOMES EMPTY
		if(left == 0 && right == (MAX - 1))
		{
			// RESET right AND left
			right = -1;
			left = -1;
		}
	}
}


void display(void)
{
	// THE QUEUE IS EMPTY
	if(left == -1 && right == -1)
	{
		printf("The queue is empty, nothing to display");
	}

	// OTHERWISE
	else
	{
		if(left <= right)
		{
			for(int i = left ; i <= right ; ++i)
			{
				printf("%d  ", deque[i]);
			}
		}

		else
		{
			for(int i = left ; i <= MAX - 1 ; ++i)
			{
				printf("%d  ", deque[i]);	
			}

			for(int i = 0 ; i <= right ; ++i)
			{
				printf("%d  ", deque[i]);
			}
		}
	}
}


// INPLENT Input restricted deuqe
void input_deque(void)
{
	int opt;
	int val;

	do
	{
		printf("\n Input restricted deque\n");
		printf("1. Insert at right\n");
		printf("2. Delete from left\n");
		printf("3. Delete from right\n");
		printf("4. Display\n");
		printf("5. Quit\n");
		printf("Enter your option: ");

		if(scanf("%d", &opt) != 1)
		{
			fprintf(stderr, "%s\n", "scanf() conversion failed");
			exit(EXIT_FAILURE);
		}

		switch(opt)
		{
			case 1 :
				// REQUEST USER INPUT
				printf("Enter an integer number: ");

				// ACCOUNT FOR scanf() FAILURE
				if(scanf("%d", &val) != 1)
				{
					fprintf(stderr, "%s\n", "scanf() conversion failed");
					exit(EXIT_FAILURE);
				}
				insert_right(val);
				break;

			case 2 :
				delete_left();
				break;

			case 3 :
				delete_right();
				break;

			case 4 :
				display();
				break;

			case 5 :
				break;

			default :
				printf("The option is not recognosed, please try again\n");
				break;
		}

	} while(opt != 5);
}


// INPLENT Output restricted deuqe
void output_deque(void)
{
	int opt;
	int val;

	do
	{
		printf("\n Output restricted deque\n");
		printf("1. Insert at right\n");
		printf("2. Insert at left\n");
		printf("3. Delete from left\n");
		printf("4. Display\n");
		printf("5. Quit\n");
		printf("Enter your option: ");

		if(scanf("%d", &opt) != 1)
		{
			fprintf(stderr, "%s\n", "scanf() conversion failed");
			exit(EXIT_FAILURE);
		}

		switch(opt)
		{
			case 1 :
				// REQUEST USER INPUT
				printf("Enter an integer number: ");

				// ACCOUNT FOR scanf() FAILURE
				if(scanf("%d", &val) != 1)
				{
					fprintf(stderr, "%s\n", "scanf() conversion failed");
					exit(EXIT_FAILURE);
				}
				insert_right(val);
				break;

			case 2 :
			// REQUEST USER INPUT
				printf("Enter an integer number: ");

				// ACCOUNT FOR scanf() FAILURE
				if(scanf("%d", &val) != 1)
				{
					fprintf(stderr, "%s\n", "scanf() conversion failed");
					exit(EXIT_FAILURE);
				}
				insert_left(val);
				break;

			case 3 :
				delete_left();
				break;

			case 4 :
				display();
				break;

			case 5 :
				break;

			default :
				printf("The option is not recognosed, please try again\n");
				break;
		}
		
	} while(opt != 5);
}


int main(int argc, char *argv[])
{
	int opt;

	// DISPLAY MAIN MENU PAGE
	printf("***** MAIN MENU *****\n");
	printf("1. Input restricted deque\n");
	printf("2. Output restricted deque\n");
	printf("Enter your option: ");

	if(scanf("%d", &opt) != 1)
	{
		fprintf(stderr, "%s\n", "scanf() conversion failed");
		exit(EXIT_FAILURE);
	}

	switch(opt)
	{
		case 1 :
			input_deque();
			break;

		case 2 :
			output_deque();
			break;
	}

	return 0;
}
