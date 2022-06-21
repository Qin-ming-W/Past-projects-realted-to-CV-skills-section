/*
	Implementation of Selection sort

----------------------------------------------------------------------------------------------------

	The psuedocode for Selection sort

	  1 2 3 4 5 6 •••• A.length
	 ––––––––––––––––––––
  A  | | | | | | |    | |
	 ––––––––––––––––––––

	for i = 1 to A.length - 1
		k = i

		for j = i + 1 to A.length 
			if A[j + 1] < A[k]
				k = j + 1
			j = j + 1		// go to the next one

		temp = A[i]
		A[i] = A[k]
		A[k] = temp

		i = i + 1			// go to the next one

----------------------------------------------------------------------------------------------------

	Outer loop invariant of Insertion sort:

	At the start of each iteration of the outer for loop, the sub-array A[1 .. i - 1] consists of 
	the fist i - 1 smallest values, in sorted order, among all values in the (original) array A[1 ..
	n].

	Initialization (loop invariant is true before the first iteration of the loop):

	Loop invariant holds before the first iteration of the outer for loop, when i = 1. The sub-array
	A[1 .. i - 1] is A[1 .. 0], therefore,  it is empty. According to the wording of loop invariant,
	the empty sub-array consists of the first 0 smallest values, in sorted order, among all values 
	in the (original) array A[1 .. n]. Hence, the loop invariant holds prior to the first for loop 
	iteration. 
	
	Maintenance (loop invariant is true before an iteration of the loop, it remains true before the
	next iteration):





	Termination (when the loop terminates, the invariant gives us a useful property that helps show
	that the algorithm is correct):

	By the time of the termination of outer for loop, i has the value n. Substituting i = n to the 
	wording of loop invariant, we have the the sub-array A[1 .. n - 1] consists of the first n - 1
	smallest values, in sorted order, among all values in the (original) array A[1 .. n]. Observing 
	the fact that the last value A[n] in the array A[1 .. n] is now guaranteed to be the largest 
	value in the original array A[1 .. N]. Hence, we con conclude that the selection sort algorithm 
	is correct.
	
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


void selection_sort(int src[], int size)
{
	for(int i = 0 ; i < size - 1 ; ++i)
	{
		int k = i;

		for(int j = i + 1 ; j < size ; ++j)
		{
			if(src[j] < src[k]) k = j;
		}

		int temp = src[i];
		src[i] = src[k];
		src[k] = temp;

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

	printf("\nBefore the Selection sort, the sequence looks like:\n");
	show_sequence(src, size);

	printf("\nThe process of Selection sort looks like: \n");
	selection_sort(src, size);

	printf("\nAfter the Selection sort, the sequence looks like:\n");
	show_sequence(src, size);

	return 0;
}
