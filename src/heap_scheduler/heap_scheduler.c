/*

	Author: Matias                           

	Reviewer:   Ishay                        

	Date: 09.4.23
	
*/

#include <stdlib.h> /*malloc, free*/
#include <unistd.h> /*sleep */
#include <stddef.h> /*NULL*/
#include <assert.h> /*assert*/
#include <time.h> /* time_t */
#include <stdio.h>

#include "heap_scheduler.h"
#include "heap_pq.h"
#include "task.h"

#define SUCCESS (0)
#define FAIL (1)
#define TRUE (1)
#define FALSE (0)

/*****************************************************************************/

struct scheduler
{
	priority_queue_t *priority_queue;
	int isrun;
};

/*****************************************************************************/

static int compare_func(const void *data, const void *new_data);
static int SchedulerTaskUidIsMatch(const void *task_uid, const void *uid);

/*****************************************************************************/

static int compare_func(const void *data,const void *new_data)
{
	return (int)(TaskGetTime((void*)data)) - (int)(TaskGetTime((void*)new_data));
}

static int SchedulerTaskUidIsMatch(const void *task_uid,const void *uid)
{
    return UIDIsMatch(TaskGetUID((task_t*)task_uid), *(ilrd_uid_t*)uid);
}



/*****************************************************************************/

scheduler_t *SchedulerCreate(void)
{
	scheduler_t *scheduler = NULL;
	
	scheduler = (scheduler_t*)malloc(sizeof(scheduler) + sizeof(int));
	if(NULL == scheduler)
	{
		return NULL;
	}
	scheduler->priority_queue = PriorityQueueCreate(compare_func);
	if(NULL == scheduler->priority_queue)
	{
		free(scheduler);
		return NULL;
	}
	scheduler->isrun = FALSE;
	
	return scheduler;
}

/*****************************************************************************/

void SchedulerDestroy(scheduler_t *scheduler)
{	
	assert(scheduler);
	
	SchedulerClear(scheduler);
	PriorityQueueDestroy(scheduler->priority_queue);
	free(scheduler);
	scheduler = NULL;
}

/*****************************************************************************/

ilrd_uid_t SchedulerAddTask(scheduler_t *scheduler, scheduler_action_func_t action, void *parameter, scheduler_clean_func_t clean, size_t interval, time_t when)
{

	task_t *task = NULL;
	
	assert(scheduler);
	
	task = TaskCreate(action, parameter, clean, interval, when);
	if(NULL == task)
	{
		return UIDGetBad();
	}
	if(SUCCESS != PriorityQueueEnqueue(scheduler->priority_queue, (void*)task))
	{
	/*TaskDestroy(task); */
		return UIDGetBad();
	}
	return TaskGetUID(task);
	
}

/*****************************************************************************/

int SchedulerRemoveTask(scheduler_t *scheduler, ilrd_uid_t task_uid)
{
	void *data = NULL;
	
	assert(scheduler);
	
	data = PriorityQueueErase(scheduler->priority_queue, SchedulerTaskUidIsMatch, &task_uid);
	
	if(scheduler->priority_queue == data)
	{
		return FAIL;
	}
	TaskDestroy(data);
	return SUCCESS;
}

/*****************************************************************************/

int SchedulerRun(scheduler_t *scheduler)
{
	task_t *task = NULL;
	int taskrun = FALSE;
	
	scheduler->isrun = TRUE;
	while(FALSE == PriorityQueueIsEmpty(scheduler->priority_queue) && \
	TRUE == scheduler->isrun && FALSE == taskrun)
	{
		task = PriorityQueueDequeue(scheduler->priority_queue);
		
		if(TaskGetTime(task) > time(NULL))
		{
			sleep(TaskGetTime(task) - time(NULL));
		}
		
		taskrun = TaskRun(task);
		
		if(-2 == taskrun)
		{
			taskrun = SUCCESS;
			TaskSetInterval(task, (size_t)0);
		}
		
		if(!TaskGetInterval(task))
		{
			continue;
		}
		
		TaskSetTime(task, TaskGetInterval(task) + TaskGetTime(task));
		if(PriorityQueueEnqueue(scheduler->priority_queue, task))
		{
			return -1;
		}
	}
	
	return taskrun;
}

/*****************************************************************************/

void SchedulerStop(scheduler_t *scheduler)
{
	scheduler->isrun = FALSE;
}

/*****************************************************************************/

size_t SchedulerSize(scheduler_t *scheduler)
{
	return PriorityQueueSize(scheduler->priority_queue);
}

/*****************************************************************************/

int SchedulerIsEmpty(scheduler_t *scheduler)
{
	return PriorityQueueIsEmpty(scheduler->priority_queue);
}

/*****************************************************************************/

void SchedulerClear(scheduler_t *scheduler)
{
	assert(scheduler);

	while(!SchedulerIsEmpty(scheduler))
	{
		TaskDestroy(PriorityQueueDequeue(scheduler->priority_queue));
	}
}

/*****************************************************************************/
