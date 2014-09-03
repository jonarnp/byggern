/*
 * byggern_25.c
 *
 * Created: 27.08.2014 13:05:25
 *  Author: helders
 */ 

#define F_CPU 4915200UL  // 4.9152 MHz
#define BAUD 38400
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>

#include "bit_op.h"
#include "drivers/usart.h"

#include "tests/sram_test.h"

int main(void)
{
	USART_Init(MYUBRR);
	XMEM_En();
	DDRB = 0x01;
	
	//SRAM test counting errors
	//uint8_t errors[512];
	//calloc(512,sizeof(uint8_t));
	//uint16_t i;
	//for(i = 0; i < 10; i++){
		//SRAM_test_count(errors);
	//}
	//for(i = 0; i < 512; i++){
		//if(errors[i]!= 0) printf("addr: %d, numErrors: %d\n", i, errors[i]);
	//}
	//
	
	//SRAM test
	SRAM_test();
	
	while(1)
	{
		////TODO:: Please write your application code
		//toggle_bit(PORTB,PB0);
		//printf("a");
		if(USART_DataReceived())
		{
			printf("Received data: %c\n", USART_Receive());
		}
		//USART_Transmit(USART_Receive());
		
		_delay_ms(1000);
	}
}