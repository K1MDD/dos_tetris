/*	Input.h
	Includes functions to initialize the IBM PC keyboard interrupt and store key presses in a global variable for general use
	Part of Tetris clone
	Marcus Drab
	2/28/2019*/
	
#ifndef INPUT_H
#define INPUT_H

volatile unsigned char key;		//Stores most recent key press

void init_kbd(void);			//Saves and replaces INT 9h ISV

void return_kbd(void);			//Restores original INT 9h ISV before program terminates

#endif
