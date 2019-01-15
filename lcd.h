#ifndef LCD_h
#define LCD_H

#include "rpi.h"

#define ZERO_ALL_PINS		0x00

#define SET_4BIT_MODE		0x20
#define SET_8BIT_MODE		0x30
#define LINE_NUMBER_2		0x08
#define FONT_SIZE			0x00

#define DISPLAY_ON			0x0C
#define CURSOR_ON			0x02
#define CURSOR_BLINK_ON		0x01

#define CLEAR_DISPLAY		0x01

#define INCREMENT_CURSOR	0x06

#define COMMAND_REG			0x00
#define DATA_REG			0x01
#define ENABLE				0x01

//control pins
#define RS					20
#define EN					21
//4 bit data pins
#define D4					6
#define D5					13
#define D6					19
#define D7					26

#define SET_REGISTER(registr, value)	value ? (GPIO_SET = 1 << registr) : (GPIO_CLR = 1 << registr);

//4 bits data
#define DATA(value)						({							\
											SET_REGISTER(D4, value & 0x1);	\
											SET_REGISTER(D5, (value & 0x2) >> 1);	\
											SET_REGISTER(D6, (value & 0x4) >> 2);	\
											SET_REGISTER(D7, (value & 0x8) >> 3);	\
										})


void lcd_init();
void lcd_toggle_e_pin();
void lcd_send_command(char);
void lcd_send_data(char);

#endif
