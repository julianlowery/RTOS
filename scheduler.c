#include <LPC17xx.h>
#include <stdio.h>

#include "scheduler.h"
#include "context.h"

uint32_t msTicks = 0;
uint32_t msp_enable_not = (1<<1);
extern tcb_t tcb_array[6];

bool push_to_stack(tcb_t *tcb, uint32_t value){
	tcb->stack_pointer--;
	
	// Check for stack overflow
	if(tcb->stack_pointer <= tcb->stack_overflow_address){
		printf("STACK OVERFLOW from task id: %d/n", tcb->task_id);
		return false;
	}
	
	*(tcb->stack_pointer) = value;
	return true;
}

uint32_t pop_from_stack(tcb_t *tcb){
	uint32_t returnValue = *(tcb->stack_pointer);
	tcb->stack_pointer++;
	
	return returnValue;
}

bool context_switch(tcb_t *old_task_tcb, tcb_t *new_task_tcb){
	
	// Store old task registers on task stack
	old_task_tcb->stack_pointer = (uint32_t*)storeContext();
	
	// Check for overflow on old task
	if(old_task_tcb->stack_pointer <= old_task_tcb->stack_overflow_address)
		return false;
	
	// Set psp to new task
	uint32_t new_task_psp = (uint32_t)new_task_tcb->stack_pointer;
	__set_PSP(new_task_psp); // CHECK THAT THIS ACTUALLY GETS SET
	
	// Restore context of new task from  stack
	restoreContext(new_task_psp);
	return true;
}

void SysTick_Handler(void) {
	const uint32_t time_slice_len = 1000;
	const uint32_t set_pendsv = (1<<28);
	static uint32_t time_slice_count = time_slice_len;
	
  msTicks++;
	time_slice_count--;
	
	if(time_slice_count == 0){
		time_slice_count = time_slice_len;
		
		// Set pendSV exception to pending
		SCB->ICSR=(SCB->ICSR|set_pendsv);
	}
}

void PendSV_Handler(void){
	const uint32_t clear_pendsv = (1<<27);

	// Test context swtich
	static uint8_t task_number = 0;
	task_number %= 6;
	
	context_switch(&tcb_array[task_number], &tcb_array[task_number+1]);
	
	// Remove pending state from pendSV exception
	SCB->ICSR=(SCB->ICSR|clear_pendsv);
}


