/*
 * highscore.h
 *
 * Created: 08.11.2014 13:44:56
 *  Author: boerikse
 */ 


#ifndef HIGHSCORE_H_
#define HIGHSCORE_H_

#include <stdio.h>
#include <stdbool.h>

#define HIGHSCORE_LENGTH 4
#define NAME_LENGTH 3
#define SET_NAME_DELAY 50

typedef struct highscore_element{
	char name[NAME_LENGTH];
	uint16_t score;
}highscore_element_t;


uint8_t receive_highscore_list();

void display_highscore_list(uint8_t offset);

bool add_to_highscore(uint16_t score);

highscore_element_t enter_initials(uint16_t score);

void send_new_highscore(highscore_element_t entry);

void highscore_reset();

#endif /* HIGHSCORE_H_ */