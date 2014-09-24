/*
 * format.c
 *
 * Created: 24.09.2014 10:43:38
 *  Author: boerikse
 */ 

#include <stdlib.h>
#include "format.h"

char *uint8_to_str(uint8_t number)
{
	char str[16];
	itoa(number,str,10);
	return str;
}

char *int16_to_str(int16_t number)
{
	char str[16];
	itoa(number,str,10);
	return str;
}