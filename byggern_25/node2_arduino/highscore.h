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
	/*
	Initialize the highscore.
	*/
	void init();
	
	/*
	Write a highscore to the system.
	@param name. char[] containing the player name. Length must be equal to NAME_LENGTH.
	@param score. uint16_t containing the player score.
	*/
	void write_highscore(char name[NAME_LENGTH], uint16_t score);
	
	/*
	Read the highscore list.
	@param highscore. Pointer to a highscore_element_t[] array where the score should be placed.
	*/
	void read_highscore(highscore_element_t* highscore);
	
	/*
	Reset the highscore list.
	*/
	void reset_highscore();
	
	/*
	Send the highscore list using the CAN bus. The highscore is sent as a series of CAN messages in the HIGHSCORE can id format.
	*/
	void send_highscore();
};


#endif /* HIGHSCORE_H_ */