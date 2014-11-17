/*
 * music.c
 *
 * Created: 08.11.2014 20:21:06
 *  Author: boerikse
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "music.h"
#include "../drivers/can/can_id.h"
#include "../drivers/can/can_msg.h"

/*
Private functions
*/

/* Send next song command over CAN */
uint8_t music_next_song();

/* Send previous song command over CAN */
uint8_t music_prev_song();


/* Music variables */
bool music_on;
music_t music_state;

void music_init()
{
	//Using CAN for spotify commands
	can_init();
	music_on = false;
	music_state = MENU_MUSIC;
}

uint8_t music_enabled()
{
	return music_on;
}

uint8_t music_toggle()
{
	//Generate and send a spotify toggle command
	can_tx_message_t message;
	message.id = SPOTIFY_COMMAND;
	message.data[0] = 1; //Toggle music
	message.length = 1;
	music_on = !music_on;
	
	return transmit_can_message(message);
}

void music_change_state(music_t mode)
{
	if (music_on && mode != music_state)
	{
		music_state = mode;
		music_next_song();
	} 
}

uint8_t music_next_song()
{
	//Generate and send a spotify next song command
	can_tx_message_t message;
	message.id = SPOTIFY_COMMAND;
	message.data[0] = 2; //Next song
	message.length = 1;
	
	return transmit_can_message(message);
}

uint8_t music_prev_song()
{
	//Generate and send a spotify previous song command
	can_tx_message_t message;
	message.id = SPOTIFY_COMMAND;
	message.data[0] = 3; //Previous song
	message.length = 1;
	
	return transmit_can_message(message);
}