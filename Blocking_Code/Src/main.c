
#include "led.h"
#include "uart.h"
#include "timebase.h"

#include<stdio.h>

int main(void)
{
	led_init();
	uart_tx_init();
	timebase_init();

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
