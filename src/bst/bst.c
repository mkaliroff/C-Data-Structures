/******************************************************************************

    writer: Matias

    reviewer: Zehorit

    date: 27.04.2023


******************************************************************************/
#include <stdlib.h> /* malloc free */
#include <assert.h> /* assert*/

#include "bst.h"

#define FALSE (0)
/*****************************************************************************/
enum children 
{
	LEFT = 0,
	RIGHT,
	CHILDRENS
};

typedef struct bst_node
{
	void *data;
	struct bst_node *parent;
	struct bst_node *children[CHILDRENS];
} bst_node_t;

struct bst
{
	bst_node_t *end;
	void *param;
	bst_compare_func_t cmp;
};

/*****************************************************************************/

#define CMP(X,Y,Z) (Z->cmp(BSTGetData(X), Y, Z->param))

#define AVAILABLE_NODE_NOT_FOUND(X,Y,Z) ((BSTGetLeftChild(X) && \
 (0 < CMP(X, Y, Z))) || (BSTGetRightChild(X) && 0 > CMP(X, Y, Z)))

/*****************************************************************************/

static bst_iter_t BSTGetLeftMostChild(bst_iter_t node);

static int Counter(void *data, void *counter);

static void SetNode(bst_iter_t node, void *data);

static void SetChildOfNodeParentTo(bst_iter_t set, bst_iter_t node);

static bst_iter_t GoToNextAvailableNode(bst_iter_t iterator, const bst_t *bst, void *data);

static bst_iter_t BSTGetParent(bst_iter_t iter);

static bst_iter_t BSTGetLeftChild(bst_iter_t iter);

static bst_iter_t BSTGetRightChild(bst_iter_t iter);

static bst_iter_t BSTGetRoot(const bst_t *bst);

static void BSTSetLeftChild(bst_iter_t iter, bst_iter_t left_child);

static void BSTSetRightChild(bst_iter_t iter, bst_iter_t right_child);

static void BSTSetParent(bst_iter_t iter, bst_iter_t parent);


/*****************************************************************************/

bst_t *BSTCreate(bst_compare_func_t cmp, void *param)
{
	bst_t *bst = (bst_t *)malloc(sizeof(bst_t));
	if(NULL == bst)
	{
		return (NULL);
	}

	bst->end = (bst_node_t *)malloc(sizeof(bst_node_t));
	if(NULL == bst->end)
	{
		free(bst);
		return (NULL);
	}

	bst->cmp = cmp;
	bst->param = param;
	BSTSetParent(bst->end, NULL);
	SetNode(bst->end, NULL);

	return (bst);
}
/*****************************************************************************/
void BSTDestroy(bst_t *bst)
{
	bst_iter_t tmp = NULL;
	bst_iter_t next = NULL;
	assert(bst);

	next = BSTBegin(bst);
	while(bst->end != next)
	{
		tmp = next;
		next = BSTNext(next);
		BSTRemove(tmp);
	}

	free(bst->end);
	free(bst);
	bst = NULL;
}
/*****************************************************************************/
int BSTIsEmpty(const bst_t *bst)
{
	assert(bst);
	return (!BSTGetRoot(bst));
}
/*****************************************************************************/
int BSTIsIterEqual(bst_iter_t node1, bst_iter_t node2)
{
	return (node1 == node2);
}
/*****************************************************************************/
size_t BSTCount(bst_t *bst)
{
	size_t counter = 0;
	assert(bst);

	BSTForEach(bst, Counter, &counter);
	return (counter);
}
/*****************************************************************************/
bst_iter_t BSTInsert(bst_t *bst, void *data)
{
	bst_iter_t node = NULL; 
	bst_iter_t iterator = NULL;
	assert(bst);

	node = (bst_node_t *)malloc(sizeof(bst_node_t));
	if(NULL == node)
	{
		return (bst->end);
	}

	SetNode(node, data);
	if(BSTIsEmpty(bst))
	{
		BSTSetParent(node, BSTEnd(bst));
		BSTSetLeftChild(BSTEnd(bst), node);
		return (node);
	}
	
	iterator = BSTGetRoot(bst);
	while(AVAILABLE_NODE_NOT_FOUND(iterator, data, bst))
	{
		iterator = GoToNextAvailableNode(iterator, bst, data);
	}

	BSTSetParent(node, iterator);
	if(0 < CMP(iterator, data, bst))
	{
		BSTSetLeftChild(iterator, node);
	}
	else
	{
		BSTSetRightChild(iterator, node);
	}
	

	return (node);
}
/*****************************************************************************/
void *BSTRemove(bst_iter_t node)
{
	bst_iter_t tmp = NULL;
	void *data = NULL;
	assert(node);

	data = BSTGetData(node);
	if(!BSTGetLeftChild(node) && !BSTGetRightChild(node))
	{
		SetChildOfNodeParentTo(NULL, node);
	}
	else if(BSTGetLeftChild(node) && !BSTGetRightChild(node))
	{
		SetChildOfNodeParentTo(BSTGetLeftChild(node), node);
		BSTSetParent(BSTGetLeftChild(node), BSTGetParent(node));
	}
	else if(!BSTGetLeftChild(node) && BSTGetRightChild(node))
	{
		SetChildOfNodeParentTo(BSTGetRightChild(node), node);
		BSTSetParent(BSTGetRightChild(node), BSTGetParent(node));
	}
	else
	{
	

		if((BSTIsIterEqual(node, BSTGetLeftChild(BSTGetParent(node)))))
		{
			tmp = BSTPrev(node);
			BSTSetLeftChild(BSTGetParent(node), BSTGetLeftChild(node));
			BSTSetParent(BSTGetLeftChild(node), BSTGetParent(node));
			BSTSetRightChild(tmp, BSTGetRightChild(node));
			BSTSetParent(BSTGetRightChild(node), tmp);
		}
		else
		{
			tmp = BSTNext(node);
			BSTSetRightChild(BSTGetParent(node), BSTGetRightChild(node));
			BSTSetParent(BSTGetRightChild(node), BSTGetParent(node));
			BSTSetLeftChild(tmp, BSTGetLeftChild(node));
			
			BSTSetParent(BSTGetLeftChild(node), tmp);
		}
	}

	free(node);
	node = NULL;
	return (data);
}
/*****************************************************************************/
bst_iter_t BSTFind(const bst_t *bst, void *data)
{
	bst_iter_t iterator = NULL;
	assert(bst);

	iterator = BSTGetRoot(bst);
	while(iterator && (size_t)iterator->data != (size_t)data)
	{
		iterator = GoToNextAvailableNode(iterator, bst, data);
	}

	return (iterator ? iterator : bst->end);
}
/*****************************************************************************/
bst_iter_t BSTBegin(const bst_t *bst)
{
	assert(bst);
	if(NULL == BSTGetRoot(bst))
	{
		return (bst->end);
	}

	return (BSTGetLeftMostChild(BSTGetRoot(bst)));
}
/*****************************************************************************/
bst_iter_t BSTEnd(const bst_t *bst)
{
	assert(bst);
	return (bst->end);
}
/*****************************************************************************/
bst_iter_t BSTPrev(bst_iter_t node)
{
	assert(node);
	if(NULL == BSTGetLeftChild(node))
	{
		for(; BSTGetLeftChild(BSTGetParent(node)) == node; node = BSTGetParent(node));
		node = BSTGetParent(node);
	}
	else
	{
		node = BSTGetLeftChild(node);
		for(; NULL != BSTGetRightChild(node); node = BSTGetRightChild(node));
	}

	return (node);
}
/*****************************************************************************/
bst_iter_t BSTNext(bst_iter_t node)
{
	assert(node);
	if(NULL ==  BSTGetRightChild(node))
	{
		for(; BSTGetRightChild(BSTGetParent(node)) == node; node = BSTGetParent(node));
		node = BSTGetParent(node);
	}
	else
	{
		node = BSTGetLeftMostChild(BSTGetRightChild(node));
	}

	return (node);
}
/*****************************************************************************/
void *BSTGetData(bst_iter_t node)
{
	assert(node);
	return (node->data);
}
/*****************************************************************************/
int BSTForEach(bst_t *bst, bst_action_func_t act, void *param)
{
	int status = 0;
	bst_iter_t end = NULL;
	bst_iter_t iterator = NULL;

	assert(bst);
	assert(act);

	end = bst->end;
	iterator = BSTBegin(bst);
	while(iterator != end && !(status = act(&(iterator->data), param)))
	{
		iterator = BSTNext(iterator);
	}

	return (status);
}

