/*
	Array representation of circular queues
*/

#include <stdio.h>
#include <stdlib.h>

// DEFINE THE MAXIMUM SIZE OF QUEUE
#define MAX 10

// front HOLDS THE POSITION OF THE FRONT END OF THE QUEUE
int front = -1;
// rear HOLDS THE POSITION OF THE REAR END OF THE QUEUE
int rear  = -1;

int queue[MAX];


void insert(int val)
{
	// THE QUEUE IS FULL
	// KNOWING THE FACT THAT THE FRONT END COMES RIGHT AFTER THE REAR END
	// WHEN THE QUEUE IS FULL
	if( (rear + 1) % MAX == front )
	{
		printf("The queue is full");
	}

	// OTHERWISE
	else
	{
		// THE QUEUE IS EMPTY
		// BOTH front AND end REQUIRE TO BE MODIFIED
		if(front == -1 && rear == -1)
		{
			/*
			front = (front + 1) % MAX;
			rear = (rear + 1) % MAX;
			*/
			front++;
			rear++;
		}

		// OTHERWISE
		// ONLY rear NEEDS TO BE MODIFIED
		else
		{
			rear = (rear + 1) % MAX;
		}

		queue[rear] = val;
	}
}


void delete()
{
	// THE QUEUE IS EMPTY
	if(front == -1 && rear == -1)
	{
		printf("The queue is empty, nothing to delete");
	}

	// OTEHRWISE
	else
	{
		// PUSH front ONE POSITION TO THE RIGHT, OR POSSIBLY ROLLS BACK
		front = (front + 1) % MAX;

		// AFTER DELETION, THE QUEUE BECOMES EMPTY
		if(rear == (MAX - 1) && front == 0)
		{
			// RESET front AND rear
			front = -1;
			rear = -1;
		}
		
		// AFTER DELETION, THE QUEUE BECOMES EMPTY
		if(rear < (MAX - 1) && front == (rear + 1))
		{
			// RESET front AND rear
			front = -1;
			rear = -1;
		}
	}
}


int peek()
{
	// THE QUEUE IS GURANTEED TO BE NON-EMPTY, WHEN peek() IS INVOKED
	return queue[front];
}


void display()
{
	// THE QUEUE IS EMPTY
	if(front == -1 && rear == -1)
	{
		printf("The circular queue is empty, nothing to display");
	}

	// OTHERWISE
	else
	{
		if(front <= rear)
		{
			for(int i = front ; i <= rear ; ++i)
			{
				printf("%d  ", queue[i]);
			}
		}

		else
		{
			// TRAVERSE THE QUEUE FROM front TO THE RIGHT, UNTIL THE END
			for(int i = front ; i < MAX ; ++i)
			{
				printf("%d  ", queue[i]);
			}

			// POSSIBLY ROLLS BACK
			// THEN TRAVERSE FROM THE BEGINNING OF THEB QUEUE TO rear
			for(int i = 0 ; i <= rear ; ++i)
			{
				printf("%d  ", queue[i]);	
			}
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
				if(front == -1 && rear == -1)
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

