/*****************************************************************************

    	writer: Matias Yoel Kaliroff Kravchik
    	reviwer: Art
    	date: 02.03.23
    
******************************************************************************/

#ifndef __STACK_H__
#define __STACK_H__

#include <stddef.h>

/*will be in c file
struct stack
{
	size_t capacity;
	size_t top;
	void **elements;
} ;*/
typedef struct stack stack_t;

/* create a new Stack */
stack_t *StackCreate(size_t );
/* add member */
void StackPush(stack_t *, void *);
/* get and remove the first member */
void StackPop(stack_t *);
/* get the first member */
void *StackPeek(stack_t *);
/* get size of Stack */
size_t StackSize(stack_t *);
/* The amount of values in the stack */
size_t StackCapacity(stack_t *);
/* if  stack is empty */
int StackIsEmpty(stack_t *);
/* destroy the stack */
void StackDestroy(stack_t *);


#endif /*__STACK_H__*/
