/*Sound.c
*/
#include "..\include\sound.h"
#include <dos.h>
#include <conio.h>

volatile unsigned int speaker_ticks = 0;
unsigned int target_speaker_ticks = 0;

void speaker_on(void)
{
	outp(0x61, inp(0x61) | 3);
}
void speaker_off(void)
{
	outp(0x61, inp(0x61) & 0xFC);
}

void init_speaker(void)
{
	outp(0x43, 0xb6);	//Put PIT 2 Timer in Square wave Mode
	
}

void beep(unsigned int frequency, unsigned int ticks)
{
	unsigned int speaker_code = (0x1234DC / frequency) & 0xFFFE;
	outp(0x42, speaker_code & 0x00FF);
	outp(0x42, speaker_code >> 8 & 0x00FF);
	target_speaker_ticks = ticks;
	speaker_on();
	speaker_ticks = 0;
}