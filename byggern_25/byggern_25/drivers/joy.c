/*
 * joy.c
 *
 * Created: 10.09.2014 09:01:45
 *  Author: jonarnp
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "joy.h"
#include "adc.h"
#include "..\bit_op.h"
#include "oled.h"
#include "../format.h"

typedef struct Joy_calib 
{
	uint8_t xMin;
	uint8_t xMiddle;
	uint8_t xMax;
	uint8_t yMin;
	uint8_t yMiddle;
	uint8_t yMax;
	
	int16_t x_k1;
	int16_t x_k2;
	int16_t y_k1;
	int16_t y_k2;
} Joy_calib_t;

Joy_calib_t Joy_calib_values;

const unsigned char PROGMEM joy_calib_str[4][7] = {
	{"Xmin: "},
	{"Xmax: "},
	{"Ymin: "},
	{"Ymax: "}	
};

void JOY_init()
{
	ADC_Init();
	oled_init();
	/* Set joystick button as input */
	clear_bit(JOY_button_DDR,JOY_button_Pin);
	
	/* Set joystick calibration button as input */
	clear_bit(JOY_calib_button_DDR,JOY_calib_button_Pin);
}

void JOY_calibrate()
{
	printf(PSTR("Calibrating joystick. Move joystick to all extrema, release it and then push calib button\n"));
	oled_clear();
	Joy_calib_values.xMax = ADC_read(JOY_X_CH);
	Joy_calib_values.xMin = Joy_calib_values.xMax;
	Joy_calib_values.yMax = ADC_read(JOY_Y_CH);
	Joy_calib_values.yMin = Joy_calib_values.yMax;
	
	// Print text to OLED
	oled_goto_line(0);
	oled_goto_column(0);
	oled_print_p(PSTR("Move joystick in all extrema to calibrate."));
	oled_goto_line(2);
	oled_goto_column(0);
	oled_print_p(joy_calib_str[0]);
	oled_goto_line(2);
	oled_goto_column(63);
	oled_print_p(joy_calib_str[1]);
	oled_goto_line(3);
	oled_goto_column(0);
	oled_print_p(joy_calib_str[2]);
	oled_goto_line(3);
	oled_goto_column(63);
	oled_print_p(joy_calib_str[3]);
	oled_goto_line(4);
	oled_goto_column(0);
	oled_print_p(PSTR("After extrema is reached, leave joystick at rest and press left slider button."));
				
	while(!get_bit(JOY_calib_button_Port, JOY_calib_button_Pin)){
		uint8_t xTemp = ADC_read(JOY_X_CH);
		uint8_t yTemp = ADC_read(JOY_Y_CH);
		
		if(xTemp > Joy_calib_values.xMax){
			Joy_calib_values.xMax = xTemp;
			printf("xMax set to: %d\n", Joy_calib_values.xMax);
			
			//Update OLED
			oled_clear_line(2);
			oled_goto_line(2);
			oled_goto_column(0);
			oled_print_p(joy_calib_str[0]);
			oled_print(uint8_to_str(Joy_calib_values.xMin));
			
			oled_goto_column(63);
			oled_print_p(joy_calib_str[1]);
			oled_print(uint8_to_str(Joy_calib_values.xMax));
		} 
		if (xTemp < Joy_calib_values.xMin){
			Joy_calib_values.xMin = xTemp;
			printf("xMin set to: %d\n", Joy_calib_values.xMin);
			
			//Update OLED
			oled_clear_line(2);
			oled_goto_line(2);
			oled_goto_column(0);
			oled_print_p(joy_calib_str[0]);
			oled_print(uint8_to_str(Joy_calib_values.xMin));
			
			oled_goto_column(63);
			oled_print_p(joy_calib_str[1]);
			oled_print(uint8_to_str(Joy_calib_values.xMax));
		}
		
		if(yTemp > Joy_calib_values.yMax){
			Joy_calib_values.yMax = yTemp;
			printf("yMax set to: %d\n", Joy_calib_values.yMax);
			
			//Update OLED
			oled_clear_line(3);
			oled_goto_line(3);
			oled_goto_column(0);
			oled_print_p(joy_calib_str[2]);
			oled_print(uint8_to_str(Joy_calib_values.yMin));
			
			oled_goto_column(63);
			oled_print_p(joy_calib_str[3]);
			oled_print(uint8_to_str(Joy_calib_values.yMax));
		}
		if (yTemp < Joy_calib_values.yMin){
			Joy_calib_values.yMin = yTemp;
			printf("yMin set to: %d\n", Joy_calib_values.yMin);
			
			//Update OLED
			oled_clear_line(3);
			oled_goto_line(3);
			oled_goto_column(0);
			oled_print_p(joy_calib_str[2]);
			oled_print(uint8_to_str(Joy_calib_values.yMin));
			
			oled_goto_column(63);
			oled_print_p(joy_calib_str[3]);
			oled_print(uint8_to_str(Joy_calib_values.yMax));
		}
	}
	printf(PSTR("Button pushed\n"));
	
	Joy_calib_values.xMiddle = ADC_read(JOY_X_CH);
	Joy_calib_values.yMiddle = ADC_read(JOY_Y_CH);
	printf("Middle set to: %d, %d\n", Joy_calib_values.xMiddle, Joy_calib_values.yMiddle);
	
	Joy_calib_values.x_k1 = (Joy_calib_values.xMiddle - Joy_calib_values.xMin);
	Joy_calib_values.x_k2 = (Joy_calib_values.xMax - Joy_calib_values.xMiddle);
	Joy_calib_values.y_k1 = (Joy_calib_values.yMiddle - Joy_calib_values.yMin);
	Joy_calib_values.y_k2 = (Joy_calib_values.yMax - Joy_calib_values.yMiddle);
	
	printf(PSTR("Joystick calibration completed\n"));
	//printf("Calibration completed, xMin %d xMax %d\n",Joy_calib_values.xMin,Joy_calib_values.xMax);
	//printf("Calibration completed, xk1 %d xk2 %d yk1 %d yk2 %d\n",Joy_calib_values.x_k1,Joy_calib_values.x_k2,Joy_calib_values.y_k1,Joy_calib_values.y_k2);
}

