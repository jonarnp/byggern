/*
 * adc.c
 *
 * Created: 03.09.2014 15:17:02
 *  Author: boerikse
 */ 

#define F_CPU 4915200UL  // 4.9152 MHz
#include <avr/io.h>
#include <avr/delay.h>

#include "adc.h"
#include "..\bit_op.h"
#include "xmem.h"

/* Start address for the ADC */
volatile char *adc_addr = (char *) ADC_ADDR;

void ADC_Init()
{
	XMEM_En();
	/*Enable INTR pin as input */
	clear_bit(INTR_DDR,INTR_Pin);
}

uint8_t ADC_read(uint8_t chan)
{	
	/* Write MUX configuration to ADC */
	*adc_addr = 4+chan-1;
	
	/* Wait for the INTR line to go low */
	//_delay_us(1);
	//while(get_bit(INTR_Port,INTR_Pin))
	//{
	//
	//}
	
	_delay_us(50);
	
	/* Read and return the ADC value */
	return *adc_addr;
} 

/*

Bruke TMR til å genererer interrupt hvert 1 ms? for å polle en og en AD kanal. Legge inn minne i ADC driveren, og returnere nyeste verdien.

*/