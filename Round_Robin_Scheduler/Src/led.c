/*
 * led.c
 *
 *  Created on: Mar 18, 2026
 *      Author: hp
 */

#include "led.h"

#define GPIOAEN  (1U<<0)
#define LED_PIN  (1U<<5)

void led_init(void)
{

	// enable the clock for led port(Port A)
	// RCC points to the base address of RCC structure as from the stm32f4xx.h and we simply dereference its AHB1 enable register
	// and set it to enable clock for Port A which is on the AHB1 bus
	RCC->AHB1ENR |= GPIOAEN;

	// Set the LED as output - GPIO structure
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);
}

void led_on(void)
{
	// Set the LED pin HIGH
	GPIOA->ODR |=LED_PIN;   //  ODR is the output data register
}

void led_off(void)
{
	// Set the LED pin LOW
	GPIOA->ODR &= ~LED_PIN;   //  ODR is the output data register
}
