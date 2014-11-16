/*
 * adc_test.h
 *
 * Created: 03.09.2014 14:45:49
 *  Author: boerikse
 */ 


#ifndef ADC_TEST_H_
#define ADC_TEST_H_

/*
Writing 1024 test values to external memory
*/
void ADC_gal_test(void);

/*
Prints the ADC value of the four channels
*/
void ADC_read_test(void);

#endif /* ADC_TEST_H_ */