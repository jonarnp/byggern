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

/*
CAN send identifiers
*/
#define GAME_CONTROLS 0x100
#define GAME_START 0x110
#define HIGHSCORE_REQUEST 0x120
#define WRITE_HIGHSCORE 0x130
#define SPOTIFY_COMMAND 0x140

/*
GAME_CONTROLS
data[0] = MSB of desired servo position, sent as int16
data[1] = LSB of desired servo position, sent as int16
data[2] = MSB of desired motor speed, sent as int16
data[3] = LSB of desired motor speed, sent as int16
data[4] = Solenoid fire, sent as uint8. Treat it as a bool

GAME_START
empty command

HIGHSCORE_REQUEST
empty command
*/

/*
CAN receive identifiers
*/
#define GAME_STATUS 0x010 /* Current score and game over */ 
#define HIGHSCORE 0x011
//#define RXF2 0x012
//#define RXF3 0x013
//#define RXF4 0x014
//#define RXF5 0x015

/*
GAME_STATUS
data[0] = Game over / finished, sent as bool/uint8
data[1] = MSB of current score, sent as uint16
data[2] = LSB of current score, sent as uint16

HIGHSCORE
data[0] = First char in name
data[1] = Second char in name
data[2] = Third char in name
data[3] = MSB of highscore, sent as uint16
data[4] = LSB of highscore, sent as uint16
*/

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
Returns the number of messages in the recieve queue.
*/
uint8_t number_of_rx_in_queue();

/*
Returns true if a queue overlow has occured. It resets the queue overflow flag when read.
*/
bool CAN_queue_overflow();

/*
Flushes the CAN recieve queue.
*/
void CAN_queue_flush();

#endif /* CAN_MSG_H_ */