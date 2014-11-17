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
#define JOY_button_Port PINB
#define JOY_button_Pin PINB2
#define JOY_button_DDR DDRB

/* Joystick calibration button pin */
#define JOY_calib_button_Port PINB
#define JOY_calib_button_Pin PINB0
#define JOY_calib_button_DDR DDRB

/* Joystick direction threshold */
#define JOY_DIR_TH 25

/* Max and min joystick values */
#define JOY_MAX 100
#define JOY_MIN -100
#define JOY_MIDDLE 0

typedef struct JOY_pos {
	int16_t x;
	int16_t y;
} JOY_pos_t;

typedef enum JOY_dir {
	MIDDLE,
	LEFT,
	RIGHT,
	UP,
	DOWN
} JOY_dir_t;

/*
Initialize the ADC, OLED and the joystick on the USB multifunction card
*/
void JOY_init();

/*
Joystick calibration function. Max, min and middle values of the joystick are stored
Prints calibration sequence both to USART and to OLED screen.
*/
void JOY_calibrate();

/*
Check joystick button
@return bool. True if joystick button is pushed, False otherwise
*/
bool JOY_button();

/*
Reads the joystick position and returns its values
@return JOY_pos_t: int16_t x, int16_t y. Range [-100,100]
*/
JOY_pos_t JOY_getPosition();

/*
Reads the joystick position and returns its direction if it is larger than threshold JOY_DIR_TH
@return JOY_dir_t enum.
*/
JOY_dir_t JOY_getDirection();

#endif /* JOY_H_ */