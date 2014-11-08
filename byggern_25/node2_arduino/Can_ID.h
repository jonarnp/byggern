/*
 * Can_ID.h
 *
 * Created: 15.10.2014 14:53:55
 *  Author: Flua
 */ 


#ifndef CAN_ID_H_
#define CAN_ID_H_

//CAN receive identifiers

#define GAME_CONTROLS 0x100
/*
CAN message which includes all setpoints for the game board.
data[0] = MSB of desired servo position, sendt as int16
data[1] = LSB of desired servo position, sendt as int16
data[2] = MSB of desired motor speed, sendt as int16
data[3] = LSB of desired motor speed, sendt as int16
data[4] = Solenoid fire, sendt as uint8. Treat it as a bool
*/

#define GAME_START 0x110
/*
GAME_START
empty command
*/	

#define HIGHSCORE_REQUEST 0x120
/*
HIGHSCORE_REQUEST
empty command
*/

#define WRITE_HIGHSCORE 0x130
/*
Frame for adding an entry to the highscore list. The frame setup is the same as the HIGHSCORE frame.
*/
#define SPOTIFY_COMMAND 0x140


//CAN transmit identifiers

#define GAME_STATUS 0x010
/*
GAME_STATUS
data[0] = Game over / finished, sent as bool/uint8
data[1] = MSB of current score, sent as uint16
data[2] = LSB of current score, sent as uint16
*/

#define HIGHSCORE 0x011
/*
Highscore entry,
data[0] = First char in name
data[1] = Second char in name
data[2] = Third char in name
data[3] = MSB of highscore, sent as uint16
data[4] = LSB of highscore, sent as uint16
*/


#endif /* CAN_ID_H_ */