/*****************************************************************************/


static bst_iter_t BSTGetLeftMostChild(bst_iter_t node)
{
	assert(node);
	for(; NULL != BSTGetLeftChild(node); node = BSTGetLeftChild(node));
	return (node);
}

static bst_iter_t GoToNextAvailableNode(bst_iter_t iterator, const bst_t *bst, void *data)
{
	if(0 < CMP(iterator, data, bst))
	{
		iterator = BSTGetLeftChild(iterator);
	}
	else
	{
		iterator = BSTGetRightChild(iterator);
	}

	return (iterator);
}

static void SetChildOfNodeParentTo(bst_iter_t set, bst_iter_t node)
{
	assert(node);
	if(BSTIsIterEqual(node, BSTGetLeftChild(BSTGetParent(node))))
	{
		BSTSetLeftChild(BSTGetParent(node), set);
	}
	else
	{
		BSTSetRightChild(BSTGetParent(node), set);
	}
}

static void SetNode(bst_iter_t node, void *data)
{
	assert(node);
	BSTSetRightChild(node, NULL);
	BSTSetLeftChild(node, NULL);
	node->data = data;
}
static int Counter(void *data, void *counter)
{
	(void)data;
	++(*(size_t *)counter);
	return 0;
}

static bst_iter_t BSTGetParent(bst_iter_t iter)
{
	return (bst_iter_t)iter->parent;
}

static bst_iter_t BSTGetLeftChild(bst_iter_t iter)
{
	return (bst_iter_t)iter->children[LEFT];
}

static bst_iter_t BSTGetRightChild(bst_iter_t iter)
{
	return (bst_iter_t)iter->children[RIGHT];
}

static bst_iter_t BSTGetRoot(const bst_t *bst)
{
	assert(bst);
	return BSTGetLeftChild(BSTEnd(bst));
}

static void BSTSetLeftChild(bst_iter_t iter, bst_iter_t left_child)
{
	iter->children[LEFT] = left_child;
}

static void BSTSetRightChild(bst_iter_t iter, bst_iter_t right_child)
{
	iter->children[RIGHT] = right_child;
}

static void BSTSetParent(bst_iter_t iter, bst_iter_t parent)
{
	iter->parent = parent;
}

/*****************************************************************************/
