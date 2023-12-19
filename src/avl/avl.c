#include <stdio.h> /* puts, printf */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include "avl.h"

#define AVL_TRUE (1)
#define AVL_FALSE (0)
#define AVL_SUCCESS (0)
#define AVL_FAIL (1)
#define UNUSED(x)((void)x)
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MAX_HEIGHT_R_L_CHILD(x)	(MAX(NodeHeight(x->children[LEFT]), \
										 NodeHeight(x->children[RIGHT])))

/*****************************************************************************/

enum children 
{
    LEFT = 0,
    RIGHT,
    NUM_OF_CHILDRENS
};

typedef struct avl_node
{
	void *data;
	size_t height;
	struct avl_node *children[NUM_OF_CHILDRENS];
} avl_node_t;

struct avl
{

	void *param;
	avl_node_t *root;
	avl_compare_func_t cmp;
} ;

/*****************************************************************************/

static void RecDestroy(avl_node_t *node);

static avl_node_t *RecInsert(avl_node_t *node_ptr, void *data, avl_compare_func_t cmp ,avl_node_t *new_node);
static avl_node_t *RecRemove(avl_node_t *root, void *data, avl_compare_func_t cmp);
static avl_node_t *FindMin(avl_node_t *root);
static avl_node_t *RecFreeMin(avl_node_t *root);
static int BalanceFactor(avl_node_t *node);
static size_t RecCount(avl_node_t *node);
static avl_node_t *LeftRotate(avl_node_t *z);
static avl_node_t *RightRotate(avl_node_t *y);

static void *RecFind(const avl_node_t *node, const void *data, avl_compare_func_t cmp);
static int RecForEachPostOrder(avl_node_t *node, avl_action_func_t act, void *arg, order_t order);
static int RecForEachInOrder(avl_node_t *node, avl_action_func_t act, void *arg, order_t order);
static int RecForEachPreOrder(avl_node_t *node, avl_action_func_t act, void *arg, order_t order);
static size_t NodeHeight(avl_node_t *node);




/*****************************************************************************/

avl_t *AVLCreate(avl_compare_func_t cmp, void *param)
{
	avl_t *avl = (avl_t*)malloc(sizeof(avl_t));
	if(NULL == avl)
	{
		return NULL;
	}
	avl->root = NULL;
	avl->cmp = cmp;
	avl->param = param;
	return avl;
}

/*****************************************************************************/

void AVLDestroy(avl_t *avl)
{
	assert(avl);
	if(AVL_FALSE == AVLIsEmpty(avl))
	{
		RecDestroy(avl->root);
	}

	free(avl);
}

/*****************************************************************************/

static void RecDestroy(avl_node_t *node)
{
	if(NULL == node)
	{
		return;
	}
	
	RecDestroy(node->children[LEFT]);
	RecDestroy(node->children[RIGHT]);
	
	free(node);
}

/*****************************************************************************/

int AVLInsert(avl_t *avl, const void *data)
{
	avl_node_t *new_node = (avl_node_t*)malloc(sizeof(avl_node_t));
	avl_node_t *node_ptr = NULL;
	assert(avl);
	if(!new_node)
	{
		return 1;
	}
	new_node->data = (void *)data;
	new_node->children[LEFT] = NULL;
	new_node->children[RIGHT] = NULL;
	new_node->height = 1;
	
	if(AVLIsEmpty(avl))
	{
		avl->root = new_node;
		return 0;
	}
	node_ptr = RecInsert((avl->root), (void *)data, avl->cmp, new_node);
	avl->root = node_ptr;
	avl->root->height = NodeHeight(avl->root);
	
	return (0);
}

/*****************************************************************************/

static avl_node_t *RecInsert(avl_node_t *node_ptr, void *data, avl_compare_func_t cmp, avl_node_t *new_node)
{
	int balance = 0;
	
	if(!node_ptr)
	{
		return (new_node);
	}
	
	if(0 < cmp(node_ptr->data, data, NULL))
	{
		node_ptr->children[LEFT] = RecInsert(node_ptr->children[LEFT], data, cmp, new_node);
	}
	else
	{
		node_ptr->children[RIGHT] = RecInsert(node_ptr->children[RIGHT], data, cmp, new_node);
	}
	
	node_ptr->height = 1 + MAX(NodeHeight(node_ptr->children[LEFT]), NodeHeight(node_ptr->children[RIGHT]));
	
	balance = BalanceFactor(node_ptr);
	
	if(1 < balance && 0 < (cmp(node_ptr->children[LEFT]->data, data, NULL)))
	{
		return (RightRotate(node_ptr));
	}
	if(1 < balance && 0 > (cmp(node_ptr->children[LEFT]->data, data, NULL)))
	{
		node_ptr->children[LEFT] = LeftRotate(node_ptr->children[LEFT]);
		return(RightRotate(node_ptr));
	}
	if(-1 > balance && 0 > (cmp(node_ptr->children[RIGHT]->data, data, NULL)))
	{
		return (LeftRotate(node_ptr));
	}
	if(-1 > balance && 0 < (cmp(node_ptr->children[RIGHT]->data, data, NULL)))
	{
		node_ptr->children[RIGHT] = RightRotate(node_ptr->children[RIGHT]);
		return(LeftRotate(node_ptr));
	}
	
	return node_ptr;
}

