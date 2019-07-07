#ifndef __rtos_h
#define __rtos_h

#include <stdio.h>
#include <cmsis_armcc.h>
#include <stdbool.h>

#include "globals.h"
#include "context.h"

// Interface Functions
void rtos_init(void);
void task_create(rtosTaskFunc_t function_pointer, void* function_arg, priority_t task_priority);

void semaphore_create(void);
void semaphore_take(void);
void semaphore_give(void);

void mutex_create(void);
void mutex_give(void);
void mutex_take(void);

// Other functions
bool push_to_stack(tcb_t *tcb, uint32_t value);
uint32_t pop_from_stack(tcb_t *tcb);
bool context_switch(tcb_t *old_task, tcb_t *new_task);

#endif //__rtos_h
