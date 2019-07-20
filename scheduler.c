#include <LPC17xx.h>
#include <stdio.h>

#include "scheduler.h"
#include "context.h"

uint32_t msTicks = 0;
uint32_t msp_enable_not = (1<<1);

extern tcb_t tcb_array[6];

tcb_t tcb_array[6];
tcb_t tcb_main;

bool run_scheduler = false;

scheduler_t scheduler = {
	.running_task = &tcb_array[0],
	.current_priority = IDLE
};


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
	const uint32_t time_slice_len = 100;
	const uint32_t set_pendsv = (1<<28);
	static uint32_t time_slice_count = time_slice_len;
	
  msTicks++;
	time_slice_count--;
	
	if(time_slice_count == 0){
		time_slice_count = time_slice_len;
		
		// Rearrange ready queue for scheduler
		tcb_t *old_task = scheduler.running_task;
		
		tcb_list_t *list = &scheduler.ready_lists[scheduler.current_priority];
		
		dequeue(list);
		enqueue(list, old_task);
		
		// Set pendSV exception to pending
		SCB->ICSR=(SCB->ICSR|set_pendsv);
	}
	if(run_scheduler){
		run_scheduler = false;
		
		// Set pendSV exception to pending
		SCB->ICSR=(SCB->ICSR|set_pendsv);
	}
}

void PendSV_Handler(void){
	
	// Check if scheduler was called for new higher priority task
	if(scheduler.current_priority < scheduler.running_task->priority){
		priority_t old_priority = scheduler.running_task->priority;
		
		tcb_t *old_tcb;
		
		// If old task just released a mutex it will be at the tail of its queue
		if(scheduler.running_task->mutex_released)
			old_tcb = scheduler.ready_lists[old_priority].tail;
		else
			old_tcb = scheduler.ready_lists[old_priority].head;
		
		tcb_t *new_tcb = scheduler.ready_lists[scheduler.current_priority].head;
		
		// Update running task to new task
		scheduler.running_task = new_tcb;
		
		// switch_to_tcb is global, was set in add_task_to_sched
		context_switch(old_tcb, new_tcb);
	}
	
		// Else the last task of the current priority level must have been removed
	else if(scheduler.ready_lists[scheduler.current_priority].head == NULL){		
		tcb_t *old_task = scheduler.running_task;
		
		while(scheduler.ready_lists[scheduler.current_priority].head == NULL){
			scheduler.current_priority++; // Incrementing is decreasing priority
		}
		
		tcb_t *new_task = scheduler.ready_lists[scheduler.current_priority].head;
		scheduler.running_task = new_task;
		context_switch(old_task, new_task);
	}
	
	// Else run round-robin for the current priority level
	else{
		tcb_t *old_task = scheduler.running_task;
		tcb_t *new_task = scheduler.ready_lists[scheduler.current_priority].head;
		
		scheduler.running_task = new_task;
		
		context_switch(old_task, new_task);
	}
	
	// Remove pending state from pendSV exception
	const uint32_t clear_pendsv = (1<<27);
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

tcb_t* dequeue(tcb_list_t *list){
	if(list->head == NULL)
		return NULL;
	else if(list->head == list->tail){
		tcb_t* temp_ptr = list->head;
		temp_ptr->tcb_pointer = NULL;
		list->head = NULL;
		list->tail = NULL;
		list->size--;
		return temp_ptr;
	}
	else{
		// Set dequeued tcb's tcb_pointer to NULL and set new head
		tcb_t* temp_ptr = list->head;
		list->head = list->head->tcb_pointer;
		temp_ptr->tcb_pointer = NULL;
		list->size--;
		return temp_ptr;
	}
}

tcb_t* remove_from_list(tcb_list_t *list, tcb_t *tcb){
	if(list->head == NULL)
		return NULL;
	else{
		tcb_t *current = list->head;
		tcb_t *previous = NULL;
		
		// If tcb to be returned is first in list
		if(current == tcb){
			if(current->tcb_pointer == NULL){
				list->head = NULL;
			}
			else{
				list->head = current->tcb_pointer;
				current->tcb_pointer = NULL;
			}
			list->size--;
			return current;
		}
		
		// Iterate through list to find tcb
		while(current != tcb && current != NULL){
			previous = current;
			current = current->tcb_pointer;
		}
		if(current == NULL){
			return NULL;
		}
		
		// Two cases, tcb being removed is last in list, or it is not last in list
		if(current->tcb_pointer == NULL){
			previous->tcb_pointer = NULL;
		}
		else{
			previous->tcb_pointer = current->tcb_pointer;
			current->tcb_pointer = NULL;
		}
		list->size--;
		return current;
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
	}
}
