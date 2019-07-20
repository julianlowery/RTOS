#ifndef __rtos_h
#define __rtos_h

#include "globals.h"
#include <LPC17xx.h>

// Interface Functions
void rtos_init(void);
void task_create(rtosTaskFunc_t function_pointer, void* function_arg, priority_t task_priority);

// Semaphore interface
void semaphore_init(semaphore_t *sem, uint8_t initial_count);
void semaphore_take(semaphore_t *sem);
void semaphore_give(semaphore_t *sem);

// Mutex interface
void mutex_init(mutex_t *mutex);
void mutex_give(mutex_t *mutex);
void mutex_take(mutex_t *mutex);

// Implement a return type for semaphore, mutex, and task_create functions

#endif //__rtos_h
