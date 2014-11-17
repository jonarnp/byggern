/*
 * game_board.h
 *
 * Created: 15.10.2014 09:38:46
 *  Author: jonarnp
 */ 


#ifndef GAME_BOARD_H_
#define GAME_BOARD_H_

/*
Initializing CAN. Necessary for sending of joystick and P1000 buttons
*/
void send_joy_pos_init();

/*
Main game board function. Will run game board until it is finished (game is lost).
*/
void play_game_board();

#endif /* GAME_BOARD_H_ */