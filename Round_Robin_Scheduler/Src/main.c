
#include "led.h"
#include "uart.h"
#include "timebase.h"
#include "osKernel.h"

#include<stdio.h>

#define QUANTA 10

void motor_run(void);
void motor_stop(void);
void valve_open(void);
void valve_close(void);

typedef uint32_t TaskProfiler;
TaskProfiler Task0_Profiler,Task1_Profiler,Task2_Profiler; // counter to check if the thread has executed

void task0(void)
{
	while(1)
	{
		Task0_Profiler++;
	}
}

void task1(void)
{
	while(1)
	{
		Task1_Profiler++;
	}
}

void task2(void)
{
	while(1)
	{
		Task2_Profiler++;
	}
}

int main()
{
	uart_tx_init();

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


