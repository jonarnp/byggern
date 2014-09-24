/*
 * menu.h
 *
 * Created: 24.09.2014 11:18:03
 *  Author: boerikse
 */ 


#ifndef MENU_H_
#define MENU_H_

#define SELECT_DELAY 254

typedef enum state{
	main_s,
	play_s,
	highscore_s,
	settings_s
}state_t;

#define MAIN_MENU_LENGTH 3
typedef enum main_menu{
	play,
	highscore,
	settings
}main_menu_t;

void menu_init();

void menu_update();

state_t get_current_state();

#endif /* MENU_H_ */