/*****************************************************************************/
/*
void *AVLRemove(avl_t *avl, const void *data)
{
	avl_node_t *result = RecRemove(avl->root,(void *)data, avl->cmp);
	
	assert(avl);
	
	avl->root = result;
	if(NULL != result)
	{
		avl->root->height = NodeHeight(avl->root);
		return((void*)data);
	}

	return NULL;
}
*/

void *AVLRemove(avl_t *avl, const void *data)
{	
	void *node_data = AVLFind(avl, (void*)data);
	if(NULL == node_data)
	{
		return NULL;
	}
	avl->root = RecRemove(avl->root, (void*)data, avl->cmp);
	
	return node_data;
}

/*****************************************************************************/

static avl_node_t *RecRemove(avl_node_t *root, void *data, avl_compare_func_t cmp)
{
	avl_node_t *tmp = NULL;
	if(NULL == root)
	{
		return NULL;
	}
	if(cmp(root->data, (void *)data, NULL) > 0)
	{
		root->children[LEFT] = RecRemove(root->children[LEFT], data, cmp);
	}
	else if(cmp(root->data, (void *)data, NULL) < 0)
	{
		root->children[RIGHT] = RecRemove(root->children[RIGHT], data, cmp);
	}
	else
	{
		if(NULL == root->children[LEFT]&& NULL == root->children[RIGHT])
		{
			free(root);
			return NULL;
		}
		else if(NULL == root->children[LEFT])
		{
			tmp = root->children[RIGHT];
			free(root);
			return tmp;
		}
		else if(NULL == root->children[RIGHT])
		{
			tmp = root->children[LEFT];
			free(root);
			return tmp;
		}
		
		tmp = FindMin(root->children[RIGHT]);
		root->data = tmp->data;
		root->children[RIGHT] = RecFreeMin(root->children[RIGHT]);
	}
	if(NULL == root)
	{
		return root;
	}
	
	root->height = MAX_HEIGHT_R_L_CHILD(root) + 1;
	
	if(BalanceFactor(root) > 1 && BalanceFactor(root->children[LEFT]) >= 0)
	{
		return RightRotate(root);
	}
	if(BalanceFactor(root) > 1 && BalanceFactor(root->children[LEFT]) < 0)
	{
		root->children[LEFT] = LeftRotate(root->children[LEFT]);
		return RightRotate(root);
	}
	if(BalanceFactor(root) < -1 && BalanceFactor(root->children[RIGHT]) <= 0)
	{
		return LeftRotate(root);
	}
	if(BalanceFactor(root) < -1 && BalanceFactor(root->children[RIGHT]) < 0)
	{
		root->children[RIGHT] = RightRotate(root->children[RIGHT]);
		return LeftRotate(root);
	}
	
	return root;
}

/*****************************************************************************/

static avl_node_t *RightRotate(avl_node_t *root)
{
	avl_node_t *root_left = root->children[LEFT];
	avl_node_t *left_right = root_left->children[RIGHT];
	
	root_left->children[RIGHT] = root;
	root->children[LEFT] = left_right;
	
	root->height = MAX_HEIGHT_R_L_CHILD(root) + 1;
	root_left->height = MAX_HEIGHT_R_L_CHILD(root) + 1;
	return root_left;
}

/*****************************************************************************/

static avl_node_t *LeftRotate(avl_node_t *root)
{
	avl_node_t *root_right = root->children[RIGHT];
	avl_node_t *right_left = root_right->children[LEFT];
	
	root_right->children[LEFT] = root;
	root->children[RIGHT] = right_left;
	
	root->height =  MAX_HEIGHT_R_L_CHILD(root) + 1;
	root_right->height =  MAX_HEIGHT_R_L_CHILD(root_right) + 1;
	return root_right;
}

/*****************************************************************************/

static size_t NodeHeight(avl_node_t *node)
{
	if(!node)
	{
		return 0;
	}
	return(node->height);
}

/*****************************************************************************/

static int BalanceFactor(avl_node_t *node)
{
	if(NULL == node)
	{
		return 0;
	}
	return(NodeHeight(node->children[LEFT]) - NodeHeight(node->children[RIGHT]));
}

/*****************************************************************************/

static avl_node_t *FindMin(avl_node_t *root)
{
	if(NULL == root->children[LEFT])
	{
		return root;
	}
	return FindMin(root->children[LEFT]);
}

