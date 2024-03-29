/*
 * adc_test.c
 *
 * Created: 03.09.2014 14:45:28
 *  Author: boerikse
 */ 

#include <avr/io.h>
#include <stdio.h>

#include "adc_test.h"

void ADC_gal_test(void)
{
	// Start address for the ADC
	volatile char *ext_adc = (char *) 0x1400;

	uint16_t i;
	unsigned char testvalue;

	//printf("Starting ADC gal test...\r\n");

	for (i = 0; i < 0x400; i++) {
		testvalue = ~(i % 256);
		ext_adc[i] = testvalue;
	}
}

void ADC_read_test(void)
{
	printf("Starting ADC read test...\r\n");
	uint8_t i;
	
	for (i = 1; i <= 4; i++) {
		printf("ADC channel %d - Raw value %d\n",i,ADC_read(i));
	}
}