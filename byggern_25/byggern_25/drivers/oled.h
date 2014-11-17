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
#define OLED_COLUMN_SPAN 127	// OLED column range [0,127]
#define OLED_LINE_SPAN 7		// OLED line range [0,7]

typedef struct oled_position{
	uint8_t line;
	uint8_t column;
} oled_position_t;

/*
Initialize the OLED on the USB multifunction card
*/
void oled_init();

/*
Draw a single byte on the OLED. Draws 8 pixels in a single column. LSB on top, MSB on bottom
@param byte. 1 is color, 0 is none
*/
void oled_print_byte(uint8_t byte);

/*
Outputs a single character to the OLED.
@param char that will be displayed
*/
void oled_putchar(char c);

/*
Outputs a single character with underscore to the OLED.
@param char that will be displayed
*/
void oled_putchar_underscore(char c);

/*
Outputs a character string to the OLED screen. Needs '\0' to terminate. Supports '\n'
@param character string
*/
void oled_print(char* data);

/*
Outputs a character string with underscore to the OLED screen. Needs '\0' to terminate. Supports '\n'
@param character string
*/
void oled_print_underscore(char* data);

/*
Outputs a character string from PROGMEM to the OLED. Needs '\0' to terminate. Supports '\n'
@param char* to PROGMEM
*/
void oled_print_p(const char* data);

/*
Outputs a character string with underscore from PROGMEM to the OLED. Needs '\0' to terminate. Supports '\n'
@param char* to PROGMEM
*/
void oled_print_underscore_p(const char* data);

/*
Sets the OLED's current_line to input parameter. If input greater than OLED_LINE_SPAN, set line to 0 
@param uint8_t in range [0,7]
*/
void oled_goto_line(uint8_t line);

/*
Sets the OLED's current_column to input parameter. If input greater than OLED_COLUMN_SPAN, set column to 0 
@param uint8_t in range [0,127]
*/
void oled_goto_column(uint8_t column);

/*
Returns the OLED's current position.
@return oled_position_t: uint8_t line, uin8_t column. 
*/
oled_position_t oled_getPos();

/*
Clear single line. Keeps column pointer at the same place after clearing the line
@param uint8_t in range [0,7]
*/
void oled_clear_line(uint8_t line);

/*
Clear entire screen. Keeps column and line pointer at the same place after clearing the screen
*/
void oled_clear();

#endif /* OLED_H_ */