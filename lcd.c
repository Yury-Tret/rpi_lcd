#include "lcd.h"

void lcd_init()
{
	lcd_send_command(ZERO_ALL_PINS);
	usleep(5000);

	lcd_send_command((SET_4BIT_MODE >> 4));
	usleep(5000);

	lcd_send_command(SET_4BIT_MODE|LINE_NUMBER_2|FONT_SIZE);
	usleep(5000);

	lcd_send_command(DISPLAY_ON|CURSOR_ON|CURSOR_BLINK_ON);
	usleep(5000);

	lcd_send_command(CLEAR_DISPLAY);
	usleep(5000);

	lcd_send_command(INCREMENT_CURSOR);
	usleep(5000);
}

void lcd_toggle_e_pin()
{
	SET_REGISTER(EN, 1);
	usleep(2);
	SET_REGISTER(EN, 0);
}

void lcd_send_command(char command)
{
	SET_REGISTER(RS, 0);								//set rs bit to 0
	SET_REGISTER(EN, 0);								//set en bit to 0

	//send 2 peaces of command because 4 bits mode
	DATA((command & 0xF0) >> 4);						//send upper nibble
	lcd_toggle_e_pin();									//allow lcd to grad data
	usleep(1);
	DATA(command & 0x0F);								//send lower nibble
	lcd_toggle_e_pin();									//allow lcd to grad data
	usleep(100);
}

void lcd_send_data(char data)
{
	SET_REGISTER(RS, 1);								//set rs bit to 0
	SET_REGISTER(EN, 0);								//set en bit to 0

	//send 2 peaces of data because 4 bits mode
	DATA((data & 0xF0) >> 4);							//send upper nibble
	lcd_toggle_e_pin();									//allow lcd to grad data
	usleep(1);
	DATA(data & 0x0F);									//send lower nibble
	lcd_toggle_e_pin();									//allow lcd to grad data
	usleep(100);
}
