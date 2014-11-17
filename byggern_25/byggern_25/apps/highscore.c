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
#include "../drivers/can/can_id.h"
#include "../drivers/can/can_msg.h"
#include "../drivers/oled.h"
#include "../drivers/joy.h"
#include "../drivers/slider.h"

/*
Private functions
*/
uint8_t send_highscore_request();


highscore_element_t highscore_list[HIGHSCORE_LENGTH];

uint8_t receive_highscore_list()
{
	can_rx_message_t rx_msg;
	
	if (send_highscore_request())
	{
		//Successfully sent highscore request
		uint8_t i = 0;
		while(i < HIGHSCORE_LENGTH) //loop until all four highscores received
		{
			while (!read_can_message(&rx_msg)); //busy-wait until highscore message received
			if (rx_msg.id == HIGHSCORE)
			{				
				highscore_list[i].name[0] = rx_msg.data[0];
				highscore_list[i].name[1] = rx_msg.data[1];
				highscore_list[i].name[2] = rx_msg.data[2];
				
				highscore_list[i].score = (rx_msg.data[3] << 8) + rx_msg.data[4];
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
		//Failed to send highscore request
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
	
	//Recently obtained score
	new_score.score = score;
	
	//Initial initials
	new_score.name[0] = 'A';
	new_score.name[1] = 'A';
	new_score.name[2] = 'A';
	
	//Stay in the enter_initials menu until left slider button is pushed
	while (!SLIDER_left_button())
	{
		if (JOY_getDirection() == UP)
		{
			if (++iter > SET_NAME_DELAY)
			{
				iter = 0;
				
				//Increment selected initial
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
				
				//Check for under-roll
				if (new_score.name[set_char] < 'A') new_score.name[set_char] = 'Z';
			}
		}
		else if (JOY_getDirection() == DOWN)
		{
			if (++iter > SET_NAME_DELAY)
			{
				iter = 0;
				
				//Increment selected initial
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
				
				//Check for over-roll
				if (new_score.name[set_char] > 'Z') new_score.name[set_char] = 'A';
			}
		}
		else if (JOY_getDirection() == LEFT)
		{
			if (++iter > SET_NAME_DELAY)
			{
				iter = 0;
				
				//Change which initial to edit
				set_char--;
				
				// Check for under-roll
				if (set_char < 0) set_char = NAME_LENGTH-1;
			}
		}
		else if (JOY_getDirection() == RIGHT)
		{
			if (++iter > SET_NAME_DELAY)
			{
				iter = 0;
				
				//Change which initial to edit
				set_char++;
				
				// Check for over-roll
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
	//Generate and send a write highscore command
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
	//Generate and send a reset highscore command
	can_tx_message_t message;
	message.id = RESET_HIGHSCORE;
	message.length = 0;
	
	transmit_can_message(message);
}