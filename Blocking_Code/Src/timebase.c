/*
 * timebase.c
 *
 *  Created on: Mar 21, 2026
 *      Author: hp
 */

#include "timebase.h"

#define CTRL_ENABLE  (1U<<0)
#define CTRL_CLKSRC  (1U<<2)
#define CTRL_TICKINT (1U<<1)
#define CTRL_CNTFLAG (1U<<16)

#define MAX_DELAY    0xFFFFFFFF

#define ONE_SEC_LOAD 16000000

volatile uint32_t global_current_tick;
volatile uint32_t global_current_tick_prime;
volatile uint32_t tick_frequency=1;

void timebase_init(void);
void tick_increment(void);
uint32_t get_tick(void);
void delay(uint32_t delay);

void timebase_init()
{
	// Reload the timer with number of cycles for one second
	SysTick->LOAD=ONE_SEC_LOAD-1; // load 16 million ie no of cycles 16MHz

	//Clear systick current value register
	SysTick->VAL = 0;

	//Select internal clock source
	SysTick->CTRL |=CTRL_CLKSRC;

	//Enable interrupt
	SysTick->CTRL |=CTRL_TICKINT;

	//Enable Systick
	SysTick->CTRL |=CTRL_ENABLE;

	//Enable global interrupt
	__enable_irq();
}

void tick_increment(void)
{
	// Increases the global tick count by one
	// Called everytime systick time out occurs
	global_current_tick +=tick_frequency;
}

void SysTick_Handler()
{
	tick_increment();
}

uint32_t get_tick(void)
{
	__disable_irq();
	global_current_tick_prime=global_current_tick;
	__enable_irq();

	return global_current_tick_prime;
}

// Function to implement the delay as given in the parameter
void delay(uint32_t delay)
{
	uint32_t tickStart=get_tick();
	uint32_t wait=delay;
	if(wait<MAX_DELAY)
	{
		wait+=(uint32_t)(tick_frequency);
	}

	while((get_tick()-tickStart)<wait){}
}
