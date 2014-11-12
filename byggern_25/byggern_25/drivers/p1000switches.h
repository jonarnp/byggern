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

#define SW0_Port PINB
#define SW0_Pin PINB3
#define SW0_DDR DDRB

void init();

bool P1000_SW0();

#endif /* P1000SWITCHES_H_ */