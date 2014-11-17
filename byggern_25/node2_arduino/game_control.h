// game_control.h

#ifndef _GAME_CONTROL_h
#define _GAME_CONTROL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

//Servo definitions
#define SERVO_PIN 12
#define MIN_SERVO_PWM 900
#define MAX_SERVO_PWM 2100

//IR definitions
#define IR_PIN 0		//IR analog input
#define TS 10			//IR filter time step
#define TF 100			//IR filter time constant
#define IR_GOAL 100		//IR Goal threshold
#define IR_NOGOAL 160	//IR NoGoal threshold

//DAC
#define DAC_ADDRESS 0b0101000 //DAC I2C address
#define DAC_MIN 52		// Motor starts to run at about 1.5 V

//I2C Commands
#define DAC_CH1_COMM 0x00 //DAC CH1 write command

//Motorbox pins
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

//Motor speed controller
#define P_GAIN 12 //Proportional gain in 0.1 basis
#define FF_DIVIDER 8 //Reference feed forward divider

//Solenoid
#define SOL_DDR DDRF
#define SOL_PORT PORTF
#define SOL_PIN PINF2
#define SOL_POS_WIDTH 2 //Defines the solenoid pulse width in 10 ms basis

class Game_control
{
 protected:


 public:
	/*
	Initialize the game control object
	*/
	void init();
	
	/*
	Set the servo position. 
	@param position. Int8_t, should be in the interval [-100, 100].
	*/
	void set_servo(int8_t position);
	
	/*
	Set desired motor speed. 
	@param speed. Int8_t, should be in the interval [-100,100].
	*/
	void set_motor_speed(int8_t speed);
	
	/*
	Activate solenoid.
	*/
	void push_solenoid();
	
	/*
	Get the current score.
	@return uint16_t.
	*/
	uint16_t get_score();
	
	/*
	Reset the score.
	*/
	void reset_score();
	
	/*
	Transmit the current game status to node 1.
	*/
	void send_game_status();
};


#endif

