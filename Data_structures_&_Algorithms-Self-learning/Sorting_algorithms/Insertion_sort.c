/*
	Implementation of Insertion sort

----------------------------------------------------------------------------------------------------

	The psuedocode of Insertion sort

	  1 2 3 4 5 6 •••• A.length
	 ––––––––––––––––––––
  A  | | | | | | |    | |
	 ––––––––––––––––––––

	 for j = 2 tO A.length
	 	key = A[j]
	 	i = j - 1
	 	while i > 0 and A[i] < key
	 		A[i + 1] = A[i]
	 		i = i - 1
	 	j = j + 1

----------------------------------------------------------------------------------------------------

	Outer loop invariant of Insertion sort:

	At the start of each iteration of the for loop, the sub-array A[1 .. j - 1] consists of elements
	originally in  A[1 .. j - 1], but in sorted order.

	Initialization (loop invariant is true before the first iteration of the loop):

	Loop invariant holds before the first iteration of for loop, when j = 2. The sub-array A[1 .. j
	- 1], therefore, consists of just a single element A[1], which is in fact the original element
	in A[1]. Moreover, this sub-array is sorted (trivially, of course), which shows that the loop
	invariant holds prior to the first for loop iteration.

	Maintenance (loop invariant is true before an iteration of the loop, it remains true before the
	next iteration):

	Informally, the body of the for loop works by moving A[j - 1], A[j - 2], A[j - 3], and so on by
	one position to the right until it finds the proper position for A[j], at which point it inserts
	the value A[j]. The sub-array then consists of elements originally in A[1 .. j], but in sorted
	order. Incrementing j for the next iteration of the for loop to preserver the loop invariant.

	Termination (when the loop terminates, the invariant gives us a useful property that helps show
	that the algorithm is correct):

	By the time of the termination of the for loop, j has the value n + 1.  Substituting j = n + 1 
	in the wording of loop invariant, we have that the sub-array A[1 .. n] consists of elements 
	originally in A[1 .. n], but in sorted order. Observing that A[1 .. n] is the entire array, we
	conclude that the entire array is sorted, hence the algorithm is correct.

----------------------------------------------------------------------------------------------------
*/	

#include <stdio.h>
#include <stdlib.h>

void show_sequence(int src[], int size)
{
	for(int i = 0 ; i < size ; ++i)
	{
		printf("%d  ", src[i]);
	}
	printf("\n");
}


void insertion_sort(int src[], int size)
{
	int i;
	int j = 1;  

	while(j < size)
	{
		int key = src[j];

		i = j - 1;
		while(i >= 0 && src[i] > key)
		{
			src[i + 1] = src[i];
			--i;
		}

		src[i + 1] = key;
		++j;
		show_sequence(src, size);
	}
}


// {2, 4, 3, 3, 0} is a good example
int main(int argc, char const *argv[])
{
	int size;

	printf("Enter the size of the array: ");
	if(scanf("%d", &size) != 1) exit(EXIT_FAILURE);

	int src[size];

	for(int i = 0; i < size ; ++i)
	{
		src[i] = rand() % size;
	}

	printf("\nBefore the Insertion sort, the sequence looks like:\n");
	show_sequence(src, size);

	printf("\nThe process of Insertion sort looks like: \n");
	insertion_sort(src, size);

	printf("\nAfter the Insertion sort, the sequence looks like:\n");
	show_sequence(src, size);

	return 0;
}
