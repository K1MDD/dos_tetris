#include "..\include\input.h"
#include "..\include\sound.h"
#include "..\include\timer.h"

int main(void)
{
	int ticks = 4;
	init_timer();
    init_speaker();
	init_kbd();
    while(key != 'q')
	{
		switch(key)
		{
			case '1':
				ticks --;
				key = ' ';
				break;
			case '2':
				ticks ++;
				key = ' ';
				break;
			case 'a':
				beep(440, ticks);
				key = ' ';
				break;
			case 'b':
				beep(494, ticks);
				key = ' ';
				break;
			case 'c':
				beep(523,ticks);
				key = ' ';
				break;
			case 'd':
				beep(587,ticks);
				key = ' ';
				break;
			case 'e':
				beep(659, ticks);
				key = ' ';
				break;
			default:
				break;
		}
	}
	
    return_kbd();
	return_timer();
    return 0;
}
