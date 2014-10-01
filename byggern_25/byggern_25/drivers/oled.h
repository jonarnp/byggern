/*
 * oled.h
 *
 * Created: 17.09.2014 10:46:35
 *  Author: boerikse
 */ 


#ifndef OLED_H_
#define OLED_H_

#include <avr/io.h>

#define OLED_COMMAND 0x1100
#define OLED_DATA 0x1300
#define OLED_COLUMN_SPAN 127	// OLED column range (0-127)
#define OLED_LINE_SPAN 7		// OLED line range (0-7)

typedef struct oled_position{
	uint8_t line;
	uint8_t column;
}oled_position_t;

void oled_init();

void oled_putchar(char c);

void oled_print(char* data);

/*
Print char from PROGMEM
*/
void oled_print_p(const char* data);

void oled_goto_line(uint8_t line);

void oled_goto_column(uint8_t column);

oled_position_t oled_getPos();

void oled_clear_line(uint8_t line);

void oled_clear();

#endif /* OLED_H_ */