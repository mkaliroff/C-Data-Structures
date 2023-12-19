/*****************************************************************************

    	writer: Matias Yoel Kaliroff Kravchik
    	reviwer: Art
    	date: 16.05.2023
    
******************************************************************************/

#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert.h */

#include "heap_pq.h"
#include "heap.h"
#include "vector.h"

#define FALSE (0)

struct priority_queue
{
	heap_t *heap;
};

/*****************************************************************************/

priority_queue_t *PriorityQueueCreate(priority_queue_compare_func_t compare)
{
	priority_queue_t *pqueue = malloc(sizeof(priority_queue_t));
	if(NULL == pqueue)
	{
		return NULL;
	}
	pqueue->heap = HeapCreate(compare);
	if(NULL == pqueue->heap)
	{
		free(pqueue);
		return NULL;
	}

	return pqueue;
}

/*****************************************************************************/

void PriorityQueueDestroy(priority_queue_t *pqueue)
{
	assert(pqueue);
	HeapDestroy(pqueue->heap);
	free(pqueue);
	pqueue = NULL;
}

/*****************************************************************************/

int PriorityQueueEnqueue(priority_queue_t *pqueue, void *data)
{
	assert(pqueue);
	return HeapPush(pqueue->heap, data);
}

/*****************************************************************************/

void *PriorityQueueDequeue(priority_queue_t *pqueue)
{
	assert(pqueue);
	return HeapPop(pqueue->heap);
}

/*****************************************************************************/

void *PriorityQueuePeek(const priority_queue_t *pqueue)
{
	assert(pqueue);
	return HeapPeek(pqueue->heap);
}

/*****************************************************************************/

int PriorityQueueIsEmpty(const priority_queue_t *pqueue)
{
	assert(pqueue);
	return HeapIsEmpty(pqueue->heap);
}

/*****************************************************************************/

size_t PriorityQueueSize(const priority_queue_t *pqueue)
{
	assert(pqueue);
	return HeapSize(pqueue->heap);
}
/*****************************************************************************/

void *PriorityQueueErase(priority_queue_t *pqueue, priority_queue_ismatch_func_t ismatch, void *parameter)
{
	void *data = NULL;
	assert(pqueue);
	data = HeapRemove(pqueue->heap, parameter, ismatch);
	if(NULL == data)
	{
		return pqueue;
	}
	return data;
}

/*****************************************************************************/

void PriorityQueueClear(priority_queue_t *pqueue)
{
	assert(pqueue);
	while(FALSE == HeapIsEmpty(pqueue->heap))
	{
		HeapPop(pqueue->heap);
	}
}

/*****************************************************************************/


