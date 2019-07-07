#include <LPC17xx.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "rtos.h"

uint32_t msTicks = 0;

void SysTick_Handler(void) {
    msTicks++;
}

/*
void task1(arg){
	static int n = 0;
	n++;
}
*/

int main(void) {
	SysTick_Config(SystemCoreClock/1000);
	printf("\nStarting...\n\n");
	
	rtos_init();
	
//	task_create(task1, NULL, LOW);
	
	uint32_t period = 1000; // 1s
	uint32_t prev = -period;
	while(true) {
		if((uint32_t)(msTicks - prev) >= period){
			printf("tick ");
			prev += period;
		}
	}
}
