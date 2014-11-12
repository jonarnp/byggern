/*
 * highscore.c
 *
 * Created: 08.11.2014 13:44:45
 *  Author: boerikse
 */ 

#include <stdio.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "highscore.h"
#include "../format.h"
#include "../drivers/can/can_msg.h"
#include "../drivers/oled.h"
#include "../drivers/joy.h"
#include "../drivers/slider.h"

uint8_t send_highscore_request();


highscore_element_t highscore_list[HIGHSCORE_LENGTH];
//highscore_element_t highscore_list_test[HIGHSCORE_LENGTH];

uint8_t receive_highscore_list()
{
	can_rx_message_t rx_msg;
	
	//printf("Sending highscore request\n");
	//Successfully sending a request to Node 2 for highscores
	if (send_highscore_request())
	{
		//printf("Successfully sendt highscore request\n");
		uint8_t i = 0;
		while(i < HIGHSCORE_LENGTH)
		{
			while (!read_can_message(&rx_msg)); //loop until highscore message received. possible deadlock..
			if (rx_msg.id == HIGHSCORE)
			{				
				highscore_list[i].name[0] = rx_msg.data[0];
				highscore_list[i].name[1] = rx_msg.data[1];
				highscore_list[i].name[2] = rx_msg.data[2];
				
				highscore_list[i].score = (rx_msg.data[3] << 8) + rx_msg.data[4];
				//printf("Highscore received %d, %d, %d, %d\n", highscore_list[i].name[0], highscore_list[i].name[1], highscore_list[i].name[2], highscore_list[i].score );		
				i++;
			}
			else
			{
				//printf("Got unexpected CAN msg when expecting highscore. id: %04x\n", rx_msg.id);
			}
		}
	}
	else 
	{
		//printf("Failed to send highscore request\n");
		return 0;
	}
	return 1;
}

uint8_t send_highscore_request()
{
	can_tx_message_t message;
	message.id = HIGHSCORE_REQUEST;
	message.length = 0;
	
	return transmit_can_message(message);
}

void display_highscore_list(uint8_t offset)
{
	// Print text to OLED
	oled_clear();
	oled_goto_line(offset);
	oled_goto_column(0);
	oled_print_p(PSTR("Highscore list"));
	
	for(int i = 0; i < HIGHSCORE_LENGTH; i++)
	{
		oled_goto_line(2+offset+i);
		oled_goto_column(0);
		for (int j = 0; j < NAME_LENGTH; j++)
		{
			oled_putchar(highscore_list[i].name[j]);
		}
		oled_print(" ");
		oled_print(uint16_to_str(highscore_list[i].score));
	}
}

bool add_to_highscore(uint16_t score)
{
	if (score > highscore_list[HIGHSCORE_LENGTH-1].score)
	{
		return true;
	}
	return false;
}

highscore_element_t enter_initials(uint16_t score)
{
	highscore_element_t new_score;
	int8_t set_char = 0;
	uint8_t iter = 0;
	
	// Print text to OLED
	oled_clear();
	oled_goto_line(0);
	oled_goto_column(0);
	oled_print_p(PSTR("Your score "));
	oled_print(uint16_to_str(score));
	
	oled_goto_line(2);
	oled_goto_column(0);
	oled_print_p(PSTR("Enter you initials"));
	
	oled_goto_line(6);
	oled_goto_column(0);
	oled_print_p(PSTR("Press left button when finished"));
	
	new_score.score = score;
	new_score.name[0] = 'A';
	new_score.name[1] = 'A';
	new_score.name[2] = 'A';
	
	while (!SLIDER_left_button())
	{
		if (JOY_getDirection() == UP)
		{
			if (++iter > SET_NAME_DELAY)
			{
				iter = 0;
				
				if (set_char == 0)
				{
					new_score.name[0]--;
				}
				else if (set_char == 1)
				{
					new_score.name[1]--;
				}
				else
				{
					new_score.name[2]--;
				}
				
				if (new_score.name[set_char] < 'A') new_score.name[set_char] = 'Z';
			}
		}
		else if (JOY_getDirection() == DOWN)
		{
			if (++iter > SET_NAME_DELAY)
			{
				iter = 0;
				
				if (set_char == 0)
				{
					new_score.name[0]++;
				}
				else if (set_char == 1)
				{
					new_score.name[1]++;
				}
				else
				{
					new_score.name[2]++;
				}
				
				if (new_score.name[set_char] > 'Z') new_score.name[set_char] = 'A';
			}
		}
		else if (JOY_getDirection() == LEFT)
		{
			if (++iter > SET_NAME_DELAY)
			{
				iter = 0;
				
				set_char--;
				
				// Check for over/under-roll
				if (set_char < 0) set_char = NAME_LENGTH-1;
				if (set_char > NAME_LENGTH-1) set_char = 0;
			}
		}
		else if (JOY_getDirection() == RIGHT)
		{
			if (++iter > SET_NAME_DELAY)
			{
				iter = 0;
				
				set_char++;
				
				// Check for over/under-roll
				if (set_char < 0) set_char = NAME_LENGTH-1;
				if (set_char > NAME_LENGTH-1) set_char = 0;
			}
		}
		
		//if change, update screen
		if(iter == 0)
		{
			oled_goto_line(3);
			oled_goto_column(0);
			for (int i = 0; i < NAME_LENGTH; i++)
			{
				if (set_char == i)
				{
					oled_putchar_underscore(new_score.name[i]);
				}
				else
				{
					oled_putchar(new_score.name[i]);
				}
			}
		}
		_delay_ms(10);
	}
	
	return new_score;
}

void send_new_highscore(highscore_element_t entry)
{
	can_tx_message_t message;
	message.id = WRITE_HIGHSCORE;
	message.data[0] = entry.name[0];
	message.data[1] = entry.name[1];
	message.data[2] = entry.name[2];
	message.data[3] = (uint8_t)((entry.score & 0xFF00)>>8);
	message.data[4] = (uint8_t)(entry.score & 0x00FF);
	message.length = 5;
	
	transmit_can_message(message);
}

void highscore_reset()
{
	can_tx_message_t message;
	message.id = RESET_HIGHSCORE;
	message.length = 0;
	
	transmit_can_message(message);
}