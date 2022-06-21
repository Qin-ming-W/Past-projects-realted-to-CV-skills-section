/*
	Implementation of Merge sort

	We are solving problems recursively !

	 1 2 3 4 5 6 •••• A.length
	 ––––––––––––––––––––
  A  | | | | | | |    | |
	 ––––––––––––––––––––
	  p                r

---------------------------------------------------------

	 The psuedocode for 'merge_sort' goes here:

	 if p < r 						\
	 	q = (p + r) / 2
	 	merge_sort(p, q)
	 	merge_sort(q + 1, r)
	 	merge(A, p, q, r)

---------------------------------------------------------

	 The psuedocode of helper function 'merge' called 
	 by 'merge_sort' goes here:

	 n1 = q - p + 1
	 n2 = r - q
	 
	 // let L[1 .. n1 + 1] and R[1 .. n2] be new arrays
	
	 for i = 1 to n1
	 	L[i] = A[p + i - 1]
	 for j = 1 to n2
	 	R[j] = A[q + j]

	 L[n1 + 1] = indicator (∞)
	 R[n2 + 1] = indicator (∞)

	 i = 1
	 j = 1

	 for k = p to r
	 	if L[i] <= R[j]
	 		A[K] = L[i]
	 		i = i + 1
	 	else
	 		A[k] = R[j]
	 		j = j + 1
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


void merge(int src[], int p, int q, int r)
{
	// Computes the length n1 of the sub-array A[p .. q]
	int n1 = q - p + 1;	

	// Computes the length n2 of the sub-array A[q + 1 .. r]
	int n2 = r - q;

	int left[n1];
	int right[n2];

	// Copies the sub-array A[p .. q] into left[0 .. n1 - 1]
	for(int i = 0 ; i < n1 ; ++i)
	{
		left[i] = src[p + i];
	}

	// Copies the sub-array A[q + 1 .. r] into right[0 .. n2 - 1]
	for(int j = 0 ; j < n2 ; ++j)
	{
		right[j] = src[q + j + 1] ;
	}

	// left[i] and right[i] are always the smallest elements of their arrays 
	// that have not been copied back to src
	int i = 0, j = 0;

/*
	// Copies elements in sub-arrays left[0 .. n1 - 1] and right[0 .. n2 - 1] into array src[p .. r]
	// , in sorted order.
	for(int k = p ; k <= r ; ++k)
	{
		if(i == n1) src[k] = right[j++];

		else if(j == n2 || left[i] < right[j]) src[k] = left[i++];

		else src[k] = right[j++];
	}
*/
	int k = p;
	while(i != n1 && j != n2)
	{
		if(left[i] <= right[j])
		{
			src[k++] = left[i++];
		} 
		else src[k++] = right[j++];
	}

	if(i == n1)
	{
		while(j < n2) src[k++] = right[j++];
	}

	else
	{
		while(i < n1) src[k++] = left[i++];
	}
}


void merge_sort(int src[], int p, int r)
{
	if(p < r)
	{
		int q = (p + r) / 2;
		merge_sort(src, p, q);
		merge_sort(src, q + 1, r);
		merge(src, p, q, r);
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

	printf("\nBefore the Merge sort, the sequence looks like:\n");

	show_sequence(src, size); 

	merge_sort(src, 0, size -1);	// Merge sort the entire array

	printf("\nAfter the Merge sort, the sequence looks like:\n");
	show_sequence(src, size);

	return 0;
}


