/*
 * can_ctrl.c
 *
 * Created: 01.10.2014 09:25:32
 *  Author: helders
 */ 
#include <avr/io.h>
#include <stdio.h>

#include "MCP2515.h"
#include "spi.h"
#include "can_ctrl.h"
#include "../../bit_op.h"

uint8_t mcp2515_init()
{
	uint8_t value;
	
	SPI_init(); // Initialize SPI
	mcp2515_reset(); // Send reset-command
	
	// Self-test
	value = mcp2515_read(MCP_CANSTAT);
	if ((value & MODE_MASK) != MODE_CONFIG) {
		printf("MCP2515 is NOT in configuration mode after reset!\n");
		return 1;
	}
	
	//Configure
	mcp2515_write(MCP_TXRTSCTRL,0x00);	//Set TXnRTS pins as digital input
	
	//set receive identifiers
	mcp2515_select_rx_identifier((uint8_t)0,RXF0);
	mcp2515_select_rx_identifier((uint8_t)1,RXF1);
	mcp2515_select_rx_identifier((uint8_t)2,RXF2);
	mcp2515_select_rx_identifier((uint8_t)3,RXF3);
	mcp2515_select_rx_identifier((uint8_t)4,RXF4);
	mcp2515_select_rx_identifier((uint8_t)5,RXF5);
	
	// Enable interrupt on receive
	mcp2515_write(MCP_CANINTE,0x03);
	
	//Enable filtering/masking of receive buffers
	mcp2515_bit_modify(MCP_RXB0CTRL,0b01100100,0xFF);
	mcp2515_bit_modify(MCP_RXB1CTRL,0b01100000,0xFF);
	
	// Set loopback mode
	mcp2515_write(MCP_CANCTRL, MODE_LOOPBACK);
	
	// Define INT pin as input on MCU
	clear_bit(MCP_Int_DDR,MCP_Int_Pin);
	
	// Set normal mode
	//mcp2515_write(MCP_CANCTRL, MODE_NORMAL);
	
	return 0;
}

uint8_t mcp2515_read(uint8_t address)
{
	uint8_t result;
	
	SPI_select();
	SPI_send(MCP_READ); // Send read command
	SPI_send(address); // Send address
	result = SPI_read(); // Read result
	SPI_deselect();
	
	return result;
}

void mcp2515_write(uint8_t address, uint8_t data)
{
	SPI_select();
	SPI_send(MCP_WRITE); // Send write command
	SPI_send(address); // Send address
	SPI_send(data); // Send data
	SPI_deselect();
}

void mcp2515_request_to_send(uint8_t buffer_nr)
{
	SPI_select();
	switch(buffer_nr)
	{
		case 0:
			SPI_send(MCP_RTS_TX0); // Send request-to-send command.
			break;
		case 1:
			SPI_send(MCP_RTS_TX1); // Send request-to-send command.
			break;
		case 2:
			SPI_send(MCP_RTS_TX2); // Send request-to-send command.
			break;
	}
	SPI_deselect();
}

void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data)
{
	SPI_select();
	SPI_send(MCP_BITMOD); // Send bit modify command
	SPI_send(address); // Send register address
	SPI_send(mask); // Send mask byte
	SPI_send(data); // Send data byte
	SPI_deselect();
}

void mcp2515_reset()
{		
	SPI_select();
	SPI_send(MCP_RESET); // Send reset command
	SPI_deselect();
}

uint8_t mcp2515_read_status()
{	
	uint8_t result;
		
	SPI_select();
	SPI_send(MCP_READ_STATUS); // Send read status command
	result = SPI_read(); // Read result
	SPI_deselect();
		
	return result;
}

void mcp2515_load_tx_data(uint8_t buffer_nr, uint8_t* data, uint8_t length)
{
	if (length > 8) length = 8;
	SPI_select();
	SPI_send(MCP_LOAD_TX0+1+2*buffer_nr);
	for (uint8_t i = 0; i<length; i++)
	{
		SPI_send(data[i]);
	}
	SPI_deselect();
	
	mcp2515_write(MCP_TXB0DLC + buffer_nr*0x10,length-1);
}

void mcp2515_select_tx_identifier(uint8_t buffer_nr, uint16_t ID)
{
	// Send load TX buffer command, start at TXBnSIDH
	uint8_t ident_low = (ID <<5);
	uint8_t ident_high = (ID >>3)&0xFF;
	SPI_select();
	SPI_send(MCP_LOAD_TX0+2*buffer_nr);
	SPI_send(ident_high);
	SPI_send(ident_low);
	SPI_deselect();
}

void mcp2515_buffer_recieve(buffer_recieve_t *recieved)
{
	uint8_t status = mcp2515_read_status();
	uint8_t ident_low;
	uint8_t ident_high;
	uint8_t ext_ident_low;
	uint8_t ext_ident_high;
	uint8_t dlc;
	if (status&0x01)
	{
		SPI_select();
		SPI_send(MCP_READ_RX0);
		ident_high = SPI_read();
		ident_low = SPI_read();
		ext_ident_high = SPI_read();
		ext_ident_low = SPI_read();
		dlc = SPI_read();
		
		// read length
		recieved->length = (dlc & 0x0F);
		
		// read data
		for (uint8_t i = 0; i < recieved->length; i++)
		{
			recieved->data[i] = SPI_read();
		}
		SPI_deselect();
		mcp2515_bit_modify(MCP_CANINTF,0x01,0);
		recieved->id = (((uint16_t)ident_high)<<3) +(ident_low>>5);
	}
	if ((status & 0x02)>>1)
	{
		SPI_select();
		SPI_send(MCP_READ_RX1);
		ident_high = SPI_read();
		ident_low = SPI_read();
		ext_ident_high = SPI_read();
		ext_ident_low = SPI_read();
		dlc = SPI_read();
		
		// read length
		recieved->length = (dlc & 0x0F);
		
		// read data
		for (uint8_t i = 0; i < recieved->length; i++)
		{
			recieved->data[i] = SPI_read();
		}
		SPI_deselect();
		mcp2515_bit_modify(MCP_CANINTF,0x02,0);
		recieved->id = (((uint16_t)ident_high)<<3) +(ident_low>>5);
	}
}
/*
//alt 1
void mcp2515_buffer_recieve(buffer_recieve_t *)


// alt 2
static buffer_recieve_t

// alt 3
malloc
*/

/*
This routine can only be used when MCP2515 is in configuration mode!
*/
void mcp2515_select_rx_identifier(uint8_t recieve_filter,uint16_t identifier)
{
	uint8_t ident_low = (identifier <<5);
	uint8_t ident_high = (identifier >>3)&0xFF;
	uint8_t address = 0;
	switch(recieve_filter)
	{
		case 0:
			address = MCP_RXF0SIDH;
			break;
		case 1:
			address = MCP_RXF1SIDH;
			break;
		case 2:
			address = MCP_RXF2SIDH;
			break;
		case 3:
			address = MCP_RXF3SIDH;
			break;
		case 4:
			address = MCP_RXF4SIDH;
			break;
		case 5:
			address = MCP_RXF5SIDH;
			break;
	}
	SPI_select();
	SPI_send(MCP_WRITE);
	SPI_send(address); // Send address	
	SPI_send(ident_high);
	SPI_send(ident_low);
	SPI_deselect();
}