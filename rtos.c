#include "rtos.h"

tcb_t tcb1;
tcb_t tcb2;
tcb_t tcb3;
tcb_t tcb4;
tcb_t tcb5;
tcb_t tcb6;
tcb_t tcb_main;

uint8_t num_tasks = 0;

void rtos_init(){
	
	// Initalize all task stack addresses (including main task stack)
	int32_t *initial_sp_pointer = (int*)0x00000000; 
	tcb_main.stack_base_address = (int*)(*initial_sp_pointer);
	
	tcb1.stack_base_address = tcb_main.stack_base_address-0x0800;
	tcb1.stack_overflow_address = tcb_main.stack_base_address-0x0C00;
	
	tcb2.stack_base_address = tcb_main.stack_base_address-0x0C00;
	tcb2.stack_overflow_address = tcb_main.stack_base_address-0x1000;
	
	tcb3.stack_base_address = tcb_main.stack_base_address-0x1000;
	tcb3.stack_overflow_address = tcb_main.stack_base_address-0x1400;
	
	tcb4.stack_base_address = tcb_main.stack_base_address-0x1400;
	tcb4.stack_overflow_address = tcb_main.stack_base_address-0x1800;
	
	tcb5.stack_base_address = tcb_main.stack_base_address-0x1800;
	tcb5.stack_overflow_address = tcb_main.stack_base_address-0x1C00;
	
	tcb6.stack_base_address = tcb_main.stack_base_address-0x1C00;
	tcb6.stack_overflow_address = tcb_main.stack_base_address-0x2000;
	
	
	
	// Copy contents of MAIN stack to new main() process stack
	
	// Set main stack pointer to base address of main() stack WHY TO BASE ADDRESS? WHY NOT TO TOP OF MAIN STACK?
	
	// Switch from using msp to psp
	
}