/*****************************************************************************/

static avl_node_t *RecFreeMin(avl_node_t *root)
{
	avl_node_t *tmp = NULL;
	if(NULL == root)
	{
		return NULL;
	}
	if(NULL == root->children[LEFT])
	{
		tmp = root->children[RIGHT];
		free(root);
		return tmp;
	}
	root->children[RIGHT] = RecFreeMin(root->children[RIGHT]);
	return root;
}



/*****************************************************************************/

int AVLIsEmpty(const avl_t *avl)
{
	assert(avl);
	return (NULL == avl->root);
}

/*****************************************************************************/


size_t AVLHeight(const avl_t *avl)
{
	assert(avl);
	return avl->root->height;
}

/*****************************************************************************/

size_t AVLCount(const avl_t *avl)
{
	assert(avl);
	return RecCount(avl->root);
}
/*****************************************************************************/

static size_t RecCount(avl_node_t *node)
{
	size_t count = 0;
	if(NULL == node)
	{
		return 0;
	}
	count += RecCount(node->children[LEFT]);
	count += RecCount(node->children[RIGHT]);
	count++;
	return count;
}
/*****************************************************************************/

void *AVLFind(const avl_t *avl, const void *data)
{
	assert(avl);

	return RecFind(avl->root, data, avl->cmp);
}

/*****************************************************************************/

static void *RecFind(const avl_node_t *node, const void *data, avl_compare_func_t cmp)
{
	int result = 0;
	if(NULL == node)
	{
		return NULL;
	}
	result = cmp(node->data, (void*)data, NULL);
	
	if (0 > result)
	{
		return RecFind(node->children[RIGHT], data, cmp);
	}
	if (0 < result)
	{
		return RecFind(node->children[LEFT], data, cmp);
	}
	else
	{
		return node->data;
	}
}

/*****************************************************************************/

int AVLForEach(avl_t *avl, avl_action_func_t act, void *arg, order_t order)
{
	assert(avl);
	assert(act);

	switch(order)
	{
		case PRE_ORDER:
		{
			return RecForEachPreOrder(avl->root, act, arg, order);
			break;
		}
		case IN_ORDER:
		{
			return RecForEachInOrder(avl->root, act, arg, order);
			break;
		}
		case POST_ORDER:
		{
			return RecForEachPostOrder(avl->root, act, arg, order);
			break;
		}
		default:
		{
			return -1;
		}
	}
}

/*****************************************************************************/

static int RecForEachPreOrder(avl_node_t *node, avl_action_func_t act, void *arg, order_t order)
{
	int result = 0;
	if(NULL == node)
	{
		return 0;
	}
	result = act(node->data, arg);
	if(0 != result)
	{
		return result;
	}
	result = RecForEachPreOrder(node->children[LEFT], act, arg, order);
	if(0 != result)
	{
		return result;
	}
	result = RecForEachPreOrder(node->children[RIGHT], act, arg, order);
	if(0 != result)
	{
		return result;
	}
	
	return 0;
}
/*****************************************************************************/

static int RecForEachInOrder(avl_node_t *node, avl_action_func_t act, void *arg, order_t order)
{
	int result = 0;
	if(NULL == node)
	{
		return 0;
	}
	
	result = RecForEachInOrder(node->children[LEFT], act, arg, order);
	if(0 != result)
	{
		return result;
	}
	
	result = act(node->data, arg);
	if(0 != result)
	{
		return result;
	}

	result = RecForEachInOrder(node->children[RIGHT], act, arg, order);
	if(0 != result)
	{
		return result;
	}
	
	return 0;
}

/*****************************************************************************/

static int RecForEachPostOrder(avl_node_t *node, avl_action_func_t act, void *arg, order_t order)
{
	int result = 0;
	if(NULL == node)
	{
		return 0;
	}

	result = RecForEachPostOrder(node->children[LEFT], act, arg, order);

	if(0 != result)
	{
		return result;
	}
	
	result = RecForEachPostOrder(node->children[RIGHT], act, arg, order);
	if(0 != result)
	{
		return result;
	}

	result = act(node->data, arg);
	if(0 != result)
	{
		return result;
	}
	
	return 0;
}

/*****************************************************************************/

ssize_t MultiFind(const avl_t *avl, avl_is_match_func_t is_match, const void *param, dll_t *dll)
{
	UNUSED(avl);
	UNUSED(is_match);
	UNUSED(param);
	UNUSED(dll);
	return 0;
}

/*****************************************************************************/

ssize_t RemoveMultiple(const avl_t *avl, avl_is_match_func_t is_match, const void *param, dll_t *dll)
{
	UNUSED(avl);
	UNUSED(is_match);
	UNUSED(param);
	UNUSED(dll);
	return 0;
}

/*****************************************************************************/


