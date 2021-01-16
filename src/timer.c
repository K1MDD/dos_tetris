/*	Timer1.c
	Includes functions to initialize the IBM PC timer interrupt and store clock ticks in a global variable for general use
	Part of Tetris clone
	Marcus Drab
	2/28/2019*/
	
#include "..\include\timer.h"
#include "..\include\sound.h"
#include <dos.h>
#include <i86.h>

volatile unsigned int clock_ticks = 0;			//Incremented approx. 18.2 times per second

void (__interrupt __far *prev_int_1C)(void);	//Stores existing INT 1C ISV, chained to interrupt handler

void __interrupt __far timer_isr(void)			//INT 1C Handler
{
	clock_ticks ++;				//Increment clock_ticks counter
	if(target_speaker_ticks > 0 && speaker_ticks >= target_speaker_ticks)
	{
		target_speaker_ticks = 0;
		outp(0x61, inp(0x61) & 0xFC);
	}
	
	speaker_ticks ++;
	//printf("int\n");
	_chain_intr(prev_int_1C);	//Execute existing INT 1C ISR
}

void init_timer(void)							//Saves and replaces INT 1C ISV
{
	prev_int_1C = _dos_getvect(0x1C);
	_dos_setvect(0x1C, timer_isr);
	
}
void return_timer(void)							//Restores original INT 1C handler
{
	_dos_setvect(0x1C, prev_int_1C);
}

unsigned int clock_ticks_since_midnight(void)	//Returns num of clock ticks since midnight (calls INT 1A)
{
	union REGS regs;
	
	regs.h.ah = 0x00;
	
	int86(0x1A, &regs, &regs);
	return(regs.w.dx);
}
