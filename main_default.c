#include <LPC17xx.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "rtos.h"

extern tcb_t tcb_array[6];
extern uint32_t msTicks;

void task1(void* arg){
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d\n", print_val);
	}
}

void task2(void* arg){
	uint32_t print_val = (uint32_t) arg;
	while(true) {
		printf("%d\n", print_val);
	}
}

void task3(void* arg){
	while(true) {
		printf("t3\n");
	}
}

void task4(void* arg){
	while(true) {
		printf("t4\n");
	}
}

void task5(void* arg){
	while(true) {
		printf("t5\n");
	}
}


int main(void) {
	
	volatile uint32_t test = 0x11111111;
	
	printf("\nStarting...\n\n");
	SysTick_Config(SystemCoreClock/1000);
	printf("1\n");
	
	rtos_init();

	task_create(task1, (void*)0x11111111, LOW);
	task_create(task2, (void*)0x01, LOW);
	task_create(task3, NULL, LOW);
	task_create(task4, NULL, LOW);
	task_create(task5, NULL, LOW);

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

