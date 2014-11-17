/*
 * spi.h
 *
 * Created: 01.10.2014 09:23:17
 *  Author: helders
 */ 


#ifndef SPI_H_
#define SPI_H_

/* Master out, slave in (MOSI) pin*/
#define MOSI_Port PORTB
#define MOSI_Pin PINB5
#define MOSI_DDR DDRB

/* Master in, slave out (MISO) pin*/
#define MISO_Port PINB
#define MISO_Pin PINB6
#define MISO_DDR DDRB

/* Serial clock (SCK) pin*/
#define SCK_Port PINB
#define SCK_Pin PINB7
#define SCK_DDR DDRB

/* Slave select (SS) pin*/
#define SS_Port PORTB
#define SS_Pin PINB4
#define SS_DDR DDRB

#define MSB_FIRST 1
#define READ_DUMMY 0x00

/*
Initialize SPI. Setting data direction, master and data order
*/
void SPI_init();

/*
Transmit a single character
@param unsigned char
*/
void SPI_send(unsigned char cData);

/*
Read a single character
@return unsigned char
*/
unsigned char SPI_read();

/*
Select slave
*/
void SPI_select();

/*
Deselect slave
*/
void SPI_deselect();

#endif /* SPI_H_ */