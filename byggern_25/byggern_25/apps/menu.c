/*
 * menu.c
 *
 * Created: 24.09.2014 11:17:41
 *  Author: boerikse
 */ 

#include <stdbool.h>
#include <avr/pgmspace.h>
#include "menu.h"
#include "game_board.h"
#include "highscore.h"
#include "pong.h"
#include "music.h"
#include "../drivers/oled.h"
#include "../drivers/joy.h"
#include "../drivers/slider.h"

/* Struct containing all menu actions */
typedef struct menu_action {
	uint8_t up_iterations;
	uint8_t down_iterations;
	uint8_t select_iterations;
	bool goUp;
	bool goDown;
	bool select;
}menu_action_t;

/* Main menu strings stored in progmem */
const unsigned char PROGMEM main_menu_str[5][20] = {
	{"Main menu"},
	{"  -Play"},
	{"  -Highscore"},
	{"  -Settings"},
	{"  -Pong"}
};

/* Settings menu strings stored in progmem */
const unsigned char PROGMEM settings_menu_str[6][20] = {
	{"Settings"},
	{"  -Joystick"},
	{"  -Sliders"},
	{"  -Music "},
	{"  -Reset highscore"},
	{"  -Back"}
};

//States
static state_t current_state;

//Variables
menu_action_t menu_select;
int8_t selected_line = 0;

void menu_update_screen();

void menu_init()
{
	current_state = main_s;
	
	music_change_state(MENU_MUSIC);
	
	oled_clear();
	
	// Initialize menu action
	menu_select.down_iterations = 0;
	menu_select.up_iterations = 0;
	menu_select.select_iterations = 0;
	menu_select.goDown = false;
	menu_select.goUp = false;
	menu_select.select = false;
	menu_update_screen();
}

void menu_action_update()
{
	if (JOY_getDirection() == DOWN)
	{
		if (++menu_select.down_iterations>SELECT_DELAY)
		{
			menu_select.down_iterations = 0;
			menu_select.goDown = true;
		}
	}
	else
	{
		menu_select.down_iterations = 0;
	}
	if (JOY_getDirection() == UP)
	{
		if (++menu_select.up_iterations>SELECT_DELAY)
		{
			menu_select.up_iterations = 0;
			menu_select.goUp = true;
		}
	}
	else
	{
		menu_select.up_iterations = 0;
	}
	//printf("Joy_button is : %d\n",JOY_button());
	if (JOY_button())
	{
		if (++menu_select.select_iterations>SELECT_BUTTON_DELAY)
		{
			menu_select.select_iterations = 0;
			menu_select.select = true;
		}
	}
	else
	{
		menu_select.select_iterations = 0;
	}	
	
	if (menu_select.goDown || menu_select.goUp || menu_select.select) menu_update_state();
}

void menu_update_state()
{
	//printf("menu_update_state: %d\n", current_state);
	if (menu_select.goUp)
	{
		selected_line--;
		menu_select.goUp = false;
	}
	if (menu_select.goDown)
	{
		selected_line++;
		menu_select.goDown = false;
	}
	switch(current_state)
	{
		case main_s:
			// Check for over/under-roll
			if (selected_line < play) selected_line = pong;
			if (selected_line > pong) selected_line = play;
		
			if (menu_select.select)
			{
				switch (selected_line)
				{
					case play:
						current_state = play_s;
						selected_line = 0;
						break;
					case highscore:
						current_state = highscore_s;
						selected_line = 0;
						break;
					case settings:
						current_state = settings_s;
						selected_line = 0;
						break;
					case pong:
						current_state = pong_s;
						selected_line = 0;
						break;
				}
			}
			break;
		case play_s:
			if (menu_select.select) current_state = main_s;
			selected_line = 0;
			break;
			
		case highscore_s:
			if (menu_select.select) current_state = main_s;
			selected_line = 0;
			break;
		
		case settings_s:
			// Check for over/under-roll
			if (selected_line < calibrate_joy) selected_line = back;
			if (selected_line > back) selected_line = calibrate_joy;
					
			if (menu_select.select)
			{
				switch (selected_line)
				{
					case calibrate_joy:
						//current_state = calibrate_joy;
						selected_line = 0;
						JOY_calibrate(); //should not be here
						break;
					case calibrate_sliders:
						//current_state = calibrate_sliders;
						selected_line = 0;
						SLIDER_calibrate(); //should not be here
						break;
					case switch_music:
						music_toggle();
						break;
					case reset_highscore:
						highscore_reset();
						current_state = main_s;
						selected_line = 0;
						break;
					case back:
						current_state = main_s;
						selected_line = 0;
						break;
				}
			}
			break;
		
		case pong_s:
			if (menu_select.select) current_state = main_s;
			selected_line = 0;
			break;

	}
	menu_select.select = false;
	menu_update_screen();
}

