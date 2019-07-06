#include "rtos.h"

tcb_t tcb_array[6];
tcb_t tcb_main;

uint8_t num_tasks = 0;

void rtos_init(){
	const uint32_t stack_size = 0x100;
	const uint32_t num_tcbs = 6;
	
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

	// Set main stack pointer to base address of main() stack 
	__set_MSP((uint32_t)tcb_main.stack_base_address);

	// Switch from using msp to psp
	uint32_t control_reg = __get_CONTROL();
	__set_CONTROL((uint32_t)(control_reg|=(1<<1)));
	
	// Set psp to top of main() stack
	uint32_t main_proc_top = main_stack_top-(stack_size*2);
	__set_PSP(main_proc_top);
}

// Context Swtich - 	NEEDS TO BE TESTED ---------------------------------------------------------------------------
void context_switch(tcb_t *old_task_tcb, tcb_t *new_task_tcb){
	old_task_tcb->stack_pointer = (uint32_t*)storeContext();
	
	uint32_t new_task_psp = (uint32_t)new_task_tcb->stack_pointer;
	__set_PSP(new_task_psp);
	
	restoreContext(new_task_psp);
}

void task_create(rtosTaskFunc_t function_pointer, void* function_arg, priority_t task_priority){
	
	
	
}
