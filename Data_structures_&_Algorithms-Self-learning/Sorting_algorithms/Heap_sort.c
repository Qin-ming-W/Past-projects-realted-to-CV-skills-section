#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define CHECK_ALLOC(ptr) if(ptr == NULL) { perror(__func__); exit(EXIT_FAILURE); }

typedef struct 
{
	int heap_size;
	int *heap;
} HEAP;


void print_tree_shape_heap(HEAP *max_heap)
{
	;
}


void print_array_shape_heap(HEAP *max_heap)
{
	for(int i = 0 ; i < max_heap->heap_size ; ++i)
	{
		printf("%d ", max_heap->heap[i]);
	}
	printf("\n");
}


HEAP *build_max_heap(int *src, int size)
{
	if(src != NULL)
	{
		HEAP *max_heap = malloc(sizeof(HEAP));
		CHECK_ALLOC(max_heap);

		max_heap->heap = malloc(sizeof(int) * size);
		CHECK_ALLOC(max_heap->heap);
 
		max_heap->heap_size = size;

		for(int i = 0; i < size ; ++i)
		{
			max_heap->heap[i] = src[i];

			int child_index = i;

			while(child_index > 0)
			{
				int parent_index = (child_index - 1) / 2;

				if(max_heap->heap[child_index] > max_heap->heap[parent_index])
				{
					int swap = max_heap->heap[parent_index];
					max_heap->heap[parent_index] = max_heap->heap[child_index];
					max_heap->heap[child_index] = swap;
					child_index = parent_index;
				}
				else { break; }
			}
		}

		return max_heap;
	}

	return NULL;	
}


void max_heapify(HEAP *max_heap, int index)
{
	if(max_heap != NULL)
	{
		int left_child_index = 2 * index + 1;
		int right_child_index = 2 * index + 2;
		int parent_index = index;

		if(max_heap->heap[left_child_index] > max_heap->heap[parent_index]
			&& left_child_index < max_heap->heap_size)
		{
			parent_index = left_child_index;
		}

		if(max_heap->heap[right_child_index] > max_heap->heap[parent_index]
			&& right_child_index < max_heap->heap_size)
		{
			parent_index = right_child_index;
		}

		if(parent_index != index)
		{
			int swap = max_heap->heap[index];
			max_heap->heap[index] = max_heap->heap[parent_index];
			max_heap->heap[parent_index] = swap;
			max_heapify(max_heap, parent_index);
		}
	}
}


HEAP *build_max_heap_recursively(int *src, int size)
{
	if(src != NULL)
	{
		HEAP *max_heap = malloc(sizeof(HEAP));
		CHECK_ALLOC(max_heap);

		max_heap->heap = malloc(sizeof(int) * size);
		CHECK_ALLOC(max_heap->heap);

		max_heap->heap_size = size;

		for(int i = 0 ; i < size ; ++i)
		{
			max_heap->heap[i] = src[i];
		}

		int last_parent_index = (size - 1 - 1) / 2;

		for(int i = last_parent_index ; i >= 0 ; --i)
		{
			max_heapify(max_heap, i);
		}

		return max_heap;
	}

	return NULL;
}


HEAP *insert_ndoe_to_max_heap(HEAP *max_heap)
{
	if(max_heap != NULL)
	{
		int value;

		printf("Enter an integer to be added: ");
		if(scanf("%d", &value) != 1)
		{
			exit(EXIT_FAILURE);
		}

		++max_heap->heap_size;

		max_heap->heap = realloc(max_heap->heap, sizeof(int) * max_heap->heap_size);
		CHECK_ALLOC(max_heap->heap);

		max_heap->heap[max_heap->heap_size - 1] = value;

		int child_index = max_heap->heap_size - 1;

		while(child_index > 0)
		{
			int parent_index = (child_index - 1) / 2;

			if(max_heap->heap[child_index] > max_heap->heap[parent_index])
			{
				int swap = max_heap->heap[parent_index];
				max_heap->heap[parent_index] = max_heap->heap[child_index];
				max_heap->heap[child_index] = swap;
				child_index = parent_index;
			}
			else { break; }
		}

		return max_heap;
	}

	return NULL;
}


int extract_node_from_max_heap(HEAP *max_heap)
{
	if(max_heap == NULL)
	{
		exit(EXIT_FAILURE);
	}

	int root_value = max_heap->heap[0];

	max_heap->heap[0] = max_heap->heap[max_heap->heap_size - 1];
	--max_heap->heap_size;

	max_heap->heap = realloc(max_heap->heap, sizeof(int) * max_heap->heap_size);
	CHECK_ALLOC(max_heap->heap);

	max_heapify(max_heap, 0);

	return root_value;
}


