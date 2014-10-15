#include "game_control.h"
#include "servo_control.h"
#include <SPI.h>
#include <Servo.h>
#include "CAN.h"
#include "Can_ID.h"
#include "game_control.h"

/* This program demonstrates simple use of the CAN library
 * by sending a single number that increments each time. 
 * It also displays any number that it receives.  You can
 * program this onto two Arduinos and they will send numbers
 * to each other and print what they receive.  */

/* If you don't have two CAN shields, this program can run on
 * just one by setting the mode to CAN_MODE_LOOPBACK instead
 * of CAN_MODE_NORMAL.  It will internally send the numbers
 * to itself instead of sending them over the bus.  */

unsigned long last;
unsigned long now;
int i;
int val;
CanMessage sendMessage;
CanMessage receiveMessage;
CanMessage message;

Servo myservo;
int pos = 0;
Game_control gameBoard;

void setup()
{
	cli();
	CAN.begin(CAN_SPEED_125000);
	CAN.setMode (MCP2515_MODE_CONFIG);
  
  
	CAN.setMode (CAN_MODE_NORMAL);
  
	Serial.begin(115200);
  
	gameBoard.init();
	
	TIMSK1 |= (1<<TOIE1);
	sei();
	pinMode(37,OUTPUT);
}

void loop()
{
	  if (CAN.available()) {
	  
	  message = CAN.getMessage ();
	  //message.print (HEX);
	  if (message.id == JOY_POS_ID)
	  {
		    int16_t x,y;
		    x = (message.data[0]<<8)+message.data[1];
		    y = (message.data[2]<<8)+message.data[3];
			//Serial.print("Recieved joystick data: \n x: ");
			//Serial.print(x);
			//Serial.print("\n y: ");
			//Serial.print(y);
			//Serial.print("\n Parsed to int8_t : ");
			
			int8_t servo_pos;
			
			if (x>100)
			{
				servo_pos = 100;
			}
			else if (x<-100)
			{
				servo_pos = -100;
			}
			else
			{
				servo_pos = (int8_t)x;
			}
			//Serial.print(servo_pos);
			//Serial.print("\n");
			gameBoard.set_servo(servo_pos);
	  }
	  
	  }
	  //digitalWrite(37,LOW);
	  ////digitalWrite(37, HIGH);   // turn the LED on (HIGH is the voltage level)
	  ////delay(1000);               // wait for a second
	  ////digitalWrite(37, LOW);    // turn the LED off by making the voltage LOW
	  ////delay(1000);
	  //
  ///* Send a new value every second */
  //now = millis();
  //if (now > last + 1000) {
    //last = now;
	//
    //sendMessage.clear ();
	//sendMessage.id = 0x200;
    //sendMessage.data[0] = 'D';
    //sendMessage.data[1] = 'a';
    //sendMessage.data[2] = 't';
    //sendMessage.data[3] = 'a';
    //sendMessage.data[4] = '\0';
	//sendMessage.len = 5;
    //sendMessage.send ();
    //i++;
  //}
//
  ///* Receive data if available */
  //if (CAN.available()) {
//
	  //message = CAN.getMessage ();
	  //message.print (HEX);
  //}
  //
	////myservo.writeMicroseconds(1000);              // tell servo to go to position in variable 'pos'
	////delay(2000);
	////myservo.writeMicroseconds(2000);
	////delay(2000);
}
