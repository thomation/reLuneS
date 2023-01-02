#include "os.h"

/* defined in entry.S */
extern void switch_to(struct context *next);

#define MAX_TASKS 10
#define STACK_SIZE 1024

uint8_t task_stack[MAX_TASKS][STACK_SIZE];
struct context ctx_tasks[MAX_TASKS];

static int _top = 0;
static int _current = -1;

static void w_mscratch(reg_t x)
{
	asm volatile("csrw mscratch, %0"
				 :
				 : "r"(x));
}
void sched_init()
{
	w_mscratch(0);
}
void schedule()
{
	if (_top <= 0)
	{
		panic("Num of task should be greater than zero!");
		return;
	}
	// First
	if (_current < 0)
	{
		_current = (_current + 1) % _top;
	}
	else
	{
		struct context *current = &(ctx_tasks[_current]);
		current->task_priority++;
	}
	_current = find_next_task();
	struct context *next = &(ctx_tasks[_current]);
	switch_to(next);
}
int find_next_task()
{
	struct context *current = &(ctx_tasks[_current]);
	for(int i = _current + 1; i < _top; i ++)
	{
		struct context *next = &(ctx_tasks[i]);
		if (next->task_priority < current->task_priority)
			return i;
	}

	for(int i = 0; i < _current; i ++)
	{
		struct context *next = &(ctx_tasks[i]);
		if (next->task_priority < current->task_priority)
			return i;
	}
	return _current;
}
int task_create(void (*start_routin)(void *param), void *param, uint8_t priority)
{
	if (_top < MAX_TASKS)
	{
		ctx_tasks[_top].sp = (reg_t)&task_stack[_top][STACK_SIZE - 1];
		ctx_tasks[_top].ra = (reg_t)start_routin;
		ctx_tasks[_top].a0 = param;
		ctx_tasks[_top].task_priority = priority;
		_top++;
		return 0;
	}
	else
	{
		return -1;
	}
}
void task_yield()
{
	schedule();
}
void task_delay(volatile int count)
{
	count *= 50000;
	while (count--)
		;
}