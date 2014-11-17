/*
 * pong.h
 *
 * Created: 15.10.2014 11:10:10
 *  Author: jonarnp
 */ 

#include <stdio.h>

#ifndef PONG_H_
#define PONG_H_

/* Frame rate for Pong game */
#define FPS 24

/* 
Main Pong function. Will run Pong until game is finished (score reach MAX_SCORE or right slider button pushed).
*/
void play_pong();

#endif /* PONG_H_ */