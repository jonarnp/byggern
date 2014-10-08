/*
 * can_ctrl.h
 *
 * Created: 01.10.2014 09:25:09
 *  Author: helders
 */ 


#ifndef CAN_CTRL_H_
#define CAN_CTRL_H_

#include <avr/io.h>

#define RXF0 0x100
#define RXF1 0x200
#define RXF2 0x300
#define RXF3 0x425
#define RXF4 0x500
#define RXF5 0x600

//Int pin
#define MCP_Int_Port PIND
#define MCP_Int_Pin PIND2
#define MCP_Int_DDR DDRD

typedef struct buffer_recieve{
	uint16_t id;
	uint8_t length;
	uint8_t data[8];
}buffer_recieve_t;

uint8_t mcp2515_init();

uint8_t mcp2515_read(uint8_t address);

void mcp2515_write(uint8_t address, uint8_t data);

void mcp2515_request_to_send(uint8_t buffer_nr);

void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data);

void mcp2515_reset();

uint8_t mcp2515_read_status();

void mcp2515_load_tx_data(uint8_t buffer_nr, uint8_t* data, uint8_t length);

void mcp2515_select_tx_identifier(uint8_t buffer_nr, uint16_t ID);

void mcp2515_buffer_recieve(buffer_recieve_t *recieved);

#endif /* CAN_CTRL_H_ */