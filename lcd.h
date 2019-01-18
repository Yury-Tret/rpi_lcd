#ifndef LCD_h
#define LCD_H

#include "rpi.h"

#define FUNCTION_SET		0x20
#define SET_4BIT_MODE		0x00
#define LINE_NUMBER_2		0x08
#define FONT_SIZE			0x00

#define DISPLAY				0x08
#define DISPLAY_ON			0x04
#define CURSOR_ON			0x02
#define CURSOR_BLINK_ON		0x01
#define CURSOR_OFF			0x00
#define CURSOR_BLINK_OFF	0x00

#define CLEAR_DISPLAY		0x01

#define RETURN_HOME			0x02

#define ENTRY_MODE_SET		0x04
#define INCREMENT_CURSOR	0x02

#define SHIFT_SCREEN_ENABLE	0x10
#define SHIFT_SCREEN_LEFT	0x08
#define SHIFT_SCREEN_RIGHT	0x0C

#define SET_DDRAM_ADDRESS	0x80
#define FIRST_ROW			0x00
#define SECOND_ROW			0x40

#define SET_CGRAM_ADDRESS	0x40

#define CONTROL				0x00
#define DATA				0x01

#define LOW					0x00
#define HIGH				0x01

#define SYMBOL_HEIGHT		8
#define SYMBOL_1			0x00
#define SYMBOL_2			0x01
#define SYMBOL_3			0x02
#define SYMBOL_4			0x03
#define SYMBOL_5			0x04
#define SYMBOL_FULL			0xFF

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
#define SET_DATA(value)						({							\
											SET_REGISTER(D4, value & 0x1);	\
											SET_REGISTER(D5, (value & 0x2) >> 1);	\
											SET_REGISTER(D6, (value & 0x4) >> 2);	\
											SET_REGISTER(D7, (value & 0x8) >> 3);	\
										})
#define GRAPH_ARRAY_SIZE	10
#define GRAPH_10			SYMBOL_1
#define GRAPH_20			SYMBOL_1, SYMBOL_2
#define GRAPH_30			SYMBOL_1, SYMBOL_2, SYMBOL_3
#define GRAPH_40			SYMBOL_1, SYMBOL_2, SYMBOL_3, SYMBOL_4
#define GRAPH_50			SYMBOL_1, SYMBOL_2, SYMBOL_3, SYMBOL_4, SYMBOL_5
#define GRAPH_60			GRAPH_50, SYMBOL_FULL
#define GRAPH_70			GRAPH_50, SYMBOL_FULL, SYMBOL_FULL
#define GRAPH_80			GRAPH_50, SYMBOL_FULL, SYMBOL_FULL, SYMBOL_FULL
#define GRAPH_90			GRAPH_50, SYMBOL_FULL, SYMBOL_FULL, SYMBOL_FULL, SYMBOL_FULL
#define GRAPH_100			GRAPH_50, SYMBOL_FULL, SYMBOL_FULL, SYMBOL_FULL, SYMBOL_FULL, SYMBOL_FULL

extern unsigned int gpio_pins[];
extern unsigned int length;
void lcd_init();
void lcd_toggle_e_pin();
void lcd_send_command(unsigned char);
void lcd_send_data(unsigned char);
void lcd_set_cursor_position(int, int);
void lcd_create_custom_char_map(unsigned char [][SYMBOL_HEIGHT], int);
void lcd_print_line(char *);
void lcd_print_custom_char_line(char *, int);
void lcd_zero_all_pins();
void lcd_print_graph(int);

#endif
