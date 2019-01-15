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

	enable_gpio_pins(gpio_pins, length);

	lcd_init();
	lcd_set_cursor_position(1, 5);
	lcd_send_data('H');
	sleep(1);

	lcd_send_command(CLEAR_DISPLAY);

	lcd_set_cursor_position(1, 10);
	lcd_send_data('A');
	sleep(1);

	lcd_send_command(CLEAR_DISPLAY);

	lcd_set_cursor_position(2, 10);
	lcd_send_data('B');
	sleep(1);

	lcd_send_command(CLEAR_DISPLAY);

	lcd_set_cursor_position(2, 5);
	lcd_send_data('T');
	sleep(1);

//	for (int i=0; i<10; i++)
//	{
//		lcd_send_command(SHIFT_SCREEN_ENABLE|SHIFT_SCREEN_LEFT);
//		usleep(5000);
//		sleep(1);
//	}

//	for (int j=0; j< 3; j++)
//	{
//		for (unsigned char i=0x30; i<0x3A; i++)
//		{
//			lcd_send_data(i);
//			usleep(5000);
//		}
//	}

}
