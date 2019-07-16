#include <LPC17xx.h>
#include <stdio.h>
#include <stdbool.h>

#include "scheduler.h"
#include "context.h"

uint32_t msTicks = 0;
uint32_t msp_enable_not = (1<<1);

tcb_t tcb_array[6];
tcb_t tcb_main;

bool run_scheduler = false;
tcb_t *switch_to_tcb;

scheduler_t scheduler = {
	.running_task = &tcb_array[0],
	.current_priority = IDLE
};

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
	// Store old task registers on task stack first thing before registers are messed with
	old_task_tcb->stack_pointer = (uint32_t*)storeContext();
	
	// Check for overflow on old task
	if(old_task_tcb->stack_pointer <= old_task_tcb->stack_overflow_address)
		return false;
	
	// Restore context of new task from stack
	restoreContext((uint32_t)new_task_tcb->stack_pointer);
	return true;
}

void SysTick_Handler(void) {
	const uint32_t time_slice_len = 1000;
	const uint32_t set_pendsv = (1<<28);
	static uint32_t time_slice_count = time_slice_len;
	
  msTicks++;
	time_slice_count--;
	
	if(time_slice_count == 0 || run_scheduler){
		time_slice_count = time_slice_len;
		run_scheduler = false;
		
		// Set pendSV exception to pending
		SCB->ICSR=(SCB->ICSR|set_pendsv);
	}
}

void PendSV_Handler(void){
	
	/*
	// For testing context swtich
	static uint8_t task_number = 0;
	task_number %= 6;
	uint8_t next_task_number = (task_number+1)%6;
	
	context_switch(&tcb_array[task_number], &tcb_array[next_task_number]);

	task_number++;
	*/
	
		// Check if scheduler was called for new high priority task
	if(scheduler.current_priority < scheduler.running_task->priority){
		priority_t old_priority = scheduler.running_task->priority;
		tcb_t *old_tcb = scheduler.ready_lists[old_priority].head;
		
		// Update running task to new task
		scheduler.running_task = switch_to_tcb;
		
		// switch_to_tcb is global, was set in add_task_to_sched
		context_switch(old_tcb, switch_to_tcb);
		switch_to_tcb = NULL;
	}
	// FIX THIS STATEMENT, very obscure and logic is wrong
	else if(scheduler.ready_lists[scheduler.current_priority].head->tcb_pointer != NULL){
		tcb_t *old_task = scheduler.running_task;
		tcb_t *new_task = scheduler.running_task->tcb_pointer;
		
		tcb_list_t *list = &scheduler.ready_lists[scheduler.current_priority];
		
		dequeue(list, old_task);
		enqueue(list, old_task);
		
		context_switch(old_task, new_task);
	}
	
	
	const uint32_t clear_pendsv = (1<<27);
	// Remove pending state from pendSV exception
	SCB->ICSR=(SCB->ICSR|clear_pendsv);
}

void enqueue(tcb_list_t *list, tcb_t *tcb){
	if(list->head == NULL){
		list->head = tcb;
		list->tail = tcb;
	}
	else{
		list->tail->tcb_pointer = tcb;
		list->tail = tcb;
	}
	list->size++;
	return;
}

void dequeue(tcb_list_t *list, tcb_t *tcb){
	if(list->head == NULL)
		return;
	else{
		// Set dequeued tcb's tcb_pointer to NULL and set new head
		list->head = tcb->tcb_pointer;
		tcb->tcb_pointer = NULL;
		
	}
}

void add_task_to_sched(scheduler_t *scheduler, tcb_t *tcb){
	priority_t priority = tcb->priority;
	
	// Get ready list of proper priority
	tcb_list_t *list = &scheduler->ready_lists[priority];
	
	enqueue(list, tcb);
	
	// Check if added task is of higher priority than current running task (lower value = higher priority)
	if(priority < scheduler->current_priority){
		// Update scheduler with new priority and new task to run
		scheduler->current_priority = priority;
		
		// Call scheduler to run next tick and set tcb global to be switched to
		run_scheduler = true;
		switch_to_tcb = tcb;
	}
}















/*
void run_scheduler(scheduler_t *scheduler){
	// Check if scheduler was called for new high priority task
	if(scheduler->current_priority > scheduler->running_task->priority){
		priority_t old_priority = scheduler->current_priority;
		tcb_t *old_tcb = scheduler->ready_lists[old_priority].head;
		context_switch(scheduler->running_task, old_tcb);
	}
	else{
		
	}
}
*/












