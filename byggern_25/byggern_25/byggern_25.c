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
#include "apps/send_joy_pos.h"
#include "apps/pong.h"
//#include "drivers/can/spi.h"

bool test = false;

int main(void)
{
	cli();
	DDRB = 0x01;
	USART_Init(MYUBRR,false);
	XMEM_En();
	sei();
	
	_delay_ms(1000);
	oled_init();
	uint8_t i=0;

	oled_clear();
	JOY_init();
	JOY_calibrate();
	menu_init();
	can_init();
	send_joy_pos_init();
	
	SLIDER_init();
	SLIDER_calibrate();
	

	pong_init();
	while(1)
	{
		play_pong();
		menu_update();
		transmit_joy_pos();
		//can_test();

		_delay_ms(10);

	}
}

