#include "game_control.h"
#include <Servo.h>
#include <Wire.h>
#include "bit_op.h"
#include "CAN.h"
#include "Can_ID.h"

//Servo
Servo Game_servo;

//Variables for the IR filter
int32_t IR_filt;	//Output of the IR filter
int32_t alfa;		//Filter coefficient

//Game variables
uint16_t score;
int16_t speed_ref = 0;

uint8_t sol_iteration = 0;	//Number of iterations the solenoid has been active, in 10ms basis
bool initialized = false;

//Flag for routines run in ISR
bool tenMSroutine = false;
bool oneSroutine = false;

void set_DAC(uint8_t data);
unsigned char Bit_Reverse( unsigned char x );
int16_t read_encoder();
void reset_encoder();
bool check_for_goal();

void Game_control::init()
{
	// Initialize servo
	Game_servo.attach(SERVO_PIN,MIN_SERVO_PWM,MAX_SERVO_PWM);
	Game_servo.writeMicroseconds(1500);
	
	//Initialize IR filter
	alfa = ((int32_t)TF*100)/(TF+TS);
	IR_filt = analogRead(IR_PIN);
	cli(); //Disable interrupts
	
	//Enable TMR3 overflow interrupt
	TIMSK3 |= (1<<TOIE3);
	sei(); //Enable interrupts
	
	Wire.begin();
	TWBR = 0x7F; //Set I2C speed
	
	score = 0;
	
	//Set DAC output
	set_DAC(0);
	
	//Set data directions
	set_bit(SOL_DDR,SOL_PIN);
	set_bit(SOL_PORT,SOL_PIN); //deactivate solenoid
	
	ENCODER_DDR = 0x00;
	
	set_bit(OE_DDR,OE_PIN);
	set_bit(RST_DDR,RST_PIN);
	set_bit(SEL_DDR,SEL_PIN);
	set_bit(EN_DDR,EN_PIN);
	set_bit(DIR_DDR,DIR_PIN);
	
	//Disable RST and OE 
	set_bit(RST_PORT,RST_PIN); //Disable RST
	set_bit(OE_PORT,OE_PIN); //Disable OE	
	set_bit(EN_PORT,EN_PIN); //Enable motor
	
	reset_encoder();
	initialized = true;
}

