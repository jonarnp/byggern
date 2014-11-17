/*
 * can_ctrl.h
 *
 * Created: 01.10.2014 09:25:09
 *  Author: helders
 */ 


#ifndef CAN_CTRL_H_
#define CAN_CTRL_H_

#include <avr/io.h>

/* Accepted CAN receive identifiers. See can_id.h for usage */
#define RXF0 0x010
#define RXF1 0x011
#define RXF2 0x012
#define RXF3 0x013
#define RXF4 0x014
#define RXF5 0x015

/* Interrupt pin */
#define MCP_Int_Port PIND
#define MCP_Int_Pin PIND2
#define MCP_Int_DDR DDRD

typedef struct buffer_recieve{
	uint16_t id;
	uint8_t length;
	uint8_t data[8];
} buffer_recieve_t;

/*
Initialize mpc2515. Setting up SPI and configuring the mcp2515: receive identifiers, enable interrupts, filtering and normal mode
*/
uint8_t mcp2515_init();

/*
Send a "Request to send" command to a specific buffer
@param uint8_t buffer_nr. Range [0,2]
*/
void mcp2515_request_to_send(uint8_t buffer_nr);

/*
Read the mcp2515 status
@return uint8_t
*/
uint8_t mcp2515_read_status();

/*
Load data into a specific transmit buffer
@param uint8_t buffer_nr. Range [0,2]
@param uint8_t* data. Pointer to a data array of uint8s
@param uint8_t length. Length of data array. Range [0,8]
*/
void mcp2515_load_tx_data(uint8_t buffer_nr, uint8_t* data, uint8_t length);

/*
Select a transmit buffer and set identifier
@param uint8_t buffer_nr. Range [0,2]
@param uint16_t ID. 11 bit identifier
*/
void mcp2515_select_tx_identifier(uint8_t buffer_nr, uint16_t ID);

/*
Select a transmit buffer and set priority
@param uint8_t buffer_nr. Range [0,2]
@param uint8_t priority. Range [0,3]
*/
void mcp2515_set_tx_priority(uint8_t buffer_nr, uint8_t priority);

/*
Read data from a specific receive buffer
@param buffer_recieve_t*: id, length and data*. Pointer to the received data struct
*/
void mcp2515_read_rx_buffer(buffer_recieve_t *recieved);

#endif /* CAN_CTRL_H_ */