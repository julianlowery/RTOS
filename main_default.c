#include <LPC17xx.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "rtos.h"

extern tcb_t tcb_array[6];
extern uint32_t msTicks;

void task1(void* arg){
	static int n = 0;
	n = (int)arg;
	n++;
}

void task2(void* arg){
	static int n = 0;
	n = (int)arg;
	n++;
}

void task3(void* arg){
	static int n = 0;
	n = (int)arg;
	n++;
}

void task4(void* arg){
	static int n = 0;
	n = (int)arg;
	n++;
}


int main(void) {
	
	printf("\nStarting...\n\n");
	SysTick_Config(SystemCoreClock/1000);
	printf("1\n");
	
	volatile uint32_t test = 0x12345678;
	
	rtos_init();

	task_create(task1, (void*)0x11111111, LOW);
	task_create(task2, NULL, LOW);
	task_create(task3, NULL, LOW);
	task_create(task4, NULL, LOW);

	uint32_t period = 1000; // 1s
	uint32_t prev = -period;
	while(true) {
		if((uint32_t)(msTicks - prev) >= period){
			printf("tick ");
			prev += period;
		}
	}
}
