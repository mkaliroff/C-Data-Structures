/*****************************************************************************

    	writer: Matias Yoel Kaliroff Kravchik
    	reviwer: Art
    	date: 02.03.23
    
******************************************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "stack.h" /* api */

/******************************* typedefstruct *******************************/

struct stack
{
	size_t capacity;
	size_t top;
	void **elements;
};

/******************************* StackCreate *********************************/

stack_t *StackCreate(size_t capacity)
{
	stack_t *stack = malloc(sizeof(stack_t));
	if(NULL == stack)
	{
		return (NULL);
	}
	
	stack -> elements = malloc(sizeof(void *) * capacity);
	if(NULL == stack -> elements)
	{
		free(stack);
		return (NULL);
	}
	
	stack -> capacity = capacity;
	stack -> top = 0;

	return (stack);
}

/******************************* StackPush ***********************************/

void StackPush(stack_t *stack, void *data)
{
	assert(stack);
	
	if(stack -> top != stack -> capacity)
	{
		stack -> elements[stack -> top] = data;
		stack -> top++;
	}
}

/******************************* StackPop ************************************/

void StackPop(stack_t *stack)
{
	assert(stack);

	if(stack -> top)
	{
		stack -> top--;
	}
}

/******************************* StackPeek ***********************************/

void *StackPeek(stack_t *stack)
{
	assert(stack);
	
	if (0 == stack -> top)
	{
		return (NULL);
	}
	else
	{
		return stack -> elements[stack -> top - 1];
	}
}

/******************************* StackSize ***********************************/

size_t StackSize(stack_t *stack)
{
	assert(stack);
	
	return (stack -> top);
}

/******************************* StackCapacity *******************************/

size_t StackCapacity(stack_t *stack)
{
	assert(stack);
	
	return (stack -> capacity);
}

/******************************* StackIsEmpty ********************************/

int StackIsEmpty(stack_t *stack)
{
	assert(stack);
	
	return (0 == stack -> top);
}

/******************************* StackDestroy ********************************/

void StackDestroy(stack_t *stack)
{
	
	free(stack -> elements);
	free(stack);
	stack = NULL;
}

/******************************************************************************/
