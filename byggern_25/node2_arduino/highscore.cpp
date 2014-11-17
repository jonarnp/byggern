/*
 * highscore.cpp
 *
 * Created: 08.11.2014 15:16:00
 *  Author: Flua
 */ 

#include "highscore.h"
#include <eeprom.h>
#include "CAN.h"
#include "Can_ID.h"

highscore_element_t highscore_list[HIGHSCORE_LENGTH]; //Local copy of the EEPROM highscore list
void write_to_EEPROM();

void Highscore::init()
{
	//Fetch the highscore from the EEPROM to the local copy
	read_highscore(highscore_list);
}

void Highscore::write_highscore(char name[NAME_LENGTH], uint16_t score)
{
	//Initial place for the new score is at the bottom of the list
	int8_t new_place = HIGHSCORE_LENGTH - 1;

	while (new_place >= 0)
	{
		if (score <= highscore_list[new_place].score) //Current highscore higher then the new score?
		{
			//Correct place found, brak the loop.
			break;
		}
		//Highscore lower than the new score, check next highscore.
		new_place--;
	}
	//new_place is now in the interval [-1,HIGHSCORE_LENGTH-1]. Make it a feasible index by increasing it by one.
	new_place++;
	
	//Check if the new score in fact is better than the lowest highscore
	if (new_place == HIGHSCORE_LENGTH )	
	{
		//Score is not better than the last entry
		return;
	}
	
	//Reorganize the highscore list to fit room for the new score
	for (uint8_t i = HIGHSCORE_LENGTH - 1 ; i > new_place ; i--)
	{
		highscore_list[i] = highscore_list[i-1];
	}
	
	//Save the new score to the local copy
	for ( uint8_t i = 0 ; i < NAME_LENGTH ; i++)
	{
		highscore_list[new_place].name[i] = name[i];		
	}
	highscore_list[new_place].score = score;
	
	//Update the EEPROM highscore list
	write_to_EEPROM();
}

void Highscore::read_highscore(highscore_element_t* highscore)
{
	for (uint8_t i = 0 ; i < HIGHSCORE_LENGTH ; i++)
	{		
		char temp[NAME_LENGTH + 2];
		
		//Read highscore name from the EEPROM.
		for (uint8_t j = 0 ; j < NAME_LENGTH + 2 ; j++)
		{
			temp[j] = EEPROM.read(i*(NAME_LENGTH + 2) + j);
		}
		for (uint8_t j = 0 ; j < NAME_LENGTH ; j++)
		{
			highscore[i].name[j] = temp[j];
		}
		
		//Read highscore score from the EEPROM.
		highscore[i].score = ((uint16_t) temp[NAME_LENGTH] << 8 ) + (uint16_t)temp[NAME_LENGTH +1];
	}
}

void Highscore::reset_highscore()
{
	//Update the local highscore list
	for (uint8_t i = 0 ; i < HIGHSCORE_LENGTH ; i++)
	{
		//Write '---' as the name
		for (uint8_t j = 0 ; j < NAME_LENGTH ; j++)
		{
			highscore_list[i].name[j] = '-';
		}
		
		//Write 0 as the score
		highscore_list[i].score = (uint16_t)0;
	}
	
	//Update the EEPROM
	write_to_EEPROM();
}

void Highscore::send_highscore()
{
	//Send the highscore list as a series of CAN messages
	for (uint8_t i = 0 ; i < HIGHSCORE_LENGTH ; i++)
	{
		CanMessage message;
		message.id = HIGHSCORE;
		
		//Add the highscore name to the CAN message
		for (uint8_t j = 0 ; j < NAME_LENGTH ; j++)
		{
			message.data[j] = highscore_list[i].name[j];
		}
		
		//Add the highscore score to the CAN message
		message.data[NAME_LENGTH] = (uint8_t)(highscore_list[i].score >> 8);
		message.data[NAME_LENGTH + 1] = (uint8_t)( highscore_list[i].score & 0x00FF);
		
		//CAN message length
		message.len = NAME_LENGTH + 2;
		
		while (!CAN.ready()); //Wait for the CAN bus to be ready
		message.send();
		
		delay(5); //Wait a short while to cope with limited reading frequency of node 1.
	}
}

void write_to_EEPROM()
{
	//Save highscore to EEPROM
	
	for (uint8_t i = 0 ; i < HIGHSCORE_LENGTH ; i++)
	{
		//Save highscore name
		for (uint8_t j = 0 ; j < NAME_LENGTH ; j++)
		{
			EEPROM.write(i*(NAME_LENGTH + 2) + j, highscore_list[i].name[j]);
		}
		EEPROM.write(i*(NAME_LENGTH + 2) + NAME_LENGTH , (uint8_t)(highscore_list[i].score >> 8) ); //Save MSB of score
		EEPROM.write(i*(NAME_LENGTH + 2) + NAME_LENGTH + 1 , (uint8_t)( highscore_list[i].score & 0x00FF) ); //Save LSB of score
	}
}