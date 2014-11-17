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

/* Highscore defines */
#define HIGHSCORE_LENGTH 4
#define NAME_LENGTH 3
#define SET_NAME_DELAY 50

/* Struct for each highscore element */
typedef struct highscore_element{
	char name[NAME_LENGTH];
	uint16_t score;
}highscore_element_t;

/*
Receiving four highscores from node 2, and storing them in highscore_list[HIGHSCORE_LENGTH].
The function starts by sending a highscore request to node 2, then loops until highscores are sent back
@return uint8_t. 1 when successfully received highscores
*/
uint8_t receive_highscore_list();

/*
Print the content of highscore_list[HIGHSCORE_LENGTH] to the OLED
@param uint8_t offset. Which line to start printing from
*/
void display_highscore_list(uint8_t offset);

/*
Checks if the input score is better than the worst score in highscore_list[HIGHSCORE_LENGTH]
@param uint16_t score. Score of recently finished game
@return bool. True score should be added to the highscore list
*/
bool add_to_highscore(uint16_t score);

/*
Display a menu for setting three initials with the joystick. Save highscore by pressing left slider button
@param uint16_t score. Score of recently finished game
@return highscore_element_t: char name[NAME_LENGTH] and uint16_t score
*/
highscore_element_t enter_initials(uint16_t score);

/*
Send a new highscore to node 2 for storing in eeprom
@param highscore_element_t: char name[NAME_LENGTH] and uint16_t score
*/
void send_new_highscore(highscore_element_t entry);

/*
Send reset highscore command to node 2 for resetting all highscores in eeprom
*/
void highscore_reset();

#endif /* HIGHSCORE_H_ */