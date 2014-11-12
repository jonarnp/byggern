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

highscore_element_t highscore_list[HIGHSCORE_LENGTH];
void write_to_EEPROM();

void Highscore::init()
{
	read_highscore(highscore_list);
}

void Highscore::write_highscore(char name[NAME_LENGTH], uint16_t score)
{
	int8_t new_place = HIGHSCORE_LENGTH - 1;

	while (new_place >= 0)
	{
		if (score <= highscore_list[new_place].score)
		{
			break;
		}
		new_place--;
	}
	new_place++;
	
	//Score is not better than the last entry
	if (new_place == HIGHSCORE_LENGTH )
	{
		return;
	}
	
	//Swap all scores down
	for (uint8_t i = HIGHSCORE_LENGTH - 1 ; i > new_place ; i--)
	{
		highscore_list[i] = highscore_list[i-1];
	}
	
	//Save new score
	for ( uint8_t i = 0 ; i < NAME_LENGTH ; i++)
	{
		highscore_list[new_place].name[i] = name[i];		
	}
	highscore_list[new_place].score = score;
	
	write_to_EEPROM();
}


void Highscore::read_highscore(highscore_element_t* highscore)
{
	for (uint8_t i = 0 ; i < HIGHSCORE_LENGTH ; i++)
	{		
		char temp[NAME_LENGTH + 2];
		for (uint8_t j = 0 ; j < NAME_LENGTH + 2 ; j++)
		{
			//Read from the EEPROM.
			temp[j] = EEPROM.read(i*(NAME_LENGTH + 2) + j);
		}
		for (uint8_t j = 0 ; j < NAME_LENGTH ; j++)
		{
			highscore[i].name[j] = temp[j];
		}
		highscore[i].score = ((uint16_t) temp[NAME_LENGTH] << 8 ) + (uint16_t)temp[NAME_LENGTH +1];
	}
}

void Highscore::reset_highscore()
{
	for (uint8_t i = 0 ; i < HIGHSCORE_LENGTH ; i++)
	{
		for (uint8_t j = 0 ; j < NAME_LENGTH ; j++)
		{
			highscore_list[i].name[j] = '-';
		}
		highscore_list[i].score = (uint16_t)0;
	}
	write_to_EEPROM();
}

void Highscore::send_highscore()
{
	for (uint8_t i = 0 ; i < HIGHSCORE_LENGTH ; i++)
	{
		//Serial.print("Sending highscore ");
		//Serial.print(i);
		//Serial.print("\n");
		CanMessage message;
		message.id = HIGHSCORE;
		for (uint8_t j = 0 ; j < NAME_LENGTH ; j++)
		{
			message.data[j] = highscore_list[i].name[j];
		}
		message.data[NAME_LENGTH] = (uint8_t)(highscore_list[i].score >> 8);
		message.data[NAME_LENGTH + 1] = (uint8_t)( highscore_list[i].score & 0x00FF);
		message.len = NAME_LENGTH + 2;
		
		while (!CAN.ready());
		message.send();
		delay(5);
		//message.print(HEX);
		//Serial.print("Finished sending highscore ");
		//Serial.print(i);
		//Serial.print("\n");
	}
	
	Serial1.print("Score sendt\n");
}

void write_to_EEPROM()
{
	//Save highscore to EEPROM
	
	for (uint8_t i = 0 ; i < HIGHSCORE_LENGTH ; i++)
	{
		for (uint8_t j = 0 ; j < NAME_LENGTH ; j++)
		{
			EEPROM.write(i*(NAME_LENGTH + 2) + j, highscore_list[i].name[j]);
		}
		EEPROM.write(i*(NAME_LENGTH + 2) + NAME_LENGTH , (uint8_t)(highscore_list[i].score >> 8) );
		EEPROM.write(i*(NAME_LENGTH + 2) + NAME_LENGTH + 1 , (uint8_t)( highscore_list[i].score & 0x00FF) );
	}
}