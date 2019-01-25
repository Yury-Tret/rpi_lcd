#include "lcd.h"
#include <string.h>
#include <stdlib.h>

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

void lcd_print_graph(int value)
{
	value -= value%10;
	char *line_1 = (char *)calloc(GRAPH_ARRAY_SIZE, sizeof(char));
	char *line_2 = (char *)calloc(GRAPH_ARRAY_SIZE, sizeof(char));
	switch (value)
	{
		case 0:
			break;
		case 10:
			memcpy(line_2, (char [GRAPH_ARRAY_SIZE]){GRAPH_10}, GRAPH_ARRAY_SIZE);
			break;
		case 20:
			memcpy(line_2, (char [GRAPH_ARRAY_SIZE]){GRAPH_20}, GRAPH_ARRAY_SIZE);
			break;
		case 30:
			memcpy(line_2, (char [GRAPH_ARRAY_SIZE]){GRAPH_30}, GRAPH_ARRAY_SIZE);
			break;
		case 40:
			memcpy(line_2, (char [GRAPH_ARRAY_SIZE]){GRAPH_40}, GRAPH_ARRAY_SIZE);
			break;
		case 50:
			memcpy(line_2, (char [GRAPH_ARRAY_SIZE]){GRAPH_50}, GRAPH_ARRAY_SIZE);
			break;
		case 60:
			memcpy(line_1, (char [GRAPH_ARRAY_SIZE]){GRAPH_10}, GRAPH_ARRAY_SIZE);
			memcpy(line_2, (char [GRAPH_ARRAY_SIZE]){GRAPH_60}, GRAPH_ARRAY_SIZE);
			break;
		case 70:
			memcpy(line_1, (char [GRAPH_ARRAY_SIZE]){GRAPH_20}, GRAPH_ARRAY_SIZE);
			memcpy(line_2, (char [GRAPH_ARRAY_SIZE]){GRAPH_70}, GRAPH_ARRAY_SIZE);
			break;
		case 80:
			memcpy(line_1, (char [GRAPH_ARRAY_SIZE]){GRAPH_30}, GRAPH_ARRAY_SIZE);
			memcpy(line_2, (char [GRAPH_ARRAY_SIZE]){GRAPH_80}, GRAPH_ARRAY_SIZE);
			break;
		case 90:
			memcpy(line_1, (char [GRAPH_ARRAY_SIZE]){GRAPH_40}, GRAPH_ARRAY_SIZE);
			memcpy(line_2, (char [GRAPH_ARRAY_SIZE]){GRAPH_90}, GRAPH_ARRAY_SIZE);
			break;
		case 100:
			memcpy(line_1, (char [GRAPH_ARRAY_SIZE]){GRAPH_50}, GRAPH_ARRAY_SIZE);
			memcpy(line_2, (char [GRAPH_ARRAY_SIZE]){GRAPH_100}, GRAPH_ARRAY_SIZE);
			break;
		default:
			printf("Incorrect graph value\n");
	}

	if (value != 0)
	{
		if (value > 50)
		{
			lcd_set_cursor_position(1, 12);
			lcd_print_custom_char_line(line_1);
		}

		lcd_set_cursor_position(2, 7);
		lcd_print_custom_char_line(line_2);
	}

	free(line_1);
	free(line_2);
}

void lcd_print_line(char *line)
{
	for (int i=0; i<strlen(line); i++)
	{
		lcd_send_data(line[i]);
	}
}

void lcd_print_custom_char_line(char *line)
{
	for (int i=0; i<GRAPH_ARRAY_SIZE; i++)
	{
		if (i==0 || (i > 0 && line[i] != 0))
		{
			lcd_send_data(line[i]);
		}
	}
}

void lcd_create_custom_char_map(unsigned char custom_char_map[][SYMBOL_HEIGHT], int map_size)
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
	position--;											//memory address starts from 0
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

void lcd_zero_all_pins()
{
	SET_REGISTER(RS, LOW);
	SET_REGISTER(EN, LOW);
	SET_DATA(LOW);
}

void lcd_send_command(unsigned char command)
{
	SET_REGISTER(RS, CONTROL);							//set rs bit to 0
	SET_REGISTER(EN, LOW);								//set en bit to 0

	//send 2 peaces of command because 4 bits mode
	SET_DATA((command & 0xF0) >> 4);					//send upper nibble
	lcd_toggle_e_pin();									//allow lcd to grab data
	usleep(1);
	SET_DATA(command & 0x0F);							//send lower nibble
	lcd_toggle_e_pin();									//allow lcd to grab data
	usleep(5000);
	lcd_zero_all_pins();
}

void lcd_send_data(unsigned char data)
{
	SET_REGISTER(RS, DATA);								//set rs bit to 0
	SET_REGISTER(EN, LOW);								//set en bit to 0

	//send 2 peaces of data because 4 bits mode
	SET_DATA((data & 0xF0) >> 4);						//send upper nibble
	lcd_toggle_e_pin();									//allow lcd to grab data
	usleep(1);
	SET_DATA(data & 0x0F);								//send lower nibble
	lcd_toggle_e_pin();									//allow lcd to grab data
	usleep(5000);
	lcd_zero_all_pins();
}
