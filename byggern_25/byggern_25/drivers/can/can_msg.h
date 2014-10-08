/*
 * can_msg.h
 *
 * Created: 01.10.2014 09:26:01
 *  Author: helders
 */ 


#ifndef CAN_MSG_H_
#define CAN_MSG_H_

#include <stdbool.h>

typedef struct can_tx_message{
	uint16_t id;
	uint8_t length;
	uint8_t data[8];
	uint8_t priority;
}can_tx_message_t;

typedef struct can_rx_message{
	uint16_t id;
	uint8_t length;
	uint8_t data[8];
}can_rx_message_t;

#define CAN_QUEUE_LENGTH 5

void can_init();

/*
Transmit a CAN message. If the transmission is unsuccessful, 0 is returned.
Priority [0,3], where 3 is the highest priority.
*/
uint8_t transmit_can_message(can_tx_message_t message);

/*
Read a message from the CAN queue. If no messages are present, 0 is returned.
*/
uint8_t read_can_message(can_rx_message_t *message);

/*
Returns true if a queue overlow has occured. It resets the queue overflow flag when read.
*/
bool CAN_queue_overflow();

/*
Flushes the CAN recieve queue.
*/
void CAN_queue_flush();

#endif /* CAN_MSG_H_ */