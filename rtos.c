#include "rtos.h"
#include "context.h"
#include <stdio.h>
#include <LPC17xx.h>

tcb_t tcb_array[6];
tcb_t tcb_main;

uint8_t num_tasks = 0;

extern void SysTick_Handler(void); // find better solution?

void rtos_init(){
	const uint32_t stack_size = 0x100; // Actaully physical stack_size/4
	const uint32_t num_tcbs = 6;
	const uint8_t main_task_num = 0;
	const uint32_t vector_table_address = 0x0;
	const uint32_t psp_enable = (1<<1);
	
	// Debug
	volatile int a = 0x12345678;
	
	tcb_array[main_task_num].task_id = main_task_num;
	tcb_array[main_task_num].stack_pointer = tcb_array[main_task_num].stack_base_address; // This will be set properly as soon as task is switched out of
	tcb_array[main_task_num].state = READY;
	tcb_array[main_task_num].priority = IDLE;
	// HANDLE TCB LIST POINTER HERE, SHOULD IT GO STRAIGHT TO READY LIST?
	tcb_array[main_task_num].stack_size = 0; // not used yet
	
	__disable_irq();
	
	// Initalize all task stack addresses (including main task stack)
	uint32_t *initial_sp_pointer = (uint32_t*)vector_table_address; 
	tcb_main.stack_base_address = (uint32_t*)(*initial_sp_pointer);
	tcb_main.stack_overflow_address = tcb_main.stack_base_address-(stack_size*2);
	
	for(uint8_t tcb_num = 0; tcb_num < num_tcbs; tcb_num++){
		tcb_array[tcb_num].stack_base_address = (tcb_main.stack_overflow_address)-(stack_size*(tcb_num));
		tcb_array[tcb_num].stack_overflow_address = tcb_main.stack_overflow_address-((stack_size*(tcb_num+1))); // remove -1
	}
	
	// Copy contents of MAIN stack to new main() process stack
	uint32_t main_stack_top = __get_MSP();
	for(uint32_t *main_loc = tcb_main.stack_base_address; (uint32_t)main_loc >= main_stack_top; main_loc--){
		uint32_t *main_task_loc = main_loc-(stack_size*2);
		*main_task_loc = *main_loc;
	}
	
	// Save top of stack in global
	tcb_main.stack_pointer = &main_stack_top;
	
	// Set main stack pointer back to base address of main() stack (access to local variables lost)
	__set_MSP((uint32_t)tcb_main.stack_base_address);
	
	// Set psp to top of main() stack (now task0 stack)
	// multiplied by four because of division made earlier to account for 32 bit pointer incrementation
	__set_PSP(*tcb_main.stack_pointer-(stack_size*2*4));
	
	// Switch from using msp to psp (local variables restored)
	__set_CONTROL((uint32_t)__get_CONTROL|psp_enable);
	
	__enable_irq();
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
	static uint8_t task_number = 1;
	uint32_t default_psr_val = 0x01000000;
	
	tcb_array[task_number].task_id = task_number;
	tcb_array[task_number].stack_pointer = tcb_array[task_number].stack_base_address;
	tcb_array[task_number].state = READY;
	tcb_array[task_number].priority = task_priority;
	// HANDLE LIST POINTER HERE, SHOULD IT GO STRAIGHT TO READY LIST?
	tcb_array[task_number].stack_size = 0; // not used yet
	
	// push all stack values incrementing psp
	push_to_stack(&tcb_array[task_number], default_psr_val);
	push_to_stack(&tcb_array[task_number], (uint32_t)function_pointer); // TEST THIS WELL
	for(uint8_t count = 0; count < 5; count++)
		push_to_stack(&tcb_array[task_number], 0x0);
	push_to_stack(&tcb_array[task_number], (uint32_t)function_arg);
	for(uint8_t count = 0; count < 8; count++)
		push_to_stack(&tcb_array[task_number], 0x0);
	
	task_number++;
}
