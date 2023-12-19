#include <assert.h> /* assert */
#include <stddef.h> /* null, size_t */
#include <stdio.h>

#include "fsa.h"

#define WORD_SIZE ((size_t)sizeof(size_t))
#define ALIGN(x) (x + (WORD_SIZE - (x % WORD_SIZE)) * ((x % WORD_SIZE) != 0))
typedef struct node
{
	struct node *next;
} node_t;

struct fsa
{
	node_t *head;
}fsa;

void *FSAAlloc(fsa_t *fsa)
{
	void *allocated_block = NULL;
	assert(fsa);
	if(NULL == fsa->head)
	{
		return NULL;
	}
	allocated_block = fsa->head;
	fsa->head = fsa->head->next;
	return allocated_block;
}

void FSAFree(fsa_t *fsa, void *block)
{
	assert(fsa);
	if(NULL == block)
	{
		return;
	}
	((node_t*)block)->next = fsa->head;
	fsa->head = (node_t*)block;
	
}

size_t FSACountFree(const fsa_t *fsa)
{
	node_t *current = NULL;
	size_t counter = 0;
	assert(fsa);
	current = fsa->head;
	for(; current; current = current->next)
	{
		counter++;
	}
	return counter;
}

size_t FSASuggestSize(size_t number_of_blocks, size_t block_size)
{
	return ALIGN(block_size) * number_of_blocks + sizeof(fsa_t) + WORD_SIZE - 1;
}

fsa_t *FSAInit(void *memory_pool, size_t suggested_size, size_t block_size)
{
	fsa_t *fsa = NULL;
	node_t *node = NULL;
	node_t *node_prev = NULL;
	size_t address = 0;
	size_t i = 1;
	size_t num_blocks = 0;
	assert(memory_pool);

	block_size = ALIGN(block_size);
	
	num_blocks = (suggested_size - 2 * WORD_SIZE + 1) / block_size;
	if((size_t)memory_pool % WORD_SIZE)
	{
		address = (WORD_SIZE - (size_t)memory_pool % WORD_SIZE) ; 
	}
	
	address = (size_t)(memory_pool) + address;
	fsa = (fsa_t*)address;
	address += sizeof(fsa_t);
	node = (node_t*)address;
	node->next = NULL;
	for(; i < num_blocks; i++)
	{
		node_prev = node;
		address = (size_t)node + block_size;
		node = (node_t*)address;
		node->next = node_prev;
	}
	fsa->head = node;
	
	return fsa;
}


