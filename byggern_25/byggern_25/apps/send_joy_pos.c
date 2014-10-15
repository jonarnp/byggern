/*
 * send_joy_pos.c
 *
 * Created: 15.10.2014 09:38:57
 *  Author: jonarnp
 */ 

#include <stdio.h>
#include "send_joy_pos.h"
#include "../drivers/joy.h"
#include "../drivers/can/can_msg.h"

void send_joy_pos_init()
{
	can_init();	
}

void transmit_joy_pos()
{
	JOY_pos_t joy_pos = JOY_getPosition();
	//printf("Joy_pos x: %d, y: %d\n",joy_pos.x,joy_pos.y);
	
	can_tx_message_t message;
	message.id = JOY_POS_CAN_ID;
	message.data[0] = (uint8_t)((joy_pos.x & 0xFF00)>>8);
	message.data[1] = (uint8_t)(joy_pos.x & 0x00FF);
	message.data[2] = (uint8_t)((joy_pos.y & 0xFF00)>>8);
	message.data[3] = (uint8_t)(joy_pos.y & 0x00FF);
	
	message.length = 4;
		
	transmit_can_message(message);
}