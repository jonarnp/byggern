/*
 * oled.h
 *
 * Created: 17.09.2014 10:46:35
 *  Author: boerikse
 */ 


#ifndef OLED_H_
#define OLED_H_

#define OLED_COMMAND 0x1100
#define OLED_DATA 0x1300

void oled_init();

void oled_home();

void oled_goto_line(line);

void oled_clear_line(line);

void oled_clear();

void oled_pos(row,column);

void oled_putchar(char c);

void oled_print(char* data);

#endif /* OLED_H_ */