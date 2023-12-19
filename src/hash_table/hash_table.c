#include <stdio.h>
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */

#include "hash_table.h"
#include "dll.h"

#define HASH_TABLE_TRUE (1)
#define HASH_TABLE_FALSE (0)
#define UNUSED(X)((void)X)
#define SUCCESS (0)
#define FAIL (1)

/*****************************************************************************/

struct hash_table
{
	size_t size;
	dll_t **buckets;
	hash_func_t hash_func;
	hash_cmp_func_t cmp;
} ;

/*****************************************************************************/

hash_table_t *HashTableCreate(size_t size, hash_func_t hash, hash_cmp_func_t cmp)
{
	size_t i = 0;
	hash_table_t *table = NULL;
	
	table = (hash_table_t*)malloc(sizeof(hash_table_t));
	if(NULL == table)
	{
		return NULL;
	}
	table->buckets = (dll_t**)malloc(size * sizeof(dll_t*));
	if(NULL == table->buckets)
	{
		free(table);
		return NULL;
	}
	for(i = 0; i < size; i++)
	{
		table->buckets[i] = DLLCreate();
		if(NULL == table->buckets[i])
		{
			for(--i ; i > 0 ; i--)
			{
				DLLDestroy(table->buckets[i]);
			}
			free(table->buckets);
			free(table);
			return NULL;
		}
	}
	table->size = size;
	table->hash_func = hash;
	table->cmp = cmp;
	
	return table;
}

/*****************************************************************************/

void HashTableDestroy(hash_table_t *table)
{
	size_t i = 0;
	assert(table);
	for(; i < table->size; i++)
	{
		DLLDestroy(table->buckets[i]);
	}
	free(table->buckets);
	free(table);
	table = NULL;
}

/*****************************************************************************/

int HashTableInsert(hash_table_t *table, void *data)
{
	size_t index = 0;
	dll_iter_t result = NULL;
	assert(table);

	index = table->hash_func(data) % table->size;
	result = DLLPushBack(table->buckets[index], data);
	return (HASH_TABLE_TRUE == DLLIterIsEqual(DLLEnd(table->buckets[index]),result));
}

/*****************************************************************************/

void HashTableRemove(hash_table_t *table, const void *data)
{
	size_t index = 0;
	void *find_data = NULL;
	assert(table);
	
	index = table->hash_func(data) % table->size;
	find_data = DLLFind(DLLBegin(table->buckets[index]), \
						DLLEnd(table->buckets[index]), \
									table->cmp, (void*)data);
	
	if(!DLLIterIsEqual(find_data, DLLEnd(table->buckets[index])))
	{
		DLLRemove(find_data);
	}
}

/*****************************************************************************/

void *HashTableFind(const hash_table_t *table, const void *data)
{
	size_t index = 0;
	void *find_data = NULL;
	assert(table);
	
	index = table->hash_func(data) % table->size;
	find_data = DLLFind(DLLBegin(table->buckets[index]), \
						DLLEnd(table->buckets[index]), \
									table->cmp, (void*)data);
	
	if(!DLLIterIsEqual(find_data, DLLEnd(table->buckets[index])))
	{
		return DLLGetData(find_data);
	}
	return NULL;
}

/*****************************************************************************/

int HashTableIsEmpty(const hash_table_t *table)
{
	size_t i = 0;
	int result = HASH_TABLE_TRUE;
	assert(table);
	for(; i < table->size ; i++)
	{
		result = DLLIsEmpty(table->buckets[i]);
		if(HASH_TABLE_FALSE == result)
		{
			return result;
		}
	
	}
	return result;
}

/*****************************************************************************/

size_t HashTableSize(const hash_table_t *table)
{
	size_t i = 0;
	size_t counter = 0;
	assert(table);
	for(; i < table->size ; i++)
	{
		counter += DLLCount(table->buckets[i]);
	}
	return counter;
}

/*****************************************************************************/

int HashForEach(hash_table_t *table, hash_action act, void *param)
{
	size_t i = 0;
	int result = 0;
	assert(table);
	for( ; 0 == result && i < table->size ; i++)
	{
		DLLForEach(DLLBegin(table->buckets[i]), DLLEnd(table->buckets[i]), \
																 act, param);
	}
	return result;
}

/*****************************************************************************/

double HashTableLoadFactor(const hash_table_t *table)
{
	assert(table);
	return 1;
}

/*****************************************************************************/

double HashTableSD(const hash_table_t *table)
{
	assert(table);
	return 1;
}

/*****************************************************************************/







