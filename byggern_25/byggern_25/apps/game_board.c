/*
 * game_board.c
 *
 * Created: 15.10.2014 09:38:57
 *  Author: jonarnp
 */ 

#include <stdio.h>
#include <stdbool.h>
#include <util/delay.h>
#include "game_board.h"
#include "highscore.h"
#include "../drivers/joy.h"
#include "../drivers/can/can_msg.h"
#include "../drivers/slider.h"

void send_joy_pos_init();
uint8_t send_start_game();
void transmit_joy_pos();


void play_game_board()
{
	bool finished = false;
	uint16_t score = 0;
	can_rx_message_t rx_msg;
	
	send_joy_pos_init();
	_delay_ms(100);
	
	//receive_highscore_list();

	if (!send_start_game())
	{
		printf("Failed to send start game command\n");
	}
	
	while (!finished)
	{
		transmit_joy_pos();
		
		if(read_can_message(&rx_msg))
		{
			if (rx_msg.id == GAME_STATUS)
			{
				finished = rx_msg.data[0];
				score = (rx_msg.data[1] << 8) + rx_msg.data[2];
			}
			else
			{
				printf("Got unexpected CAN msg in play_game_board\n");
			}
		}
		_delay_ms(10);
	}
	
	if (add_to_highscore(score))
	{
		highscore_element_t entry = enter_initials(score);
		send_new_highscore(entry);
	}
}

void send_joy_pos_init()
{
	can_init();	
}

uint8_t send_start_game()
{
	can_tx_message_t message;
	message.id = GAME_START;
	message.length = 0;
	
	return transmit_can_message(message);
}

void transmit_joy_pos()
{
	JOY_pos_t joy_pos = JOY_getPosition();
	//printf("Joy_pos x: %d, y: %d\n",joy_pos.x,joy_pos.y);
	
	can_tx_message_t message;
	message.id = GAME_CONTROLS;
	message.data[0] = (uint8_t)((joy_pos.x & 0xFF00)>>8);
	message.data[1] = (uint8_t)(joy_pos.x & 0x00FF);
	message.data[2] = (uint8_t)((joy_pos.y & 0xFF00)>>8);
	message.data[3] = (uint8_t)(joy_pos.y & 0x00FF);
	message.data[4] = SLIDER_right_button();
	
	message.length = 5;
		
	transmit_can_message(message);
}