
#include "led.h"
#include "uart.h"
#include "timebase.h"

#include<stdio.h>

void motor_run(void);
void motor_stop(void);
void valve_open(void);
void valve_close(void);

int main_motor()
{
	while(1)
	{
		motor_run();
		delay(1);
		motor_stop();
		delay(1);
	}
}
int main(void)
{
	led_init();
	uart_tx_init();
	timebase_init();

	main_motor();
	while(1)
	{
		printf("A second occurred \n\r");
		delay(1);
		led_on();
		printf("LED on for 5 seconds \n\r");
		delay(5);
		led_off();
		printf("LED switched off after 5 seconds \n\r");
		printf("Ticks done %ld \n\r",get_tick());
	}
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


