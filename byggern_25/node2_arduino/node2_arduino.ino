#include "game_control.h"
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

int i;
CanMessage message;

//Objects
Game_control gameBoard;
Highscore scores;
SpotifyControl music;

void setup()
{
	//Setup CAN
	CAN.begin(CAN_SPEED_125000);
	CAN.setMode(MCP2515_MODE_CONFIG);
	CAN.setMode(CAN_MODE_NORMAL);
  
	//Setup serial lines
	Serial.begin(115200);
	Serial1.begin(9600);
  
	//Initialize gameboard
	gameBoard.init();
	
	//Initialize highscore
	scores.init();
}

void loop()
{
	//Send alive message
	if (++i>10000)
	{
		i = 0;
		Serial1.print("Alive\n");		
	}
	
	//Command available?
	if (CAN.available()) {
		message = CAN.getMessage ();
	  
		switch (message.id)
		{
			case GAME_CONTROLS:
				int16_t desired_servo_pos,desired_motor_speed;
				bool pushing;
				
				//Read out message data
				pushing = message.data[4];
				desired_servo_pos = (message.data[0]<<8)+message.data[1];
				desired_motor_speed = (message.data[2]<<8)+message.data[3];
				
				//Set desired motor speed and servo position
				gameBoard.set_motor_speed((int8_t)desired_motor_speed);
				gameBoard.set_servo((int8_t)desired_servo_pos);
				
				//Activate solenoid?
				if (pushing)
				{
					gameBoard.push_solenoid();
				}
			
				//Send game status to node 1
				gameBoard.send_game_status();
				break;
				
			case GAME_START:
				//Serial1.print("Staring game\n");
				
				//Reset the score counter
				gameBoard.reset_score();
				break;
				
			case HIGHSCORE_REQUEST:
				//Serial1.print("Request for highscores\n");
				
				//Send the highscore list to node 1
				scores.send_highscore();
				break;
				
			case WRITE_HIGHSCORE:
				//Serial1.print("Write Highscore\n");
				
				//Player name
				char name[3];
				for (uint8_t j = 0; j < NAME_LENGTH; j++)
				{
					name[j] = message.data[j];
				}
				
				//Player score
				uint16_t score;
				score = ( (uint16_t)message.data[NAME_LENGTH] << 8 ) + message.data[NAME_LENGTH + 1];
				
				//Add score to highscore list
				scores.write_highscore(name,score);
				break;
				
			case SPOTIFY_COMMAND:
				//Serial1.print("Got SPOTIFY command\n");
				
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
				
			case RESET_HIGHSCORE:
				//Serial1.print("Highscore reset\n");
				
				//Reset the highscore list
				scores.reset_highscore();
				break;
				
			default:
				Serial1.print("Unknown CAN message received. ID: ");
				Serial1.print(message.id);
				Serial1.print("\n");
				break;
		}
	}
}
