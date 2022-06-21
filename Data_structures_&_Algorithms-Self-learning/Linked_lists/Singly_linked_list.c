/*
	Implementation of all basic operation (Insertion, Deletion •••)
	of a singly linked list.
*/


#include <stdlib.h>
#include <stdio.h>

typedef struct _node
{
	int val;
	struct _node *next;
} NODE;


// REQUEST USER INPUT AND CREAT A NEW NODE
NODE *creat_node(void)
{
	int val;

	// REQUEST USER INPUT
	printf("Enter an integer: ");

	// ACCOUNT FOR scanf() FAILURE
	if( scanf("%d", &val) != 1 )
	{
		fprintf(stderr, "%s\n", "scanf() failed");
		exit(EXIT_FAILURE);
	}

	// ATTEMPTS TO ALLOCATE MEMORY FOR new NDOE
	NODE *new = malloc( sizeof(NODE) );

	// ACCOUNT FOR malloc() FAILURE
	if(new == NULL)
	{
		fprintf(stderr, "%s\n", "malloc() failed");
		exit(EXIT_FAILURE);	
	}

	// INITIALISE FIELDS
	new->val = val;
	new->next = NULL;

	return new;
}


// SUPPORT OPTION 1
void display(NODE *start)
{
	// THE LIST IS EMPTY
	if(start == NULL)
	{
		printf("The list is empty, nothing to display\n");
	}

	// OTHERWISE
	else
	{
		NODE *temp = start;

		while(temp != NULL)
		{
			printf("%d––>", temp->val);

			// GO TO NEXT ONE
			temp = temp->next;
		}

		printf("\n");
	}
}


// SUPPORT OPTION 2
NODE *insert_head(NODE *start)
{
	NODE *new = creat_node();

	// ADD TO THE HEAD
	new->next = start;

	// new THEN BECOMES THE HEAD OF THE LIST
	start = new;

	return start;
}


// SUPPORT OPTION 3
NODE *insert_tail(NODE *start)
{
	NODE *new = creat_node();

	NODE *temp = start;

	// THE LIST IS EMPTY
	if(start == NULL)
	{
		// ADD TO THE TAIL(HEAD)
		new->next = start;

		// new THEN BECOMES THE HEAD(TAIL) OF THE LIST
		start = new;
	}

	// OTHERWISE
	else
	{
		// TRAVERSE THE ENTIRE LIST UNTIL REACH ITS TAIL
		// temp NOEW POINTS TO THE LAST NODE OF THE LIST
		while(temp->next != NULL)
		{
			temp = temp->next;
		}

		// APPEND new TO TAIL
		temp->next = new;	
	}

	return start;
}


// SUPPORT OPTION 4
NODE *insert_before(NODE *start)
{
	int cond;

	// REQUEST USER INPUT (THE CONDITIONAL VALUE)
	printf("Enter the conditional value before which the data has to be inserted: ");

	// ACCOUNT FOR scanf() FAILURE
	if( scanf("%d", &cond) != 1 )
	{
		fprintf(stderr, "%s\n", "scanf() failed");
		exit(EXIT_FAILURE);
	}


	// INSERTION OFFCIALLY BEGINS FROM HERE


	NODE *new = creat_node();

	NODE *preptr = start;
	NODE *ptr = start;	

	// TRAVERSE THROUGH THE LIST TO FIND THE NODE THAT CONTAINS THE CONDITIONAL VALUE
	while(ptr != NULL && ptr->val != cond)
	{
		preptr = ptr;

		// GO TO NEXT ONE
		ptr = ptr->next;
	}

	// EXHAUST THE LIST, CONDITIONAL VALUE IS NOT FOUND
	if (ptr == NULL)
	{
		printf("The conditional value is not found, no operation has been performed\n");
	}

	// EDGE CASE, THE CONDITIONAL NODE IS THE HEAD OF THE LIST
	// OPERATION IS THEN EQUIVALENT TO insert_head()
	else if(ptr == start)
	{
		new->next = ptr;
		start = new;
	}

	// INSERT new IN THE MIDDLE OF LIST
	else
	{
		// ADD new BEFORE CONDITIONAL NODE
		new->next = ptr;

		// MAKE SURE THE LINKED LIST IS NOT BROKEN
		preptr->next = new;
	}

	return start;
}


