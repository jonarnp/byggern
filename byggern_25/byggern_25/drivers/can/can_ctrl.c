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
	
	// Set loopback mode
	mcp2515_write(MCP_CANCTRL, MODE_LOOPBACK);
	
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
	/*
	HER ER VI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	Legg inn buffer_nr i sendegreia.
	*/
	SPI_select();
	SPI_send(MCP_RTS_ALL); // Send request-to-send command. Could use MCP_RTS_TX[0|1|2]
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

void mcp2515_transmit(uint8_t buffer_nr, unsigned char* data, uint8_t length)
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

void mcp2515_select_tx_address(uint8_t buffer_nr, uint16_t address)
{
	// Send load TX buffer command, start at TXBnSIDH
	uint8_t address_low = (address <<5);
	uint8_t address_high = (address >>3)&0xFF;
	SPI_select();
	SPI_send(MCP_LOAD_TX0+2*buffer_nr);
	SPI_send(address_high);
	SPI_send(address_low);
	SPI_deselect();
}
