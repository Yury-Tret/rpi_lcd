#include <stdio.h>
#include "rpi.h"
#include "lcd.h"
#include <string.h>

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

	int value = 0;
	char char_value[5];

	while(1)
	{
		value = get_cpu_usage(1);
		sprintf(char_value, "%d", value);

		lcd_send_command(CLEAR_DISPLAY);

		lcd_print_graph(value);

		lcd_set_cursor_position(1, 1);
		lcd_print_line("CPU");
		lcd_set_cursor_position(2, 1);
		lcd_print_line(char_value);
	}
//	while(1)
//	{
//		char str[50];
//		int row;
//		printf("Enter string and row: ");
//		scanf("%s %d", str, &row);
//		if (strcmp("exit", str)==0)
//		{
//			break;
//		}
//		if (strlen(str)>0 && (row==1 || row==2))
//		{
//			lcd_set_cursor_position(row, 1);
//			lcd_print_line(str);
//		}
//		else
//		{
//			printf("Incorrect data\n");
//		}
//	}
//
//	for (int i=0; i<6; i++)
//	{
//		lcd_send_command(SHIFT_SCREEN_ENABLE|SHIFT_SCREEN_LEFT);
//		sleep(1);
//	}

}