void menu_update_screen()
{
	
	oled_clear();
	oled_goto_column(0);
	oled_goto_line(0);
	switch(current_state)
	{
		case main_s:
			music_change_state(MENU_MUSIC);
		
			oled_print_p(main_menu_str[0]);
			
			oled_goto_column(0);
			oled_goto_line(1);
			oled_print_p(main_menu_str[1]);
			if (selected_line == play)
			{
				oled_print(" *");
			}
			
			oled_goto_column(0);
			oled_goto_line(2);
			oled_print_p(main_menu_str[2]);
			if (selected_line == highscore)
			{
				oled_print(" *");
			}
			
			oled_goto_column(0);
			oled_goto_line(3);
			oled_print_p(main_menu_str[3]);
			if (selected_line == settings)
			{
				oled_print(" *");
			}
			
			oled_goto_column(0);
			oled_goto_line(4);
			oled_print_p(main_menu_str[4]);
			if (selected_line == pong)
			{
				oled_print(" *");
			}
			break;
		
		case play_s:
			//oled_print("PLAAAAAAAAY");
			music_change_state(PLAY_MUSIC);
			play_game_board();
			
			menu_init(); //finished playing
			break;
		
		case settings_s:
			music_change_state(MENU_MUSIC);
		
			oled_print_p(settings_menu_str[0]);
			
			oled_goto_column(0);
			oled_goto_line(1);
			oled_print_p(settings_menu_str[1]);
			if (selected_line == calibrate_joy)
			{
				oled_print(" *");
			}
			
			oled_goto_column(0);
			oled_goto_line(2);
			oled_print_p(settings_menu_str[2]);
			if (selected_line == calibrate_sliders)
			{
				oled_print(" *");
			}
			
			oled_goto_column(0);
			oled_goto_line(3);
			oled_print_p(settings_menu_str[3]);
			if (music_enabled())
			{
				oled_print("ON");
			}
			else
			{
				oled_print("OFF");
			}
			if (selected_line == switch_music)
			{
				oled_print(" *");
			}
			
			oled_goto_column(0);
			oled_goto_line(4);
			oled_print_p(settings_menu_str[4]);
			if (selected_line == reset_highscore)
			{
				oled_print(" *");
			}
			
			oled_goto_column(0);
			oled_goto_line(5);
			oled_print_p(settings_menu_str[5]);
			if (selected_line == back)
			{
				oled_print(" *");
			}
			break;
		
		case highscore_s:
			music_change_state(MENU_MUSIC);
			
			//oled_print("Gruppe 25 :)");
			receive_highscore_list();
			display_highscore_list(0);
			
			oled_goto_column(0);
			oled_goto_line(7);
			oled_print_p(PSTR("-Back *"));
			break;
		case pong_s:
			//oled_print("Pong it up!");
			music_change_state(PLAY_MUSIC);
			play_pong();
			
			menu_init(); //finished playing
			break;
	}
	//printf("End of update screen, goDown: %d, goUp: %d, select: %d\n",menu_select.goDown,menu_select.goUp,menu_select.select);
}