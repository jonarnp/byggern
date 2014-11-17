/*
 * oled.c
 *
 * Created: 17.09.2014 10:54:38
 *  Author: boerikse
 */ 

#include "oled.h"
#include <avr/pgmspace.h>
#include <avr/io.h>
#include "../fonts/font_5x7.h"

/*
Private functions
*/
void oled_select_column(uint8_t col_nr);
void oled_select_line(uint8_t line_nr);


volatile char *oled_command = (char *) OLED_COMMAND;
volatile char *oled_data = (char *) OLED_DATA;

uint8_t current_line = 0;	//0xB0;
uint8_t current_column = 0;

void oled_init()
{
	*oled_command = 0xae; // display off
	*oled_command = 0xa1; //segment remap
	*oled_command = 0xda; //common pads hardware: alternative
	*oled_command = 0x12;
	*oled_command = 0xc8; //common output scan direction:com63~com0
	*oled_command = 0xa8; //multiplex ration mode:63
	*oled_command = 0x3f;
	*oled_command = 0xd5; //display divide ratio/osc. freq. mode
	*oled_command = 0x80;
	*oled_command = 0x81; //contrast control
	*oled_command = 0x50;
	*oled_command = 0xd9; //set pre-charge period
	*oled_command = 0x21;
	*oled_command = 0x20; //Set Memory Addressing Mode to 0x00 Horizontal mode. 0x02 for page mode
	*oled_command = 0x02; 	
	*oled_command = 0xdb; //VCOM deselect level mode
	*oled_command = 0x30;
	*oled_command = 0xad; //master configuration
	*oled_command = 0x00;
	*oled_command = 0xa4; //out follows RAM content
	*oled_command = 0xa6; //set normal display
	*oled_command = 0xaf; // display on
}

void oled_print_byte(uint8_t byte)
{
	*oled_data = byte;
	current_column += 1;
}

void oled_putchar(char c)
{	
	uint8_t remaining_col = OLED_COLUMN_SPAN-current_column; // Remaining columns on current line
	
	if (remaining_col<C_WIDTH)
	{
		// Clear remaining columns
		for (current_column; current_column <= OLED_COLUMN_SPAN;current_column++)
		{
			*oled_data = 0x00;
		}
		
		// Select leftmost column
		current_column = 0;
		oled_select_column(current_column);
		
		// Select next line. Reset to 0 if end of screen
		if (++current_line > OLED_LINE_SPAN) current_line = 0;
		oled_select_line(current_line);
	}
	
	for (uint8_t i = 0; i < C_WIDTH; i++)
	{
		*oled_data = pgm_read_byte(&myfont[c-' '][i]);
		++current_column;
	}
}

void oled_putchar_underscore(char c)
{
	uint8_t remaining_col = OLED_COLUMN_SPAN-current_column; // Remaining columns on current line
	
	if (remaining_col<C_WIDTH)
	{
		// Clear remaining columns
		for (current_column; current_column <= OLED_COLUMN_SPAN;current_column++)
		{
			*oled_data = 0x00;
		}
		
		// Select leftmost column
		current_column = 0;
		oled_select_column(current_column);
		
		// Select next line. Reset to 0 if end of screen
		if (++current_line > OLED_LINE_SPAN) current_line = 0;
		oled_select_line(current_line);
	}
	
	for (uint8_t i = 0; i < C_WIDTH; i++)
	{
		*oled_data = pgm_read_byte(&myfont[c-' '][i]) | 0x80;
		++current_column;
	}
}

void oled_print(char* data)
{
	while (*data != '\0')
	{
		if(*data == '\n')
		{
			if(++current_line > OLED_LINE_SPAN) current_line = 0; 
			*data++;
			
			oled_select_column(0);
		}
		else
		{
			oled_putchar(*data++);
		}
	}
}

void oled_print_underscore(char* data)
{
	while (*data != '\0')
	{
		if(*data == '\n')
		{
			if(++current_line > OLED_LINE_SPAN) current_line = 0;
			*data++;
			
			oled_select_column(0);
		}
		else
		{
			oled_putchar_underscore(*data++);
		}
	}
}


void oled_print_p(const char* data)
{
	while (pgm_read_byte(data) != '\0')
	{
		if(pgm_read_byte(data) == '\n')
		{
			if(++current_line > OLED_LINE_SPAN) current_line = 0;
			data++;
			
			oled_select_column(0);
		}
		else
		{
			oled_putchar(pgm_read_byte(data++));
		}
	}
}

void oled_print_underscore_p(const char* data)
{
	while (pgm_read_byte(data) != '\0')
	{
		if(pgm_read_byte(data) == '\n')
		{
			if(++current_line > OLED_LINE_SPAN) current_line = 0;
			data++;
			
			oled_select_column(0);
		}
		else
		{
			oled_putchar_underscore(pgm_read_byte(data++));
		}
	}
}

void oled_goto_line(uint8_t line)
{
	if (line > OLED_LINE_SPAN)
	{
		current_line = 0;
	}
	else
	{
		current_line = line;
	}
	oled_select_line(current_line);
}

void oled_goto_column(uint8_t column)
{
		if (column > OLED_COLUMN_SPAN)
		{
			current_column = 0;
		}
		else
		{
			current_column = column;
		}
		oled_select_column(current_column);
}

oled_position_t oled_getPos()
{
	oled_position_t pos;
	pos.column = current_column;
	pos.line = current_line;
	
	return pos;
}

void oled_clear_line(uint8_t line)
{
	oled_select_line(line);
	
	for (uint8_t i = 0; i<= OLED_COLUMN_SPAN ;i++)
	{
		*oled_data = 0x00;
	}
}

void oled_clear()
{
	for (uint8_t i = 0; i <= OLED_LINE_SPAN; i++)
	{
		oled_clear_line(i);	
	}
	
	// Select "old" line and column
	oled_select_line(current_line);
	oled_select_column(current_column);
}

/*
Private functions
*/
void oled_select_column(uint8_t col_nr)
{
	// Page addressing mode
	uint8_t nibble1 = (col_nr & 0x0F);
	uint8_t nibble2 = ((col_nr & 0xF0) >> 4);
	
	//Select column
	*oled_command = 0x10 + nibble2;
	*oled_command = 0x00 + nibble1;
}

void oled_select_line(uint8_t line_nr)
{
	*oled_command = 0xB0 + line_nr;
}
