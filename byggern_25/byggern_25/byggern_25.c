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

#include "drivers/usart.h"

#define toggle_bit(reg,bit) (reg^= (1 << bit))

int main(void)
{
	cli();
	USART_Init(MYUBRR,true);
	DDRB = 0x01;
	sei();
	while(1)
	{
		////TODO:: Please write your application code
		//toggle_bit(PORTB,PB0);
		//printf("a");
		_delay_ms(1);
		//if(USART_DataReceived())
		//{
			////printf("Received data: %c\n", USART_Receive());
			//USART_Transmit(USART_Receive());
		//}
		//USART_Transmit(USART_Receive());
	}
}

ISR(USART0_RXC_vect)
{
	USART_Transmit(USART_Receive());
}