/*	Screen.c
	Includes functions to perform various graphics routines using the INT 10h BIOS interrupt
	Part of Tetris clone
	Marcus Drab
	2/28/2019*/

#include "..\include\screen.h"
#include <i86.h>
#include <string.h>

#define FRAME_OFFSET_X 6
#define FRAME_OFFSET_Y 2

void draw_frame(char start_x, char start_y, char x, char y, char page)		//Draws a frame around a box specified by (start_x, start_y, x, y)
{

	int i;												//Counter
	move_cursor((start_y - 1), start_x - 1, page);		//Upper left corner
	write_char(0xC9, 1, page, white);
	move_cursor((start_y - 1), start_x, page);			//Top of frame
	write_char(0xCD, x, page, white);
	move_cursor((start_y - 1), (start_x + x), page);	//Upper right corner
	write_char(0xBB, 1, page, white);
	for(i = (start_y); i < (start_y + y); i++){			//Left and right sides
		move_cursor(i, (start_x - 1), page);
		write_char(0xBA, 1, page, white);
		move_cursor(i, (start_x + x), page);
		write_char(0xBA, 1, page, white);
	}
	move_cursor((start_y + y), (start_x - 1), page);	//Bottom left corner
	write_char(0xC8, 1, page, white);
	move_cursor((start_y + y), start_x, page);			//Bottom of frame
	write_char(0xCD, x, page, white);
	move_cursor((start_y + y), (start_x + x), page);	//Bottom right corner
	write_char(0xBC, 1, page, white);
}



COLOR get_box_color(BOX_COORDINATES b)		//Returns the color attribute stored at box coordinates b
{
	union REGS regs;
	
	char row = FRAME_OFFSET_Y + b.y;		//Convert box coordinates to char coordinates on screen
	char col = FRAME_OFFSET_X + b.x * 3;	//Same as above
	move_cursor(row, col, 0);				//Move cursor to correct char cell
	
	regs.h.ah = 0x08;						//AH = 8 (read char and attrib)
	regs.h.bh = 0;							//Page 0 (should probably be set by a parameter but this works for now)
	
	int86(0x10, &regs, &regs);				//Execute INT 10,08
	
	return(regs.h.ah);						//AH contains char attribute (color)
}

void draw_box(BOX_COORDINATES b, COLOR clr)	//Draws one box cell of a tetris piece at the location of b, with color clr
{
	char row = FRAME_OFFSET_Y + b.y;		//Convert box coordinates to char coordinates on screen
	char col = FRAME_OFFSET_X + b.x * 3;	//Same as above
	move_cursor(row, col, 0);				
	write_char(0xDB, 3, 0, clr);			//Write 3 solid blocks at (row, col) with color clr
}
char check_box(BOX_COORDINATES b)
{
	char row = FRAME_OFFSET_Y + b.y;		//Convert box coordinates to char coordinates on screen
	char col = FRAME_OFFSET_X + b.x * 3;	//Same as above
	move_cursor(row, col, 0);
	return(read_char(0));					//Read first char at box coordinates
}

void del_row(char y)				//Deletes a row of tetris pieces
{
	char i;							//Counter
	BOX_COORDINATES b;				//Box coordinates to be passed to del_box function
	b.y = y;						//y = row to be deleted
	for(i = 0; i < 10; i++){		//Deletes every box at column y
		b.x = i;
		del_box(b);
	}
}

void del_box(BOX_COORDINATES b)				//Deletes box at location of b
{
	char row = FRAME_OFFSET_Y + b.y;		//Convert box coordinates to char coordinates on screen
	char col = FRAME_OFFSET_X + b.x * 3;	//Same as above
	move_cursor(row, col, 0);
	write_char(0x20, 3, 0, 0);				//Delete 3 chars at box coordinates
}

void write_char(char c, char num, char page, unsigned char clr)		//Writes a char to the screen at current cursor position, must first be set by move_cursor
{
	union REGS regs;
	
	regs.w.ax = c;					//AL = char to write
	regs.h.ah = 0x09;				//AH = 09
	regs.w.bx = clr;				//BL = char attribute
	regs.h.bh = page;				//BH = display page
	regs.w.cx = num;				//CL = number of times char is repeated
	
	int86(0x10, &regs, &regs);		//Execute INT 10,09
}

void change_page(char page)			//Changes display page (default = 0)
{
	union REGS regs;
	
	regs.w.ax = page;				//AL = display page to switch to
	regs.h.ah = 0x05;				//AH = 05
	int86(0x10, &regs, &regs);		//Execute INT 10,05
}

