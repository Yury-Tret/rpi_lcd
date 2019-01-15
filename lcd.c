#include "lcd.h"

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

void lcd_set_cursor_position(int row, int position)
{
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
	SET_REGISTER(RS, CONTROL);								//set rs bit to 0
	SET_REGISTER(EN, LOW);								//set en bit to 0

	//send 2 peaces of command because 4 bits mode
	SET_DATA((command & 0xF0) >> 4);						//send upper nibble
	lcd_toggle_e_pin();									//allow lcd to grad data
	usleep(1);
	SET_DATA(command & 0x0F);								//send lower nibble
	lcd_toggle_e_pin();									//allow lcd to grad data
	usleep(5000);
}

void lcd_send_data(unsigned char data)
{
	SET_REGISTER(RS, DATA);								//set rs bit to 0
	SET_REGISTER(EN, LOW);								//set en bit to 0

	//send 2 peaces of data because 4 bits mode
	SET_DATA((data & 0xF0) >> 4);							//send upper nibble
	lcd_toggle_e_pin();									//allow lcd to grad data
	usleep(1);
	SET_DATA(data & 0x0F);									//send lower nibble
	lcd_toggle_e_pin();									//allow lcd to grad data
	usleep(5000);
}
