#include "lcd.h"

/*
void lcd_init();
void lcd_clear_display();
void lcd_set_cursor();
void lcd_send_command();
void lcd_send_data();
*/

void lcd_init()
{
	lcd_set_4bit_mode();
	lcd_clear_display();
	//lcd_set_cursor(0, 0);
}

void lcd_set_4bit_mode()
{
	lcd_send_command(SET_4BIT_MODE);
}

void lcd_clear_display()
{
	lcd_send_command(CLEAR_DISPLAY);
}

void lcd_set_cursor(unsigned int row, unsigned int position)
{

}

void lcd_send_command(char command)
{
	//set rs bit to 0
	SET_REGISTER(RS, 0);
	sleep(1);
	//send data twice because 4 bits mode
	DATA((command & 0xF0) >> 4);	//upper nibble
	sleep(1);
	DATA(command & 0x0F);			//lower nibble
	sleep(1);
	//set enable pin to 1 to secure data
	SET_REGISTER(EN, 1);
	usleep(1000*50);
	sleep(1);
	SET_REGISTER(EN, 0);
	sleep(1);
}

void lcd_send_data(char data)
{
	//set rs bit to 0
	SET_REGISTER(RS, 1);
	sleep(1);
	//send data twice because 4 bits mode
	DATA((data & 0xF0) >> 4);	//upper nibble
	sleep(1);
	DATA(data & 0x0F);			//lower nibble
	sleep(1);
	//set enable pin to 1 to secure data
	SET_REGISTER(EN, 1);
	usleep(1000*50);
	sleep(1);
	SET_REGISTER(EN, 0);
	sleep(1);
}
