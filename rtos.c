#include "rtos.h"
#include "scheduler.h"
#include <stdio.h>

extern tcb_t tcb_array[6];
extern tcb_t tcb_main;

extern scheduler_t scheduler;
extern bool run_scheduler;

//uint8_t num_tasks = 0; can't remember why I had this here

//extern void SysTick_Handler(void); // find better solution?

void rtos_init(){
	const uint32_t stack_size = 0x100; // Actaully physical stack_size/4 to account for 32 bit pointer incrementation
	const uint32_t num_tcbs = 6;
	const uint8_t main_task_num = 0;
	const uint32_t vector_table_address = 0x0;
	const uint32_t psp_enable = (1<<1);
	
	// Debug
	volatile int a = 0x22222222;
	
	tcb_array[main_task_num].task_id = main_task_num;
	tcb_array[main_task_num].stack_pointer = tcb_array[main_task_num].stack_base_address; // This will be set properly as soon as task is switched out of
	tcb_array[main_task_num].state = READY;
	tcb_array[main_task_num].priority = IDLE;
	// HANDLE TCB LIST POINTER HERE, SHOULD IT GO STRAIGHT TO READY LIST?
	tcb_array[main_task_num].stack_size = 0; // not used yet
	
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
	
	add_task_to_sched(&scheduler, &tcb_array[main_task_num]);
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
		push_to_stack(&tcb_array[task_number], 0x1);
	push_to_stack(&tcb_array[task_number], (uint32_t)function_arg);
	for(uint8_t count = 0; count < 8; count++)
		push_to_stack(&tcb_array[task_number], 0x1);
	
	add_task_to_sched(&scheduler, &tcb_array[task_number]);
	
	task_number++;
}

void semaphore_init(semaphore_t *sem, uint8_t initial_count){
	sem->count = initial_count;
	sem->block_list.head = NULL;
	sem->block_list.tail = NULL;
	sem->block_list.size = 0;
}

void semaphore_take(semaphore_t *sem){
	if(sem->count > 0)
		sem->count--;
	else{
		enqueue(&sem->block_list, scheduler.running_task);
		dequeue(&scheduler.ready_lists[scheduler.current_priority]);
		run_scheduler = true;
	}
}

void semaphore_give(semaphore_t *sem){
	// The running task is fine unless a higher priority task is released by the mutex
	// Just dequeue from block list and enqueue to its proper queue in priority level
	
	if(sem->block_list.head == NULL)
		sem->count++;
	else{
		tcb_t * freed_task = dequeue(&sem->block_list);
		// Add unblocked task to scheduler ready queue
		enqueue(&scheduler.ready_lists[freed_task->priority], freed_task);
		if(freed_task->priority < scheduler.running_task->priority){ // lower number is higher priority
			// Update priority to scheduler can switch to higher priority
			scheduler.current_priority = freed_task->priority;
			run_scheduler = true;
		}
	}
}




