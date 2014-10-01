/*
 * spi.c
 *
 * Created: 01.10.2014 09:23:06
 *  Author: helders
 */ 

#include <avr/io.h>
#include "../../bit_op.h"
#include "spi.h"

void SPI_init()
{
	// Set pin directions
	set_bit(MOSI_DDR,MOSI_Pin);		// Set MOSI as output
	set_bit(SCK_DDR,SCK_Pin);		// Set SCK as output
	set_bit(SS_DDR,SS_Pin);			// Set SS as output
	clear_bit(MISO_DDR,MISO_Pin);	// Set MISO as input
	
	//Enable SPI as Master, and speed as fosc/16
	SPCR = (1 << SPE)|(1 << MSTR)|(1<<SPR0);
	
	//Select data order
	if (MSB_FIRST == 1)
	{
		clear_bit(SPCR,DORD);	//Transmit MSB first
	}
	else
	{
		set_bit(SPCR,DORD);		//Transmit LSB first
	}
}

void SPI_send(unsigned char cData)
{
	//Start transmission
	SPDR = cData;	
	
	//Wait for transmission to complete
	while(!(SPSR & (1<<SPIF)));
}

unsigned char SPI_read()
{		
	//Start dummy transmission
	SPDR = READ_DUMMY;
		
	//Wait for dummy transmission to complete
	while(!(SPSR & (1<<SPIF)));
	
	return SPDR;
}

void SPI_select()
{
	//Select slave
	clear_bit(SS_Port,SS_Pin);
}

void SPI_deselect()
{
	//Deselect slave
	set_bit(SS_Port,SS_Pin);
}