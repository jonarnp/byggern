#include "game_control.h"
#include "servo_control.h"
#include <SPI.h>
#include <Servo.h>
#include "CAN.h"
#include "Can_ID.h"
#include "game_control.h"
#include <Wire.h>
#include "bit_op.h"
#include "highscore.h"
#include <eeprom.h>
#include "spotifyControl.h"

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
int16_t tull;

Servo myservo;
int pos = 0;
Game_control gameBoard;
Highscore scores;
SpotifyControl music;

highscore_element_t testscore[HIGHSCORE_LENGTH];

void setup()
{
	CAN.begin(CAN_SPEED_125000);
	CAN.setMode (MCP2515_MODE_CONFIG);
  
  
	CAN.setMode (CAN_MODE_NORMAL);
  
	Serial.begin(115200);
  
	gameBoard.init();
	scores.init();
	
	pinMode(37,OUTPUT);
	scores.reset_highscore();
	scores.read_highscore(testscore);
	//testscore[0].score = (uint16_t)1625;
	tull = 0;
}

void loop()
{
	delay(10);
	//music.play_pause();
	if (CAN.available()) {
	message = CAN.getMessage ();
	  
	switch (message.id)
	{
		case GAME_CONTROLS:
			int16_t desired_servo_pos,desired_motor_speed;
			bool pushing;
			pushing = message.data[4];
			desired_servo_pos = (message.data[0]<<8)+message.data[1];
			desired_motor_speed = (message.data[2]<<8)+message.data[3];
					    
			int8_t servo_pos;
			gameBoard.set_motor_speed(desired_motor_speed);
			if (desired_servo_pos>100)
			{
				servo_pos = 100;
			}
			else if (desired_servo_pos<-100)
			{
				servo_pos = -100;
			}
			else
			{
				servo_pos = (int8_t)desired_servo_pos;
			}
			if (pushing)
			{
				gameBoard.push_solenoid();
			}
			gameBoard.set_servo(servo_pos);
			break;
		case GAME_START:
			gameBoard.reset_score();
			break;
		case HIGHSCORE_REQUEST:
			Serial.print("Request for highscores");
			Serial.print("\n");
			scores.send_highscore();
			break;
		case WRITE_HIGHSCORE:
			break;
		case SPOTIFY_COMMAND:
			//Serial.print("Got SPOTIFY command\n");
			switch (message.data[0])
			{
				case 1:
					music.play_pause();
					break;
				case 2:
					music.next_song();
					break;
				case 3:
					music.prev_song();
					break;
				default:
					Serial.print("Wrong command\n");
					break;
			}
			break;
		default:
		Serial.print("Unknown CAN message received\n");
		break;
	}
	}
	  //
	  //if (message.id == GAME_CONTROLS)
	  //{
		    //int16_t desired_servo_pos,desired_motor_speed;
		    //desired_servo_pos = (message.data[0]<<8)+message.data[1];
		    //desired_motor_speed = (message.data[2]<<8)+message.data[3];
			//bool push = message.data[4];
			////Serial.print("Recieved joystick data: \n desired_motor_speed: ");
			////Serial.print(desired_motor_speed);
			////Serial.print("\n");
			////Serial.print("\n desired servo pos: ");
			////Serial.print(desired_servo_pos);
			////Serial.print("\n");
			//
			//int8_t servo_pos;
			//gameBoard.set_motor_speed(desired_motor_speed);
			//if (desired_servo_pos>100)
			//{
				//servo_pos = 100;
			//}
			//else if (desired_servo_pos<-100)
			//{
				//servo_pos = -100;
			//}
			//else
			//{
				//servo_pos = (int8_t)desired_servo_pos;
			//}
			//if (push)
			//{
				//gameBoard.push_solenoid();
				////Serial.print("Push!\n");
			//}
			////Serial.print(servo_pos);
			////Serial.print("\n");
			//gameBoard.set_servo(servo_pos);
	  //}
	  //else
	  //{
		  //Serial.print("Another CAN recieved\n");
	  //}
	  //if (message.id == HIGHSCORE_REQUEST)
	  //{
		  //Serial.print("Request for highscores");
		  //Serial.print("\n");
		  //scores.send_highscore();
	  //}
	  //
	  //}
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
