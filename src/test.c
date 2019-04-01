#include "..\include\screen.h"
#include "..\include\input.h"
#include "..\include\game.h"
#include "..\include\timer.h"
#include "..\include\disk.h"
#include <stdio.h>

#define FRAME_OFFSET_X 6
#define FRAME_OFFSET_Y 2

int main(void)
{
	int i;
	load_scores();
	scores[2].initials[0] = 'I';
	scores[2].initials[1] = 'H';
	scores[2].initials[2] = 'C';
	scores[2].score = 1921680;
	save_scores();
	for(i = 0; i < 10; i++){
		printf("%c%c%c\t%lu\n", scores[i].initials[0], scores[i].initials[1], scores[i].initials[2], scores[i].score);
	}
	return(0);
}

