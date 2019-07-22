#include <LPC17xx.h>
#include <stdint.h>
#include <stdio.h>

#include "rtos.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ GENERAL OS DEMO ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*

semaphore_t block_sem;

void task1(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d", print_val);
		delay_count++;
		if(delay_count == 1000){
			printf("--------------------FIVE-----------------------------");
			semaphore_take(&block_sem);
		}
	}
}

void task2(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) { 
		printf("%d", print_val);
		delay_count++;
		if(delay_count == 500){
			printf("--------------------FOUR-----------------------------");
			semaphore_take(&block_sem);
		}
	}
}

void task3(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d", print_val);
		delay_count++;
		if(delay_count == 1000){
			printf("--------------------THREE-----------------------------");
			semaphore_take(&block_sem);
		}
	}
}

void task4(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d", print_val);
		delay_count++;
		if(delay_count == 500){
			printf("--------------------TWO-----------------------------");
			semaphore_take(&block_sem);
		}
	}
}

void task5(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d", print_val);
		delay_count++;
		if(delay_count == 500){
			printf("--------------------ONE-----------------------------");
			semaphore_take(&block_sem);
		}
	}
}

int main(void) {
	
	volatile uint32_t test = 0x11111111;
	
	printf("\nStarting...\n\n");
	SysTick_Config(SystemCoreClock/1000);
	
	rtos_init();
	
	semaphore_init(&block_sem, 0);

	task_create(task1, (void*)0x1, LOW);
	task_create(task2, (void*)0x2, LOW);
	task_create(task3, (void*)0x3, MEDIUM);
	task_create(task4, (void*)0x4, MEDIUM);
	task_create(task5, (void*)0x5, HIGH);
	
	while(true) {
		printf("0");
	}
}

*/



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SEMAPHORE TEST ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*

// Tasks 1 -> MEDIUM
// Tasks 2,3 -> LOW

// 0 - Tasks 1 run at medium priority.
// ONE - Tasks 1 takes and blocks on semaphore, tasks 2,3 run on low priority.
// TWO - Task 2 gives semaphore, unblocking task 1, task 1 runs on medium priority.
// THREE - Task 1 gives 4 to semaphore.
// FOUR - Task 1 takes 3 from semaphore.
// FIVE - Task 1 takes 1 from semaphore
// SIX - Task 1 takes 1 from semaphore
// SEVEN - Task 2 takes 1 from semaphore

semaphore_t blocker_semaphore;

void task1(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d", print_val);
		delay_count++;
		if(delay_count == 500){
			printf("--------------------ONE-----------------------------");
			semaphore_take(&blocker_semaphore);
		}
		if(delay_count == 1000){
			printf("--------------------THREE--------------------------");
			semaphore_give(&blocker_semaphore);
			semaphore_give(&blocker_semaphore);
			semaphore_give(&blocker_semaphore);
			semaphore_give(&blocker_semaphore);
		}
		
		if(delay_count == 1500){
			printf("-----------------FOUR--------------------------");
			semaphore_take(&blocker_semaphore);
			semaphore_take(&blocker_semaphore);
			semaphore_take(&blocker_semaphore);
		}
		if(delay_count == 2000){
			printf("-------------------FIVE---------------------------");
			semaphore_take(&blocker_semaphore);
		}
		if(delay_count == 2500){
			printf("---------------------SIX---------------------------");
			semaphore_take(&blocker_semaphore);
		}
	}
}

void task2(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d", print_val);
		delay_count++;
		if(delay_count == 500){
			printf("--------------------TWO-----------------------------");
			semaphore_give(&blocker_semaphore);
		}
		if(delay_count == 1300){
			printf("--------------------SEVEN-----------------------------");
			semaphore_take(&blocker_semaphore);
		}
	}
}

void task3(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d", print_val);
		delay_count++;
	}
}

//void task4(void* arg){
//	static uint32_t delay_count = 0;
//	uint32_t print_val = (uint32_t) arg;
//	while(true) {
//		printf("%d", print_val);
//		delay_count++;
//		if(delay_count == 2000){
//			semaphore_take(&blocker_semaphore);
//		}
//	}
//}

