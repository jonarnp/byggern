/*
 * can_id.h
 *
 * Created: 17.11.2014 10:46:06
 *  Author: boerikse
 */ 


#ifndef CAN_ID_H_
#define CAN_ID_H_

/*
CAN send identifiers
*/
#define GAME_CONTROLS 0x100
#define GAME_START 0x110
#define HIGHSCORE_REQUEST 0x120
#define WRITE_HIGHSCORE 0x130
#define SPOTIFY_COMMAND 0x140
#define RESET_HIGHSCORE 0x150

/*
CAN receive identifiers
*/
#define GAME_STATUS 0x010 /* Current score and game over */
#define HIGHSCORE 0x011


/*
Usage of identifiers
*/

/*
GAME_CONTROLS
data[0] = MSB of desired servo position, sent as int16
data[1] = LSB of desired servo position, sent as int16
data[2] = MSB of desired motor speed, sent as int16
data[3] = LSB of desired motor speed, sent as int16
data[4] = Solenoid fire, sent as uint8. Treat it as a bool

GAME_START
empty command

HIGHSCORE_REQUEST
empty command

WRITE_HIGHSCORE
data[0] = First char in name
data[1] = Second char in name
data[2] = Third char in name
data[3] = MSB of highscore, sent as uint16
data[4] = LSB of highscore, sent as uint16

SPOTIFY_COMMAND
data[0] = Spotify command:
			1 - Toggle music
			2 - Next song
			3 - Prev song
			
HIGHSCORE_REQUEST
empty command. resets highscore list


GAME_STATUS
data[0] = Game over / finished, sent as bool/uint8
data[1] = MSB of current score, sent as uint16
data[2] = LSB of current score, sent as uint16

HIGHSCORE
data[0] = First char in name
data[1] = Second char in name
data[2] = Third char in name
data[3] = MSB of highscore, sent as uint16
data[4] = LSB of highscore, sent as uint16
*/

#endif /* CAN_ID_H_ */