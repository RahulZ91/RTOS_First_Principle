/*
 * uart.c
 *
 *  Created on: Mar 18, 2026
 *      Author: hp
 */

#include "uart.h"
#define GPIO_PIN_2    (1U<<8)|(1U<<9)|(1U<<10)
#define GPIOAEN       (1U<<0)
#define GPIO_MODE_AF  (1U<<5)
#define UART2_EN      (1U<<17)

#define SYS_FREQ      160000000
#define APB1_CLK      SYS_FREQ
#define UART_BAUDRATE 115200

static void uart_compute_BaudRate(uint32_t peri_clock,uint32_t baud_rate);
static void uart_set_baud_rate(uint32_t peri_clock,uint32_t baud_rate);

void uart_tx_init(void)
{
	// Enable clock for Port A
	RCC->AHB1ENR=GPIOAEN;

	//Set alternate mode for PA2
	GPIOA->MODER &= ~(1U<<4);
	GPIOA->MODER |= GPIO_MODE_AF;

	//Set alternate function to AF7 for the above PA2
	GPIOA->AFR[0] &= ~(1U<<11);
	GPIOA->AFR[0] |= GPIO_PIN_2;

	// Enable clock for USART2 - USART2 is chosen since it is the same peripheral that the USB uses
	RCC->APB1ENR |= UART2_EN;

	// Configure the Baud rate
	uart_set_baud_rate(APB1_CLK,UART_BAUDRATE);

	// Set the transfer direction to TX

	//Enable the UART module
}

static void uart_set_baud_rate(uint32_t peri_clock,uint32_t baud_rate)
{
	USART2->BRR=UART_Compute_BaudRate(peri_clock,baud_rate);
}

static void uart_compute_BaudRate(uint32_t peri_clock,uint32_t baud_rate)
{
	((peri_clock+(baud_rate/2U))/baud_rate);
}
