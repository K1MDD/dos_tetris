/*	Timer1.h
	Includes functions to initialize the IBM PC timer interrupt and store clock ticks in a global variable for general use
	Part of Tetris clone
	Marcus Drab
	2/28/2019*/

#ifndef TIMER_H
#define TIMER_H

volatile unsigned int clock_ticks;					//Incremented approx. 18.2 times per second

void init_timer(void);								//Saves and replaces INT 1Ch ISV

void return_timer(void);							//Restores original INT 1Ch handler

unsigned int clock_ticks_since_midnight(void);		//Returns num of clock ticks since midnight (calls INT 1Ah)

#endif
