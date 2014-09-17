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

volatile char *oled_command = (char *) OLED_COMMAND;
volatile char *oled_data = (char *) OLED_DATA;

uint8_t current_line = 0xB0;
uint8_t current_column = 0x21;

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

void oled_putchar(char c)
{
	*oled_command = current_line;
	
	for (uint8_t i = 0; i < C_WIDTH; i++)
	{
		*oled_data = pgm_read_byte(&myfont[c-' '][i]);
	}
}

void oled_print(char* data)
{
	while (*data != '\0')
	{
		if(*data == '\n')
		{
			current_line++;
			if(current_line > 0xB7) current_line = 0xB0; 
			*data++;
			
			*oled_command = 0x21; //reset coloumn to left
			*oled_command = 0x00;
		}
		else
		{
			oled_putchar(*data++);
		}
	}
}


void oled_clear_line(line)
{
	*oled_command = 0x21; //Select column addresses
	*oled_command = 0x00;
	*oled_command = 0x7F;
	//
	//*oled_command = 0x22; //Select page address
	//*oled_command = line;
	//*oled_command = line;
	
	*oled_command = 0xB0 + line; //Select Page Address

	//*oled_command = 0x00;
	
	for (uint8_t i = 0; i<128;i++)
	{
		*oled_data = 0x00;
	}
}

void oled_clear()
{
	for (uint8_t i = 0; i < 8; i++)
	{
		oled_clear_line(i);	
	}
	current_line = 0xB0; //Set line to top
	current_column = 0x21; //Set coloumn left
}