bool JOY_button()
{
	return !get_bit(JOY_button_Port,JOY_button_Pin);
}

JOY_pos_t JOY_getPosition()
{
	JOY_pos_t pos;
	int16_t V_x = ADC_read(JOY_X_CH)-Joy_calib_values.xMiddle;
	int16_t V_y = ADC_read(JOY_Y_CH)-Joy_calib_values.yMiddle;
	
	if (V_x>JOY_MIDDLE)
	{
		pos.x = (JOY_MAX*V_x)/Joy_calib_values.x_k2;
	}
	else
	{
		pos.x = (JOY_MAX*V_x)/Joy_calib_values.x_k1;
	}
	
	if (V_y>JOY_MIDDLE)
	{
		pos.y = (JOY_MAX*V_y)/Joy_calib_values.y_k2;
	}
	else
	{
		pos.y = (JOY_MAX*V_y)/Joy_calib_values.y_k1;
	}
	
	/* Within calibrated values */
	if (pos.x < JOY_MIN)
	{
		pos.x = JOY_MIN;
	}
	else if (pos.x > JOY_MAX)
	{
		pos.x = JOY_MAX;
	}
	
	if (pos.y < JOY_MIN)
	{
		pos.y = JOY_MIN;
	}
	else if (pos.y > JOY_MAX)
	{
		pos.y = JOY_MAX;
	}
	
	return pos;
}

JOY_dir_t JOY_getDirection()
{	
	JOY_pos_t pos = JOY_getPosition();
	
	if(pos.x > JOY_DIR_TH)
	{
		return RIGHT;
	}
	else if (pos.x < -JOY_DIR_TH)
	{
		return LEFT;
	}
	
	if(pos.y > JOY_DIR_TH)
	{
		return UP;
	}
	else if (pos.y < -JOY_DIR_TH)
	{
		return DOWN;
	}
	
	return MIDDLE;
}