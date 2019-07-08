#include "rtos.h"
#include "context.h"
#include <stdio.h>

tcb_t tcb_array[6];
tcb_t tcb_main;

uint8_t num_tasks = 0;

void rtos_init(){
	const uint32_t stack_size = 0x100;
	const uint32_t num_tcbs = 6;
	volatile int a = 0x12345678;
	
	// Initalize all task stack addresses (including main task stack)
	uint32_t *initial_sp_pointer = (uint32_t*)0x0; 
	tcb_main.stack_base_address = (uint32_t*)(*initial_sp_pointer);
	tcb_main.stack_overflow_address = tcb_main.stack_base_address-(stack_size*2);
	
	for(uint8_t tcb_num = 0; tcb_num < num_tcbs; tcb_num++){
		tcb_array[tcb_num].stack_base_address = (tcb_main.stack_overflow_address)-(stack_size*(tcb_num));
		tcb_array[tcb_num].stack_overflow_address = tcb_main.stack_overflow_address-(stack_size*(tcb_num+1));
	}
	
	// Copy contents of MAIN stack to new main() process stack
	uint32_t main_stack_top = __get_MSP();
	for(uint32_t *main_loc = tcb_main.stack_base_address; (uint32_t)main_loc >= main_stack_top; main_loc--){
		uint32_t *main_task_loc = main_loc-(stack_size*2);
		*main_task_loc = *main_loc;
	}
		printf("1\n");

	// Set main stack pointer to base address of main() stack 
	__set_MSP((uint32_t)tcb_main.stack_base_address);
		printf("2\n");

	// Switch from using msp to psp
	uint32_t control_reg = __get_CONTROL();
	__set_CONTROL((uint32_t)(control_reg|=(1<<1))); // HARD FAULT HERE
		printf("3\n");
	
	// Set psp to top of main() stack
	uint32_t main_proc_top = main_stack_top;// is this supposed to be -(stack_size*2);
	__set_PSP(main_proc_top);
}

bool context_switch(tcb_t *old_task_tcb, tcb_t *new_task_tcb){
	
	// Store old task registers on task stack
	old_task_tcb->stack_pointer = (uint32_t*)storeContext();
	
	// Check for overflow on old task
	if(old_task_tcb->stack_pointer <= old_task_tcb->stack_overflow_address)
		return false;
	
	// Set psp to new task
	uint32_t new_task_psp = (uint32_t)new_task_tcb->stack_pointer;
	__set_PSP(new_task_psp);
	
	// Restore context of new task from  stack
	restoreContext(new_task_psp);
	return true;
}

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

void task_create(rtosTaskFunc_t function_pointer, void* function_arg, priority_t task_priority){
	static uint8_t task_number = 0;
	uint32_t default_psr_val = 0x01000000;
	
	tcb_array[task_number].task_id = task_number;
	tcb_array[task_number].stack_pointer = tcb_array[task_number].stack_base_address;
	tcb_array[task_number].state = READY;
	tcb_array[task_number].priority = task_priority;
	// HANDLE LIST POINTER HERE, SHOULD IT GO STRAIGHT TO READY LIST?
	tcb_array[task_number].stack_size = 0; // not used yet
	
	// push all stack values incrementing psp
	push_to_stack(&tcb_array[num_tasks], default_psr_val);
	push_to_stack(&tcb_array[num_tasks], (uint32_t)function_pointer); // TEST THIS WELL
	for(uint8_t count = 0; count < 5; count++)
		push_to_stack(&tcb_array[num_tasks], 0x0);
	push_to_stack(&tcb_array[num_tasks], (uint32_t)function_arg);
	for(uint8_t count = 0; count < 8; count++)
		push_to_stack(&tcb_array[num_tasks], 0x0);
	
	task_number++;
}

