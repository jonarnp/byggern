// 
// 
// 

#include "game_control.h"
#include <Servo.h>
#include <Wire.h>
#include "bit_op.h"

Servo Game_servo;
int32_t IR_filt;
int32_t alfa;
uint16_t score;

unsigned char Bit_Reverse( unsigned char x );

void Game_control::init()
{
	// Initialize servo
	Game_servo.attach(SERVO_PIN,MIN_SERVO_PWM,MAX_SERVO_PWM);
	Game_servo.writeMicroseconds(1500);
	
	//Initialize filter
	alfa = ((int32_t)TF*100)/(TF+TS);
	IR_filt = analogRead(IR_PIN);
	
	cli();
	
	TIMSK3 |= (1<<TOIE3);
	sei();
	
	Wire.begin();
	TWBR = 0x7F;
	
	score = 0;
	set_DAC(0);
	
	//Set DAC output
	
	//Set data directions
	ENCODER_DDR = 0x00;
	set_bit(OE_DDR,OE_PIN);
	set_bit(RST_DDR,RST_PIN);
	set_bit(SEL_DDR,SEL_PIN);
	set_bit(EN_DDR,EN_PIN);
	set_bit(DIR_DDR,DIR_PIN);
	
	set_bit(RST_PORT,RST_PIN);
	set_bit(OE_PORT,OE_PIN);
	
	clear_bit(EN_PORT,EN_PIN);
	
	set_bit(DIR_PORT,DIR_PIN);
	set_DAC(50);
	set_bit(EN_PORT,EN_PIN);
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

uint16_t Game_control::get_score()
{
	return score;
}

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

void Game_control::set_DAC(uint8_t data)
{
	Wire.beginTransmission(DAC_ADDRESS);
	uint8_t transmission[2];
	transmission[0] = DAC_CH1_COMM;
	transmission[1] = data;
	Wire.write(transmission,2);
	Wire.endTransmission();
}

int16_t Game_control::read_encoder()
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
	Serial.print("Encoder reading is: MSB: ");
	Serial.print(temp);
	
	
	//Select low byte
	set_bit(SEL_PORT,SEL_PIN);
	
	delayMicroseconds(20);
	
	temp = ENCODER_PORT;
	temp = Bit_Reverse(temp);
	
	encoder_value += (int16_t)temp;
	Serial.print(" LSB: ");
	Serial.print(temp);
	Serial.print("\n");
	
	Serial.print("Encoder reading is: ");
	Serial.print(encoder_value);
	Serial.print("\n");
	
	
	//Reset encoder
	//clear_bit(RST_PORT,RST_PIN);
	//delayMicroseconds(20);
	//set_bit(RST_PORT,RST_PIN);
	
	//Disable encoder output
	set_bit(OE_PORT,OE_PIN);
	
	return encoder_value;
}

ISR(TIMER3_OVF_vect)
{
	sei();
	static uint8_t j = 0;
	static uint16_t k = 0;
	if (++j>4)
	{
		//Serial.print("Alfa is:");
		//Serial.print(alfa);
		//Serial.print("\n\n");
		//Serial.print(alfa);
		//Serial.print("\n");
		int16_t IR_VAL = analogRead(IR_PIN);
		IR_filt = ((alfa*IR_filt) + ((100-alfa)*IR_VAL))/100;
		//Serial.print(IR_filt);
		//Serial.print(testeee);
		//Serial.print("\n\n");
		j=0;
		//Serial.print("Goal is: ");
		//Serial.print(check_for_goal());
		//Serial.print("\n\n");
	}
	if (++k>490)
	{
		k=0;
		if (!check_for_goal())
		{
			score++;
		}
	}
}

unsigned char Bit_Reverse( unsigned char x )
{
	x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
	x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
	x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
	return x;
}

//Game_control Game_control;
