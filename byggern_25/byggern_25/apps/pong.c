/*
 * pong.c
 *
 * Created: 15.10.2014 11:10:21
 *  Author: jonarnp
 */ 

#include <stdbool.h>
#include <util/delay.h>
#include "pong.h"
#include "../drivers/oled.h"

#define PONG_RES_WIDTH 1270
#define PONG_RES_HEIGHT 550
#define BALL_SIZE 1
#define SLIDER_HEIGHT 8

ball_t ball;
slider_t s1, s2;

void pong_init();
void pong_update();
void update_screen();
void draw_sliders();
void draw_ball();

void play_pong()
{
	pong_init();
	update_screen();
	
	bool finished = false;
	//while(!finished)
	//{
		pong_update();
		update_screen();
		
	//}
}

void pong_init()
{
	oled_clear();
	oled_goto_column(0);
	oled_goto_line(0);
	oled_print_underscore("Player 1:    Player 2: ");
	
	ball.x_pos = PONG_RES_WIDTH/2;
	ball.y_pos = PONG_RES_HEIGHT/2;
	ball.x_vel = 2;
	ball.y_vel = 1;
	
	s1.y_pos = PONG_RES_HEIGHT/2;
	s2.y_pos = PONG_RES_HEIGHT/2;
}

void pong_update()
{
	ball.x_pos += ball.x_vel;
	ball.y_pos += ball.y_vel;
}

/*
* Ved veggkollisjon endre fortegn på ball.?_vel
*/

void update_screen()
{
	draw_sliders();
	draw_ball();
}

void draw_sliders()
{
	//DOES NOT WORK. Conversion to uint8_t does not work
	uint8_t line;
	
	//Slider 1
	oled_goto_column(4);
	line = 1 + ((int) (6*s1.y_pos/PONG_RES_HEIGHT));
	//printf("S1 at line: %d\n", line);
	oled_goto_line((uint8_t)line);
	oled_print_byte(0xFF);
	
	//Slider 2
	oled_goto_column(127-4);
	line = 1 + ((int) (6*s2.y_pos/PONG_RES_HEIGHT));
	//printf("S2 at line: %d\n", line);
	oled_goto_line((uint8_t)line);
	oled_print_byte(0xFF);
}

void draw_ball()
{
	
}