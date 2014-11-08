/*
 * menu.h
 *
 * Created: 24.09.2014 11:18:03
 *  Author: boerikse
 */ 


#ifndef MENU_H_
#define MENU_H_

#define SELECT_DELAY 10
#define SELECT_BUTTON_DELAY 20
#define MAIN_MENU_LENGTH 4
#define SETTINGS_MENU_LENGTH 3

typedef enum state{
	main_s,
	play_s,
	highscore_s,
	settings_s,
	pong_s
} state_t;

typedef enum main_menu{
	play,
	highscore,
	settings,
	pong
} main_menu_t;

typedef enum settings_menu{
	back,
	calibrate_joy,
	calibrate_sliders
} settings_menu_t;

void menu_init();

void menu_update();

state_t get_current_state();

#endif /* MENU_H_ */