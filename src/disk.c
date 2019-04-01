#include <dos.h>
#include "..\include\disk.h"
#include <fcntl.h>

#define SCORE_TABLE "scores.dat"
HIGH_SCORE scores[10];

void load_scores(void)
{
	unsigned char i, j, k, l;
	unsigned long int tens = 0;
	unsigned char buffer[120];
	unsigned int length;
	int handle = get_score_sheet_handle();
	_dos_read(handle, buffer, 120, &length);
	_dos_close(handle);
	j = 0;
	for(i = 0; i < 10; i++){
		while(buffer[j] != ';' && j < length){
			j++;
		}
		if(j >= length){
			scores[i].initials[0] = '-';
			scores[i].initials[1] = '-';
			scores[i].initials[2] = '-';
			scores[i].score = 0;
		}else if(j < length - 1){
			k = j + 1;
			do{
				j++;
				scores[i].initials[j - k] = buffer[j];
			}while(j - k < 2 && buffer[j] != '_');
			while(buffer[j] != '_' && j < length){
				j++;
			}
			if(j == length){
				scores[i].score = 0;
			}else if(j < length - 1){
				k = j + 1;
				do{
					j++;
					for(l = 0; l <= j - k; l++){
						if(l == 0){
							tens = 1;
						}else{
								tens = tens * 10;
							
						}
					}
					if(buffer[j] - 0x30 >= 0 && buffer[j] - 0x30 < 10){
						scores[i].score += (buffer[j] - 0x30) * tens;
					}
					
					
				} while(j < length && buffer[j + 1] != ';');
			}
		}
	}
}

void save_scores(void)
{
	unsigned char i, j;
	unsigned long int score;
	unsigned len_written;
	unsigned char buffer[12];
	int handle;
	if(!_dos_creat(SCORE_TABLE, _A_NORMAL, &handle)){
		
		for(i = 0; i < 10; i++){
			if(scores[i].score != 0){
				buffer[0] = ';';
				buffer[1] = scores[i].initials[0];
				buffer[2] = scores[i].initials[1];
				buffer[3] = scores[i].initials[2];
				buffer[4] = '_';
				score = scores[i].score;
				j = 5;
				while(score > 0 && j < 12){
					buffer[j] = score % 10 + 0x30;
					
					score = score / 10;
					j++;
				}
				_dos_write(handle, buffer, j, &len_written);
			}
		}
		_dos_close(handle);
	}
}

int get_score_sheet_handle(void)
{
	int handle;
	int error_code = _dos_creatnew(SCORE_TABLE, _A_NORMAL, &handle);
	if(!error_code){
		return(handle);
		
	}else{
		error_code = _dos_open(SCORE_TABLE, O_RDWR, &handle);
		if(!error_code){
			return(handle);
		}
		
	}
	return(0);
}
