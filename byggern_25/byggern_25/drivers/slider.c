/*
 * slider.c
 *
 * Created: 17.09.2014 08:24:52
 *  Author: boerikse
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "slider.h"
#include "adc.h"
#include "oled.h"
#include "..\bit_op.h"

typedef struct Slider_calib
{
	uint8_t lMin;
	uint8_t lMax;
	uint8_t rMin;
	uint8_t rMax;
} Slider_calib_t;

Slider_calib_t Slider_calib_values;

const unsigned char PROGMEM slider_calib_str[4][7] = {
	{"lmin: "},
	{"lmax: "},
	{"rmin: "},
	{"rmax: "}
};

void SLIDER_init()
{
	ADC_Init();
	
	/* Set slider calibration button as input */
	clear_bit(SLIDER_calib_button_DDR,SLIDER_calib_button_Pin);
	
	clear_bit(SLIDER_left_button_DDR,SLIDER_right_button_Pin);
	clear_bit(SLIDER_right_button_DDR,SLIDER_right_button_Pin);
}

void SLIDER_calibrate()
{
	printf(PSTR("Calibrating sliders. Move sliders to all endpoints, then push calib button\n"));
	oled_clear();
	
	Slider_calib_values.lMax = 127; /* Half of max adc value */
	Slider_calib_values.lMin = Slider_calib_values.lMax;
	Slider_calib_values.rMax = 127; /* Half of max adc value */
	Slider_calib_values.rMin = Slider_calib_values.rMax;
	
	// Print text to OLED
	oled_goto_line(0);
	oled_goto_column(0);
	oled_print_p(PSTR("Move sliders to all endpoints."));
	oled_goto_line(2);
	oled_goto_column(0);
	oled_print_p(slider_calib_str[0]);
	oled_goto_line(2);
	oled_goto_column(63);
	oled_print_p(slider_calib_str[1]);
	oled_goto_line(3);
	oled_goto_column(0);
	oled_print_p(slider_calib_str[2]);
	oled_goto_line(3);
	oled_goto_column(63);
	oled_print_p(slider_calib_str[3]);
	oled_goto_line(4);
	oled_goto_column(0);
	oled_print_p(PSTR("After extrema is reached, press left slider button."));
		
	while(!get_bit(SLIDER_calib_button_Port, SLIDER_calib_button_Pin)){
		uint8_t lTemp = ADC_read(SLIDER_LEFT_CH);
		uint8_t rTemp = ADC_read(SLIDER_RIGHT_CH);
			
		if(lTemp > Slider_calib_values.lMax){
			Slider_calib_values.lMax = lTemp;
			printf("lMax set to: %d\n", Slider_calib_values.lMax);
			
			//Update OLED
			oled_clear_line(2);
			oled_goto_line(2);
			oled_goto_column(0);
			oled_print_p(slider_calib_str[0]);
			oled_print(uint8_to_str(Slider_calib_values.lMin));
						
			oled_goto_column(63);
			oled_print_p(slider_calib_str[1]);
			oled_print(uint8_to_str(Slider_calib_values.lMax));
		}
		if (lTemp < Slider_calib_values.lMin){
			Slider_calib_values.lMin = lTemp;
			printf("lMin set to: %d\n", Slider_calib_values.lMin);
			
			//Update OLED
			oled_clear_line(2);
			oled_goto_line(2);
			oled_goto_column(0);
			oled_print_p(slider_calib_str[0]);
			oled_print(uint8_to_str(Slider_calib_values.lMin));
						
			oled_goto_column(63);
			oled_print_p(slider_calib_str[1]);
			oled_print(uint8_to_str(Slider_calib_values.lMax));
		}
			
		if(rTemp > Slider_calib_values.rMax){
			Slider_calib_values.rMax = rTemp;
			printf("rMax set to: %d\n", Slider_calib_values.rMax);
			
			//Update OLED
			oled_clear_line(3);
			oled_goto_line(3);
			oled_goto_column(0);
			oled_print_p(slider_calib_str[2]);
			oled_print(uint8_to_str(Slider_calib_values.rMin));
						
			oled_goto_column(63);
			oled_print_p(slider_calib_str[3]);
			oled_print(uint8_to_str(Slider_calib_values.rMax));
		}
		if (rTemp < Slider_calib_values.rMin){
			Slider_calib_values.rMin = rTemp;
			printf("rMin set to: %d\n", Slider_calib_values.rMin);
			
			//Update OLED
			oled_clear_line(3);
			oled_goto_line(3);
			oled_goto_column(0);
			oled_print_p(slider_calib_str[2]);
			oled_print(uint8_to_str(Slider_calib_values.rMin));
						
			oled_goto_column(63);
			oled_print_p(slider_calib_str[3]);
			oled_print(uint8_to_str(Slider_calib_values.rMax));
		}
	}
	printf(PSTR("Button pushed\n"));
		
	printf(PSTR("Slider calibration completed\n"));
	//printf("Calibration completed, lMin %d lMax %d\n",Slider_calib_values.lMin,Slider_calib_values.lMax);
}

SLIDER_pos_t SLIDER_getPosition()
{
	SLIDER_pos_t pos;
	int16_t V_l = ADC_read(SLIDER_LEFT_CH);
	int16_t V_r = ADC_read(SLIDER_RIGHT_CH);
	//printf("Vx = %d Vy = %d",V_l,V_r);
	
	pos.left = (SLIDER_MAX*(V_l-Slider_calib_values.lMin))/(Slider_calib_values.lMax-Slider_calib_values.lMin);
	pos.right = (SLIDER_MAX*(V_r-Slider_calib_values.rMin))/(Slider_calib_values.rMax-Slider_calib_values.rMin);
	
	/* Within calibrated values */
	if(pos.left < SLIDER_MIN)
	{
		pos.left = SLIDER_MIN;
	}
	else if(pos.left > SLIDER_MAX)
	{
		pos.left = SLIDER_MAX;
	}
	
	if(pos.right < SLIDER_MIN)
	{
		pos.right = SLIDER_MIN;
	}
	else if(pos.right > SLIDER_MAX)
	{
		pos.right = SLIDER_MAX;
	}
	
	return pos;
}

bool SLIDER_left_button()
{
	return get_bit(SLIDER_left_button_Port,SLIDER_left_button_Pin);
}

bool SLIDER_right_button()
{
	return get_bit(SLIDER_right_button_Port,SLIDER_right_button_Pin);
}