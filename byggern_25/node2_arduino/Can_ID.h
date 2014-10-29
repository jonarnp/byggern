/*
 * Can_ID.h
 *
 * Created: 15.10.2014 14:53:55
 *  Author: Flua
 */ 


#ifndef CAN_ID_H_
#define CAN_ID_H_

#define GAME_CONTROLS 0x100
/*
CAN message which includes all setpoints for the game board.
data[0] = MSB of desired servo position, sendt as int16
data[1] = LSB of desired servo position, sendt as int16
data[2] = MSB of desired motor speed, sendt as int16
data[3] = LSB of desired motor speed, sendt as int16
data[4] = Solenoid fire, sendt as uint8. Treat it as a bool
*/



#endif /* CAN_ID_H_ */