/*
Set servo position. position is in the interval [-100,100].
*/
void Game_control::set_servo(int8_t position)
{
	if (position>100)
	{
		position = 100;
	}
	else if (position<-100)
	{
		position = -100;
	}
	
	int16_t pwm_value = MIN_SERVO_PWM + (position+100)*6; //Calculate PWM value. Results in span of 1200us.
	
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

/*
Set desired motor speed. speed is in the inerval [-100,100]
*/
void Game_control::set_motor_speed(int8_t speed)
{
	if (speed > 100)
	{
		speed = 100;
	}
	else if (speed < -100)
	{
		speed = -100;
	}
	else
	
	//Calculate internal speed reference.
	speed_ref = -speed;//(3*speed)/2;
}

/*
Activate solenoid.
*/
void Game_control::push_solenoid()
{
	clear_bit(SOL_PORT,SOL_PIN);
	sol_iteration = 0;
}

/*
Get the current score.
*/
uint16_t Game_control::get_score()
{
	return score;
}

/*
Reset the score.
*/
void Game_control::reset_score()
{
	score = 0;
}

/*
Transmit the current game status to node 1.
*/
void Game_control::send_game_status()
{
	//Serial.print("Sending game status\n");
	CanMessage message;
	message.id = GAME_STATUS;
	message.data[0] = (uint8_t)check_for_goal();
	message.data[1] = (uint8_t)(score >> 8);
	message.data[2] = (uint8_t)(score & 0xFF);
	message.len = 3;
	while (!CAN.ready());
	message.send();
	//message.print(HEX);
}
	
/*
Read the IR value, and iterate the filter.
*/
void runIRfilter()
{
	int16_t IR_VAL = analogRead(IR_PIN);
	IR_filt = ((alfa*IR_filt) + ((100-alfa)*IR_VAL))/100;
	//Serial.print(IR_filt);
	//Serial.print("\n");
}
/*
Check of the solenoid shoud be reset.
*/
void solenoidReset()
{
	if (!get_bit(SOL_PORT,SOL_PIN))
	{
		//Pos width over?
		if(++sol_iteration > SOL_POS_WIDTH) set_bit(SOL_PORT,SOL_PIN);
		//Serial.print(sol_iteration);
		//Serial.print("\n");
	}
}
/*
Speed controller
*/
void runController()
{
	if (initialized)
	{
		static int16_t prev_encoder_val = 0;
		
		//Read encoder to get speed
		int16_t encoder_val = read_encoder();
		int16_t control_signal;
		
		uint8_t DAC8;
		if(speed_ref == 0) //Set motor control to zero
		{
			DAC8 = 0;
			control_signal = 0;
		}
		else //Run controller
		{
			//Compute control error
			//Serial.print("Ref: ");
			//Serial.println(speed_ref);
			int16_t e = (int16_t)speed_ref - (encoder_val-prev_encoder_val);
			//Serial.print("e: ");
			//Serial.println(e);
			//Compute control signal
			control_signal = (P_GAIN*e)/10+speed_ref/FF_DIVIDER;
			
			//Serial.print("FF: ");
			//Serial.println(speed_ref/FF_DIVIDER);
			//Serial.print("FB: ");
			//Serial.println((P_GAIN*e)/10);
			//Compute DAC value
			int16_t DAC16 = DAC_MIN + abs(control_signal);
			if (DAC16>0xFF)
			{
				DAC8 = 0xFF;
			}
			else
			{
				DAC8 = (uint8_t)DAC16;
			}
		}
		
		//Set DAC and motor direction
		set_DAC(DAC8);
		if (control_signal>0)
		{
			clear_bit(DIR_PORT,DIR_PIN);
		}
		else
		{
			set_bit(DIR_PORT,DIR_PIN);
		}
		
		//Save encoder value for next iteration
		prev_encoder_val = encoder_val;
	}
}
ISR(TIMER3_OVF_vect)
{
	sei(); //Enable nested interrupts
	
	//Iteration variables
	static uint8_t j = 0;
	static uint16_t k = 0;
	
	if ((++j > 4) & !tenMSroutine) //10ms routine
	{
		tenMSroutine = true; //Make sure routine is not called until it is finished
		j=0;
		
		runIRfilter(); //Run IR filter routine
		solenoidReset(); //Check if solenoid should be reset
		runController(); //Motor speed controller
		
		tenMSroutine = false; //Make routine available
	}
	if ((++k > 490) & !oneSroutine) //1s routine
	{
		oneSroutine = true; //Make sure routine is not called until it is finished
		k=0;
		
		if (!check_for_goal())
		{
			score++;
		}
		
		oneSroutine = false; //Make routine available
	}
}

unsigned char Bit_Reverse( unsigned char x )
{
	x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
	x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
	x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
	return x;
}

/*
Check for a goal.
*/
bool check_for_goal()
{
	static bool flag = false;
	if (IR_filt < IR_GOAL)
	{
		flag = true;
	}
	if (IR_filt > IR_NOGOAL)
	{
		flag = false;
	}
	return flag;
}

/*
Set the DAC value. data is in the range [0,255].
*/
void set_DAC(uint8_t data)
{
	Wire.beginTransmission(DAC_ADDRESS);
	uint8_t transmission[2];
	transmission[0] = DAC_CH1_COMM;
	transmission[1] = data;
	Wire.write(transmission,2);
	Wire.endTransmission();
}

/*
Read the motor encoder. Does not reset the encoder by reading.
*/
int16_t read_encoder()
{
	uint8_t temp;
	int16_t encoder_value = 0;
	
	// Activate encoder output
	clear_bit(OE_PORT,OE_PIN);
	
	//Select high byte
	clear_bit(SEL_PORT,SEL_PIN);
	
	delayMicroseconds(20);
	temp = ENCODER_PORT;
	temp = Bit_Reverse(temp);
	
	encoder_value = ((int16_t)temp)<<8;
	//Serial.print("Encoder reading is: MSB: ");
	//Serial.print(temp);
	
	
	//Select low byte
	set_bit(SEL_PORT,SEL_PIN);
	
	delayMicroseconds(20);
	
	temp = ENCODER_PORT;
	temp = Bit_Reverse(temp);
	
	encoder_value += (int16_t)temp;
	//Serial.print(" LSB: ");
	//Serial.print(temp);
	//Serial.print("\n");
	//
	//Serial.print("Encoder reading is: ");
	//Serial.print(encoder_value);
	//Serial.print("\n");
	//
	//Disable encoder output
	set_bit(OE_PORT,OE_PIN);
	
	return encoder_value;
}

void reset_encoder()
{
	//Reset encoder
	clear_bit(RST_PORT,RST_PIN);
	delayMicroseconds(20);
	set_bit(RST_PORT,RST_PIN);
}
//Game_control Game_control;
