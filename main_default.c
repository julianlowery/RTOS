#include <LPC17xx.h>
#include <stdint.h>
#include <stdio.h>

#include "rtos.h"

//extern tcb_t tcb_array[6];
extern uint32_t msTicks;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SEMAPHORE TEST ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
semaphore_t blocker_semaphore;

void task1(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d\n", print_val);
		delay_count++;
		if(delay_count == 500){
			semaphore_take(&blocker_semaphore);
		}
		if(delay_count == 1500)
			semaphore_take(&blocker_semaphore);
		if(delay_count == 2500){
			semaphore_take(&blocker_semaphore);
			semaphore_take(&blocker_semaphore);
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
			semaphore_give(&blocker_semaphore);
			semaphore_give(&blocker_semaphore);
			semaphore_give(&blocker_semaphore);
			semaphore_give(&blocker_semaphore);
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
			semaphore_give(&blocker_semaphore);
			semaphore_give(&blocker_semaphore);
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
			semaphore_take(&blocker_semaphore);
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
			semaphore_take(&blocker_semaphore);
		}
	}
}

int main(void) {
	
	volatile uint32_t test = 0x11111111;
	
	printf("\nStarting...\n\n");
	SysTick_Config(SystemCoreClock/1000);
	printf("1\n");
	
	rtos_init();
	
	semaphore_init(&blocker_semaphore, 0);

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

// 0 - high priority takes and blocks on semaphore
// 500 - low priority takes mutex
// 1000 - low priority gives semaphore, unblocking higher task
// 1500 - high priority takes and blocks on mutex (low priority is promoted) (runs alone on high priority)
// 2000 - low priority gives mutex, is demoted, high priority takes mutex and runs alone on high priority

mutex_t blocker_mutex;
semaphore_t block_sem;

void task1(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d\n", print_val);
		delay_count++;
		if(delay_count == 500){
			printf("-------------------------------500-------------------------------------------");
			mutex_take(&blocker_mutex);
		}
		if(delay_count == 1000){
			printf("-------------------------------1000------------------------------------------");
			semaphore_give(&block_sem);
		}
		if(delay_count == 1500){
			printf("-------------------------------2000------------------------------------------");
			mutex_give(&blocker_mutex);
		}
	}
}

void task2(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	semaphore_take(&block_sem);
	while(true) { // starts at 1000
		printf("%d\n", print_val);
		delay_count++;
		if(delay_count == 500){
			printf("-------------------------------1500------------------------------------------");
			mutex_take(&blocker_mutex);
		}
	}
}
/*
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
*/
int main(void) {
	
	volatile uint32_t test = 0x11111111;
	
	printf("\nStarting...\n\n");
	SysTick_Config(SystemCoreClock/1000);
	printf("1\n");
	
	rtos_init();
	
	mutex_init(&blocker_mutex);
	semaphore_init(&block_sem, 0);

	task_create(task1, (void*)0x1, IDLE);
	task_create(task2, (void*)0x2, HIGH);
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
