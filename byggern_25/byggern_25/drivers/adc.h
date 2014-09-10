/*
 * adc.h
 *
 * Created: 03.09.2014 15:17:14
 *  Author: boerikse
 */ 


#ifndef ADC_H_
#define ADC_H_

/* Interrupt pin from ADC */
#define INTR_Port PIND
#define INTR_Pin PIND3
#define INTR_DDR DDRD

/* Start address for the ADC */
#define ADC_ADDR 0x1400

/*
 Initialize the ADC for USB multifunction card input
*/
void ADC_Init();

/*
 Read ADC channel
*/
uint8_t ADC_read(uint8_t chan);

#endif /* ADC_H_ */