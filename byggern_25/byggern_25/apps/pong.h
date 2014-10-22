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
	uint16_t x_pos;
	uint16_t y_pos;
	uint16_t x_vel;
	uint16_t y_vel;
}ball_t;

/* Position [0,55] */
typedef struct game_slider {
	uint16_t y_pos;
	uint8_t col;
}game_slider_t;

void play_pong();

void pong_init();

#endif /* PONG_H_ */