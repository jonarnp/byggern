/*
 * p1000switches.h
 *
 * Created: 12.11.2014 16:18:40
 *  Author: jonarnp
 */ 


#ifndef P1000SWITCHES_H_
#define P1000SWITCHES_H_

#include <avr/io.h>
#include <stdbool.h>

/* P1000 switch pin */
#define SW0_Port PINB
#define SW0_Pin PINB3
#define SW0_DDR DDRB

/*
Initialize the bottom switch on the USB multifunction card
*/
void P1000_init();

/*
Check switch button
@return bool. True if switch button is pushed, False otherwise
*/
bool P1000_SW0();

#endif /* P1000SWITCHES_H_ */