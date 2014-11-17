/*
 * music.h
 *
 * Created: 08.11.2014 20:21:14
 *  Author: boerikse
 */ 

#include <stdio.h>

#ifndef MUSIC_H_
#define MUSIC_H_

/* Defined two music states */
typedef enum music{
	MENU_MUSIC,
	PLAY_MUSIC
} music_t;

/* Initialize music to off */
void music_init();

/*
Check if music is enabled
@return uint8_t. 1 if music is on, 0 if off
*/
uint8_t music_enabled();

/* 
Toggle music 
@return uint8_t. 0 if unsuccessful transmission, otherwise 1.
*/
uint8_t music_toggle();

/*
Change music state, but only if music is on
@param music_t. The desired music state
*/
void music_change_state(music_t mode);

#endif /* MUSIC_H_ */