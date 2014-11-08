/*
 * pong.c
 *
 * Created: 15.10.2014 11:10:21
 *  Author: jonarnp
 */ 

#include <stdbool.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "pong.h"
#include "menu.h"
#include "../drivers/oled.h"
#include "../drivers/slider.h"
#include "../drivers/joy.h"

#define MAX_SCORE 99
#define GAME_PX_WIDTH 128
#define GAME_PX_HEIGHT 56
#define PONG_RES_WIDTH ((10*GAME_PX_WIDTH) - 1)	//((10*OLED_COLUMN_SPAN) + 9)	/* 1279 */
#define PONG_RES_HEIGHT	((10*GAME_PX_HEIGHT) - 1)	//((10*8*OLED_LINE_SPAN) - 1)	/* 559 */
#define BALL_SIZE 1
#define SLIDER_HEIGHT 8
#define SLIDER_1_COLOUMN 4
#define SLIDER_2_COLOUMN (OLED_COLUMN_SPAN-SLIDER_1_COLOUMN)

bool finished;
ball_t ball_now;
ball_t ball_prev;
game_slider_t s1, s2;
game_slider_t s1_prev, s2_prev;
uint8_t score_p1, score_p2;
uint16_t bounces;
uint8_t iterations;

void pong_init();
void reset_ball();
void pong_update();
void update_score(uint8_t player);
void draw_score();
void update_screen();
void update_game_sliders();
void draw_slider_byte(game_slider_t s, uint8_t byte);
void update_game_ball();
void draw_ball_byte(ball_t b, uint8_t draw);

void play_pong()
{
	pong_init();
	update_screen();
	
	uint8_t ms_delay = 1000/FPS;
	
	while(!finished) //if(!finished)
	{
		pong_update();
		update_screen();
		_delay_ms(ms_delay);
		
		if (JOY_button())
		{
			if (iterations++ > SELECT_DELAY) 
			{
				finished = true;
				iterations = 0;
				//printf("finished playing pong\n");
			}
		}
		else
		{
			iterations = 0;
		}
	}
}

void pong_init()
{
	finished = false;
	iterations = 0;
	
	oled_clear();
	oled_goto_column(0);
	oled_goto_line(0);
	oled_print_underscore_p(PSTR(" Player 1:    Player 2:  "));
	score_p1 = 0;
	score_p2 = 0;
	draw_score();
	
	reset_ball();
	
	s1.y_pos = 0;
	s1.col = SLIDER_1_COLOUMN;
	s2.y_pos = 0;
	s2.col = SLIDER_2_COLOUMN;
	s1_prev = s1;
	s2_prev = s2;
}

void reset_ball()
{
	bounces = 0;
	
	ball_now.x_pos = PONG_RES_WIDTH/2;
	ball_now.y_pos = PONG_RES_HEIGHT/2;
	ball_now.x_vel = 3;
	ball_now.y_vel = 0;
	
	ball_prev = ball_now;
}

void pong_update()
{
	//Save previous state of ball and sliders
	ball_prev = ball_now;
	s1_prev = s1;
	s2_prev = s2;
	
	//Get new state of ball and sliders
	ball_now.x_pos += ball_now.x_vel;
	ball_now.y_pos += ball_now.y_vel;
	
	SLIDER_pos_t read_pos = SLIDER_getPosition();
	s1.y_pos = (uint16_t)((GAME_PX_HEIGHT-1)*read_pos.left)/100;
	s2.y_pos = (uint16_t)((GAME_PX_HEIGHT-1)*read_pos.right)/100;
	//printf("Read pos: %d, %d\n", read_pos.left, read_pos.right);
	
	//Change y direction of ball when colliding with top or bottom wall
	if (ball_now.y_pos > (PONG_RES_HEIGHT-10) || ball_now.y_pos < 10)
	{
		ball_now.y_vel = -ball_now.y_vel;
	}
	
	//Change x direction of ball when colliding with game sliders
	if ((ball_now.x_pos/10) <= (s1.col+1) && (ball_now.y_pos/10) >= (s1.y_pos-1) && (ball_now.y_pos/10) <= s1.y_pos+SLIDER_HEIGHT)
	{
		int8_t dist_slider_mid = (ball_now.y_pos/10) - s1.y_pos - (SLIDER_HEIGHT/2);
		ball_now.y_vel = dist_slider_mid;
		ball_now.x_vel = -ball_now.x_vel;
		ball_now.x_vel++;
		bounces++;
		//printf("ball x, y: %d, %d; slider: %d\n", ball_now.x_pos, ball_now.y_pos, s1.y_pos);
	}
	else if ((ball_now.x_pos/10) >= (s2.col-1) && (ball_now.y_pos/10) >= (s2.y_pos-1) && (ball_now.y_pos/10) <= s2.y_pos+SLIDER_HEIGHT)
	{
		int8_t dist_slider_mid = (ball_now.y_pos/10) - s2.y_pos - (SLIDER_HEIGHT/2);
		ball_now.y_vel = dist_slider_mid;
		ball_now.x_vel = -ball_now.x_vel;
		ball_now.x_vel--;
		bounces++;
		//printf("ball x, y: %d, %d; slider: %d\n", ball_now.x_pos, ball_now.y_pos, s1.y_pos);
	}
	//Give points if ball goes behind game sliders and reset ball position
	else if ((ball_now.x_pos/10) <= s1.col)
	{
		draw_ball_byte(ball_prev, 0x00);
		reset_ball();
		
		update_score(2);
		
		//printf("Point to Player 2!\n");
	}
	else if ((ball_now.x_pos/10) >= s2.col)
	{	
		draw_ball_byte(ball_prev, 0x00);
		reset_ball();
		ball_now.x_vel = -ball_now.x_vel;
		
		update_score(1);
		
		//printf("Point to Player 1!\n");
	}

	//printf("Ball pos: %d, %d\n", ball_now.x_pos, ball_now.y_pos);
}

