/*
	Fixed size array implementation of Multiple queues
*/

#include <stdio.h>
#include <stdlib.h>

// DEFINE THR MAXIMUM SIZE OF THE ARRAY THAT WILL BE USED TO HOLD TWO QUEUES
#define MAX 10

int arr[MAX];

int frontA = -1, rearA = -1;
int frontB = MAX, rearB = MAX;


void insertA(void)
{
	int val;

	// REQUEST USER INPUT
	printf("Enter an integer: ");

	// ACCOUNT FOR scanf() FAILURE
	if(scanf("%d", &val) != 1)
	{
		fprintf(stderr, "%s\n", "scanf failed");
		exit(EXIT_FAILURE);
	}

	// arr OVERFLOW
	if( (rearA + 1) == rearB )
	{
		printf("Insufficient space\n");
	}

	else
	{
		// QUEUE_A IS EMPTY
		if(frontA == -1 && rearA == -1)
		{
			rearA = frontA = 0;
			arr[rearA] = val;
		}

		// OTEHRWISE
		else
		{
			arr[++rearA] = val;
		}
	}
}


void insertB(void)
{
	int val;

	// REQUEST USER INPUT
	printf("Enter an integer: ");

	// ACCOUNT FOR scanf() FAILURE
	if(scanf("%d", &val) != 1)
	{
		fprintf(stderr, "%s\n", "scanf failed");
		exit(EXIT_FAILURE);
	}

	// arr OVERFLOW
	if( (rearA + 1) == rearB )
	{
		printf("Insufficient space\n");
	}

	else
	{
		// QUEUE_B IS EMPTY
		if(rearB == MAX && frontB == MAX)
		{
			rearB = frontB = (MAX - 1);
			arr[rearB] = val;
		}

		//OTHERWISE
		else
		{
			arr[--rearB] = val;
		}
	}
}
 

void deleteA(void)
{
	// QUEUE_A IS EMPTY
	if(frontA == -1 && rearA == -1)
	{
		printf("Queue_A is empty\n");
	}

	// OTHERWISE
	else
	{
		// POP OFF VALUE FROM FAR LEFT OF arr
		++frontA;

		// AFTER DELETION, IF QUEUE_A THEN BECOMES EMPTY
		if(frontA > rearA)
		{
			// RESET frontA AND rearA
			rearA = frontA = 0;
		}
	}
}


void deleteB(void)
{
	// QUEUE_B IS EMPTY
	if(frontB == MAX && rearB == MAX)
	{
		printf("Queue_B is empty\n");
	}

	// OTHERWISE
	else
	{
		// POP OFF VALUE FROM FAR RIGHT OF arr
		--frontB;

		// AFTER DELETION, IF QUEUE_B THEN BECOMES EMPTY
		if(frontB < rearB)
		{
			// RESET frontB AND rearB
			rearB = frontB = MAX;
		}
	}
}


void displayA(void)
{
	// QUEUE_A IS EMPTY
	if(frontA == -1 && rearA == -1)
	{
		printf("Queue_A is empty, nothing to display\n");
	}

	// OTHERWISE
	else
	{
		for(int i = frontA ; i <= rearA ; ++i)
		{
			printf("%d  ", arr[i]);
		}

		printf("\n");
	}
}


void displayB(void)
{
	// QUEUE_B IS EMPTY
	if(frontB == MAX && rearB == MAX)
	{
		printf("Queue_B is empty, nothing to display\n");
	}

	// OTHERWISE
	else
	{
		for(int i = frontB ; i >= rearB ; --i)
		{
			printf("%d  ", arr[i]);
		}

		printf("\n");
	}
}


int main(int argc, char *argv[])
{
	int opt;

	do
	{
		// DISPLAY MAIN MENU PAGE
		printf(" ***** MAIN MENU *****\n");
		printf("1. Insert in queue A\n");
		printf("2. Insert in queue B\n");
		printf("3. Delete from queue A\n");
		printf("4. Delete from queue B\n");
		printf("5. Display queue A\n");
		printf("6. Display queue B\n");
		printf("7. Exit\n");

		// REQUEST USER INPUT
		printf("Enter your option: ");

		// ACCOUNT FOR scanf() FAILURE
		if(scanf("%d", &opt) != 1)
		{
			fprintf(stderr, "%s\n", "scanf() failed");
			exit(EXIT_FAILURE);
		}

		switch(opt)
		{
			case 1 :
				insertA();
				break;

			case 2 :
				insertB();
				break;

			case 3 :
				deleteA();
				break;

			case 4 :
				deleteB();
				break;

			case 5 :
				displayA();
				break;

			case 6 :
				displayB();
				break;
			
			case 7 :
				break;

			default :
				fprintf(stderr, "%s\n", "Option is not recognised, try again");
				break;
		}

	} while(opt != 7);
 
	return 0;
}
