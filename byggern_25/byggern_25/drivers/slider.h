/*
 * slider.h
 *
 * Created: 17.09.2014 08:25:09
 *  Author: boerikse
 */ 


#ifndef SLIDER_H_
#define SLIDER_H_

#include <avr/io.h>

/* ADC channels */
#define SLIDER_LEFT_CH 3
#define SLIDER_RIGHT_CH 4

/* Slider calibration button pin */
#define SLIDER_calib_button_Port PINB
#define SLIDER_calib_button_Pin PINB0
#define SLIDER_calib_button_DDR DDRB

typedef struct SLIDER_pos {
	int16_t left;
	int16_t right;
} SLIDER_pos_t;

void SLIDER_init();

void SLIDER_calibrate();

SLIDER_pos_t SLIDER_getPosition();

#endif /* SLIDER_H_ */