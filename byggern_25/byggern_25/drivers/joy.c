/*
 * joy.c
 *
 * Created: 10.09.2014 09:01:45
 *  Author: jonarnp
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include "joy.h"
#include "adc.h"
#include "..\bit_op.h"

typedef struct Calib 
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
} Calib_t;

Calib_t Calib_values;

void JOY_init()
{
	ADC_Init();
	
	/* Set joystick button as input */
	clear_bit(JOY_button_DDR,JOY_button_Pin);
	
	/* Set joystick calibration button as input */
	clear_bit(JOY_calib_button_DDR,JOY_calib_button_Pin);
}

void JOY_calibrate()
{
	printf("Calibrating joystick. Move joystick to all extrema, release it and then push calib button\n");
	
	Calib_values.xMax = ADC_read(JOY_X_CH);
	Calib_values.xMin = Calib_values.xMax;
	Calib_values.yMax = ADC_read(JOY_Y_CH);
	Calib_values.yMin = Calib_values.yMax;
			
	while(!get_bit(JOY_calib_button_Port, JOY_calib_button_Pin)){
		uint8_t xTemp = ADC_read(JOY_X_CH);
		uint8_t yTemp = ADC_read(JOY_Y_CH);
		
		if(xTemp > Calib_values.xMax){
			Calib_values.xMax = xTemp;
			printf("xMax set to: %d\n", Calib_values.xMax);
		} 
		if (xTemp < Calib_values.xMin){
			Calib_values.xMin = xTemp;
			printf("xMin set to: %d\n", Calib_values.xMin);
		}
		
		if(yTemp > Calib_values.yMax){
			Calib_values.yMax = yTemp;
			printf("yMax set to: %d\n", Calib_values.yMax);
		}
		if (yTemp < Calib_values.yMin){
			Calib_values.yMin = yTemp;
			printf("yMin set to: %d\n", Calib_values.yMin);
		}
	}
	printf("Button pushed\n");
	
	Calib_values.xMiddle = ADC_read(JOY_X_CH);
	Calib_values.yMiddle = ADC_read(JOY_Y_CH);
	printf("Middle set to: %d, %d\n", Calib_values.xMiddle, Calib_values.yMiddle);
	
	Calib_values.x_k1 = (Calib_values.xMiddle - Calib_values.xMin);
	Calib_values.x_k2 = (Calib_values.xMax - Calib_values.xMiddle);
	Calib_values.y_k1 = (Calib_values.yMiddle - Calib_values.yMin);
	Calib_values.y_k2 = (Calib_values.yMax - Calib_values.yMiddle);
	
			
	printf("Calibration completed, xMin %d xMax %d\n",Calib_values.xMin,Calib_values.xMax);
	printf("Calibration completed, xk1 %d xk2 %d yk1 %d yk2 %d\n",Calib_values.x_k1,Calib_values.x_k2,Calib_values.y_k1,Calib_values.y_k2);
}

bool JOY_button()
{
	return !get_bit(JOY_button_Port,JOY_button_Pin);
}

JOY_pos_t JOY_getPosition()
{
	JOY_pos_t pos;
	int16_t V_x = ADC_read(JOY_X_CH)-Calib_values.xMiddle;
	int16_t V_y = ADC_read(JOY_Y_CH)-Calib_values.yMiddle;
	
	if (V_x>0)
	{
		pos.x = (100*V_x)/Calib_values.x_k2;
	}
	else
	{
		pos.x = (100*V_x)/Calib_values.x_k1;
	}
	
	if (V_y>0)
	{
		pos.y = (100*V_y)/Calib_values.y_k2;
	}
	else
	{
		pos.y = (100*V_y)/Calib_values.y_k1;
	}
	
	/* Within calibrated values */
	if (pos.x < -100)
	{
		pos.x = -100;
	}
	else if (pos.x > 100)
	{
		pos.x = 100;
	}
	
	if (pos.y < -100)
	{
		pos.y = -100;
	}
	else if (pos.y > 100)
	{
		pos.y = 100;
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