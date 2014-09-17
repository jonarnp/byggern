/*
 * joy_test.c
 *
 * Created: 10.09.2014 15:23:42
 *  Author: jonarnp
 */ 

#include "joy_test.h"
#include "..\drivers\joy.h"
#include <stdio.h>

void Test_joy()
{
	JOY_pos_t pos = JOY_getPosition();

	printf("Joystick position: X=%d , Y=%d\n", pos.x, pos.y);
	
	JOY_dir_t dir = JOY_getDirection();
	printf("Joystick position: enum=%d\n", dir);
}