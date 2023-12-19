/*****************************************************************************

    	writer: Matias Yoel Kaliroff Kravchik
    	reviwer: Tal
    	date: 06.03.23
    
******************************************************************************/

#include <stdlib.h> /* malloc realoc free */
#include <assert.h> /* assert */
#include "vector.h"

/******************************* typedefstruct *******************************/

struct vector
{
	size_t capacity;
	size_t size;
	void **element;
};

/******************************* DVectorCreate *******************************/

vector_t *DVectorCreate(size_t capacity)
{
	vector_t *vector = malloc(sizeof(vector_t));
	if(NULL == vector)
		return NULL;
	
	vector -> element = malloc(sizeof(void *) * capacity);
	if(NULL == vector -> element)
	{
		free(vector -> element);
		return NULL;
	}
	vector -> capacity = capacity;
	vector -> size = 0;

	return vector;
}

/******************************* DVectorDestroy ******************************/

void DVectorDestroy(vector_t *vector)
{
	assert(vector);
	free(vector -> element);
	free(vector);
	vector = NULL;
}

/******************************* DVectorCapacity *****************************/

size_t DVectorCapacity(const vector_t *vector)
{
	assert(vector);
	
	return vector -> capacity;
}

/******************************* DVectorSize *********************************/

size_t DVectorSize(const vector_t *vector)
{
	assert(vector);
	
	return vector -> size;
}

/******************************* DVectorIsEmpty ******************************/

int DVectorIsEmpty(const vector_t *vector)
{
	assert(vector);
	
	return (0 == vector -> size);
}

/******************************* DVectorPush *********************************/

int DVectorPushBack(vector_t *vector, void *new_element)
{
	assert(vector);
	
	if (vector -> size == vector -> capacity)
	{
		vector -> capacity *= 2;
		vector -> element = realloc(vector -> element,
						(sizeof(void *) * vector -> capacity));
						
		if(NULL == vector -> element)
		{
			free(vector -> element);
			return 1;		
		}
	}
		
	vector -> element[vector -> size] = new_element;
	++(vector -> size);
	
	return 0;
}

/******************************* DVectorPop **********************************/

int DVectorPopBack(vector_t *vector)
{
	assert(vector);
	
	--(vector -> size);

	if(0 == vector -> size)
		return 2;
	if (6 <= vector -> size && vector -> size <= (vector -> capacity / 4))
	{
		vector -> capacity /= 2;
		vector -> element = realloc(vector -> element,
						(sizeof(void *) * vector -> capacity));
						
		if(NULL == vector -> element)
		{
			free(vector -> element);
			return 1;
		}			
	}

	
	return 0;
}

/******************************* DVectorSafePop ******************************/

void DVectorSafePop(vector_t *vector)
{
	void **temp = vector -> element;
	
	vector -> size--;	
	assert(vector);	
	
	if(6 <= vector -> size && vector -> size <= (vector -> capacity / 4))
	{
		vector -> capacity /= 2;
		vector -> element = realloc(vector -> element,
							(sizeof(void *) * vector -> capacity));
		if(NULL == vector -> element)
			vector -> element = temp;	
	}

}

/******************************* DVectorSetElement ***************************/

void DVectorSetElement(vector_t *vector, size_t index, void *element)
{
	assert(vector);
	vector -> element[index] = element;
	if(vector -> size < index)
		vector -> size = index;
}
/******************************* DVectorGetElement ***************************/

void *DVectorGetElement(const vector_t *vector, size_t index)
{
	assert(vector);
	if(index <= vector -> size)
		return vector -> element[index];
		
	return NULL;
}

/******************************* DVectorReserve ******************************/

int DVectorReserve(vector_t *vector, size_t reserve)
{
	assert(vector);
	if(reserve <= vector -> capacity - vector -> size)
		return 2;
		
	vector -> capacity = vector -> size + reserve;
	vector -> element = realloc(vector -> element,
								(sizeof(void *) * vector -> capacity));
								
	if(NULL == vector -> element)
	{
		free(vector -> element);
		return 1;
	}
	return 0;
}

/******************************* DVectorShrinkToFit **************************/

int DVectorShrinkToFit(vector_t *vector)
{
	assert(vector);
	if(0 == vector -> size)
		return 2;
	vector -> capacity = vector -> size;	
	vector -> element = realloc(vector -> element,
							sizeof(void *) * vector -> size);
							
	if(NULL == vector -> element)
	{
		free(vector -> element);
		return 1;
	}
	return 0;
}

/*****************************************************************************/