// SUPPORT OPTION 5
NODE *insert_after(NODE *start)
{
	int cond;

	// REQUEST USER INPUT (THE CONDITIONAL VALUE)
	printf("Enter the conditional value after which the data has to be inserted: ");

	// ACCOUNT FOR scanf() FAILURE
	if( scanf("%d", &cond) != 1 )
	{
		fprintf(stderr, "%s\n", "scanf() failed");
		exit(EXIT_FAILURE);
	}

	
	// INSERTION OFFICIALLY BEGINS FROM HERE


	NODE *new = creat_node();

	NODE *ptr = start;

	// TRAVERSE THROUGH THE LIST TO FIND THE NODE THAT CONTAINS THE CONDITIONAL VALUE
	while(ptr != NULL && ptr->val != cond)
	{
		ptr = ptr->next;
	}

	// EXHAUST THE LIST, CONDITIONAL NODE IS NOT FOUND
	if(ptr == NULL)
	{
		printf("The conditional value is not found, no operation has been performed\n");
	}

	else
	{	
		NODE *cpy = ptr->next;

		// APPEND new TO THE CONDITIONAL NODE
		ptr->next = new;

		// MAKE SURE THE LINKED LIST IS NOT BROKEN
		new->next = cpy;
	}

	return start;
}


// SUPPORT OPTION 6
NODE *delete_head(NODE *start)
{
	// THE LIST IS EMPTY
	if(start == NULL)
	{
		printf("The list is empty, no operation has been performed\n");
	}

	// OTHERWISE
	else
	{
		NODE *cpy = start;

		// DELETE THE HEAD OF LIST
		start = start->next;

		// DEALLOCATE MEMORY OF DELETED NODE
		free(cpy);
	}

	return start;
}


// SUPPORT OPTION 7
NODE *delete_tail(NODE *start)
{
	// THE LIST IS EMPTY
	if(start == NULL)
	{
		printf("The list is empty, no operation has been performed\n");
	}

	// OTHERWISE
	else
	{
		// ptr WILL POINT TO THE LAST NODE OF THE LIST
		NODE *ptr = start;
		NODE *preptr = start;

		// TRAVERSE THROUGH THE LSIT TO FIND THE LAST NODE
		while(ptr->next != NULL)
		{
			preptr = ptr;
			ptr = ptr->next;
		}

		// EDGE CASE, ONLY ONE NODE IN THE LIST
		if(ptr == start)
		{
			// AFTER DELETION, THE LIST SIMPLY DOES NOT EXIST ANY MORE
			start = NULL;
		}

		else
		{	
			// CUT OFF THE LAST NODE
			preptr->next = NULL;

			// DEALLCOATE MEMORY OF DELETED NODE
			free(ptr);
		}
	}

	return start;
}


// SUPPORT OPTION 8
NODE *delete_node(NODE *start)
{
	int cond;

	// REQUEST USER INPUT (THE CONDITIONAL VALUE)
	printf("Enter the value of the node which has to deleted: ");

	// ACCOUNT FOR scanf() FAILURE
	if( scanf("%d", &cond) != 1 )
	{
		fprintf(stderr, "%s\n", "scanf() failed");
		exit(EXIT_FAILURE);
	}


	// DELETION OFFCIALY BEGINS FROM HERE


	NODE *preptr = start;
	NODE *ptr = start;

	// ATTEMPTS TO FIND THE CONDITIONAL NODE BY TRAVERSING THROUGH THE LIST
	while(ptr != NULL && ptr->val != cond)
	{
		preptr = ptr;

		// GO TO THE NEXT ONE
		ptr = ptr->next;
	}

	// THE CONDITIONAL VALUE IS NOT FOUND
	if(ptr == NULL)
	{
		printf("The conditional node is not found, no deletion has been performed\n");
	}

	// EDGE CASE, CONDITIONAL NODE SITS AT THE HEAD OF LIST
	// THE DELETION IS THEN EQUIVALENT TO delete_head()
	else if(ptr == start)
	{
		NODE *cpy = start;

		// DELETE HEAD
		start = start->next;

		// DEALLOCATE MEMORY OF DELETED NODE
		free(cpy);
	}

	else
	{
		NODE *next_one = ptr->next;

		// DEALLOCATE MEMORY OF DELETED NODE
		free(ptr);

		// MAKE SURE THE LIST IS NOT BROKEN
		preptr->next = next_one;
	}

	return start;
}


