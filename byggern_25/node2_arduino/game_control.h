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
#define TF 50

class Game_control
{
 protected:


 public:
	void init();
	
	/*
	Set the servo position. Position should be in the interval -100 to 100.
	*/
	void set_servo(int8_t position);
	
	bool check_for_goal();
};


#endif

