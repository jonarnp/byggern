/*
 * highscore.cpp
 *
 * Created: 08.11.2014 15:16:00
 *  Author: Flua
 */ 

#include "highscore.h"
#include <EEPROM/EEPROM.h>

uint8_t places[HIGHSCORE_LENGTH];
highscore_element_t highscore_list[HIGHSCORE_LENGTH];

void Highscore::init()
{
	for (uint8_t i = 0; i<HIGHSCORE_LENGTH; i++)
	{
		places[i] = EEPROM.read(i);
	}
	read_highscore(highscore_list);
}
void Highscore::write_highscore(char* name, uint16_t score)
{
	uint8_t place;
	for (place = 0 ; place < HIGHSCORE_LENGTH ; place++)
	{
		if (1)
		{
		}
	}
}


uint8_t Highscore::read_highscore(highscore_element_t* highscore)
{
	uint8_t num_of_scores = 0;
	
	for (uint8_t i = 0 ; i < HIGHSCORE_LENGTH ; i++)
	{
		if (places[i] == 255)
		{
			//Next highscore is empty. Break
			break;
		}
		
		num_of_scores++; //Increment the number of scores
		
		char temp[NAME_LENGTH + 2];
		for (uint8_t j = 0 ; j < NAME_LENGTH + 2 ; j++)
		{
			//Read from the EEPROM. Places[i] contain the index of the next highscore
			temp[j] = EEPROM.read(HIGHSCORE_LENGTH + places[i]*(NAME_LENGTH + 2) + j);
		}
		for (uint8_t j = 0 ; j < NAME_LENGTH ; j++)
		{
			highscore[i].name[j] = temp[j];
		}
		highscore[i].score = ((uint16_t) temp[NAME_LENGTH] << 8 ) + (uint16_t)temp[NAME_LENGTH +1];
	}
	
	return num_of_scores;
}