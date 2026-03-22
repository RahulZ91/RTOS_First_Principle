/*
 * uart.c
 *
 *  Created on: Mar 18, 2026
 *      Author: hp
 */

#include "uart.h"
#include<stdio.h>
#include<stdint.h>
#define GPIO_PIN_2    (1U<<8)|(1U<<9)|(1U<<10)
#define GPIOAEN       (1U<<0)
#define GPIO_MODE_AF  (1U<<5)
#define UART2_EN      (1U<<17)

#define SYS_FREQ      16000000
#define APB1_CLK      SYS_FREQ
#define UART_BAUDRATE 115200

#define CR1_TE		  (1U<<3)
#define CR1_UE        (1U<<13)
#define SR_TXE        (1U<<7)

static uint16_t uart_compute_BaudRate(uint32_t peri_clock,uint32_t baud_rate);
static void uart_set_baud_rate(uint32_t peri_clock,uint32_t baud_rate);
static void uart_write(int ch);
int __io_putchar(int ch);

int __io_putchar(int ch)
{
	uart_write(ch);
	return ch;
}

void uart_tx_init(void)
{
	// Enable clock for Port A
	RCC->AHB1ENR |=GPIOAEN;

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

	// Set the transfer direction to TX - USART control register 1
	USART2->CR1 = CR1_TE; // set everything to 0 except TE

	//Enable the UART module
	USART2->CR1 |=CR1_UE;
}

static void uart_write(int ch)
{
	// ensure that the transmit data register is empty- if empty then only transmit
	while(!(USART2->SR & SR_TXE)){}

	// write the data
	USART2->DR = (ch & 0xFF);

}

static void uart_set_baud_rate(uint32_t peri_clock,uint32_t baud_rate)
{
	USART2->BRR=uart_compute_BaudRate(peri_clock,baud_rate);
}

static uint16_t uart_compute_BaudRate(uint32_t peri_clock,uint32_t baud_rate)
{
	return ((peri_clock+(baud_rate/2U))/baud_rate);
}
