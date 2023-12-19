/*=============================================================================

                           Customer requirements:

===============================================================================



=============================================================================*/

#ifndef __HASH_H__
#define __HASH_H__

#include <stddef.h> /* size_t */

typedef struct hash_table hash_table_t;

typedef size_t (*hash_func_t)(const void *key);
typedef int (*hash_action)(void *data, void *param);
typedef int (*hash_cmp_func_t)(void *data, void *param);

/*****************************************************************************/

/*	T.C : O(n)
	S.C : O(n)
*/

hash_table_t *HashTableCreate(size_t size, hash_func_t hash, hash_cmp_func_t cmp);

/*****************************************************************************/

/*	T.C : O(n)
	S.C : O(1)
*/

void HashTableDestroy(hash_table_t *table);

/*****************************************************************************/

/*	T.C : O(1)
	S.C : O(1)
*/

int HashTableInsert(hash_table_t *table, void *data);

/*****************************************************************************/

/*	T.C : average O(1), worst O(n)
	S.C : O(1)
*/

void HashTableRemove(hash_table_t *table, const void *data);

/*****************************************************************************/

/*	T.C : O(1)
	S.C : O(1)
*/

void *HashTableFind(const hash_table_t *table, const void *data);

/*****************************************************************************/

/*	T.C : O(n)
	S.C : O(1)
*/

int HashTableIsEmpty(const hash_table_t *table);

/*****************************************************************************/

/*	T.C : O(n)
	S.C : O(1)
*/

size_t HashTableSize(const hash_table_t *table);

/*****************************************************************************/

/*	T.C : O(n)
	S.C : O(1)
*/

int HashForEach(hash_table_t *table, hash_action act, void *param);

/*****************************************************************************/

/*	T.C : O(TBA)
	S.C : O(1)
*/

double HashTableLoadFactor(const hash_table_t *table);

/*****************************************************************************/

/*	T.C : O(TBA)
	S.C : O(1)
*/

double HashTableSD(const hash_table_t *table);

/*****************************************************************************/

#endif /* __HASH_H__ */
