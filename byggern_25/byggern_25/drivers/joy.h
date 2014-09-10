/*
 * joy.h
 *
 * Created: 10.09.2014 09:01:55
 *  Author: jonarnp
 */ 



#ifndef JOY_H_
#define JOY_H_

#include <stdbool.h>
#include <avr/io.h>

/* ADC channels */
#define JOY_X_CH 1
#define JOY_Y_CH 2

/* Joystick button pin */
#define JOY_button_Port PIND
#define JOY_button_Pin PIND3
#define JOY_button_DDR DDRD

/* Joystick calibration button pin */
#define JOY_calib_button_Port PINB
#define JOY_calib_button_Pin PINB0
#define JOY_calib_button_DDR DDRB

typedef struct JOY_pos {
	int16_t x;
	int16_t y;
} JOY_pos_t;

typedef enum JOY_dir {
	LEFT,
	RIGHT,
	UP,
	DOWN
} JOY_dir_t;


void JOY_init();

void JOY_calibrate();

bool JOY_button();

JOY_pos_t JOY_getPosition();

JOY_dir_t JOY_getDirection();


#endif /* JOY_H_ */