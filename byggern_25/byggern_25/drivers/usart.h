/*
 * usart.h
 *
 * Created: 27.08.2014 13:36:57
 *  Author: helders
 */ 


#ifndef USART_H_
#define USART_H_

void USART_Init( unsigned int ubrr);

void USART_Transmit( unsigned char data );

unsigned char USART_Receive( void );

bool USART_DataReceived(void);

#endif /* USART_H_ */