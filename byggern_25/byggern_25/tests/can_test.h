/*
 * can_test.h
 *
 * Created: 08.10.2014 15:53:56
 *  Author: boerikse
 */ 


#ifndef CAN_TEST_H_
#define CAN_TEST_H_

/*
Transmits a 6 byte CAN message with identifier 0x200. 
If 'a' sent on the serial line (usart), send another CAN message.
If 'r' sent on the serial line (usart), print incoming CAN message.
*/
void can_test();

#endif /* CAN_TEST_H_ */