/*
 * menu.h
 *
 * Created: 24.09.2014 11:18:03
 *  Author: boerikse
 */ 


#ifndef MENU_H_
#define MENU_H_

/* Defines for menu system */
#define SELECT_DELAY 10
#define SELECT_BUTTON_DELAY 20
#define MAIN_MENU_LENGTH 4
#define SETTINGS_MENU_LENGTH 4

/* Defined system states */
typedef enum state{
	main_s,
	play_s,
	highscore_s,
	settings_s,
	pong_s
} state_t;

/* Defined main menu items */
typedef enum main_menu{
	play,
	highscore,
	settings,
	pong
} main_menu_t;

/* Defined settings menu items */
typedef enum settings_menu{
	calibrate_joy,
	calibrate_sliders,
	switch_music,
	reset_highscore,
	back
} settings_menu_t;

/*
Initialize the menu system
*/
void menu_init();

/*
Check for menu actions from the joystick. If action occurred, update the system states accordingly
*/
void menu_action_update();

#endif /* MENU_H_ */