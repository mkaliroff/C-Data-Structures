/*=============================================================================

                           Costumer requirements:

===============================================================================

*

===============================================================================

                           VSA functionalities:

===============================================================================

1.)

==============================================================================*/

#ifndef __VSA_H__
#define __VSA_H__

#include <stddef.h>

typedef struct vsa vsa_t;
/******************************************************************************


Function: VSAInit - Initialize the VSA.

Usage: vsa_t *vsa = VSAInit(memory_pool, suggested_size, block_size)

Description: The function will initialize a memory VSA(pool).

Arguments: memory_pool - The base pointer to the managed pool, 
           size - the number of bytes of the memory pool that the user allocated(including bytes for the overhead).

Return value: A pointer to the new VSA created.

Complexity: Time complexity: O(1), Space complexity: O(1) */

vsa_t *VSAInit(void *memory_pool, size_t pool_size);

/*****************************************************************************
Function: VSAAlloc - Allocates a block from the VSA to be used.

Usage: void *block = VSAAlloc(vsa)

Description: VSAAlloc function will allocate an aligned block.

Arguments: vsa - pointer to the VSA it self, block_size - a block size for allocation. 

Return value: Return the address of the block to the caller function, 
              if the VSA is full the function will returns NULL.

Complexity: Time complexity: O(n), Space complexity: O(1) */

void *VSAAlloc(vsa_t *vsa, size_t block_size);

/*****************************************************************************
Function: VSAFree - Free a block in the vsa.

Usage: VSAFree(block)

Description: The function will free(de allocate) a block from the VSA, if ptr is NULL then nothing happens.

Arguments: Pointer that is used as a key to the block to be freed.

Return value: No return value.

Complexity: Time complexity: O(1), Space complexity: O(1) */

void VSAFree(void *ptr);

/*****************************************************************************
Function: VSALargestChunkAvailable - Does defragmetation and returns the size of the largest chunk.

Usage: size_t largest = VSALargestChunkAvailable(vsa)

Description: The function will return the largest chunk available.

Arguments: Pointer to the VSA it self.

Return value: size_t - the size of the largest chunk.

Complexity: Time complexity: O(n), Space complexity: O(1) */

size_t VSALargestChunkAvailable(vsa_t *vsa);

/*****************************************************************************/


#endif /* __VSA_H__ */
