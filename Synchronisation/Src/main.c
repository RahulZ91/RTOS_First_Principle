
#include "led.h"
#include "uart.h"
#include "timebase.h"
#include "osKernel.h"

#include<stdio.h>

#define QUANTA 3

void motor_run(void);
void motor_stop(void);
void valve_open(void);
void valve_close(void);

typedef uint32_t TaskProfiler;
TaskProfiler Task0_Profiler,Task1_Profiler,Task2_Profiler,periodicProfiler; // counter to check if the thread has executed

int32_t semaphore1,semaphore2;

void task3(void)
{
	// in this periodic thread there is no while(1)
	periodicProfiler++;
}

void task0(void)
{
	while(1)
	{
		Task0_Profiler++;
		osThreadYeild();
	}
}

void task1(void)
{
	while(1)
	{
		osSemaphoreWait(&semaphore1);
		Task1_Profiler++;
		motor_run();
		osSemaphoreSet(&semaphore1);
	}
}

void task2(void)
{
	while(1)
	{
		osSemaphoreWait(&semaphore2);
		Task2_Profiler++;
		valve_open();
	}
}

int main()
{
	uart_tx_init();

	timer2_init();  // HW timer init

	// Initialise sempahore
	osSemaphoreInit(&semaphore1, 1); // active
	osSemaphoreInit(&semaphore2, 0); // inactive

	// Initialise kernel
	osKernelInit();

	// Add threads
	osKernelAddThreads(&task0,&task1,&task2);

	// Set quanta for round robin
	osKernelLaunch(QUANTA);

}

void motor_run(void)
{
	printf("Motor starts \n\r");
}

void motor_stop(void)
{
	printf("Motor stops \n\r");
}

void valve_open(void)
{
	printf("Valve open \n\r");
}

void valve_close(void)
{
	printf("Valve close \n\r");
}

void TIM2_IRQHandler(void)
{
	// Clear the interrupt flag
	TIM2->SR |=~(1<<0);


}
