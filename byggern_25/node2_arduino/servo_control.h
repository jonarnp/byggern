// servo_control.h

#ifndef _SERVO_CONTROL_h
#define _SERVO_CONTROL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class Servo_controlClass
{
 protected:


 public:
	void init();
};

extern Servo_controlClass Servo_control;

#endif

