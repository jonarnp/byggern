/*
 * slider_test.c
 *
 * Created: 17.09.2014 09:31:17
 *  Author: boerikse
 */ 


#include "slider_test.h"
#include "..\drivers\slider.h"
#include <stdio.h>

void Test_slider()
{
	SLIDER_pos_t pos = SLIDER_getPosition();

	printf("Slider position: Left=%d , Right=%d\n", pos.left, pos.right);
}