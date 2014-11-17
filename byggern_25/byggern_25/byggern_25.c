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
#include "drivers/p1000switches.h"

/*
Initializes the system. Sets up the external memory, oled, sliders, joystick, CAN and applications
*/
void setup();

/*
Main loop. Initializes the system and continuously checks for menu updates from controls
*/
int main(void)
{
	setup();

	while(1)
	{
		menu_update();
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
	P1000_init();
	can_init();
	music_init();
	_delay_ms(100);
	
	JOY_calibrate();
	_delay_ms(100);
	SLIDER_calibrate();
	_delay_ms(100);
	
	menu_init();
	//printf("setup finished\n");
}