/*	Screen.h
	Includes functions to perform various graphics routines using the INT 10h BIOS interrupt
	Part of Tetris clone
	Marcus Drab
	2/28/2019*/

#ifndef SCREEN_H
#define SCREEN_H

typedef enum video_mode {			//Video modes:
	mono_40col,							//Mono 40 Columns 					(Text)
	color16_40col,						//16 Color 40 Columns 				(Text)
	grey16_80col,						//16 Shade Gray Scale 80 Columns 	(Text)
	color16_80col,						//16 Color 80 Columns				(Text)
	color4_320x200,						//4 Color 320x200 Pixels			(Graphics)
	mono_640x200 = 6					//Mono 620x200 Pixels				(Graphics)
} VIDEO_MODE;

typedef enum color {				//In order of BIOS attrib number
	black,
	blue,
	green,
	cyan,
	red,
	magenta,
	brown,
	light_grey,
	dark_grey,
	light_blue,
	light_green,
	light_cyan,
	light_red,
	light_magenta,
	yellow,
	white
} COLOR;

typedef struct box_coordinates {	//(x,y) coordinate pair of Tetris box cell (one box is one character high by 3 characters long, measured relative to top-left (x,y) offset of frame
	char x;
	char y;
} BOX_COORDINATES;


void draw_frame(char start_x, char start_y, char x, char y, char page);				//Draws a frame around a box specified by (start_x, start_y, x, y)

void draw_box(BOX_COORDINATES b, COLOR clr);										//Draws one box cell of a tetris piece at the location of b, with color clr

void del_box(BOX_COORDINATES b);													//Deletes box at location of b

char check_box(BOX_COORDINATES b);													//Returns the char stored at box coordinates b; 0x20 == empty, 0xDB = box exists

char read_char(char page);															//Returns the char at the current cursor position, must be set first by move_cursor

void write_char(char c, char num, char page, unsigned char clr);					//Writes a char to the screen at current cursor position, must first be set by move_cursor

void set_video_mode(VIDEO_MODE vm);													//Changes video mode

void move_cursor(char row, char col, char page);									//Moves cursor on screen

void init_screen(void);																//Sets video mode to 16 color 80 col text and makes cursor invisible

void return_screen(void);															//Clears screen, makes cursor visible, and returns cursor to (0, 0)

void del_row(char y);																//Deletes a row of tetris pieces

void write_string(char row, char col, char* string, char page, COLOR clr);			//Writes a string of chars to the screen

char write_num(char row, char col, unsigned long int num, char page, COLOR clr);	//Converts an int to its ASCII text representation and writes to screen, returns num of chars written

void change_page(char page);														//Changes display page (default = 0)

void scroll_page_up(char lines);													//Scrolls entire screen up, lines = how many lines to scroll

void scroll_game_down(char row);													//Scrolls game window down, row = bottom row of window to scroll

COLOR get_box_color(BOX_COORDINATES b);												//Returns the color attribute stored at box coordinates b

#endif
