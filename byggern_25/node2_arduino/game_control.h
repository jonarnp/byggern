// game_control.h

#ifndef _GAME_CONTROL_h
#define _GAME_CONTROL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define SERVO_PIN 12
#define MIN_SERVO_PWM 900
#define MAX_SERVO_PWM 2100

#define IR_PIN 0
#define TS 10
#define TF 100
#define IR_GOAL 100
#define IR_NOGOAL 200

//DAC
#define DAC_ADDRESS 0b0101000
//Commands
#define DAC_CH1_COMM 0x00

//Motorbox
#define ENCODER_DDR DDRK
#define ENCODER_PORT PINK

#define OE_DDR DDRF
#define OE_PORT PORTF
#define OE_PIN PINF7

#define RST_DDR DDRF
#define RST_PORT PORTF
#define RST_PIN PINF6

#define SEL_DDR DDRF
#define SEL_PORT PORTF
#define SEL_PIN PINF5

#define EN_DDR DDRF
#define EN_PORT PORTF
#define EN_PIN PINF4

#define DIR_DDR DDRF
#define DIR_PORT PORTF
#define DIR_PIN PINF3

class Game_control
{
 protected:


 public:
	void init();
	
	/*
	Set the servo position. Position should be in the interval -100 to 100.
	*/
	void set_servo(int8_t position);
	
	uint16_t get_score();
	
	bool check_for_goal();
	
	void set_DAC(uint8_t data);
	
	int16_t read_encoder();
};


#endif

