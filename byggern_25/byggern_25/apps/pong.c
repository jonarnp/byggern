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
#include "../drivers/oled.h"
#include "../drivers/slider.h"
#include "../drivers/joy.h"

/* Pong game definitions */
#define MAX_SCORE 99
#define GAME_PX_WIDTH 128
#define GAME_PX_HEIGHT 56
#define PONG_RES_WIDTH ((10*GAME_PX_WIDTH) - 1)	//((10*OLED_COLUMN_SPAN) + 9)	/* 1279 */
#define PONG_RES_HEIGHT	((10*GAME_PX_HEIGHT) - 1)	//((10*8*OLED_LINE_SPAN) - 1)	/* 559 */
#define BALL_SIZE 1
#define SLIDER_HEIGHT 8
#define SLIDER_1_COLOUMN 4
#define SLIDER_2_COLOUMN (OLED_COLUMN_SPAN-SLIDER_1_COLOUMN)

/* Definition of ball structure */
typedef struct ball {
	uint16_t x_pos;
	uint16_t y_pos;
	uint16_t x_vel;
	uint16_t y_vel;
} ball_t;

/* Position [0, 46] 46 = GAME_PX_HEIGHT-SLIDER_HEIGHT */
typedef struct game_slider {
	uint16_t y_pos;
	uint8_t col;
} game_slider_t;

/*
Private functions
*/

/* Print a welcome message, resets the ball and sliders. Press left slider button to continue */
void pong_init();

/* Print the welcome message to the OLED */
void welcome_msg();

/* Reset ball position, velocity and bounces */
void reset_ball();

/* Update the physics of the ball, check for collisions and give points */
void pong_update();

/*
Increments the score for the specific player. Draw the scores. Set finished flag if MAX_SCORE is reached
@param uint8_t player. Range [1,2]
*/
void update_score(uint8_t player);

/* Draw scores on the OLED */
void draw_score();

/* Draw updated ball and sliders */
void update_screen();

/* Draw updated sliders and clears sliders from previous frame */
void update_game_sliders();

/*
Draw the pixels of the slider on the OLED
@param game_slider_t. Slider struct
@param uint8_t byte. 0xFF for drawing, 0x00 for clearing
*/
void draw_slider_byte(game_slider_t s, uint8_t byte);

/* Draw updated ball and clears ball from previous frame */
void update_game_ball();

/*
Draw the pixels of the ball on the OLED
@param ball_t. Ball struct
@param uint8_t draw. 0x01 for drawing, 0x00 for clearing
*/
void draw_ball_byte(ball_t b, uint8_t draw);


/* Pong game variables */
bool finished;
ball_t ball_now;
ball_t ball_prev; //kept for clearing ball in previous frame
game_slider_t s1, s2;
game_slider_t s1_prev, s2_prev; //kept for clearing sliders in previous frame
uint8_t score_p1, score_p2;
uint16_t bounces; //ball speed in x direction increases with each bounce

void play_pong()
{
	pong_init();
	update_screen();
	
	uint8_t ms_delay = 1000/FPS;
	
	while(!finished)
	{
		pong_update();
		update_screen();
		_delay_ms(ms_delay);
		
		if (SLIDER_right_button())
		{
			//Finished playing pong
			finished = true;
		}
	}
}

void pong_init()
{
	finished = false;
	
	welcome_msg();
	
	//Print player "names"
	oled_clear();
	oled_goto_column(0);
	oled_goto_line(0);
	oled_print_underscore_p(PSTR(" Player 1:    Player 2:  "));
	score_p1 = 0;
	score_p2 = 0;
	draw_score();
	
	reset_ball();
	
	//Reset sliders
	s1.y_pos = 0;
	s1.col = SLIDER_1_COLOUMN;
	s2.y_pos = 0;
	s2.col = SLIDER_2_COLOUMN;
	s1_prev = s1;
	s2_prev = s2;
}

