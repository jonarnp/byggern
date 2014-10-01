/*
 * spi.h
 *
 * Created: 01.10.2014 09:23:17
 *  Author: helders
 */ 


#ifndef SPI_H_
#define SPI_H_

#define MOSI_Port PINB
#define MOSI_Pin PINB5
#define MOSI_DDR DDRB

#define MISO_Port PINB
#define MISO_Pin PINB6
#define MISO_DDR DDRB

#define SCK_Port PINB
#define SCK_Pin PINB7
#define SCK_DDR DDRB

#define SS_Port PINB
#define SS_Pin PINB4
#define SS_DDR DDRB

#define MSB_FIRST 1

void SPI_init();

void SPI_send();

char SPI_read();


#endif /* SPI_H_ */