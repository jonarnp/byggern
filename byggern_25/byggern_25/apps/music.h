/*
 * music.h
 *
 * Created: 08.11.2014 20:21:14
 *  Author: boerikse
 */ 

#include <stdio.h>

#ifndef MUSIC_H_
#define MUSIC_H_

void music_init();

uint8_t music_enabled();

uint8_t music_toggle();

uint8_t music_next_song();

uint8_t music_prev_song();

#endif /* MUSIC_H_ */