void update_score(uint8_t player)
{
	if (player == 1)
	{
		score_p1 += 1;
		
		if(score_p1 >= MAX_SCORE)
		{
			finished = true;
			oled_clear();
			oled_goto_line(2);
			oled_goto_column(28);
			oled_print_p(PSTR("Player 1 wins!"));
			
			oled_goto_line(4);
			oled_goto_column(15);
			oled_print_p(PSTR("Player 2, you suck!"));
		}
	}
	else
	{
		score_p2 += 1;
		
		if(score_p2 >= MAX_SCORE)
		{
			finished = true;
			oled_clear();
			oled_goto_line(2);
			oled_goto_column(28);
			oled_print_p(PSTR("Player 2 wins!"));
			
			oled_goto_line(4);
			oled_goto_column(15);
			oled_print_p(PSTR("Player 1, you suck!"));
		}
	}
	
	draw_score();
}

void draw_score()
{
	char buffer[4];
	oled_goto_line(0);
	
	//Player 1
	oled_goto_column(50);
	snprintf(buffer, 10, "%d", score_p1);
	oled_print_underscore(buffer);
	
	//Player 2
	oled_goto_column(115);
	snprintf(buffer, 10, "%d", score_p2);
	oled_print_underscore(buffer);
}

void update_screen()
{
	update_game_sliders();	
	update_game_ball();
}

void update_game_sliders()
{
	//Clear previous sliders
	draw_slider_byte(s1_prev, 0x00);
	draw_slider_byte(s2_prev, 0x00);
	
	//Draw updated sliders
	draw_slider_byte(s1, 0xFF);
	draw_slider_byte(s2, 0xFF);
}

void draw_slider_byte(game_slider_t s, uint8_t byte)
{
	uint8_t line, offset;
	uint16_t temp;
	
	temp = 1 + (s.y_pos/8);
	line = (uint8_t)temp;
	//printf("S at pos and line: %d, %d\n", s.y_pos, line);
	
	offset = s.y_pos % 8;
	
	oled_goto_column(s.col);
	if (line >= 7)
	{
		oled_goto_line(7);
		oled_print_byte(byte);
	}
	else if (offset == 0)
	{
		oled_goto_line(line);
		oled_print_byte(byte);
	}
	else
	{
		oled_goto_line(line);
		oled_print_byte((byte << offset));
		oled_goto_column(s.col);
		oled_goto_line(line+1);
		oled_print_byte((byte >> (8-offset)));
	}
}

void update_game_ball()
{
	//Clear previous ball
	draw_ball_byte(ball_prev, 0x00);
	
	//Draw updated ball
	draw_ball_byte(ball_now, 0x01);
}

void draw_ball_byte(ball_t b, uint8_t draw)
{
	uint8_t column, row, line, offset;
	uint16_t temp;
	
	temp = b.x_pos/10;
	column = (uint8_t)temp;
	oled_goto_column(column);
	
	temp = b.y_pos/10;
	row = (uint8_t)temp;
	line = 1 + (row/8);
	offset = row % 8;
	oled_goto_line(line);
	oled_print_byte((draw << offset));

	//printf("Ball pixel: %d, %d, %d, %02x\n", column, line, offset, (draw << offset));
}