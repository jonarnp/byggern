/*
 * sram_test.h
 *
 * Created: 03.09.2014 11:49:06
 *  Author: boerikse
 */ 


#ifndef SRAM_TEST_H_
#define SRAM_TEST_H_

/*
Writing and reading 2048 test values in SRAM. 
Prints write and read errors, as well as total count for each phase
*/
void SRAM_test(void);

/*
Writing and reading 2048 test values in SRAM. 
For the 512 first, the input array gets incremented when an error occurs
*/
void SRAM_test_count(uint8_t errors[512]);

#endif /* SRAM_TEST_H_ */