/*
 * xmem.c
 *
 * Created: 03.09.2014 11:20:46
 *  Author: boerikse
 */ 

#include <avr/io.h>
#include "..\bit_op.h"
#include "xmem.h"

void XMEM_En(void)
{
	/* Enables the External Memory Interface */
	set_bit(MCUCR,SRE);
	
	/* Mask out 4 MSB for JTAG */
	set_bit(SFIOR,XMM2);
}