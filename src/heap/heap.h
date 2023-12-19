

#include <stddef.h>/* size_t */

#ifndef __HEAP_H__
#define __HEAP_H__

/* if returns positive then comes after data*/
typedef int (*heap_compare_func_t) (const void *data, const void *new_data);
typedef int (*heap_ismatch_func_t) (const void *data, const void *new_data);
typedef struct heap heap_t;

/* Time Complexity: O(1)*/
heap_t *HeapCreate(heap_compare_func_t cmp);

/* Time Complexity: O(1)*/
void HeapDestroy(heap_t *heap);

/* Time Complexity: O(log n)*/
int HeapPush(heap_t *heap, void *data);

/* Time Complexity: O(log n)*/
void *HeapPop(heap_t *heap);

/* Time Complexity: O(1)*/
void *HeapPeek(const heap_t *heap);

/* Time Complexity: O(1)*/
size_t HeapSize(const heap_t *heap);

/* Time Complexity: O(1)*/
int HeapIsEmpty(const heap_t *heap);


/* Time Complexity: O(n)*/
void *HeapRemove(heap_t *heap, const void *data, heap_ismatch_func_t ismatch);




#endif /* __HEAP_H__ */
