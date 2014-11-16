/*
 * format.h
 *
 * Created: 24.09.2014 10:43:26
 *  Author: boerikse
 */ 


#ifndef FORMAT_H_
#define FORMAT_H_

#include <avr/io.h>

/*
Converts uint8 decimal to character string
@param uint8_t
@return char[16]
*/
char *uint8_to_str(uint8_t number);

/*
Converts uint16 decimal to character string
@param uint16_t
@return char[16]
*/
char *uint16_to_str(uint16_t number);

/*
Converts int16 decimal to character string
@param int16_t
@return char[16]
*/
char *int16_to_str(int16_t number);

#endif /* FORMAT_H_ */