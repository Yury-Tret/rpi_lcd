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

	int gpio_pins[] = {RS, EN, D4, D5, D6, D7};
	int length = sizeof(gpio_pins)/sizeof(int);
	enable_gpio_pins(gpio_pins, length);

	lcd_init();
	usleep(5000);

	lcd_send_data('R');
	usleep(5000);
	lcd_send_data('2');
	usleep(5000);
	lcd_send_data('D');
	usleep(5000);
	lcd_send_data('3');
	usleep(5000);
	lcd_send_data('s');
	usleep(5000);
	lcd_send_data('h');
	usleep(5000);
	lcd_send_data('-');
	usleep(5000);
	lcd_send_data('*');
	usleep(5000);
}
