#ifndef __scheduler_h
#define __scheduler_h

#include "globals.h"

// Push and pop used to manipulate task stacks
bool push_to_stack(tcb_t *tcb, uint32_t value);
uint32_t pop_from_stack(tcb_t *tcb);

// Used to switch between two tasks
bool context_switch(tcb_t *old_task_tcb, tcb_t *new_task_tcb);

// Interrupt handlers
void SysTick_Handler(void);
void PendSV_Handler(void);

// Used to manipulate queues (ready queues, block queues)
void enqueue(tcb_list_t *list, tcb_t *tcb);
tcb_t* dequeue(tcb_list_t *list);
tcb_t* remove_from_list(tcb_list_t *list, tcb_t *tcb);

// Used to add a task to the scheduler
void add_task_to_sched(scheduler_t *scheduler, tcb_t *tcb);

#endif // __scheduler_h