void welcome_msg()
{
	oled_clear();
	oled_goto_column(0);
	oled_goto_line(0);
	oled_print_p(PSTR("Start Pong with left slider button"));
	
	oled_goto_column(0);
	oled_goto_line(3);
	oled_print_p(PSTR("Player 1: left slider"));
	
	oled_goto_column(0);
	oled_goto_line(4);
	oled_print_p(PSTR("Player 2: right slider"));
	
	oled_goto_column(0);
	oled_goto_line(6);
	oled_print_p(PSTR("Exit with right slider button"));
	
	//Show message until left slider button is pushed
	while(!SLIDER_left_button());
}

void reset_ball()
{
	bounces = 0;
	
	//Resets ball position and velocity
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
	
	//Read slider positions
	SLIDER_pos_t read_pos = SLIDER_getPosition();
	s1.y_pos = (uint16_t)((GAME_PX_HEIGHT-SLIDER_HEIGHT)*read_pos.left)/100;
	s2.y_pos = (uint16_t)((GAME_PX_HEIGHT-SLIDER_HEIGHT)*read_pos.right)/100;
	
	//Change y direction of ball when colliding with top or bottom wall
	if (ball_now.y_pos > (PONG_RES_HEIGHT-10) || ball_now.y_pos < 10)
	{
		ball_now.y_vel = -ball_now.y_vel;
	}
	
	//Change x direction of ball when colliding with slider 1
	if ((ball_now.x_pos/10) <= (s1.col+1) && (ball_now.y_pos/10 + 1) >= s1.y_pos && (ball_now.y_pos/10) <= s1.y_pos+SLIDER_HEIGHT)
	{
		int8_t dist_slider_mid = (ball_now.y_pos/10) - s1.y_pos - (SLIDER_HEIGHT/2);
		ball_now.y_vel = dist_slider_mid;
		ball_now.x_vel = -ball_now.x_vel;
		ball_now.x_vel++;
		bounces++;
	}
	//Change x direction of ball when colliding with slider 2
	else if ((ball_now.x_pos/10) >= (s2.col-1) && (ball_now.y_pos/10 + 1) >= s2.y_pos && (ball_now.y_pos/10) <= s2.y_pos+SLIDER_HEIGHT)
	{
		int8_t dist_slider_mid = (ball_now.y_pos/10) - s2.y_pos - (SLIDER_HEIGHT/2);
		ball_now.y_vel = dist_slider_mid;
		ball_now.x_vel = -ball_now.x_vel;
		ball_now.x_vel--;
		bounces++;
	}
	//Give points to player 2 if ball goes behind slider 1 and reset ball position
	else if ((ball_now.x_pos/10) <= s1.col)
	{
		draw_ball_byte(ball_prev, 0x00);
		reset_ball();
		
		update_score(2);
	}
	//Give points to player 1 if ball goes behind slider 2 and reset ball position
	else if ((ball_now.x_pos/10) >= s2.col)
	{	
		draw_ball_byte(ball_prev, 0x00);
		reset_ball();
		ball_now.x_vel = -ball_now.x_vel;
		
		update_score(1);
	}
}

void update_score(uint8_t player)
{
	if (player == 1)
	{
		//Point to Player 1!
		score_p1 += 1;
		
		//Check if MAX_SCORE reached
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
		//Point to Player 2!
		score_p2 += 1;
		
		//Check if MAX_SCORE reached
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
	
	//Calculate start line for the slider
	temp = 1 + (s.y_pos/8);
	line = (uint8_t)temp;
	
	//Calculate offset over to line below
	offset = s.y_pos % 8;
	
	//Draw slider depending on the line and offset
	oled_goto_column(s.col);
	if (line >= 7)
	{
		//Slider at bottom line
		oled_goto_line(7);
		oled_print_byte(byte);
	}
	else if (offset == 0)
	{
		//Slider at top of a line
		oled_goto_line(line);
		oled_print_byte(byte);
	}
	else
	{
		//Slider covers two lines
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
	
	//Calculate ball x position
	temp = b.x_pos/10;
	column = (uint8_t)temp;
	oled_goto_column(column);
	
	//Calculate ball y position line and row
	temp = b.y_pos/10;
	row = (uint8_t)temp;
	line = 1 + (row/8);
	offset = row % 8;
	oled_goto_line(line);
	oled_print_byte((draw << offset));
}