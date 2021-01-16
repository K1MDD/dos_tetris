/*Sound.c
*/
#include "..\include\sound.h"
#include <dos.h>
#include <conio.h>

volatile unsigned int speaker_ticks = 0;
unsigned int target_speaker_ticks = 0;

void beep(unsigned int frequency, unsigned int ticks)
{
	unsigned int speaker_code = 0x1234DC / 
}

void speaker_on(void)
{
	outp(0x61, inp(0x61 | 3));
}
void speaker_off(void)
{
	outp(0x61, inp(0x61) & 0xFC);
}

void init_speaker(void)
{
	outp(0x43, 0xb0);	//Put PIT 2 Timer in Square Ch. Mode
	
}