/*
 * xmem.c
 *
 * Created: 03.09.2014 11:20:46
 *  Author: boerikse
 */ 

#include <avr/io.h>
#include "xmem.h"

void XMEM_En(void)
{
	/* Enables the External Memory Interface */
	MCUCR |= (1<<SRE);
}