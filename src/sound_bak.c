/*Sound.c
*/
#include "..\include\sound.h"
#include "..\include\sound_track.h"
#include <dos.h>
#include <conio.h>

const unsigned int COUNTER = ((0x1234DC / 12000) & 0xfffe);	//8000 Hz Sample Rate

unsigned volatile int sound_seg_num = 0;
unsigned volatile int sound_track_offset = 0;

unsigned char sound_data = 0;

unsigned char bios_tick = 0;

unsigned volatile int sub_ticks = 0;

void (__interrupt __far *prev_int_08)(void);    //Stores existing INT 08 ISV, chained to interrupt handler

void (__interrupt __far tick_interrupt)(void)                   //INT 08 Handler
{
		
		if(*(sound_track[sound_seg_num] + sound_track_offset) == 0x00)
		{
			sound_track_offset = 0;
			if(sound_seg_num >= num_segs)
				sound_seg_num = 0;
			else
				sound_seg_num++;
		}
        sound_data = (*(sound_track[sound_seg_num] + sound_track_offset) >> 1);
		
		outp(0x43, 0xb0);
        outp(0x42, sound_data);
		outp(0x42, 0);
		
		_asm
		{
			mov     ax, sub_ticks   ; Add microsecond count to the counter
			add     ax, COUNTER
			mov     sub_ticks, ax
			jnc     nobios         ; If carry, it's time for a BIOS call
			mov		al, 1
			mov		bios_tick, al
			jmp 	fin
		nobios:
			mov		al,0
			mov		bios_tick, al
			mov     al, 0x20        ; If not, then acknowledge the IRQ
			out     0x20, al
		fin:
		}
		sound_track_offset++;
		if(bios_tick)
			_chain_intr(prev_int_08);
}

void init_sound(void)
{
        //Set up ISV & save BIOS int
        
        prev_int_08 = _dos_getvect(0x08);
        _dos_setvect(0x08, tick_interrupt);

        //Attach speaker to PIT ch. 2
        outp(0x61, inp(0x61) | 3);

        //Reprogram PIT Channel 0 to fire IRQ0 at 16kHz
		_disable();
        outp(0x43, 0x36);
        outp(0x40, COUNTER & 0x00FF);
		outp(0x40, COUNTER >> 8 & 0x00FF);
        _enable();
		_asm
		
}

void return_sound(void)
{
        //Slow the timer back down to 18.2 Hz
		_disable();
        outp(0x43, 0x36);
        outp(0x40, 0x00);
		outp(0x40, 0x00);
        _enable();
		
		//Turn off speaker
        outp(0x61, inp(0x61) & 0xfc);

        //Restore orig IRQ0
        _dos_setvect(0x08, prev_int_08);
}
