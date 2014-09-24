/*
 * format.h
 *
 * Created: 24.09.2014 10:43:26
 *  Author: boerikse
 */ 


#ifndef FORMAT_H_
#define FORMAT_H_

#include <avr/io.h>

char *uint8_to_str(uint8_t number);

char *int16_to_str(int16_t number);

#endif /* FORMAT_H_ */