// 
// 
// 

#include "game_control.h"
#include <Servo.h>

Servo Game_servo;
int16_t IR_value;

void Game_control::init()
{
	// Initialize servo
	Game_servo.attach(SERVO_PIN,MIN_SERVO_PWM,MAX_SERVO_PWM);
	Game_servo.writeMicroseconds(1500);
}

void Game_control::set_servo(int8_t position)
{
	int16_t pwm_value = MIN_SERVO_PWM + (position+100)*6;
	
	if (pwm_value < MIN_SERVO_PWM)
	{
		pwm_value = MIN_SERVO_PWM;
	}
	else if (pwm_value > MAX_SERVO_PWM )
	{
		pwm_value = MAX_SERVO_PWM;
	}
	
	Game_servo.writeMicroseconds(pwm_value);
}

bool check_for_goal()
{
	
}

ISR(TIMER1_OVF_vect)
{
	static uint8_t j = 0;
	if (++j>4)
	{
		//Serial.print("j");
		digitalWrite(37,!digitalRead(37));
		j=0;
	}
}

//Game_control Game_control;

