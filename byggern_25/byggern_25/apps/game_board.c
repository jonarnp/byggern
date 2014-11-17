/*
 * game_board.c
 *
 * Created: 15.10.2014 09:38:57
 *  Author: jonarnp
 */ 

#include <stdio.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "game_board.h"
#include "highscore.h"
#include "../format.h"
#include "../drivers/joy.h"
#include "../drivers/can/can_id.h"
#include "../drivers/can/can_msg.h"
#include "../drivers/slider.h"
#include "../drivers/oled.h"
#include "../drivers/p1000switches.h"

/*
Private functions
*/

/*
Send a start game command to node 2
@return uint8_t. 0 if transmission was unsuccessful
*/
uint8_t send_start_game();

/*
Send joystick position and P1000 SW0 to node 2
*/
void transmit_joy_pos();

/*
Send joystick position as 0,0 and P1000 SW0 as 0 to when the game is finished
*/
void transmit_joy_pos_zero();


void send_joy_pos_init()
{
	can_init();
}

void play_game_board()
{
	bool finished = false;
	uint16_t score = 0;
	can_rx_message_t rx_msg;
	
	//Get highscore list from node 2 and display it while playing
	receive_highscore_list();
	display_highscore_list(2);
	
	//Send start game command to node 2 
	if (!send_start_game())
	{
		//printf("Failed to send start game command\n");
	}
	
	//Live score
	oled_goto_line(0);
	oled_goto_column(0);
	oled_print_p(PSTR("Current score "));
	oled_goto_column(70);
	oled_print(uint16_to_str(score));
	
	//Loop until game is finished
	while (!finished)
	{
		//Sending game controls every iteration
		transmit_joy_pos();
		
		//Busy-wait until message received from node 2
		while(!read_can_message(&rx_msg));

		if (rx_msg.id == GAME_STATUS)
		{
			//Received data
			finished = rx_msg.data[0];
			score = ((uint16_t)rx_msg.data[1] << 8) + rx_msg.data[2];
				
			//Live score
			oled_goto_line(0);
			oled_goto_column(70);
			oled_print(uint16_to_str(score));
			oled_print("    \0");
		}
		else
		{
			//printf("Got unexpected CAN msg in play_game_board. id: %04x\n", rx_msg.id);
		}
		
		_delay_ms(20); //Loop delay
	}
	
	//Stop motor and set servo to middle position
	transmit_joy_pos_zero();
	
	//Check if any highscore was beaten
	if (add_to_highscore(score))
	{
		//Sending new highscore to node 2
		highscore_element_t entry = enter_initials(score);
		send_new_highscore(entry);
		_delay_ms(200);
	}
}

uint8_t send_start_game()
{
	//Generate and send game start command
	can_tx_message_t message;
	message.id = GAME_START;
	message.length = 0;
	
	return transmit_can_message(message);
}

void transmit_joy_pos()
{
	JOY_pos_t joy_pos = JOY_getPosition();
	
	//Generate and send game controls
	can_tx_message_t message;
	message.id = GAME_CONTROLS;
	message.data[0] = (uint8_t)((joy_pos.y & 0xFF00)>>8);
	message.data[1] = (uint8_t)(joy_pos.y & 0x00FF);
	message.data[2] = (uint8_t)((joy_pos.x & 0xFF00)>>8);
	message.data[3] = (uint8_t)(joy_pos.x & 0x00FF);
	message.data[4] = P1000_SW0();
	
	message.length = 5;
		
	transmit_can_message(message);
}

void transmit_joy_pos_zero()
{
	//Generate and send game controls with all zeros
	can_tx_message_t message;
	message.id = GAME_CONTROLS;
	message.data[0] = 0;
	message.data[1] = 0;
	message.data[2] = 0;
	message.data[3] = 0;
	message.data[4] = 0;
	
	message.length = 5;
	
	transmit_can_message(message);
}