/*
 * highscore.h
 *
 * Created: 08.11.2014 15:16:38
 *  Author: Flua
 */ 


#ifndef HIGHSCORE_H_
#define HIGHSCORE_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define HIGHSCORE_LENGTH 4
#define NAME_LENGTH 3

typedef struct highscore_element{
	char name[NAME_LENGTH];
	uint16_t score;
}highscore_element_t;

class Highscore
{
 protected:


 public:
	void init();
	
	void write_highscore(char* name, uint16_t score);
	
	uint8_t read_highscore(highscore_element_t* highscore);
};


#endif /* HIGHSCORE_H_ */