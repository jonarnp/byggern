/*
 * CFile1.c
 *
 * Created: 12.11.2014 16:18:01
 *  Author: jonarnp
 */ 

#include "p1000switches.h"
#include <avr/io.h>
#include <stdbool.h>
#include "../bit_op.h"

void init()
{
	/* Set switches as input */
	clear_bit(SW0_DDR,SW0_Pin);
}

bool P1000_SW0()
{
	return !get_bit(SW0_Port,SW0_Pin);
}