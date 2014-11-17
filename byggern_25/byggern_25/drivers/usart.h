/*
 * usart.h
 *
 * Created: 27.08.2014 13:36:57
 *  Author: helders
 
 This driver operates the USART0 port of the ATmega162 MCU.
 The port is selected to be in asynchronous operation, 8 data bits, no parity and 2 stop bits.

 The driver overrides normal operation of RXD0 and TXD0 pins.

 */ 


#ifndef USART_H_
#define USART_H_
 
/*
 Initialize the USART0 port of the ATmega162. 
 @param ubbr contains the baud-rate generator value. 
 @param recieveInterupt selects if receive interrupt is to be enabled.
 */
void USART_Init( unsigned int ubrr, bool recieveInterupt);

/*
Transmit a single character to the USART0 port
@param data. Transmitted char
*/
void USART_Transmit( unsigned char data );

/*
Read a single character from the USART0 port. The function will halt if no data is received. The function USART_DataReceived can be used to check if data is received.
@return unsigned char received
*/
unsigned char USART_Receive( void );

/*
Check if data is received to the USART port. 
@return bool. TRUE if data received, FALSE if no data is received.
*/
bool USART_DataReceived(void);

#endif /* USART_H_ */