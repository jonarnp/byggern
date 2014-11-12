/*
 * music.c
 *
 * Created: 08.11.2014 20:21:06
 *  Author: boerikse
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "music.h"
#include "../drivers/can/can_msg.h"

uint8_t music_next_song();
uint8_t music_prev_song();

bool music_on;
music_t music_state;

void music_init()
{
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
	can_tx_message_t message;
	message.id = SPOTIFY_COMMAND;
	message.data[0] = 1;
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
	printf("music state: %d\n", music_state);
}

uint8_t music_next_song()
{
	can_tx_message_t message;
	message.id = SPOTIFY_COMMAND;
	message.data[0] = 2;
	message.length = 1;
	
	return transmit_can_message(message);
}

uint8_t music_prev_song()
{
	can_tx_message_t message;
	message.id = SPOTIFY_COMMAND;
	message.data[0] = 3;
	message.length = 1;
	
	return transmit_can_message(message);
}