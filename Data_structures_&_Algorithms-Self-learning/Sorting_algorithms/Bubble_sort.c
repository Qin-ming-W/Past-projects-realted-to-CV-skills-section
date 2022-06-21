/*
	Implementation of Bubble sort

	Bubble sort is a popular, but inefficient, sorting algorithm. It works by repeatedly swapping
	adjacent elements that are out of order.

----------------------------------------------------------------------------------------------------

	The psuedocode of Bubble sort

	  1 2 3 4 5 6 •••• A.length
	 ––––––––––––––––––––
  A  | | | | | | |    | |
	 ––––––––––––––––––––

	 for i = 1 to A.length - 1

	 	for j = A.length downto i + 1
	 		if A[j] < A[j - 1]
	 			exchange A[j] with A[j - 1]
	 		j = j - 1 		// Go to the next one

	 	i = i + 1			// Go to the next one

----------------------------------------------------------------------------------------------------
	
	Inner loop invariant of Bubble sort
	
	
	Initialization (loop invariant is true before the first iteration of the loop):



	Maintenance (loop invariant is true before an iteration of the loop, it remains true before the
	next iteration):



	Termination (when the loop terminates, the invariant gives us a useful property that helps show
	that the algorithm is correct):



----------------------------------------------------------------------------------------------------

	Outer loop invariant of Bubble sort by using the termination condition of the inner loop 
	invariant:


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


void bubble_sort(int src[], int size)
{
	for(int i = 0 ; i < size - 1 ; ++i)
	{
		for(int j = size - 1 ; j >= i + 1 ; --j)
		{
			if(src[j] < src[j - 1])
			{
				int temp = src[j - 1];
				src[j - 1] = src[j];
				src[j] = temp;
			}
		}
		show_sequence(src, size);
	}
}


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

	printf("\nBefore the Bubble sort, the sequence looks like:\n");
	show_sequence(src, size);

	printf("\nThe process of Bubble sort looks like:\n");
	bubble_sort(src, size);

	printf("\nAfter the Bubble sort, the sequence looks like:\n");
	show_sequence(src, size);

	return 0;
}