// SUPPORT OPTION 9
NODE *delete_after(NODE *start)
{
	int cond;

	// REQUEST USER INPUT (THE CONDITIONAL VALUE)
	printf("Enter the value after which the node has to deleted: ");

	// ACCOUNT FOR scanf() FAILURE
	if( scanf("%d", &cond) != 1 )
	{
		fprintf(stderr, "%s\n", "scanf() failed");
		exit(EXIT_FAILURE);
	}


	// DELETION OFFCIALY BEGINS FROM HERE


	NODE *ptr = start;

	// ATTEMPTS TO FIND THE CONDITIONAL NODE BY TRAVERSING THROUGH THE LIST
	while(ptr != NULL && ptr->val != cond)
	{
		ptr = ptr->next;
	}

	// THE CONDITIONAL VALUE IS NOT FOUND
	if(ptr == NULL)
	{
		printf("The conditional node is not found, no deletion has been performed\n");
	}

	else
	{
		NODE *node_to_be_deleted = ptr->next;

		if(node_to_be_deleted != NULL)
		{
			NODE *next_one = node_to_be_deleted->next;

			// DEALLOCATE MEMORY OF DELETED NODE
			free(node_to_be_deleted);

			// MAKE SURE THE LINKED LIST IS NOT BROKEN
			ptr->next = next_one;
		}
		
	}

	return start;
}


// SUPPORT OPTION 10
NODE *delete_list(NODE *start)
{
	// LIST EXISTS, DELETION AND MEMORY DEALLOCATION PERFORM
	if(start != NULL)
	{
		// TRAVERSE THROUGH THE LIST TILL END
		while(start != NULL)
		{
			NODE *next_one = start->next;

			// DEALLOCATE MEMORY OF THE HEAD OF LIST
			free(start);

			// GO TO NEXT ONE
			start = next_one;
		}
	}

	return start;
}


// SUPPORT OPTION 11
// THE LIST WILL BE SORTED IN ASCENDING ORDER
NODE *sort_list(NODE *start)
{
	// LIST EXISTS, SORTING PERFORMS
	if(start != NULL)
	{
		NODE *preptr = start;
		NODE *ptr = start;

		// temp HOLDS TEMPERORY SWAP-VALUE
		int temp;

		// OUTER while LOOP ITERATES EACH NODE, ONE AT A TIME
		while(preptr->next != NULL)
		{
			ptr = preptr->next;

			// INNER while LOOP ATTEMPTS TO FIND A NODE WITH SMALLER DATA VALUE THAN
			// THE CURRENT MEMBER IN OUTER while LOOP
			while(ptr != NULL)
			{
				// SWAP IF NODE WITH SMALLER VALUE IS FOUND
				if(preptr->val > ptr->val)
				{
					temp = preptr->val;

					preptr->val = ptr->val;
					ptr->val = temp;
				}

				// GO TO NEXT ONE
				ptr = ptr->next;
			}				

			// GO TO NEXT ONE
			preptr = preptr->next;
		}
	}

	return start;
}


NODE *reverse_list(NODE *start)
{
	if(start == NULL)
	{
		printf("The list is empty\n");
	}

	else
	{
		NODE *prev = NULL;
		NODE *ptr = start;

		while(ptr != NULL)
		{
			NODE *cpy = ptr->next;

			if(ptr == start) ptr->next = NULL;
			else ptr->next = prev;

			prev = ptr;
			ptr = cpy;
		}

		start = prev;
	}

	return start;
}


int main(int argc, char *argv[])
{
	int opt;

	NODE *start = NULL;

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
		printf("12 : Reverse the list\n");
		printf("13 : Exit\n");

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
				display(start);
				break;

			case 2 :
				start = insert_head(start);
				break;

			case 3 :
				start = insert_tail(start);
				break;

			case 4 :
				start = insert_before(start);
				break;

			case 5 :
				start = insert_after(start);
				break;

			case 6 :
				start = delete_head(start);
				break;

			case 7 :
				start = delete_tail(start);
				break;

			case 8 :
				start = delete_node(start);
				break;

			case 9 :
				start = delete_after(start);
				break;

			case 10 :
				start = delete_list(start);

			case 11 :
				start = sort_list(start);
				break;

			case 12:
				start = reverse_list(start);

			case 13 :
				break;

			default :
				printf("Option is not recognized, try again\n");
				break;
		}

	} while(opt != 13);

	return 0;
}
