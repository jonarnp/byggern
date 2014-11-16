/*
 * can_test.c
 *
 * Created: 08.10.2014 15:54:06
 *  Author: boerikse
 */ 
#include <stdbool.h>
#include "can_test.h"
#include <avr/io.h>
#include "../drivers/can/can_msg.h"
#include "../drivers/can/can_ctrl.h"
#include "../bit_op.h"
#include "../drivers/usart.h"

void can_test()
{
	static uint8_t nummer = 1;
	
	can_tx_message_t message;
	message.id = 0x200;
	message.length = 6;
	message.data[0] = 'J';
	message.data[1] = 'a';
	message.data[2] = 'd';
	message.data[3] = 'a';
	message.data[4] = '\0';
	message.data[5] = nummer;
	message.priority = 0;
	transmit_can_message(message);
	printf("Data sendt\n");
			
	if(USART_DataReceived())
	{
		char c = USART_Receive();
		if (c=='a')
		{
			can_tx_message_t message;
			message.id = 0x200;
			message.length = 6;
			message.data[0] = 'J';
			message.data[1] = 'a';
			message.data[2] = 'd';
			message.data[3] = 'a';
			message.data[4] = '\0';
			message.data[5] = nummer;
			message.priority = 0;
			transmit_can_message(message);
			printf("Data sendt\n");
			++nummer;
		}
		if(c=='r')
		{
			can_rx_message_t message;
			if(read_can_message(&message)==0)
			{
				printf("Queue empty!\n");
			}
			else
			{
				printf("Recieved CAN message is:\nID: %03x\nLength: %d\n",message.id,message.length);
				printf("Data: %s\n",&message.data[0]);
				printf("Nummer: %d\n",message.data [5]);
			}
		}
	}
	
	printf("Overflow? %d\n",CAN_queue_overflow());
}