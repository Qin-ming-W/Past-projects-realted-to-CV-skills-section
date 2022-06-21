/*
	Implementation of Quick sort
*/

#include <stdio.h>

int partition(int A[], int p, int r)
{
	int pivot = A[r];
	int i = p - 1;

	for(int j = p ; j < r ; ++j)
	{
		if(A[j] <= pivot)
		{
			++i;
			int swap = A[i];
			A[i] = A[j];
			A[j] = swap;
		}
	}
	int swap = A[i + 1];
	A[i + 1] = A[r];
	A[r] = swap;
	return i + 1;
}


void quick_sort(int A[], int p, int r)
{
	if(p < r)
	{
		int q = partition(A, p, r);
		quick_sort(A, p, q - 1);
		quick_sort(A, q + 1, r);
	}
}

int main(void)
{
	int A[] = {2, 4, 1, 9, 8, 5, 7};
	int size_A = sizeof(A) / sizeof(A[0]);

	quick_sort(A, 0, size_A - 1);

	for(int i = 0 ; i < size_A ; ++i)
	{
		printf("%d ", A[i]);
	}
	printf("\n");
	return 0;
}
