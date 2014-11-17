/*
 * can_msg.h
 *
 * Created: 01.10.2014 09:26:01
 *  Author: helders
 */ 


#ifndef CAN_MSG_H_
#define CAN_MSG_H_

#include <stdbool.h>

#define CAN_QUEUE_LENGTH 5

/* Definition of a transmit CAN message struct */
typedef struct can_tx_message{
	uint16_t id;
	uint8_t length;
	uint8_t data[8];
	uint8_t priority;
} can_tx_message_t;

/* Definition of a receive CAN message struct */
typedef struct can_rx_message{
	uint16_t id;
	uint8_t length;
	uint8_t data[8];
} can_rx_message_t;

/*
Initialize CAN, setting up mpc2515 and enables interrupts
*/
void can_init();

/*
Transmit a CAN message. 
@param can_tx_message_t. message.priority [0,3], where 3 is the highest priority. 8 byte max data size
@return uint8_t. 0 if unsuccessful transmission, otherwise 1.
*/
uint8_t transmit_can_message(can_tx_message_t message);

/*
Read a message from the CAN queue.
@param can_rx_message_t*. Received message is stored at this address. 8 byte max data size
@return uint8_t. If no messages are present, 0 is returned, otherwise 1.
*/
uint8_t read_can_message(can_rx_message_t *message);

/*
Returns the number of messages in the receive queue.
@return uint8_t. Range [0,CAN_QUEUE_LENGTH]
*/
uint8_t number_of_rx_in_queue();

/*
Check queue for overflow
@return bool. true if a queue overflow has occurred. It resets the queue overflow flag when read.
*/
bool CAN_queue_overflow();

/*
Flushes the CAN receive queue.
*/
void CAN_queue_flush();

#endif /* CAN_MSG_H_ */