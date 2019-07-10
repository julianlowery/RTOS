#ifndef __scheduler_h
#define __scheduler_h

#include <stdbool.h>

#include "globals.h"

bool push_to_stack(tcb_t *tcb, uint32_t value);
uint32_t pop_from_stack(tcb_t *tcb);

bool context_switch(tcb_t *old_task_tcb, tcb_t *new_task_tcb);

void SysTick_Handler(void);
void PendSV_Handler(void);

#endif // __scheduler_h
