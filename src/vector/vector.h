/*****************************************************************************

    	writer: Matias Yoel Kaliroff Kravchik
    	reviwer: Tal
    	date: 06.03.23
    
******************************************************************************/

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stddef.h>

typedef struct vector vector_t;

/* O(1) - memory allocation by capacity */
vector_t *DVectorCreate(size_t capacity);

/* O(1) - Clean */
void DVectorDestroy(vector_t *vector);

/* O(1) - returns capacity */
size_t DVectorCapacity(const vector_t *vector);

/* O(1) - returns size */
size_t DVectorSize(const vector_t *vector);

/* O(1) - TRUE if size equals to 0 */
int DVectorIsEmpty(const vector_t *vector);

/* O(n)? - push element to vector, increase capacity if needed, return 0 on success*/
int DVectorPushBack(vector_t *vector, void *element);

/* O(n)? - pop last element , decrease capacity if needed, return 0 on success */
int DVectorPopBack(vector_t *vector);

/* O(n)? - pop last element */
void DVectorSafePop(vector_t *vector);

/* O(1) - set element in index*/
void DVectorSetElement(vector_t *vector, size_t index, void *element);

/* O(1) - get indexed element*/
void *DVectorGetElement(const vector_t *vector, size_t index);

/* O(n)? - may increase capacity to held reserve, return 0 on success*/
int DVectorReserve(vector_t *vector, size_t reserve);

/* O(1)? - decrease capacity, returns 0 on success*/
int DVectorShrinkToFit(vector_t *vector);

/******************************************************************************/

#endif /*__VECTOR_H__*/

