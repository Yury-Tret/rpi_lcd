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

	unsigned char custom_char_map[][SYMBOL_HEIGHT]={{
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00001,
		0b01111
	},
	{
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00111,
		0b11111,
		0b11111
	},
	{
		0b00000,
		0b00000,
		0b00000,
		0b00001,
		0b01111,
		0b11111,
		0b11111,
		0b11111
	},
	{
		0b00000,
		0b00000,
		0b00011,
		0b11111,
		0b11111,
		0b11111,
		0b11111,
		0b11111
	},
	{
		0b00001,
		0b01111,
		0b11111,
		0b11111,
		0b11111,
		0b11111,
		0b11111,
		0b11111
	}};

	int map_size = sizeof(custom_char_map)/sizeof(*custom_char_map);
	lcd_create_custom_char_map(custom_char_map, map_size);

	lcd_set_cursor_position(2, 7);
	char line1[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	lcd_print_custom_char_line(line1, sizeof(line1));

	lcd_set_cursor_position(1, 12);
	char line[] = {0x00, 0x01, 0x02, 0x03, 0x04};
	lcd_print_custom_char_line(line, sizeof(line));

	lcd_set_cursor_position(1, 1);
	lcd_print_line("CPU");
	lcd_set_cursor_position(2, 1);
	lcd_print_line("5%");

//	for (int i=0; i<10; i++)
//	{
//		lcd_send_command(SHIFT_SCREEN_ENABLE|SHIFT_SCREEN_LEFT);
//		usleep(5000);
//		sleep(1);
//	}

}
