#ifndef __rtos_h
#define __rtos_h

#include <stdbool.h>

#include "globals.h"
#include <LPC17xx.h>

// Interface Functions
void rtos_init(void);
void task_create(rtosTaskFunc_t function_pointer, void* function_arg, priority_t task_priority);

void semaphore_init(semaphore_t *sem, uint8_t initial_count);
void semaphore_take(semaphore_t *sem);
void semaphore_give(semaphore_t *sem);

void mutex_init(void);
void mutex_give(void);
void mutex_take(void);

#endif //__rtos_h
