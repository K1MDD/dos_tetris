/*	Input.c
	Includes functions to initialize the IBM PC keyboard interrupt and store key presses in a global variable for general use
	Part of Tetris clone
	Marcus Drab
	2/28/2019*/

#include "..\include\input.h"
#include <dos.h>
#include <bios.h>

volatile unsigned char key = 0;						//Stores most recent key press

void (__interrupt __far *prev_int_09)(void);		//Null void pointer to store existing INT 9H ISV

void __interrupt __far keyboard_isr(void)			//Custom INT 9h ISR
{
	
	prev_int_09();									//Calls existing INT 9h ISR
	
	if(_bios_keybrd(_KEYBRD_READY)){				//Checks for new key press (uses INT 16h)
		key = _bios_keybrd(_KEYBRD_READ) & 0xFF;	//Gets ASCII value of pressed key from BIOS and stores in char key
	}
}

void init_kbd(void)
{
	prev_int_09 = _dos_getvect(0x09);				//Gets existing ISV for INT9H and stores in prev_int_09
    _dos_setvect(0x09, keyboard_isr);				//Replaces INT 9H ISV with pointer to custom ISR
}

void return_kbd(void)
{
	_dos_setvect(0x09, prev_int_09);				//Restores original INT 9H ISV before program terminates
}
