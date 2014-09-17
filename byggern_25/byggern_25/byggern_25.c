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

int main(void)
{
	cli();
	DDRB = 0x01;
	USART_Init(MYUBRR,true);
	XMEM_En();
	//JOY_init();
	SLIDER_init();
	sei();
	
	//JOY_calibrate();	
	//SLIDER_calibrate();
	_delay_ms(1000);
	oled_init();
	uint8_t i=0;
	oled_clear_line(1);
	
	while(1)
	{
		if (++i >=255) i=0;
		
		oled_clear();
		//oled_putchar('B');
		oled_print("Test mellomrom\n Line 2");
		_delay_ms(100);
		//ADC_gal_test();
		//_delay_ms(100);
		//test = ADC_read(4);
		//printf("ADC channel 0 %d\n",test);
		//printf("Pin test %d\n", get_bit(PINB,PINB1));
		//ADC_read_test();
		//Test_joy();
		//Test_slider();
	}
}