//void task5(void* arg){
//	static uint32_t delay_count = 0;
//	uint32_t print_val = (uint32_t) arg;
//	while(true) {
//		printf("%d", print_val);
//		delay_count++;
//		if(delay_count == 2500){
//			semaphore_take(&blocker_semaphore);
//		}
//	}
//}

int main(void) {
	
	volatile uint32_t test = 0x11111111;
	
	printf("\nStarting...\n\n");
	SysTick_Config(SystemCoreClock/1000);
	
	rtos_init();
	
	semaphore_init(&blocker_semaphore, 0);

	task_create(task1, (void*)0x1, MEDIUM);
	task_create(task2, (void*)0x2, LOW);
	task_create(task3, (void*)0x3, LOW);
//	task_create(task4, (void*)0x4, MEDIUM);
//	task_create(task5, (void*)0x5, MEDIUM);

//	uint32_t period = 1000; // 1s
//	uint32_t prev = -period;
	while(true) {
//		if((uint32_t)(msTicks - prev) >= period){
//			printf("tick ");
//			prev += period;
//		}
		printf("IDLE ");
	}
}

*/



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MUTEX TEST ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


/*

// Task 1,3 -> LOW
// Task 2 -> HIGH

// START - high priority takes and blocks on semaphore
// ONE - low priority takes mutex
// TWO - other low priority task tries to give mutex, does not work
// THREE - low priority gives semaphore, unblocking higher task
// FOUR - high priority takes and blocks on mutex (low priority is promoted) (runs alone on high priority)
// FIVE - low priority gives mutex, is demoted, high priority takes mutex and runs alone on high priority

mutex_t blocker_mutex;
semaphore_t block_sem;

void task1(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d", print_val);
		delay_count++;
		if(delay_count == 500){
			printf("----------------ONE----------------------");
			mutex_take(&blocker_mutex);
		}
		if(delay_count == 1500){
			printf("--------------THREE---------------------");
			semaphore_give(&block_sem);
		}
		if(delay_count == 2000){
			printf("---------------FIVE-----------------------");
			mutex_give(&blocker_mutex);
		}
	}
}

void task2(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	semaphore_take(&block_sem);
	while(true) { // starts at 1000
		printf("%d", print_val);
		delay_count++;
		if(delay_count == 500){
			printf("----------------FOUR-------------------");
			mutex_take(&blocker_mutex);
		}
	}
}

void task3(void* arg){
	static uint32_t delay_count = 0;
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d", print_val);
		delay_count++;
		if(delay_count == 1000){
			printf("--------------TWO---------------------");
			mutex_give(&blocker_mutex);
		}
	}
}

//void task4(void* arg){
//	static uint32_t delay_count = 0;
//	uint32_t print_val = (uint32_t) arg;
//	while(true) {
//		printf("%d", print_val);
//		delay_count++;
//		if(delay_count == 2000){
//		}
//	}
//}

//void task5(void* arg){
//	static uint32_t delay_count = 0;
//	uint32_t print_val = (uint32_t) arg;
//	while(true) {
//		printf("%d", print_val);
//		delay_count++;
//		if(delay_count == 2500){
//		}
//	}
//}

int main(void) {
	
	volatile uint32_t test = 0x11111111;
	
	printf("\nStarting...\n\n");
	SysTick_Config(SystemCoreClock/1000);
	
	rtos_init();
	
	mutex_init(&blocker_mutex);
	semaphore_init(&block_sem, 0);

	task_create(task1, (void*)0x1, LOW);
	task_create(task2, (void*)0x2, HIGH);
	task_create(task3, (void*)0x3, LOW);
//	task_create(task4, (void*)0x4, LOW);
//	task_create(task5, (void*)0x5, LOW);

//	uint32_t period = 1000; // 1s
//	uint32_t prev = -period;
	while(true) {
//		if((uint32_t)(msTicks - prev) >= period){
//			printf("tick ");
//			prev += period;
//		}
		printf("0");
	}
}
*/
