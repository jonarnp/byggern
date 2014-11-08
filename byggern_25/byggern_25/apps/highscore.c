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

uint8_t send_highscore_request();


highscore_element_t highscore_list[HIGHSCORE_LENGTH];
//highscore_element_t highscore_list_test[HIGHSCORE_LENGTH];

uint8_t receive_highscore_list()
{
	can_rx_message_t rx_msg;
	
	//Successfully sending a request to Node 2 for highscores
	if (send_highscore_request())
	{
		uint8_t i;
		for(i = 0; i < HIGHSCORE_LENGTH; i++)
		{
			while (!read_can_message(&rx_msg)); //loop until highscore message received. possible deadlock..
			
			if (rx_msg.id == HIGHSCORE)
			{
				highscore_list[i].name[0] = rx_msg.data[0];
				highscore_list[i].name[1] = rx_msg.data[1];
				highscore_list[i].name[2] = rx_msg.data[2];
				
				highscore_list[i].score = (rx_msg.data[3] << 8) + rx_msg.data[4];
			}
		}
	}
	else 
	{
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

void display_highscore_list()
{
	//Test data
	//for (int j = 0; j < HIGHSCORE_LENGTH; j++)
	//{
		//highscore_list_test[j].name[0] = 'a';
		//highscore_list_test[j].name[1] = 'b';
		//highscore_list_test[j].name[2] = 'c';
		//highscore_list_test[j].score = 10;
	//}
	
	
	// Print text to OLED
	oled_clear();
	oled_goto_line(0);
	oled_goto_column(0);
	oled_print_p(PSTR("Highscore list"));
	
	for(int i = 0; i < HIGHSCORE_LENGTH; i++)
	{
		oled_goto_line(2+i);
		oled_goto_column(0);
		for (int j = 0; j < NAME_LENGTH; j++)
		{
			oled_putchar(highscore_list[i].name[j]);
			//oled_putchar(highscore_list_test[i].name[j]);
		}
		oled_print(" ");
		oled_print(uint16_to_str(highscore_list[i].score));
		//oled_print(uint16_to_str(highscore_list_test[i].score));
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
	
}

void send_new_highscore(highscore_element_t entry)
{
	can_tx_message_t message;
	message.id = WRITE_HIGHSCORE;
	message.data[0] = entry.name[0];
	message.data[1] = entry.name[1];
	message.data[2] = entry.name[2];
	message.data[3] = entry.score;
	message.data[4] = entry.score;
	message.length = 0;
	
	transmit_can_message(message);
}