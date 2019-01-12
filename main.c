#include <stdio.h>
#include "rpi.h"
#include "lcd.h"

int main(int argc, char **argv)
{
	if(map_peripheral(&gpio) == -1)
	{
		printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
		return -1;
	}
	int gpio_pins[6] = {10, 11, 12, 13, 14, 15};
	enable_gpio_pins(gpio_pins);
	lcd_init();
	lcd_send_data('A');

	// Define pin 18 as output
//	INP_GPIO(18);
//	OUT_GPIO(18);

//	for (int i=0; i < 10; i++)
//	{
//	// Toggle pin 18 (blink a led!)
//		printf("On\n");
//		GPIO_SET = 1 << 18;
//		sleep(1);
//		printf("Off\n");
//		GPIO_CLR = 1 << 18;
//		sleep(1);
//	}
}
