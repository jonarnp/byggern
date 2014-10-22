/*
 * slider.c
 *
 * Created: 17.09.2014 08:24:52
 *  Author: boerikse
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include "slider.h"
#include "adc.h"
#include "..\bit_op.h"

typedef struct Slider_calib
{
	uint8_t lMin;
	uint8_t lMax;
	uint8_t rMin;
	uint8_t rMax;
} Slider_calib_t;

Slider_calib_t Slider_calib_values;

void SLIDER_init()
{
	ADC_Init();
	
	/* Set slider calibration button as input */
	clear_bit(SLIDER_calib_button_DDR,SLIDER_calib_button_Pin);
}

void SLIDER_calibrate()
{
	printf("Calibrating sliders. Move sliders to all extrema, release it and then push calib button\n");
		
	Slider_calib_values.lMax = ADC_read(SLIDER_LEFT_CH);
	Slider_calib_values.lMin = Slider_calib_values.lMax;
	Slider_calib_values.rMax = ADC_read(SLIDER_RIGHT_CH);
	Slider_calib_values.rMin = Slider_calib_values.rMax;
		
	while(!get_bit(SLIDER_calib_button_Port, SLIDER_calib_button_Pin)){
		uint8_t lTemp = ADC_read(SLIDER_LEFT_CH);
		uint8_t rTemp = ADC_read(SLIDER_RIGHT_CH);
			
		if(lTemp > Slider_calib_values.lMax){
			Slider_calib_values.lMax = lTemp;
			printf("lMax set to: %d\n", Slider_calib_values.lMax);
		}
		if (lTemp < Slider_calib_values.lMin){
			Slider_calib_values.lMin = lTemp;
			printf("lMin set to: %d\n", Slider_calib_values.lMin);
		}
			
		if(rTemp > Slider_calib_values.rMax){
			Slider_calib_values.rMax = rTemp;
			printf("rMax set to: %d\n", Slider_calib_values.rMax);
		}
		if (rTemp < Slider_calib_values.rMin){
			Slider_calib_values.rMin = rTemp;
			printf("rMin set to: %d\n", Slider_calib_values.rMin);
		}
	}
	printf("Button pushed\n");
		
	printf("Calibration completed, lMin %d lMax %d\n",Slider_calib_values.lMin,Slider_calib_values.lMax);
}

SLIDER_pos_t SLIDER_getPosition()
{
	SLIDER_pos_t pos;
	int16_t V_l = ADC_read(SLIDER_LEFT_CH);
	int16_t V_r = ADC_read(SLIDER_RIGHT_CH);
	//printf("Vx = %d Vy = %d",V_l,V_r);
	
	pos.left = (100*(V_l-Slider_calib_values.lMin))/(Slider_calib_values.lMax-Slider_calib_values.lMin);
	pos.right = (100*(V_r-Slider_calib_values.rMin))/(Slider_calib_values.rMax-Slider_calib_values.rMin);
	
	/* Within calibrated values */
	if(pos.left < 0)
	{
		pos.left = 0;
	}
	else if(pos.left > 100)
	{
		pos.left = 100;
	}
	
	if(pos.right < 0)
	{
		pos.right = 0;
	}
	else if(pos.right > 100)
	{
		pos.right = 100;
	}
	
	return pos;
}