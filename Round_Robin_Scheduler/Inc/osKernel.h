/*
 * osKernerl.h
 *
 *  Created on: Apr 5, 2026
 *      Author: hp
 */

#ifndef OSKERNEL_H_
#define OSKERNEL_H_
#include<stdint.h>
#include "stm32f4xx.h"

void osKernelInit();
void osKernelStackInit(int i);
void osKernelLaunch(uint32_t quanta);
uint8_t osKernelAddThreads(void (*task0)(void), void (*task1)(void), void (*task2)(void));

#endif /* OSKERNEL_H_ */
