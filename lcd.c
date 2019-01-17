#include "lcd.h"
#include <string.h>

unsigned int gpio_pins[] = {RS, EN, D4, D5, D6, D7};
unsigned int length = sizeof(gpio_pins)/sizeof(int);

void lcd_init()
{
	lcd_send_command(((FUNCTION_SET|SET_4BIT_MODE) >> 4));
	lcd_send_command(FUNCTION_SET|SET_4BIT_MODE|LINE_NUMBER_2|FONT_SIZE);
	lcd_send_command(DISPLAY|DISPLAY_ON|CURSOR_OFF|CURSOR_BLINK_OFF);
	lcd_send_command(CLEAR_DISPLAY);
	lcd_send_command(ENTRY_MODE_SET|INCREMENT_CURSOR);
}

void lcd_print_line(char *line)
{
	for (int i=0; i<strlen(line); i++)
	{
		lcd_send_data(line[i]);
	}
}

void lcd_print_custom_char_line(char line[], int length)
{
	for (int i=0; i<length; i++)
	{
		lcd_send_data(line[i]);
	}
}

void lcd_create_custom_char_map(unsigned char custom_char_map[][8], int map_size)
{
	lcd_send_command(SET_CGRAM_ADDRESS);
	for (int i=0; i<map_size; i++)
	{
		for (int j=0; j<SYMBOL_HEIGHT; j++)
		{
			lcd_send_data(custom_char_map[i][j]);
		}
	}
	lcd_set_cursor_position(1, 1);
}

void lcd_set_cursor_position(int row, int position)
{
	position--;												//memory address starts from 0
	if (row == 1  && position < 40)
	{
		lcd_send_command(SET_DDRAM_ADDRESS|FIRST_ROW|position);
	}
	else if (row==2  && position < 40)
	{
		lcd_send_command(SET_DDRAM_ADDRESS|SECOND_ROW|position);
	}
	else
	{
		printf("Wrong row number or position, row 1 or 2, position < 40\n");
	}
}

void lcd_toggle_e_pin()
{
	SET_REGISTER(EN, HIGH);
	usleep(2);
	SET_REGISTER(EN, LOW);
}

void lcd_send_command(unsigned char command)
{
	SET_REGISTER(RS, CONTROL);							//set rs bit to 0
	SET_REGISTER(EN, LOW);								//set en bit to 0

	//send 2 peaces of command because 4 bits mode
	SET_DATA((command & 0xF0) >> 4);					//send upper nibble
	lcd_toggle_e_pin();									//allow lcd to grad data
	usleep(1);
	SET_DATA(command & 0x0F);							//send lower nibble
	lcd_toggle_e_pin();									//allow lcd to grad data
	usleep(5000);
}

void lcd_send_data(unsigned char data)
{
	SET_REGISTER(RS, DATA);								//set rs bit to 0
	SET_REGISTER(EN, LOW);								//set en bit to 0

	//send 2 peaces of data because 4 bits mode
	SET_DATA((data & 0xF0) >> 4);						//send upper nibble
	lcd_toggle_e_pin();									//allow lcd to grad data
	usleep(1);
	SET_DATA(data & 0x0F);								//send lower nibble
	lcd_toggle_e_pin();									//allow lcd to grad data
	usleep(5000);
}
