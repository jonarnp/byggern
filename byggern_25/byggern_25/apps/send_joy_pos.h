/*
 * send_joy_pos.h
 *
 * Created: 15.10.2014 09:38:46
 *  Author: jonarnp
 */ 


#ifndef SEND_JOY_POS_H_
#define SEND_JOY_POS_H_

#define JOY_POS_CAN_ID 0x100

void send_joy_pos_init();

void transmit_joy_pos();


#endif /* SEND_JOY_POS_H_ */