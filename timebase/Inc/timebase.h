/*
 * timebase.h
 *
 *  Created on: Mar 21, 2026
 *      Author: hp
 */

#ifndef TIMEBASE_H_
#define TIMEBASE_H_

#include "stm32f4xx.h"

void timebase_init(void);
void tick_increment(void);
uint32_t get_tick(void);
void delay(uint32_t delay);

#endif /* TIMEBASE_H_ */
