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

#include "bit_op.h"
#include "drivers/usart.h"
#include "drivers/xmem.h"

#include "tests/sram_test.h"
#include "tests/adc_test.h"

int main(void)
{
	cli();
	DDRB = 0x01;
	USART_Init(MYUBRR,true);
	XMEM_En();
	sei();
	
	SRAM_test();
	while(1)
	{
		//ADC_gal_test();
		_delay_ms(1);	
	}
}

