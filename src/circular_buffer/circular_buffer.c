/*****************************************************************************

    	writer: Matias Yoel Kaliroff Kravchik
    	reviwer: Art
    	date: 14.03.23
    
******************************************************************************/

#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */

#include "circular_buffer.h"

/*****************************************************************************/

struct cbuff
{
    size_t start;
    size_t size;
    size_t bufsiz;
    char buffer[1];
};

/*****************************************************************************/

cbuff_t *CircularBufferCreate(size_t capacity)
{
	cbuff_t *buffer = (cbuff_t*)malloc(3 * sizeof(size_t) + capacity);
	if(NULL == buffer)
		return NULL;
	buffer -> bufsiz = capacity;
	buffer -> start = 0;
	buffer -> size = 0;
	
	return buffer;
}

/*****************************************************************************/

void CircularBufferDestroy(cbuff_t *buffer)
{
	assert(buffer);
	free(buffer);
	buffer = NULL;
}

/*****************************************************************************/

ssize_t CircularBufferWrite(cbuff_t *buffer, const void *src, size_t amount)
{
	size_t counter = 0;
	size_t freespace = buffer -> bufsiz - buffer -> size;
	size_t end = (buffer -> start + buffer -> size) % buffer -> bufsiz;
	const char *src_ptr = src;

	assert(buffer);
	
	while(counter < amount && 0 < freespace)
	{
		buffer -> buffer[end] = *src_ptr;
		src_ptr++;
		end = (end + 1) % buffer -> bufsiz;
		counter++;
		freespace --;
		buffer -> size++;		
	}
	return counter;
}

/*****************************************************************************/

ssize_t CircularBufferRead(cbuff_t *buffer, void *dest, size_t amount)
{
	size_t counter = 0;
	char *dest_ptr = (char*)dest;

	assert(buffer);
	
	while(counter < amount && 0 < buffer -> size)
	{
		*dest_ptr = buffer -> buffer[buffer -> start];
		dest_ptr++;
		buffer -> start = (buffer -> start + 1) % buffer -> bufsiz;
		counter++;
		buffer -> size--;
	}
	
	return counter;
}

/*****************************************************************************/

int CircularBufferIsEmpty(const cbuff_t *buffer)
{
	assert(buffer);
	return(0 == buffer -> size);
}

/*****************************************************************************/

size_t CircularBufferFreeSpace(const cbuff_t *buffer)
{
	assert(buffer);
	return buffer -> bufsiz - buffer -> size;
}

/*****************************************************************************/

size_t CircularBufferBufsiz(const cbuff_t *buffer)
{
	assert(buffer);
	return buffer -> bufsiz;
}

/*****************************************************************************/