int push_then_pop_on_max_heap(HEAP *max_heap)
{
	int value;

	printf("Enter an integer value: ");
	if(scanf("%d", &value) != 1)
	{
		exit(EXIT_FAILURE);
	}

	if(max_heap == NULL) { return value; }

	if(max_heap->heap[0] < value) { return value; }

	int root_value = max_heap->heap[0];
	max_heap->heap[0] = value;
	max_heapify(max_heap, 0);

	return root_value;
}


int search_max_heap(HEAP *max_heap, int value)
{
	if(max_heap != NULL)
	{
		for(int i = 0 ; i < max_heap->heap_size ; ++i)
		{
			if(max_heap->heap[i] == value) return i;
		}

		return -1;
	}

	return -1;
}


HEAP *pop_node_from_max_heap(HEAP *max_heap)
{
	if(max_heap != NULL)
	{
		int value;

		printf("Enter the value to be deleted: ");
		if(scanf("%d", &value) != 1) { exit(EXIT_FAILURE); }

		int index = search_max_heap(max_heap, value);

		if(index != -1)
		{
			int  popped_value = max_heap->heap[index];
			max_heap->heap[index] = max_heap->heap[max_heap->heap_size - 1];

			--max_heap->heap_size;
			max_heap->heap = realloc(max_heap->heap, sizeof(int) * max_heap->heap_size);
			CHECK_ALLOC(max_heap->heap);

			if(max_heap->heap[index] < popped_value)
			{
				max_heapify(max_heap, index); // down heap to restore heap prpoperty
			}

			if(max_heap->heap[index] > popped_value)
			{
				// up heap to restore the heap property
				int child_index = index;

				while(child_index > 0)
				{
					int parent_index = (child_index - 1) / 2;

					if(max_heap->heap[child_index] > max_heap->heap[parent_index])
					{
						int swap = max_heap->heap[parent_index];
						max_heap->heap[parent_index] = max_heap->heap[child_index];
						max_heap->heap[child_index] = swap;
						child_index = parent_index;
					}
					else { break; }
 				}
			}
		}

		return max_heap;
	}

	return NULL;
}


HEAP *decrease_key_on_max_heap(HEAP *max_heap)
{
	if(max_heap != NULL)
	{
		int value;

		printf("Enter the value to be decreased: ");
		if(scanf("%d", &value) != 1) { exit(EXIT_FAILURE); }

		int index = search_max_heap(max_heap, value);

		if(index != -1)
		{
			--max_heap->heap[index];
			max_heapify(max_heap, index);
		}

		return max_heap;
	}

	return NULL;
}


HEAP *increase_key_on_max_heap(HEAP *max_heap)
{
	if(max_heap != NULL)
	{
		int value;

		printf("Enter the value to be decreased: ");
		if(scanf("%d", &value) != 1) { exit(EXIT_FAILURE); }

		int index = search_max_heap(max_heap, value);

		if(index != -1)
		{
			++max_heap->heap[index];

			int child_index = index;

			while(child_index > 0)
			{
				int parent_index = (child_index - 1) / 2;

				if(max_heap->heap[child_index] > max_heap->heap[parent_index])
				{
					int swap = max_heap->heap[parent_index];
					max_heap->heap[parent_index] = max_heap->heap[child_index];
					max_heap->heap[child_index] = swap;
					child_index = parent_index;
				}
				else { break; }
			}
		}
	}

	return NULL;
}


HEAP *heap_sort(int *src, int size)
{
	if(src != NULL)
	{
		HEAP *max_heap = build_max_heap(src, size);

		for(int i = max_heap->heap_size - 1 ; i > 0 ; --i)
		{
			int swap = max_heap->heap[i];
			max_heap->heap[i] = max_heap->heap[0];
			max_heap->heap[0] = swap;
			--max_heap->heap_size;

			max_heapify(max_heap, 0);
		}

		return max_heap;
	}

	return NULL;
}


int main(void)
{
	int size; 

	printf("Enter the size of source array: ");
	if(scanf("%d", &size) != 1)
	{
		exit(EXIT_FAILURE);
	}

	int *src = malloc(sizeof(int) * size);
	CHECK_ALLOC(src)

	printf("Enter %d integers:\n", size);
	for(int i = 0 ; i < size ; ++i)
	{
		if(scanf("%d", &src[i]) != 1)
		{
			exit(EXIT_FAILURE);
		}
	}

	HEAP *heap = heap_sort(src, size);
	
	for(int i = 0; i < size ; ++i)
	{
		printf("%d ", heap->heap[i]);
	}
	printf("\n");

	return 0;
}
