#include <LPC17xx.h>
#include <stdint.h>
#include <stdio.h>

#include "rtos.h"

//extern tcb_t tcb_array[6];
extern uint32_t msTicks;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SEMAPHORE TEST ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
semaphore_t blocker;

void task1(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d\n", print_val);
		delay_count++;
		if(delay_count == 500){
			semaphore_take(&blocker);
		}
		if(delay_count == 1500)
			semaphore_take(&blocker);
		if(delay_count == 2500){
			semaphore_take(&blocker);
			semaphore_take(&blocker);
		}
	}
}

void task2(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d\n", print_val);
		delay_count++;
		if(delay_count == 1000){
			semaphore_give(&blocker);
		}
		if(delay_count == 2000){
			semaphore_give(&blocker);
			semaphore_give(&blocker);
			semaphore_give(&blocker);
			semaphore_give(&blocker);
		}
	}
}

void task3(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d\n", print_val);
		delay_count++;
		if(delay_count == 1500){
			semaphore_give(&blocker);
			semaphore_give(&blocker);
		}
	}
}

void task4(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d\n", print_val);
		delay_count++;
		if(delay_count == 2000){
			semaphore_take(&blocker);
		}
	}
}

void task5(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d\n", print_val);
		delay_count++;
		if(delay_count == 2500){
			semaphore_take(&blocker);
		}
	}
}

int main(void) {
	
	volatile uint32_t test = 0x11111111;
	
	printf("\nStarting...\n\n");
	SysTick_Config(SystemCoreClock/1000);
	printf("1\n");
	
	rtos_init();
	
	semaphore_init(&blocker, 0);

	task_create(task1, (void*)0x1, LOW);
	task_create(task2, (void*)0x2, IDLE);
//	task_create(task3, (void*)0x3, IDLE);
//	task_create(task4, (void*)0x4, LOW);
//	task_create(task5, (void*)0x5, LOW);

//	uint32_t period = 1000; // 1s
//	uint32_t prev = -period;
	while(true) {
//		if((uint32_t)(msTicks - prev) >= period){
//			printf("tick ");
//			prev += period;
//		}
		printf("t_main\n");
	}
}
*/



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MUTEX TEST ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void task1(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d\n", print_val);
		delay_count++;
		if(delay_count == 500){
		}
		if(delay_count == 1500){
		}
		if(delay_count == 2500){
		}
	}
}

void task2(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d\n", print_val);
		delay_count++;
		if(delay_count == 1000){
		}
		if(delay_count == 2000){
		}
	}
}

void task3(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d\n", print_val);
		delay_count++;
		if(delay_count == 1500){
		}
	}
}

void task4(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d\n", print_val);
		delay_count++;
		if(delay_count == 2000){
		}
	}
}

void task5(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d\n", print_val);
		delay_count++;
		if(delay_count == 2500){
		}
	}
}

int main(void) {
	
	volatile uint32_t test = 0x11111111;
	
	printf("\nStarting...\n\n");
	SysTick_Config(SystemCoreClock/1000);
	printf("1\n");
	
	rtos_init();

	task_create(task1, (void*)0x1, LOW);
	task_create(task2, (void*)0x2, IDLE);
	task_create(task3, (void*)0x3, IDLE);
	task_create(task4, (void*)0x4, LOW);
	task_create(task5, (void*)0x5, LOW);

//	uint32_t period = 1000; // 1s
//	uint32_t prev = -period;
	while(true) {
//		if((uint32_t)(msTicks - prev) >= period){
//			printf("tick ");
//			prev += period;
//		}
		printf("t_main\n");
	}
}
