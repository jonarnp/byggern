/*
 * pong.h
 *
 * Created: 15.10.2014 11:10:10
 *  Author: jonarnp
 */ 

#include <stdio.h>

#ifndef PONG_H_
#define PONG_H_

typedef struct ball {
	int16_t x_pos;
	int16_t y_pos;
	int16_t x_vel;
	int16_t y_vel;
}ball_t;

typedef struct slider {
	uint8_t y_pos;
}slider_t;

void play_pong();

#endif /* PONG_H_ */