void scroll_page_up(char lines)		//Scrolls entire screen up, lines = how many lines to scroll
{
	union REGS regs;
	
	regs.w.ax = lines;				//AL = number of lines to scroll
	regs.h.ah = 0x06;				//AH = 06
	regs.h.bh = 0;					//BH = 0 (display page)
	regs.w.cx = 0;					//CL = 0 (starting col)
	regs.h.ch = 0;					//CH = 0 (starting row)
	regs.w.dx = 79;					//DL = 79 (ending col)
	regs.h.dh = 24;					//DH = 24 (ending row)
	int86(0x10, &regs, &regs);		//Execute INT 10,06
}

void scroll_game_down(char row)			//Scrolls game window down, row = bottom row of window to scroll
{
	union REGS regs;
	
	regs.w.ax = 1;						//AL = 1 (scroll by 1 line)
	regs.h.ah = 0x07;					//AH = 07
	regs.h.bh = 0;						//BH = 0 (display page)
	regs.w.cx = FRAME_OFFSET_X;			//CL = starting col of game matrix
	regs.h.ch = FRAME_OFFSET_Y;			//CH = starting row of game matrix
	regs.w.dx = FRAME_OFFSET_X + 29;	//DL = last col of game matrix
	regs.h.dh = FRAME_OFFSET_Y + row;	//DH = starting row of game matrix + last row to scroll
	int86(0x10, &regs, &regs);			//Execute INT 10,07
}

void write_string(char row, char col, char* string, char page, COLOR clr)			//Writes a string of chars to the screen
{
	unsigned char i = 0;						//Counter
	for(i = 0; i < strlen(string); i++){		//Traverse through string
		move_cursor(row, col + i, page);		//Start cursor at (row, col) and increment col for each char
		write_char(string[i], 1, page, clr);	//Write char to screen
	}
}

char write_num(char row, char col, unsigned long int num, char page, COLOR clr)		//Converts an int to its ASCII text representation and writes to screen, returns num of chars written
{
	char c;
	if(num < 10){												//Writes most significant digit to screen
		move_cursor(row, col, page);
		write_char(num + 0x30, 1, page, clr);
		return(1);
	}else{														//If not most significant digit, call function again with num / 10
		c = write_num(row, col, num / 10, page, clr);
		move_cursor(row, col + c, page);						//Column is incremented by each iteration of the function to indicate how many digits have already been written
		write_char(num % 10 + 0x30, 1, page, clr);
		return(c + 1);											//c = number of digits which have been written to the screen
	}
}

char read_char(char page)		//Returns the char at the current cursor position, must be set first by move_cursor
{
	union REGS regs;
	
	regs.h.ah = 0x08;			//AH = 08
	regs.h.bh = page;			//BH = display page
	
	int86(0x10, &regs, &regs);	//Execute INT 10,08
	
	return(regs.w.ax & 0xFF);	//AL contains character
}

void set_video_mode(VIDEO_MODE vm)		//Changes video mode
{
	union REGS regs;
	
	regs.w.ax =	vm;				//AH = 0, AL = video mode
	
	int86(0x10, &regs, &regs);	//Execute INT 10,00
}

void move_cursor(char row, char col, char page)		//Moves cursor on screen
{
	union REGS regs;
	
	regs.h.ah = 2;				//AH = 02
	regs.h.bh = page;			//BH = display page
	regs.w.dx = col;			//DL = cursor column
	regs.h.dh = row;			//DH = cursor row
	
	int86(0x10, &regs, &regs);	//Execute INT 10,02
}

void return_screen(void)			//Clears screen, makes cursor visible, and returns cursor to (0, 0)
{
	union REGS regs;
	set_video_mode(color16_80col);	//Clear screen
	move_cursor(0, 0, 0);			//Return cursor to (0, 0)
	regs.h.ah = 1;					//AH = 01 (set cursor type)
	regs.w.cx = 0x0607;				//CX = 0x0607 (Corresponds to standard cursor)
	
	int86(0x10, &regs, &regs);		//Execute INT 10,01 (restores visible text cursor)
	
}

void init_screen(void)				//Sets video mode to 16 color 80 col text and makes cursor invisible
{
	union REGS regs;
	set_video_mode(color16_80col);	//Clear screen and initialize video mode to 16 color 80 column text

	regs.h.ah = 1;					//AH = 01 (set cursor type)
	regs.w.cx = 0x2607;				//CX = 0x0607 (Corresponds to invisible cursor)
	
	int86(0x10, &regs, &regs);		//Execute INT 10,01 (sets cursor to be invisible)
	
}
