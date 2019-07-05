#include "globals.h"

void rtos_init(void);
void task_create(void);

void semaphore_create(void);
void semaphore_take(void);
void semaphore_give(void);

void mutex_create(void);
void mutex_give(void);
void mutex_take(void);
