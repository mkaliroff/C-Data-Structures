#include <assert.h> /* assert */
#include <stdlib.h> /* malloc free */

#include "queue.h"
#include "sll.h"

struct queue
{
	sll_t *sll;
};


/*****************************************************************************/

queue_t *QueueCreate(void)
{
	queue_t *queue = (queue_t*)malloc(sizeof(queue_t));
	if(NULL == queue)
		return NULL;
		
	queue -> sll = SLLCreate();
	if(NULL == queue -> sll)
	{
		free(queue);
		return NULL;
	}	
	return queue;
}

/*****************************************************************************/

void QueueDestroy(queue_t *queue)
{
	assert(queue);
	
	SLLDestroy(queue -> sll);
	queue -> sll = NULL;
	
	free(queue);
}

/*****************************************************************************/

int QueueEnqueue(queue_t *queue, void *data)
{
	assert(queue);
	return !(SLLInsert(SLLEnd(queue -> sll), data));
}

/*****************************************************************************/

void QueueDequeue(queue_t *queue)
{
	assert(queue);
	SLLRemove(SLLBegin(queue -> sll));
}

/*****************************************************************************/

void *QueuePeek(const queue_t *queue)
{
	assert(queue);
	return (void*)SLLGetData(SLLBegin(queue -> sll));
}

/*****************************************************************************/

int QueueIsEmpty(const queue_t *queue)
{
	assert(queue);
	return SLLIsEmpty(queue -> sll);
}

/*****************************************************************************/

size_t QueueSize(const queue_t *queue)
{
	assert(queue);
	return SLLCount(queue -> sll);
}

/*****************************************************************************/

void QueueAppend(queue_t *dest, queue_t *source)
{
	assert(dest || source);
	SLLAppend(dest -> sll, source -> sll);
	
}

/*****************************************************************************/


