/*
 * OsKernel.c
 *
 *  Created on: Apr 5, 2026
 *      Author: hp
 */


#include "osKernerl.h"

#define NUM_OF_THREADS 3
#define STRUCT_SIZE    100 // 100 is 100 4 byte values so 400 bytes

struct tcb
{
	int32_t *stackPt;
	struct tcb *nextPt;
};

typedef struct tcb tcb_type; // alias struct tcb as tcb_type

tcb_type tcbs[NUM_OF_THREADS];
tcb_type *current_ptr;

// Initialise the stack in each thread
int32_t TCB_STACK[NUM_OF_THREADS][STRUCT_SIZE]; // basically mem alocation - 3 thread with stack of 400 byte each

void osKernelStackInit(int i)
{
	tcbs[i].stackPt = &TCB_STACK[i][STRUCT_SIZE - 16]; // stack pointer
	TCB_STACK[i][STRUCT_SIZE - 1] = (1<<21); // Set T Bit in PSR

	// Dummy value in  fed in regsiters to verify the value later on in the memory window
	TCB_STACK[i][STRUCT_SIZE - 3] = 0xAAAAAAAA;
	TCB_STACK[i][STRUCT_SIZE - 4] = 0xAAAAAAAA;
	TCB_STACK[i][STRUCT_SIZE - 5] = 0xAAAAAAAA;
	TCB_STACK[i][STRUCT_SIZE - 6] = 0xAAAAAAAA;
	TCB_STACK[i][STRUCT_SIZE - 7] = 0xAAAAAAAA;
	TCB_STACK[i][STRUCT_SIZE - 8] = 0xAAAAAAAA;
	TCB_STACK[i][STRUCT_SIZE - 9] = 0xAAAAAAAA;
	TCB_STACK[i][STRUCT_SIZE - 10] = 0xAAAAAAAA;
	TCB_STACK[i][STRUCT_SIZE - 11] = 0xAAAAAAAA;
	TCB_STACK[i][STRUCT_SIZE - 12] = 0xAAAAAAAA;
	TCB_STACK[i][STRUCT_SIZE - 13] = 0xAAAAAAAA;
	TCB_STACK[i][STRUCT_SIZE - 14] = 0xAAAAAAAA;
	TCB_STACK[i][STRUCT_SIZE - 15] = 0xAAAAAAAA;
	TCB_STACK[i][STRUCT_SIZE - 16] = 0xAAAAAAAA;

}

uint8_t osKernelAddThreads(void (*task0)(void), void (*task1)(void), void (*task2)(void))
{
	// disable global interrupts
	__disable_irq();
	tcbs[0].nextPt = &tcbs[1]; // next thread to be executed after quanta completes
	tcbs[1].nextPt = &tcbs[2]; // next thread to be executed after quanta completes
	tcbs[2].nextPt = &tcbs[0]; // next thread to be executed after quanta completes

	// Initialise for thread 0
	osKernelStackInit(0);
	//Initialise PC
	TCB_STACK[0][STRUCT_SIZE - 2] = (uint32_t)(task0);

	// Initialise for thread 1
	osKernelStackInit(1);
	//Initialise PC
	TCB_STACK[1][STRUCT_SIZE - 2] = (uint32_t)(task1);

	// Initialise for thread 2
	osKernelStackInit(2);
	//Initialise PC
	TCB_STACK[2][STRUCT_SIZE - 2] = (uint32_t)(task2);

	//Start from thread 0
	current_ptr= &tcbs[0];

	// enable global interrupts
	__enable_irq();

	return 1;

}

void osKernelInit()
{

}

void osKernelLaunch(uint32_t quanta)
{

}


