/*
 * OsKernel.c
 *
 *  Created on: Apr 5, 2026
 *      Author: hp
 */


#include <osKernel.h>

#define NUM_OF_THREADS 3
#define STRUCT_SIZE    100 // 100 is 100 4 byte values so 400 bytes

#define BUS_FREQ         16000000
uint32_t MILI_PRESCALER;            // provides clock in ms
#define CTRL_ENABLE  (1U<<0)
#define CTRL_CLKSRC  (1U<<2)
#define CTRL_TICKINT (1U<<1)
#define CTRL_CNTFLAG (1U<<16)

#define SYSTICK_RST  0
#define SYSTICK_VAL 0

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
	MILI_PRESCALER = (BUS_FREQ/1000); // 16M is 1s, 16k is 1ms
}

void osSchedulerLaunch()
{
	// Load address of current pt in R0
	__asm("LDR R0,=current_ptr ");

	//load R2 from address R0 ie R2 equals current pt
	__asm("LDR R2,[R0]");

	//load Cortex M SP from address equal R2
	__asm("LDR SP,[R2]");

	//restore the manual registers r4,r5,r6,r7,8,r9,r10,11
	__asm("POP {R4-R11}");

	//restore r0,r1,r2,r3
	__asm("POP {R0-R3}");

	//skip LR and PSR
	__asm("ADD SP,SP,#4");

	//create a new start location by popping LR
	__asm("POP {LR}");

	// add 4 to SP to move to PSR
	__asm("ADD SP,SP,#4");

	//enable global interrupt
	__asm("CPSIE I");

	//return from the exception
	__asm("BX LR");
}

void osKernelLaunch(uint32_t quanta)
{
	//Reset Systick
	SysTick->CTRL=SYSTICK_RST;

	//Clear Systick with current value as 0
	SysTick->VAL = SYSTICK_VAL;

	// Load quanta value
	SysTick->LOAD = (quanta * MILI_PRESCALER) - 1;

	//Set systick to lowest priority
	NVIC_SetPriority(SysTick_IRQn,15);

	//Enable systick, select internal clock
	SysTick->CTRL |=CTRL_CLKSRC | CTRL_ENABLE;

	//Enable systick interrupt
	SysTick->CTRL |=CTRL_TICKINT;

	//Launch scheduler
	osSchedulerLaunch();

}

// Saved automatically on the stack : r0,r1,r2,r3,r12,LR,PC,PSR
// The Systick Handler does the context switch
__attribute__((naked)) void SysTick_Handler()
{
	// On the trigger of the ISR we want to suspend the current thread and choose the next thread
	//Suspend
	//Disable global interrupts
	__asm("CPSID I");// Change processor state, Interrupt disable - No new interrupts can interrupt this code from now on.
	// Save R4-R11
	__asm("PUSH {R4-R11}");
	//Load address of current pt into R0
	__asm("LDR R0, =current_ptr ");

	// Load address in R0 in R1
	__asm("LDR R1,[R0]");

	//Store cortex M SP at address equals R1 ie save SP into TCB
	__asm("STR SP,[R1]");
	//Choose next thread
	__asm("LDR R1,[R1,#4]"); // load R1 with value stored the address 4 bytes above R1's address

	//Store R1 at address equal R0 ie current_ptr=R1
	__asm("STR R1,[R0]");

	//load CortexM SP from address equal R1; ie SP=current_ptr->stackPt
	__asm("LDR SP,[R1]");

	//Restore the r4-r11
	__asm("POP {R4-R11}");

	//Enable global interrupts
	__asm("CPSIE I");

	//Return from exception and restore r0,r1,r2,r3,r12,LR,PC,PSR - this is automatic load
	__asm("BX LR");

}
