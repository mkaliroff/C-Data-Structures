/****************************************************************************** 
	
	Coder: Matias Yoel Kaliroff Kravchik
	
	Tester: Maya Mazor
	
******************************************************************************/

#include <assert.h> /* assert */
#include <stddef.h> /* null, size_t */


#include "vsa.h"

#define WORD_SIZE ((size_t)sizeof(size_t))
#define ALIGN(x) (x + (WORD_SIZE - (x % WORD_SIZE)) * ((x % WORD_SIZE) != 0))
#define TRUE (1)
#define FALSE (0)

#ifndef NDEBUG
#define MAGIC (0x66A5FE81)
#endif

typedef struct header
{
	void *next;
	#ifndef NDEBUG
	size_t magic;
	#endif
} header_t;

struct vsa
{
	void* vsa_end;
} ;

void *PartialDefrag(vsa_t *vsa, size_t size);

vsa_t *VSAInit(void *memory_pool, size_t pool_size)
{
	size_t offset = 0;
	void *address = memory_pool; 
	vsa_t *vsa = NULL;
	header_t *header = NULL;
	if((size_t)memory_pool % WORD_SIZE)
	{
		offset = (WORD_SIZE - (size_t)memory_pool % WORD_SIZE) ; 
	}
	/* example: offset = 7 */
	address = (void*)((size_t)address + offset);
	/* example: example:address = 1001+ 7 = 1008 */
	vsa = (vsa_t *)address;
	/* example: vsa @ 1008 */
	
	pool_size -= pool_size % WORD_SIZE;
	vsa->vsa_end = (void*)((size_t)vsa + pool_size - offset);
	address = (void*)((size_t)address + (size_t)sizeof(vsa_t));
	/* example: address = 1008 + 8 = 1016 */
	header = (header_t *)address;
	/* example: header @ 1016 */
	header->next = vsa->vsa_end;
	
	#ifndef NDEBUG
	header->magic = MAGIC;
	#endif
	
	
	return vsa;
}

void *VSAAlloc(vsa_t *vsa, size_t block_size)
{
	void *address = NULL;
	header_t *next_free = NULL;
	header_t *header = NULL;
	assert(vsa);
	
	if(0 == block_size)
	{
		return NULL;
	}
	
	block_size = ALIGN(block_size);
	
	header = (header_t*)PartialDefrag(vsa, block_size);
	
	address = (void*)((size_t)header->next & ~1);

	if((size_t)header >= (size_t)vsa->vsa_end)
	{
		return NULL;
	}
	if((size_t)address - (size_t)header > block_size + 2 * \
									(size_t)sizeof(header_t))
	{
		next_free = (header_t*)((size_t)header + block_size + \
									(size_t)sizeof(header_t));
		next_free->next = header->next;
		#ifndef NDEBUG
		next_free->magic = MAGIC;
		#endif
		header->next = next_free;
		
	}
	
	header->next = (void*)((size_t)(header->next) | 1);
	
	return (void*)((size_t)header + (size_t)sizeof(header_t));
}

void VSAFree(void *ptr)
{
	header_t *header = NULL;
	if(NULL == ptr)
	{
		return;
	}
	header = (header_t*)((size_t)ptr - (size_t)sizeof(header_t));
	#ifndef NDEBUG
	if(MAGIC != header->magic)
	{
		return;
	}
	#endif
	if((size_t)header->next % 2)
	{
		header->next = (void*)((size_t)header->next & ~1);
	}
}

size_t VSALargestChunkAvailable(vsa_t *vsa)
{

	header_t *header = NULL;
	header_t *next = NULL;
	size_t max = 0;
	int header_is_allocated = FALSE;
	int next_is_allocated = FALSE;
	
	assert(vsa);
	
	header = (header_t*)((size_t)vsa + (size_t)sizeof(vsa_t));
	
	while((size_t)header < (size_t)vsa->vsa_end)
	{

		next = (header_t*) ((size_t)(header->next) & ~1);
		header_is_allocated = (size_t)header->next % 2;

		if((size_t)next < (size_t)vsa->vsa_end)
		{
			next_is_allocated = (size_t)next->next % 2;
		}
		else
		{
			next_is_allocated = TRUE;
		}
		/* if end of memory_pool next_is_allocated is treated as allocated*/
		
		if(FALSE == header_is_allocated && max < (size_t)next - (size_t)header)
		{
			max = (size_t)next - (size_t)header;
		}
		
		if(FALSE == header_is_allocated && FALSE == next_is_allocated)
		{
			if((size_t)next < (size_t)vsa->vsa_end)
			{
				header->next = next->next;
			}
		}
		else
		{
			header = next;
		}
	}

	if(max >= sizeof(header_t))
	{
		return max - sizeof(header_t);
	}
	return (0);
}

void *PartialDefrag(vsa_t *vsa, size_t size)
{
	header_t *header = NULL;
	header_t *next = NULL;
	size_t max = 0;
	int header_is_allocated = FALSE;
	int next_is_allocated = FALSE;
	
	assert(vsa);
	
	header = (header_t*)((size_t)vsa + (size_t)sizeof(vsa_t));
	
	while((size_t)header < (size_t)vsa->vsa_end)
	{

		next = (header_t*) ((size_t)(header->next) & ~1);
		header_is_allocated = (size_t)header->next % 2;

		if((size_t)next < (size_t)vsa->vsa_end)
		{
			next_is_allocated = (size_t)next->next % 2;
		}
		else
		{
			next_is_allocated = TRUE;
		}

		if(FALSE == header_is_allocated && max < (size_t)next - (size_t)header)
		{
			max = (size_t)next - (size_t)header;
			if(max - sizeof(header_t) >= size)
			{
				return (void*)header;
			}
		}
		
		
		if(FALSE == header_is_allocated && FALSE == next_is_allocated)
		{
			if((size_t)next < (size_t)vsa->vsa_end)
			{
				header->next = next->next;
			}
		}
		else
		{
			header = next;
		}

	}
	return (void*)header;
}


