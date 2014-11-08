/*
 * byggern_25.c
 *
 * Created: 27.08.2014 13:05:25
 *  Author: helders
 */ 

#define F_CPU 4915200UL  // 4.9152 MHz
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include <avr/pgmspace.h>

#include "bit_op.h"
#include "drivers/usart.h"
#include "drivers/xmem.h"
#include "tests/joy_test.h"
#include "drivers/joy.h"
#include "tests/slider_test.h"
#include "drivers/slider.h"
#include "drivers/oled.h"
#include "format.h"
#include "drivers/can/can_ctrl.h"
#include "drivers/can/can_msg.h"
#include "apps/game_board.h"
#include "apps/pong.h"
#include "apps/menu.h"

void setup();


state_t system_state;

int main(void)
{
	setup();

	while(1)
	{
		system_state = get_current_state();
		
		switch(system_state)
		{
			case main_s:
				//printf("byggern_25.c: main_s\n");
				menu_update();
				break;
				
			case play_s:
				//printf("byggern_25.c: play_s\n");
				play_game_board();
				menu_init(); //when finished playing
				break;
				
			case highscore_s:
				//printf("byggern_25.c: highscore_s\n");
				//receive_highscore_list();
				//display_highscore_list();
				menu_update();
				break;
				
			case settings_s:
				//printf("byggern_25.c: settings_s\n");
				menu_update();
				break;
				
			case pong_s:
				//printf("byggern_25.c: pong_s\n");
				play_pong();
				menu_init(); //when finished playing
				break;
		}
		_delay_ms(10);
	}
}

void setup()
{
	DDRB = 0x01;
	USART_Init(MYUBRR,false);
	XMEM_En();
	oled_init();
	oled_clear();
	SLIDER_init();
	JOY_init();
	send_joy_pos_init();
	music_init();
	_delay_ms(100);
	
	JOY_calibrate();
	_delay_ms(100);
	SLIDER_calibrate();
	_delay_ms(100);
	
	menu_init();
}