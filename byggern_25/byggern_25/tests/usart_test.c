/*
 * usart_test.c
 *
 * Created: 03.09.2014 14:51:44
 *  Author: boerikse
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include "usart_test.h"
#include "../drivers/usart.h"

void USART_test(void)
{
	if(USART_DataReceived())
	{
	//printf("Received data: %c\n", USART_Receive());
	USART_Transmit(USART_Receive());
	}
	USART_Transmit(USART_Receive());
}

ISR(USART0_RXC_vect)
{
	USART_Transmit(USART_Receive());
}