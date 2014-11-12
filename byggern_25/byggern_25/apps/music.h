/*
 * music.h
 *
 * Created: 08.11.2014 20:21:14
 *  Author: boerikse
 */ 

#include <stdio.h>

#ifndef MUSIC_H_
#define MUSIC_H_

typedef enum music{
	MENU_MUSIC,
	PLAY_MUSIC
} music_t;

void music_init();

uint8_t music_enabled();

uint8_t music_toggle();

void music_change_state(music_t mode);

#endif /* MUSIC_H_ */