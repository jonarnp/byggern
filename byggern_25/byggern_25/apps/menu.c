/*
 * menu.c
 *
 * Created: 24.09.2014 11:17:41
 *  Author: boerikse
 */ 

#include "menu.h"
#include "../drivers/oled.h"
#include <avr/pgmspace.h>
#include "../drivers/joy.h"
#include <stdbool.h>

typedef struct menu_action {
	uint8_t up_iterations;
	uint8_t down_iterations;
	uint8_t select_iterations;
	bool goUp;
	bool goDown;
	bool select;
}menu_action_t;

// Strings
const unsigned char PROGMEM main_menu_str[4][20] = {
	{"Main menu"},
	{"  -Play"},
	{"  -Highscore"},
	{"  -Settings"}
};

// States
state_t current_state;

//Variables
menu_action_t menu_select;
int8_t selected_line = 0;

void menu_update_screen();

void menu_init()
{
	current_state = main_s;
	oled_init();
	oled_clear();
	JOY_init();
	
	// Initialize menu action
	menu_select.down_iterations = 0;
	menu_select.up_iterations = 0;
	menu_select.select_iterations = 0;
	menu_select.goDown = false;
	menu_select.goUp = false;
	menu_select.select = false;
	menu_update_screen();
}

void menu_update()
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
		if (++menu_select.select_iterations>SELECT_DELAY)
		{
			menu_select.select_iterations = 0;
			menu_select.select = true;
		}
	}
	else
	{
		menu_select.select_iterations = 0;
	}	
	
	if (menu_select.goDown || menu_select.goUp || menu_select.select) menu_update_screen();
}

void menu_update_screen()
{
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
	
	oled_clear();
	oled_goto_column(0);
	oled_goto_line(0);
	switch(current_state)
	{
		case main_s:
			// Check for over/under-roll
			if (selected_line < play) selected_line = settings;
			if (selected_line > settings) selected_line = play;
		
			//char *str;
			oled_print_p(main_menu_str[0]);
			
			oled_goto_column(0);
			oled_goto_line(1);
			oled_print_p(main_menu_str[1]);
			if (selected_line == play)
			{
					oled_print(" *");
					if (menu_select.select) current_state = play_s;
			}
			
			oled_goto_column(0);
			oled_goto_line(2);
			oled_print_p(main_menu_str[2]);
			if (selected_line == highscore)
			{
				oled_print(" *");
				if (menu_select.select) current_state = highscore_s;
			}
			
			oled_goto_column(0);
			oled_goto_line(3);
			oled_print_p(main_menu_str[3]);
			if (selected_line == settings)
			{
				oled_print(" *");
				if (menu_select.select) current_state = settings_s;
			}
			break;
		
		case play_s:
			oled_print("PLAAAAAAAAY");
			if (menu_select.select) current_state = main_s;
			break;
		
		case settings_s:
		oled_print("Settings??? hihi");
		if (menu_select.select) current_state = main_s;
		break;
		
		case highscore_s:
		oled_print("Gruppe 25 :)");
		if (menu_select.select) current_state = main_s;
		break;
	}
	menu_select.select = false;
}