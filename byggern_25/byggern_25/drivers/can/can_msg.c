/*
 * can_msg.c
 *
 * Created: 01.10.2014 09:25:49
 *  Author: helders
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include "can_msg.h"
#include "can_ctrl.h"
#include "../../bit_op.h"

can_rx_message_t queue[CAN_QUEUE_LENGTH];
uint8_t front = 0;	//The front position of the queue
uint8_t count = 0;	//Number of elements in the queue
bool queue_overflow = false;

void can_init()
{
	mcp2515_init();
	
	//Enable interupt from CAN source
	cli();
	set_bit(MCUCR,ISC01);
	clear_bit(MCUCR,ISC00);
	set_bit(GICR,INT0);
	sei();
}


uint8_t transmit_can_message(can_tx_message_t message)
{
	uint8_t status = mcp2515_read_status();
	uint8_t buffer_nr;
	if ((status&0x40) == 0)
	{
		//Transmit using buffer 2
		buffer_nr = 2;
	}
	else if ((status&0x10) == 0)
	{
		//Transmit using buffer 1
		buffer_nr = 1;
	}
	else if ((status&0x04) == 0)
	{
		//Transmit using buffer 0
		buffer_nr = 0;
	}
	else
	{
		return 0;
	}
	mcp2515_select_tx_identifier(buffer_nr, message.id);
	mcp2515_load_tx_data(buffer_nr,message.data,message.length);
	mcp2515_set_tx_priority(buffer_nr,message.priority);
	mcp2515_request_to_send(buffer_nr);
	return 1;
}

uint8_t read_can_message(can_rx_message_t *message)
{
	if (count == 0) return 0; //Queue is empty
	
	//get next element
	message->id = queue[front].id;
	for (uint8_t i = 0;i<queue[front].length;i++) message->data[i] = queue[front].data[i];
	message->length = queue[front].length;
	
	//Delete the oldest message
	front=(front+1)%CAN_QUEUE_LENGTH;
	--count;
	return 1;
}

bool CAN_queue_overflow()
{
	bool temp = queue_overflow;
	queue_overflow = false;
	return temp;
}

void CAN_queue_flush()
{
	front = 0;
	count = 0;
}

void read_can_buffer()
{
	uint8_t status = mcp2515_read_status();
	buffer_recieve_t message;
	
	while (status & 0x03)
	{
		mcp2515_read_rx_buffer(&message);
		//Queue full?
		if (count == CAN_QUEUE_LENGTH)
		{
			//Delete the oldest message
			front=(front+1)%CAN_QUEUE_LENGTH;
			--count;
			queue_overflow = true;
		}

		uint8_t next_queue_pos = (front+count)%CAN_QUEUE_LENGTH;
		queue[next_queue_pos].id = message.id;
		for (uint8_t i = 0;i<message.length;i++) queue[next_queue_pos].data[i] = message.data[i];
		queue[next_queue_pos].length = message.length;
		++count;
		
		//Check for more data
		status = mcp2515_read_status();
	}
}

ISR(INT0_vect)
{
	read_can_buffer();
}