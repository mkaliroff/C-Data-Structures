/*****************************************************************************

    	writer: Matias Yoel Kaliroff Kravchik
    	reviwer: Art
    	date: 15.05.2023
    
******************************************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "heap.h"
#include "vector.h"

#define STARTING_CAPACITY (100)
#define HEAP_FAIL (1)
#define HEAP_SUCCESS (0)
#define HEAP_TRUE (1)

#define LEFT_CHILD(X)((X + 1) * 2 - 1)
#define RIGHT_CHILD(X)((X + 1) * 2)
#define PARENT(X)((X - 1) / 2)
#define CMP_RESULT(X,Y)(heap->cmp(DVectorGetElement(heap->vector, X), \
								DVectorGetElement(heap->vector, Y)))


/*****************************************************************************/

struct heap
{
	vector_t *vector;
	heap_compare_func_t cmp;
};

/*****************************************************************************/

static void HeapifyUp(heap_t *heap, size_t index);
static void HeapifyDown(heap_t *heap, size_t index);
static void SwapElement(vector_t *vector, size_t x, size_t y);
static size_t FindElement(heap_t *heap, void *data, heap_ismatch_func_t is_match);

/*****************************************************************************/

heap_t *HeapCreate(heap_compare_func_t cmp)
{
	heap_t *heap = malloc(sizeof(heap_t));
	if(NULL == heap)
	{
		return NULL;
	}
	heap->vector = DVectorCreate((size_t)STARTING_CAPACITY);
	if(NULL == heap->vector)
	{
		free(heap);
		return NULL;
	}
	heap->cmp = cmp;
	
	return heap;
}

/*****************************************************************************/

void HeapDestroy(heap_t *heap)
{
	assert(heap);
	DVectorDestroy(heap->vector);
	free(heap);
}

/*****************************************************************************/

int HeapPush(heap_t *heap, void *data)
{
	assert(heap);
	
	if(HEAP_SUCCESS != DVectorPushBack(heap->vector, data))
	{
		return HEAP_FAIL;
	}
	
	if(1 != HeapSize(heap))
	{
		HeapifyUp(heap, HeapSize(heap) - 1);
	}
	
	return HEAP_SUCCESS;
}

/*****************************************************************************/

void *HeapPop(heap_t *heap)
{
	size_t last_element = 0;
	void *data = NULL;
	assert(heap);
	last_element = DVectorSize(heap->vector) - 1;
	data = DVectorGetElement(heap->vector, 0);
	if(0 == last_element)
	{
		DVectorSafePop(heap->vector);
	}
	else
	{
		SwapElement(heap->vector, 0, last_element);
		DVectorSafePop(heap->vector);
		HeapifyDown(heap, 0);
	}
	return data;
}

/*****************************************************************************/

void *HeapPeek(const heap_t *heap)
{
	assert(heap);
	if(0 == HeapSize(heap))
	{
		return NULL;
	}
	return DVectorGetElement(heap->vector, 0);
}

/*****************************************************************************/

size_t HeapSize(const heap_t *heap)
{
	assert(heap);
	return DVectorSize(heap->vector);
}

/*****************************************************************************/

int HeapIsEmpty(const heap_t *heap)
{
	assert(heap);
	return DVectorIsEmpty(heap->vector);
}

/*****************************************************************************/

void *HeapRemove(heap_t *heap, const void *data, heap_ismatch_func_t is_match)
{

	void *result = NULL;
	size_t index = 0;
	assert(heap);
	index = FindElement(heap, (void*)data, is_match);
	if(HeapSize(heap) == index)
	{
		return NULL;
	}
	
	result = DVectorGetElement(heap->vector, index);
	
	if(1 == HeapSize(heap))
	{
		HeapPop(heap);
		return result;
	}
	
	SwapElement(heap->vector, HeapSize(heap) - 1, index);
	DVectorSafePop(heap->vector);
	

	if(0 < CMP_RESULT(index, PARENT(index)))
	{
		HeapifyUp(heap, index);
	}
	else
	{
		HeapifyDown(heap, index);
	}
	return result;
}

/*****************************************************************************/

static void HeapifyUp(heap_t *heap, size_t index)
{
	size_t parent = PARENT(index);

	if(0 == index)
	{
		return;
	}
	if(0 >= CMP_RESULT(index, parent))
	{
		SwapElement(heap->vector, index, parent);
		HeapifyUp(heap, parent);
	}
}

/*****************************************************************************/

static void HeapifyDown(heap_t *heap, size_t index)
{
	size_t left_child = LEFT_CHILD(index);
	size_t right_child = RIGHT_CHILD(index);
	size_t target_child = 0;

	if(left_child >= HeapSize(heap) -1)
	{
		return;
	}
	if(right_child <= HeapSize(heap) -1)
	{
		if(0 <= CMP_RESULT(left_child, right_child))
		{
			target_child = right_child;
		}
		else
		{
			target_child = left_child;
		}
	}
	else
	{
		target_child = left_child;
	}
	SwapElement(heap->vector, index, target_child);
	HeapifyDown(heap, target_child);
}

/*****************************************************************************/

static void SwapElement(vector_t *vector, size_t x, size_t y)
{
	void *temp = DVectorGetElement(vector, x);
	DVectorSetElement(vector, x, DVectorGetElement(vector, y));
	DVectorSetElement(vector, y, temp);
}

/*****************************************************************************/

static size_t FindElement(heap_t *heap, void *data, heap_ismatch_func_t is_match)
{
	size_t i = 0;
	for(;  i < HeapSize(heap) ; i++)
	{
		if(HEAP_TRUE == is_match(DVectorGetElement(heap->vector, i), data))
		{
			return i;
		}
	}
	return i;
}

